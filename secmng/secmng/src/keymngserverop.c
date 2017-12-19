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
	
	return 0;
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
	memset(&msgKeyRes, 0, sizeof(MsgKey_Res));
	
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
	
	ret = MsgEncode(&msgKeyRes, ID_MsgKey_Res,  outData, datalen);
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func MsgEncode()");
		goto End;	
	}
	
	//协商密钥
	
	//写共享内存
	
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
	
	
	return 0 ;

}


int MngServer_Revoke(MngServer_Info *svrInfo, MsgKey_Req *msgkeyReq, unsigned char **outData, int *datalen)
{
	return 0 ;
}