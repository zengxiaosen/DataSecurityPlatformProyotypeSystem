



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
	int ret = 0;

	strcpy(pCltInfo->clientId, "1111");
	strcpy(pCltInfo->AuthCode, "1111");
	strcpy(pCltInfo->serverId, "0001");
	strcpy(pCltInfo->serverip, "127.0.0.1");
	
	pCltInfo->serverport = 8001;
	pCltInfo->maxnode = 8001;
	pCltInfo->shmkey = 0x1111;
	pCltInfo->shmhdl = 0;

	/*
		typedef struct _MngClient_Info
		{
			char			clientId[12];	//客户端编号
			char			AuthCode[16];	//认证码
			char			serverId[12];	//服务器端编号
			
			char			serverip[32];
			int 			serverport;
			
			int				maxnode; //最大网点数 客户端默认1个
			int 			shmkey;	 //共享内存keyid 创建共享内存时使用	 
			int 			shmhdl; //共享内存句柄	
		}MngClient_Info;
	*/

	//客户端共享内存 初始化
	ret = KeyMng_ShmInit(pCltInfo->shmkey, pCltInfo->maxnode, &(pCltInfo->shmhdl));
	if(ret != 0){
		printf("func KeyMng_ShmInit() err:%d \n", ret);
		return ret;
	}


	
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

	char tmpbuf[128] = {0};
	
	//接受报文
	unsigned char 	*resData = NULL;
	int				resDataLen = 0;
 	
 	//服务器端的应答报文
 	MsgKey_Res			*pMsgKeyRes = NULL;
 	int 				iMsgKeyResType = 0;

	//共享内存节点信息NodeSHMInfo *pNodeInfo
	NodeSHMInfo NodeInfo;
	memset(&NodeSHMInfo, 0, sizeof(NodeSHMInfo));

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
	
	
	//组织共享内存节点信息

	/*
	typedef struct _NodeSHMInfo{
		int status; //密钥状态， 0-有效 1无效
		char clientId[12]; //客户端id
		char serverId[12]; //服务器短id
		int seckeyid; //对称密钥id
		unsigned char seckey[128]; //对称密钥 //hash1 hash256 md5

	}NodeSHMInfo;

	*/
	NodeInfo.status = 0;
	strcpy(NodeInfo.clientId, msgKeyReq.clientId);
	strcpy(NodeInfo.serverId, msgKeyReq.serverId);
	//对称密钥id
	NodeInfo.seckeyid = pMsgKeyRes->seckeyid;

	//按照一定规则写共享内存 r1 r2
	//r1 abcdefg...
	//r2 123456...
	//		a1b2c3d4...

	for(i=0; i< 64; i++){
		//msgKeyReq 是请求报文
		//tmpbuf[2*i] = msgKeyReq.r1[i];
		NodeInfo.seckey[2*i] = msgKeyReq.r1[i];

		//pMsgKeyRes 是应答报文
		//tmpbuf[2*i+1] = pMsgKeyRes->r2[i];
        NodeInfo.seckey[2*i + 1] = pMsgKeyRes->r2[i];
	}

	/*
	typedef struct _MngClient_Info
	{
		char			clientId[12];	//客户端编号
		char			AuthCode[16];	//认证码
		char			serverId[12];	//服务器端编号
		
		char			serverip[32];
		int 			serverport;
		
		int				maxnode; //最大网点数 客户端默认1个
		int 			shmkey;	 //共享内存keyid 创建共享内存时使用	 
		int 			shmhdl; //共享内存句柄	
	}MngClient_Info;

	*/

	//客户端 写共享内存
	ret = KeyMng_ShmWrite(pCltInfo->shmhdl, pCltInfo->maxnode, &NodeInfo);
	if(ret != 0){
		KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "func KeyMng_ShmWrite() err");
		goto End;
	}

	printf("客户端收到的keyid:%d \n", pMsgKeyRes->seckeyid);
	


		
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
/*
1 组织请求报文
1-1 读共享内存 节点信息
2 请求报文编码
3 发送报文
4 接受报文
5 解析应答报文
6 判断 是否校验成功
*/
int MngClient_Check(MngClient_Info *pCltInfo)
{
	int ret = 0;
	MsgKey_Req msgKeyReq;
	int mytime = 3;

	unsigned char *outData = NULL;
	int outLen = 0;
	int connfd = 0;

	//接受报文 
	unsigned char *resData = NULL;
	int resDataLen = 0;

	//服务器端的应答报文
	MsgKey_Res *pMsgKeyRes = NULL;
	int iMsgKeyResType = 0;

	//共享内存节点信息
	NodeSHMInfo NodeInfo;

	memset(&msgKeyReq, 0, sizeof(MsgKey_Req));
	memset(&NodeSHMInfo, 0, sizeof(NodeInfo));

	//组织请求报文
	//读共享内存
	ret = KeyMng_ShmRead(pCltInfo->shmhdl, pCltInfo->clientId, pCltInfo->serverId, pCltInfo->maxnode, &NodeInfo);
	if(ret != 0){
		KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "func MngClient_InitInfo() err");
		goto End;
	}
	msgKeyReq.cmdType = KeyMng_Check; //密钥校验 //modify bug iambombing
	strcpy(msgKeyReq.clientId, pCltInfo->clientId);
	strcpy(msgKeyReq.AuthCode, pCltInfo->AuthCode);
	strcpy(msgKeyReq.serverId, pCltInfo->serverId);

	//共享内存的前8个字节进行校验
	memcpy(msgKeyReq.r1, NodeInfo.seckey, 8);

	//编码请求报文
	ret = MsgEncode(&msgKeyReq, ID_MsgKey_Req, &outData, &outLen); // mem ok
	if(ret != 0){
		KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "func MngClient_InitInfo() err");
		goto End;
	}

	//发送报文
	//客户端 初始化
	ret = sckClient_init();
	//客户端 链接服务器
	ret = sckClient_connect(pCltInfo->serverip, pCltInfo->serverport, mytime, &connfd);
	if(ret != 0){
		KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "func sckClient_connect() err:%d serverip:%s, serverport:%d\n", pCltInfo->serverip, pCltInfo->serverport);
		goto End;
	}

	//客户端 发送报文
	ret = sckClient_send(connfd, mytime, outData, outLen);
	if(ret != 0){
		KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "func sckClient_send() err\n");
		goto End;
	}

	//客户端接受报文
	ret = sckClient_rev(connfd, mytime, &resData, &resDataLen);
	if(ret != 0){
		KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "func sckClient_rev() err\n");
		goto End;
	}

	//解析应答报文
	ret = MsgDecode(resData, resDataLen, (void **)&pMsgKeyRes, &iMsgKeyResType);
	if(ret != 0){
		KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "func MsgDecode() err\n");
		goto End;
	}

	if(pMsgKeyRes->rv == 0){
		printf("服务器校验结果 ok\n");
	}else{
		printf("服务器校验结果 fail\n");
	}

	printf("客户端接收到的keyid:%d \n", pMsgKeyRes->seckeyid);

End:
	//客户端 释放内存
	if(outData != NULL){
		MsgMemFree((void **)&outData, 0);
	}

	if(resData != NULL){
		sck_FreeMem((void **) &resData);
	}

	//客户端 释放内存
	if(pMsgKeyRes != NULL){
		MsgMemFree((void **)&pMsgKeyRes, iMsgKeyResType);
	}


	//客户端 关闭和服务器的链接
	sckClient_closeconn(connfd);

	//客户端 释放
	sckClient_destroy();

	return ret;

}

int MngClient_Revoke(MngClient_Info *pCltInfo)
{
	//读出节点信息
	//写入共享内存
	return 0;	
}

int MngClient_view(MngClient_Info *pCltInfo)
{
	return 0;	
}