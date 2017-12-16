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
	
	strcpy(data, "1322222222222234444444444444444");
	
	handle = arg;
		
	//客户端 socket池 获取一条连接 
	ret = sckCltPool_getConnet(handle, &connfd);
	if (ret != 0)
	{
		printf("func sckCltPool_getConnet() err:%d \n", ret);
		return NULL;
	}
	
	for (i=0; i<10; i++)
	{
		//客户端 socket池 发送数据 
		ret = sckCltPool_send(handle, connfd,  data, datalen);
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

}

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
	
	SCKClitPoolParam scpp;
	pthread_t 	pidArray[100];
	memset(&scpp, 0, sizeof(SCKClitPoolParam));
	
	strcpy(scpp.serverip, "127.0.0.1");
	scpp.serverport = 8001;
	scpp.bounds = 10; //池容量
	scpp.connecttime = 3;
	scpp.sendtime = 3;
	scpp.revtime = 3;

	//客户端 socket池初始化
	ret = sckCltPool_init(&handle, &scpp);
	if (ret != 0)
	{
		printf("func sckCltPool_init() err:%d \n", ret);
		return ret; 
	}
	
	for (i=0; i<7; i++)
	{
		//启动业务线程  处理连接
	 	pthread_create(&pidArray[i],NULL,myclient_routine, (void *)handle);		
	}

	//等待所有的子线程都运行完毕 才结束进程
	for (i=0; i<7; i++)
	{
		pthread_join(pidArray[i], NULL);
	}
	
	sckCltPool_destroy(handle);
	
	printf("client hello....\n");
	return 0;
}



