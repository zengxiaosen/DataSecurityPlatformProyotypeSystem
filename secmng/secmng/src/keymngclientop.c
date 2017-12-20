



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <pthread.h>


#include "keymng_msg.h"
#include "myipc_shm.h"
#include "poolsocket.h"
#include "keymnglog.h"
#include "keymngclientop.h"
#include "keymng_shmop.h"
//#include "keymng_shmop.h"

int MngClient_InitInfo(MngClient_Info *pCltInfo)
{
	strcpy(pCltInfo->clientId, "1111");
	strcpy(pCltInfo->AuthCode, "1111");
	strcpy(pCltInfo->serverId, "0001");
	strcpy(pCltInfo->serverip, "127.0.0.1");
	
	pCltInfo->serverport = 8001;
	pCltInfo->maxnode = 8001;
	pCltInfo->shmkey = 0x1111;
	pCltInfo->shmhdl = 0;

	//客户端共享内存 初始化
	KeyMng_ShmInit(pCltInfo->shmkey, )
	
	printf("读配置文件ok\n");

	return 0;	
}


int MngClient_Quit(MngClient_Info *pCltInfo)
{
	return 0;	
}


/*
1 组织请求报文
2 请求报文编码
3 发送报文
4 接受报文
5 解析应答报文
6 协商出密钥
7 写共享内存
*/

int MngClient_Agree(MngClient_Info *pCltInfo)
{
	
	int 		i = 0, ret = 0;
	MsgKey_Req		msgKeyReq;
	int 			mytime = 3;

	unsigned char	*outData = NULL;
	int				outLen  = 0;
	int 			connfd = 0;
	
	//接受报文
	unsigned char 	*resData = NULL;
	int				resDataLen = 0;
 	
 	//服务器端的应答报文
 	MsgKey_Res			*pMsgKeyRes = NULL;
 	int 				iMsgKeyResType = 0;

	memset(&msgKeyReq, 0, sizeof(MsgKey_Req));
	

	//组织 请求报文
	msgKeyReq.cmdType = KeyMng_NEWorUPDATE;
	strcpy(msgKeyReq.clientId, pCltInfo->clientId);
	strcpy(msgKeyReq.AuthCode, pCltInfo->AuthCode); 
	strcpy(msgKeyReq.serverId, pCltInfo->serverId); 
	for (i=0; i<64; i++)
	{
		msgKeyReq.r1[i] = 'a' + i;
	}
	
	//编码 请求报文	
	ret = MsgEncode(&msgKeyReq, ID_MsgKey_Req, &outData, &outLen); //mem ok
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], 4,"func MngClient_InitInfo() err:%d", ret);
		goto End;
	}
	
	//发送报文
	//客户端 初始化
	ret = sckClient_init();
	
	//客户端 连接服务器
	ret =  sckClient_connect(pCltInfo->serverip, pCltInfo->serverport, mytime, &connfd);
	if (ret != 0)
	{
		//KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], 4,"func sckClient_connect() err:%d", ret); //错误的做法
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], 4,"func sckClient_connect() err:%d serverip:%s, serverport:%d \n", ret, pCltInfo->serverip, pCltInfo->serverport); //错误的做法
		goto End;
	}
	
	//客户端 发送报文
	ret =  sckClient_send(connfd, mytime, outData, outLen); 
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], 4,"func sckClient_send() err:%d", ret);
		goto End;
	}
	
	//客户端 接受报文
	ret =  sckClient_rev(connfd, mytime,  &resData, &resDataLen); //1 //mem ok
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], 4,"func sckClient_rev() err:%d", ret);
		goto End;
	}
	
	// 解析应答报文
	ret = MsgDecode(resData, resDataLen, (void **)&pMsgKeyRes, &iMsgKeyResType); //mem ok
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], 4,"func sckClient_rev() err:%d", ret);
		goto End;
	}
	
	//按照一定规则写共享内存 r1 r2
		
End:
	
	//客户端 释放内存
	if (outData != NULL) 
	{
		MsgMemFree((void **)&outData, 0);
	}
	
	if (resData != NULL) 
	{
		sck_FreeMem((void **) &resData);
	}
	
	//客户端 释放内存
	if (pMsgKeyRes != NULL) 
	{
		MsgMemFree((void **)&pMsgKeyRes, iMsgKeyResType);
	}
	
	//客户端 关闭和服务端的连接
	sckClient_closeconn(connfd);
	//客户端 释放
	sckClient_destroy();

	return ret;	
}

int MngClient_Check(MngClient_Info *pCltInfo)
{
	return 0;	
}

int MngClient_Revoke(MngClient_Info *pCltInfo)
{
	return 0;	
}

int MngClient_view(MngClient_Info *pCltInfo)
{
	return 0;	
}