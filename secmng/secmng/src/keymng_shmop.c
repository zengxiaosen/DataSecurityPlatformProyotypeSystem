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