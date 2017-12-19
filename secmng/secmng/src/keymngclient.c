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
#include "keymnglog.h"

#include "poolsocket.h"
#include "keymngclientop.h"
#include "keymng_msg.h"

int Usage()
{
    int nSel = -1;
    
    system("clear");    
    printf("\n  /*************************************************************/");
    printf("\n  /*************************************************************/");
    printf("\n  /*     1.密钥协商                                            */");
    printf("\n  /*     2.密钥校验                                            */");
    printf("\n  /*     3.密钥注销                                            */");
    printf("\n  /*     4.密钥查看                                            */");
    printf("\n  /*     0.退出系统                                            */");
    printf("\n  /*************************************************************/");
    printf("\n  /*************************************************************/");
    printf("\n\n  选择:");
    scanf("%d", &nSel);
    while(getchar() != '\n');
    
    return nSel;
}

int mainxxx()
{
	//void KeyMng_Log(const char *file, int line, int level, int status, const char *fmt, ...);
	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[0], 0,"func MngClient_InitInfo() 0000");
	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[1], 1,"func MngClient_InitInfo() 1111");
	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[2], 2,"func MngClient_InitInfo() 2222");
	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[3], 3,"func MngClient_InitInfo() 3333");
	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], 4,"func MngClient_InitInfo() 4444");
	
	printf("hello.....\n");
	return 0;
}

int main()
{

	int 			ret = 0, nSel = 0;
	
	MngClient_Info 	mngClientInfo;

	//信息系统的初始化
	memset(&mngClientInfo, 0, sizeof(MngClient_Info));	
	
	ret = MngClient_InitInfo(&mngClientInfo);
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], 4,"func MngClient_InitInfo() err:%d", ret);
		return ret;
	}
	
	while (1)
	{
		nSel = Usage();
		switch (nSel)
		{
		case 0:
			ret = MngClient_Quit(&mngClientInfo);
			return ret;	
		case KeyMng_NEWorUPDATE:   //密钥协商
			ret = MngClient_Agree(&mngClientInfo);
			break;
			
		case KeyMng_Check: //密钥校验
			ret = MngClient_Check(&mngClientInfo);
			break;
			
		case KeyMng_Revoke:
			ret = MngClient_Revoke(&mngClientInfo);
			break;	
	
		default:
			ret =  -3;
			printf("输入选择不支持\n");	
		}
		
		if (ret != 0)
		{
			printf("\n!!!!!!!!!!!!!!!!!!!!ERROR!!!!!!!!!!!!!!!!!!!!");
			printf("\n错误码是：%x\n", ret);
		}
		else
		{
			printf("\n!!!!!!!!!!!!!!!!!!!!SUCCESS!!!!!!!!!!!!!!!!!!!!\n");
		}
		getchar();
	}


	return 0;
}




