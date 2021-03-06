#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#include "keymnglog.h"
#include "keymngserverop.h"
#include "poolsocket.h"
#include "keymng_msg.h"
#include "icdbapi.h"

int g_EndTag = 0;

MngServer_Info			mngserverInfo;

//线程题
void *(mystart_routine) (void *arg)
{
	int					ret = 0;
	int 				timeout = 3;
	unsigned char 		*out = NULL;
	int 				outlen = 0;
	
	//请求报文信息
	MsgKey_Req			*pMsgKeyReq = NULL;
	int					iMsgKeyReqType = 0;
	
	//应答报文信息
	unsigned char 		*resout = NULL;
	int 				resoutlen = 0;
	
	
	printf("func mystart_routine() Begin\n");
	
	int  connfd = (int)arg;
	while (1)
	{
		if(g_EndTag == 1){
			break;
		}
		//服务器端端  接受报文
		ret = sckServer_rev(connfd,  timeout, &out, &outlen); //1 //mem real
		if (ret == Sck_ErrTimeOut)
		{
			continue;
		}
		else if (ret == Sck_ErrPeerClosed)
		{
			printf("服务器检测到对方已经关闭\n");
			break;
		}
		else if (ret != 0)
		{
			printf("fun sckServer_rev() err: ret:%d \n", ret);
			break;
		}
		
		//服务器端 解码报文
		ret = MsgDecode(out, outlen, (void **)&pMsgKeyReq, &iMsgKeyReqType); //mem real
		if (ret != 0)
		{
			printf("func MsgDecode() err:%d\n", ret);
			break;
		}

		switch (pMsgKeyReq->cmdType)
		{
			case KeyMng_NEWorUPDATE:
				ret = MngServer_Agree(&mngserverInfo, pMsgKeyReq, &resout, &resoutlen);
				if (ret != 0)
				{
					printf("func MngServer_Agree() err:%d\n", ret);
					break;
				}
			break;
			
			case KeyMng_Check:
				ret = MngServer_Check(&mngserverInfo, pMsgKeyReq, &resout, &resoutlen);
				if (ret != 0)
				{
					printf("func KeyMngServer_Agree() err:%d\n", ret);
					break;
				}
			
			break;
			
			case KeyMng_Revoke:
				ret = MngServer_Revoke(&mngserverInfo, pMsgKeyReq, &resout, &resoutlen);
				if (ret != 0)
				{
					printf("func MngServer_Revoke() err:%d\n", ret);
					break;
				}
			break;
			
			default:
				printf("业务类型不支持cmdType err:%d\n", pMsgKeyReq->cmdType);
				ret = 23;
			break;
			
			//错误处理 失败以后,也要告诉客户端 发生了什么错误
			if (ret != 0)
			{
				if (out != NULL) 			sck_FreeMem((void **) &out);
				if (pMsgKeyReq != NULL)		MsgMemFree( (void **)&pMsgKeyReq, iMsgKeyReqType);
				if (resout != NULL) 		MsgMemFree( (void **)&resout, 0);
					
				MsgKey_Res 			tmpMsgKesRes;
				tmpMsgKesRes.rv = ret;
				//tmpMsgKesRes.clientId;
				//tmpMsgKesRes.serverId;
				//编 码 应答报文
				//回发应答报文
				continue;				
			}
		}
		
		//服务器端发送报文
		ret = sckServer_send(connfd, timeout, resout, resoutlen);
		if (ret == Sck_ErrPeerClosed)
		{
			sck_FreeMem((void **)&out); //避免野指针
			printf("服务器检测到对方已经关闭\n");
			break;
		}
		else if (ret == 0)
		{
			;
		}
		else 
		{
			printf("fun sckServer_send() err: ret:%d \n", ret);
			break;
		}
		
		if (out != NULL) 			sck_FreeMem((void **) &out);
		if (pMsgKeyReq != NULL)		MsgMemFree( (void **)&pMsgKeyReq, iMsgKeyReqType);
		if (resout != NULL) 		MsgMemFree( (void **)&resout, 0);
	}
	
	sckServer_close(connfd);
	printf("func mystart_routine() End\n");
	return NULL;
} 
//创建守护进程
//把父进程干掉，然后让子进程去执行 setsid
//孩子又创建一个孙子进程，然后又把孩子进程干掉，只留下孙子进程
/*
孙子进程不再具有连接前端会话器的能力。但孩子还是具有链接前台的能力

*/
#define INIT_DAEMON \
{ \
	if(fork() > 0) exit(0); \
	setsid(); \
	if(fork() > 0) exit(0); \
}

int EndTag = 0;


int mainx(){
	fork();
	fork();
	fork();
	printf("aaa\n");
	//一共print8次
}

void mysighandler_t(int arg){
	g_EndTag = 1;
	printf("系统收到信号\n");
}

//函数指针做函数参数
//1 看到含所有 函数指针做函数参数的api函数，它的意思是说：谁调用我，谁提供函数的入口地址
//2 意思： 以前人写的框架，能调用后来人写的代码--》可扩展c++多态
		// 任务的编写者 和任务的调用者进行解耦合

int main()
{
	int 		ret = 0;

	int 		listenfd = 0;
	int 		timeout = 3;
	int 		connfd = 0;
	pthread_t 	pid;
	
	//对全局变量初始化
	memset(&mngserverInfo, 0, sizeof(MngServer_Info));
	signal(SIGUSR1, mysighandler_t); // 告诉linux内核，如何处理信号 （安装信号的处理函数，供条件满足时，被linux内核调用）

	INIT_DAEMON;
	
	//服务器端 配置参数 初始化
	ret = MngServer_InitInfo(&mngserverInfo);
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], 4,"func MngServer_InitInfo() err:%d", ret);
		return ret;
	}
	printf("func MngServer_InitInfo() ok\n ");
	
	
	ret = sckServer_init(mngserverInfo.serverport, &listenfd);
	if (ret != 0)
	{
		printf("func sckServer_init() err:%d \n", ret);
		return ret;	
	}
	printf("func sckServer_init() ok\n ");
	
	while (1)
	{
		ret = sckServer_accept(listenfd, timeout, &connfd);
		if (ret == Sck_ErrTimeOut)
		{
			printf("func sckServer_accept() err Sck_ErrTimeOut 超时\n");
			continue;
		}
		else if (ret != 0)
		{
			break;
		}
	
		//启动业务线程  处理连接
	 	pthread_create(&pid,NULL,mystart_routine, (void *)connfd);		
		
	}
	sleep(1);

	sckServer_close(listenfd);
	//服务器端环境释放 
	sckServer_destroy();

	//释放数据库链接池
	IC_DBApi_PoolFree();
	printf("系统收到信号后，安全退出...\n");	
	return 0;
}

