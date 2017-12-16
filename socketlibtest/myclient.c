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

int main()
{
	int 		ret = 0;
	char 		*ip = "127.0.0.1";
	int 		port = 8001;
	int 		time = 3;
	int 		connfd = 0;
	
	char 		mydata[128] = {0};
	int 		datalen = 9;
	
	unsigned char 	*myout = NULL;
	int 			outlen = 0;
	
	strcpy(mydata, "123456789890dabdefgeewqerewqre");
	ret = sckClient_init();
	if (ret != 0)
	{
		printf("func sckClient_init() err:%d \n", ret);
		return ret; 
	}
	
	ret = sckClient_connect(ip, port, time, &connfd);
	if (ret != 0)
	{
		printf("func sckClient_init() err:%d \n", ret);
		return ret; 
	}
	
	//客户端 发送报文
	ret = sckClient_send(connfd, time, mydata, datalen);
	if (ret != 0)
	{
		printf("func sckClient_send() err:%d \n", ret);
		return ret; 
	}
	
	//客户端 接受报文
	ret = sckClient_rev(connfd, time, &myout, &outlen); //1
	if (ret != 0)
	{
		printf("func sckClient_rev() err:%d \n", ret);
		return ret; 
	}
	
	printf("myout:%s \n", myout);
	
	
	//客户端 释放内存
	sck_FreeMem((void **)&myout);
	
	sckClient_closeconn(connfd);
	
	//客户端 释放
	sckClient_destroy();
	
	
	printf("client hello....\n");
	return 0;
}



