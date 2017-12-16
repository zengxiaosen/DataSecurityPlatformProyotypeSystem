#include <unistd.h>
#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include "poolsocket.h"

typedef struct _ThreadInfo
{
	void 	*handle;
	int 	iLoop;
	int 	iArrayIndex;
}ThreadInfo;

//线程题
void * myclient_routine (void *arg)
{
	int					ret = 0, i = 0;
	
	int 				timeout = 3;
	
	int 				connfd = 0;
	
	void 				*handle = NULL;
	
	unsigned char 		data[128] = {0};
	int 				datalen = 9;
	unsigned char *out = NULL;
	int 				outlen = 0;
	
	ThreadInfo			*pThreadInfo;
	
	strcpy(data, "1322222222222234444444444444444");
	
	pThreadInfo = (ThreadInfo *)arg;
	handle = pThreadInfo->handle;
		
	//客户端 socket池 获取一条连接 
	ret = sckCltPool_getConnet(handle, &connfd);
	if (ret != 0)
	{
		printf("func sckCltPool_getConnet() err:%d \n", ret);
		return NULL;
	}
	
	for (i=0; i<pThreadInfo->iLoop; i++)
	{
		memset(data, 0, sizeof(data));
		sprintf(data, "第%d线程的第%d圈", pThreadInfo->iArrayIndex+1, i+1);
		//客户端 socket池 发送数据 
		ret = sckCltPool_send(handle, connfd,  data, strlen(data));
		if (ret != 0)
		{
			printf("func sckCltPool_send() err:%d \n", ret);
			break;
		}
		
		//客户端 socket池 接受数据
		ret = sckCltPool_rev(handle, connfd, &out, &outlen); //1
		if (ret != 0)
		{
			printf("func sckCltPool_send() err:%d \n", ret);
			break;
		}
		printf("out:%s \n", out);
		sck_FreeMem((void **)&out);
		outlen = 0;
	}
	
	//客户端 socket池 把连接放回 socket池中 
	ret = sckCltPool_putConnet(handle, connfd, 0); //0正常 1
	
	free(pThreadInfo);
	return NULL;

}

//让用户输入线程的个数 和每个线程 运行的圈数 。。。做压力测试
int main()
{
	
	int 		ret = 0, i = 0;
	char 		*ip = "127.0.0.1";
	int 		port = 8001;
	int 		time = 3;
	int 		connfd = 0;
	
	void 		*handle = NULL;
	
	char 		mydata[128] = {0};
	int 		datalen = 9;
	
	unsigned char 	*myout = NULL;
	int 			outlen = 0;
	
	int 			iLoop = 0;
	int 			iThreadNum = 0;
	
	
	
	
	SCKClitPoolParam scpp;
	pthread_t 	pidArray[300]; //不能超过300
	memset(&scpp, 0, sizeof(SCKClitPoolParam));
	
	
	strcpy(scpp.serverip, "127.0.0.1");
	scpp.serverport = 8001;
	scpp.bounds = 10; //池容量
	scpp.connecttime = 3;
	scpp.sendtime = 3;
	scpp.revtime = 3;


	printf("\n请输入线程的个数: ");
	scanf("%d", &iThreadNum);
	
	printf("\n请输入每个线程运行的圈数: ");
	scanf("%d", &iLoop);

	
	//客户端 socket池初始化
	ret = sckCltPool_init(&handle, &scpp);
	if (ret != 0)
	{
		printf("func sckCltPool_init() err:%d \n", ret);
		return ret; 
	}
	
	//向线程中传递结构体
	for (i=0; i<iThreadNum; i++)
	{
		ThreadInfo		*pthreadInfo = (ThreadInfo *)malloc(sizeof(ThreadInfo));
		memset(pthreadInfo, 0, sizeof(ThreadInfo));
		pthreadInfo->handle = handle;
		pthreadInfo->iLoop = iLoop;
		pthreadInfo->iArrayIndex = i;
		//启动业务线程  处理连接
	 	pthread_create(&pidArray[i],NULL,myclient_routine, (void *)pthreadInfo);		
	}

	//等待所有的子线程都运行完毕 才结束进程
	for (i=0; i<iThreadNum; i++)
	{
		pthread_join(pidArray[i], NULL);
	}
	
	sckCltPool_destroy(handle);
	
	printf("client hello....\n");
	return 0;
}



