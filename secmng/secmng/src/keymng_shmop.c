#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "keymnglog.h"
#include "keymng_shmop.h"
#include "myipc_shm.h"

//先检验共享内存是否存在
//1 若存在则使用旧的
//2 若不存在，则创建

int KeyMng_ShmInit(int key, int maxnodenum, int *shmhdl){
    int ret = 0;
    int tmpshmhdl;
    void *mapaddr = NULL;
    ret = IPC_OpenShm(key, maxnodenum, &tmpshmhdl);
    //若共享内存不存在
    if(ret == MYIPC_NotEXISTErr){
        printf("系统检测到没有共享内存，开始创建共享内存");
        //创建共享内存
        ret = IPC_CreatShm(key, maxnodenum*sizeof(NodeSHMInfo), &tmpshmhdl);
        if(ret != 0){
            printf("创建共享那日村失败\n");
            goto End;
        }
        //创建共享内存成功，则链接共享内存
        ret = IPC_MapShm(tmpshmhdl, &mapaddr);
        if(ret != 0){
            printf("链接共享内存失败\n");
            goto End;
        }

        //清除共享内存
        memset(mapaddr, 0, maxnode*sizeof(NodeSHMInfo));
        IPC_UnMapShm(mapaddr);

    }
    else if (ret == 0){
        printf("系统检测到共享内存存在\n");
        *shmhdl = tmpshmhdl;
    }
    else{
        printf("系统 IPC_OpenShm() 失败:%d\n", ret);
    }

    


End:
    return ret;
}

//写共享内存节点
// 1 若共享内存节点已经存在，则修改
// 2 若共享内存不存在，则找空位置插入
int KeyMng_ShmWrite(int shmhdl, int maxnodenum, NodeSHMInfo *pNodeInfo){
    int ret = 0, i = 0;
    NodeSHMInfo tmpNodeInfo;
    NodeSHMInfo *pNode = NULL;
    void *mapaddr = NULL;

    //链接共享内存
    ret = IPC_MapShm(shmhdl, &mapaddr);
    if(ret != 0){
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "IPC_MapShm() Err");
        goto End;
    }

    //判断共享内存节点是否已经存在，若存在则修改
    for(i=0; i< maxnode; i++){
        pNode = (NodeSHMInfo *)( mapaddr + i*sizeof(NodeSHMInfo));
        if(memcpy(pNode, &tmpNodeInfo, sizeof(NodeSHMInfo)) != 0){
            if(strcmp(pNode->clientId, pNodeInfo->clientId) == 0 && strcmp(pNode->serverId, pNodeInfo->serverId) == 0){
                KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[3], ret, "系统检测到有旧的节点信息");
                memcpy(pNode, pNodeInfo, sizeof(NodeSHMInfo));
                goto End;
            }
        }
    }

    //遍历节点数组，找个空位置插入
    for(i=0; i< maxnodenum; i++){
        pNode = (NodeSHMInfo *)(mapaddr + i*sizeof(NodeSHMInfo));
        if(memcpy(pNode, &tmpNodeInfo, sizeof(NodeSHMInfo)) == 0){
            KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[3], ret, "系统检测到有空节点信息");
            memcpy(pNode, pNodeInfo, sizeof(NodeSHMInfo));
            goto End;
        }
    }

    if(i == maxnodenum){
        ret = 33;
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "系统检测到共享内存节点已满 错误");
        goto End;
    }

End:
    KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[2], ret, "KeyMng_ShmWrite() ddddddddddddddddddddd");
    IPC_UnMapShm(mapaddr);
    return ret;
}

//读共享内存
int KeyMng_ShmRead(int shmhdl, char *clientId, char *serverId, int maxnodenum, NodeSHMInfo *pNodeInfo){
    int ret = 0, i = 0;
    NodeSHMInfo tmpNodeInfo;
    NodeSHMInfo *pNode = NULL;
    void *mapaddr = NULL;

    memset(&tmpNodeInfo, 0, sizeof(NodeSHMInfo));

    //链接共享内存
    ret = IPC_MapShm(shmhdl, &mapaddr);
    if(ret != 0){
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "IPC_MapShm() Err");
        goto End;
    }

    //判断 共享内存节点已存在 则修改
    for(i=0; i< maxnodenum; i++){
        pNode = (NodeSHMInfo *)( mappaddr + i*sizeof(NodeSHMInfo) );
        if( strcmp(pNode->clientId, clientId) == 0 && strcmp(pNode->serverId, serverId) == 0){
            KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[3], ret, "系统检测到有旧的节点信息");
            memcpy(pNodeInfo, pNode, sizeof(NodeSHMInfo));
            goto End;
        }
    }

    if(i ==  maxnodenum){
        ret = 33;
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "系统恶密友找到符合条件的共享内存节点clientId:%s, serverId:%s", clientId, serverId);
        goto End;
    }

End:
    IPC_UnMapShm(mapaddr);
    return ret;
}