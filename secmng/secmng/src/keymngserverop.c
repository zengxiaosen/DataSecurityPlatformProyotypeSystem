#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <pthread.h>

#include "keymnglog.h"
#include "keymngserverop.h"
#include "poolsocket.h"
#include "keymng_msg.h"
#include "myipc_shm.h"

//#include "keymng_shmop.h"

#include "icdbapi.h"
//#include "keymng_dbop.h"


//初始化服务器 全局变量
int MngServer_InitInfo(MngServer_Info *svrInfo)
{
	int ret = 0;
	
	//从配置文件中 获取数据库的连接信息
	strcpy(svrInfo->serverId, "0001");
	strcpy(svrInfo->dbuse, "SECMNG");
	strcpy(svrInfo->dbpasswd, "SECMNG");
	strcpy(svrInfo->dbsid, "orcl");
	strcpy(svrInfo->serverip, "127.0.0.1"); //从数据库中获取
	
	svrInfo->dbpoolnum = 10;
	svrInfo->serverport = 8001;
	svrInfo->maxnode = 30;
	svrInfo->shmkey = 0x0001;
	svrInfo->shmhdl = 0;

	//从数据中 读取服务器启动的配置信息

	//初始化共享内存
	//int KeyMng_ShmInit(int key, int maxnodenum, int *shmhdl)
	ret = KeyMng_ShmInit(svrInfo->shmkey, svrInfo->maxnode, &svrInfo->shmhdl);
	if(ret != 0){
		ret = 33;
		printf("seckeyserver系统初始化共享内存失败\n");
	}

	//初始化数据库连接池
	ret = IC_DBApi_PoolInit(svrInfo->dbpoolnum, svrInfo->dbsid, svrInfo->dbuse, svrInfo->dbpasswd);
	if(ret != 0){
		printf("func IC_DBApi_PoolInit() err:%d \n", ret);
		return ret;
	}
	printf("初始化链接数据库 ok \n");
	
	
	return ret;
}

//服务器端密钥协商 应答业务流程
//1 密钥写共享内存
//2 密钥写数据库
//3 组织应答报文 ()
//4 编码应答报文

//从数据库中获取keyid
int MngServer_Agree(MngServer_Info *svrInfo, MsgKey_Req *msgkeyReq, unsigned char **outData, int *datalen)
{
	
	int 			ret = 0, i = 0;
	MsgKey_Res		msgKeyRes;
	NodeSHMInfo nodeShmInfo;
	memset(&msgKeyRes, 0, sizeof(MsgKey_Res));
	memset(&nodeShmInfo, 0, sizeof(NodeSHMInfo));
	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[2], ret,"func MngServer_Agree() Begin");
	
	if (svrInfo==NULL || msgkeyReq==NULL || outData==NULL || datalen==NULL )
	{
		ret = MngSvr_ParamErr;
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func MngServer_Agree() check svrInfo==NULL || msgkeyReq==NULL || outData==NULL || datalen==NULL");
		goto End;		
	}
	
	//服务器端 组织应答报文
	msgKeyRes.rv = 0;
	strcpy(msgKeyRes.clientId, msgkeyReq-> clientId);	//客户端编号
	strcpy(msgKeyRes.serverId, msgkeyReq-> serverId);	//服务器编号

	for (i=0; i<64; i++)
	{
		msgKeyRes.r2[i] = 'a' + i;
	}
	msgKeyRes.seckeyid = 99;//应该从数据库中获取递增序列号
	ret = MsgEncode(&msgKeyRes, ID_MsgKey_Res,  outData, datalen);
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func MsgEncode()");
		goto End;	
	}
	
	//组织共享内存节点信息
	/*
	//将网点密钥信息写共享内存，网点共享内存结构体
	typedef struct _NodeSHMInfo{
		int status; //密钥状态， 0-有效 1无效
		char clientId[12]; //客户端id
		char serverId[12]; //服务器短id
		int seckeyid; //对称密钥id
		unsigned char seckey[128]; //对称密钥 //hash1 hash256 md5

	}NodeSHMInfo;
	*/
	nodeShmInfo.status = 0;
	strcpy(nodeShmInfo.clientId, msgkeyReq->clientId);
	strcpy(nodeShmInfo.serverId, msgkeyReq->serverId);
	nodeShmInfo.seckeyid = msgKeyRes.seckeyid;
	//协商密钥
	for(i=0; i< 64; i++){
		nodeShmInfo.seckey[2*i] = msgkeyReq->r1[i];
		nodeShmInfo.seckey[2*i + 1] = msgKeyRes.r2[i];
	}

	//写共享内存
	ret = KeyMng_ShmWrite(svrInfo->shmhdl, svrInfo->maxnode, &nodeShmInfo);
	if(ret != 0){
		KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "func KeyMng_ShmWrite() err");
		goto End;
	}
	//写数据库



End:
	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[2], ret,"func MngServer_Agree() End");
	return ret ;
}


//服务器端密钥协商 应答业务流程
//1 根据客户端传递的clientid和serverid, 读共享内存
//2 比较 校验共享内存
//3 组织应答报文 ()
//4 编码应答报文
int MngServer_Check(MngServer_Info *svrInfo, MsgKey_Req *msgkeyReq, unsigned char **outData, int *datalen)
{
	int ret = 0;
	MsgKey_Res msgKeyRes;
	NodeSHMInfo nodeShmInfo;
	memset(&msgKeyRes, 0, sizeof(MsgKey_Res));
	memset(&nodeShmInfo, 0, sizeof(NodeSHMInfo));

	KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[2], ret, "func MngServer_Agree() Begin");
	if(svrInfo == NULL || msgkeyReq == NULL || outData == NULL || dataLen == NULL){
		ret = MngSvr_ParamErr;
		KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "func MngServer_Agree() check svrInfo == NULL || msgkeyReq == NULL || outData == NULL || dataLen == NULL");
		goto End;
	}

	//根据客户端传送的clientid和serverid 查找共享内存节点
	ret = KeyMng_ShmRead(svrInfo->shmhdl, msgkeyReq->clientId, msgkeyReq->serverId, svrInfo->maxnode, &nodeShmInfo);
	if(ret != 0){
		KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "func KeyMng_ShmRead() err");
		goto End;
	}

	//服务器端组织应答报文
	strcpy(msgKeyRes.clientId, msgkeyReq->clientId);//客户端编号
	strcyp(msgKeyRes.serverId, msgkeyReq->serverId);//服务器编号
	if(memcpy(nodeShmInfo.seckey, msgkeyReq->r1, 8) == 0){
		//只校验客户端共享内存的前8个字节
		msgKeyRes.rv = 0;
	}else{
		msgKeyRes.rv = 35;
	}
	msgKeyRes.seckeyid = 909;//应该从数据库中获取，递增序列号
	ret = MsgEncode(&msgKeyRes, ID_MsgKey_Res, outData, dataLen);

	if(ret != 0){
		KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "func MsgEncode() err");
		goto End;
	}

	//写数据库
End:
	KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "func MngServer_Check() End");
	return ret;
}


int MngServer_Revoke(MngServer_Info *svrInfo, MsgKey_Req *msgkeyReq, unsigned char **outData, int *datalen)
{
	return 0 ;
}