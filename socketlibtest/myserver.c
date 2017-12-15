#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include "poolsocket.h"





//  int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
//                          void *(*start_routine) (void *), void *arg);
//两大知识体系 	1:指针做函数参数 做接口的封装和设计
//					2: 函数指针函数参数 回调函数
//1 回调函数 : 1) 谁调用  函数指针函数参 这样的api函数,谁提供函数的入口地址
//				2) 回调函数的作用是:把任务的编写者和任务的调用者 进行解耦合  他是多态的基础


//线程题
void *(mystart_routine) (void *arg)
{
	int					ret = 0;
	int 				timeout = 3;
	unsigned char 		*out = NULL;
	int 				outlen = 0;
	
	printf("func mystart_routine() Begin\n");
	
	int  connfd = (int)arg;
	while (1)
	{
			//服务器端端接受报文
		ret = sckServer_rev(connfd,  timeout, &out, &outlen); //1
		if (ret == Sck_ErrPeerClosed)
		{
			printf("服务器检测到对方已经关闭\n");
			break;
		}
		else if (ret == 0)
		{
			;
		}
		else 
		{
			continue;
		}
		
		printf("out:%s \n", out); //危险
		
		//服务器端发送报文
		ret = sckServer_send(connfd, timeout, out, outlen);
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
			continue;
		}
		sck_FreeMem((void **)&out);
	}
	
	sckServer_close(connfd);
	printf("func mystart_routine() End\n");
} 


int main()
{
	int 		ret = 0;
	int 		listenfd = 0;
	int 		timeout = 3;
	int 		connfd = 0;
	pthread_t 	pid;
	
	ret = sckServer_init(8001, &listenfd);
	if (ret != 0)
	{
		printf("func sckServer_init() err:%d \n", ret);
		return ret;	
	}
	
	while (1)
	{
		ret = sckServer_accept(listenfd, timeout, &connfd);
		if (ret == Sck_ErrTimeOut)
		{
			printf("func sckServer_accept() err Sck_ErrTimeOut 超时\n", ret);
			continue;
		}
		else if (ret != 0)
		{
			break;
		}
	
		//启动业务线程  处理连接
	 	pthread_create(&pid,NULL,mystart_routine, (void *)connfd);		
		
	}

	sckServer_close(listenfd);
	//服务器端环境释放 
	sckServer_destroy();
 
	printf("hello...\n");	
}

