#ifndef _KEYMNG_SHMOP_H_
#define _KEYMNG_SHMOP_H_
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif

//将网点密钥信息写共享内存，网点共享内存结构体
typedef struct _NodeSHMInfo{
    int status; //密钥状态， 0-有效 1无效
    char clientId[12]; //客户端id
    char serverId[12]; //服务器短id
    int seckeyid; //对称密钥id
    unsigned char seckey[128]; //对称密钥 //hash1 hash256 md5

}NodeSHMInfo;

//int KeyMng_ShmInit(int keyid, int keysize, void *shmid)
//打开共享内存，共享内存存在则使用，不存在则创建
int KeyMng_ShmInit(int key, int maxnodenum, int *shmhdl);

/*
1 写的网点密钥时，先判断网点密钥是否存在
    若存在则修改
    若不存在，则在空的位置，写入网店信息
*/
int KeyMng_ShmWrite(int shmhdl, int maxnodenum, NodeSHMInfo *pNodeInfo);

int KeyMng_ShmRead(int shmhdl, char *clientId, char *serverId, int maxnodenum, NodeSHMInfo *pNodeInfo);

#ifdef __cplusplus    
}
#endif
#endif