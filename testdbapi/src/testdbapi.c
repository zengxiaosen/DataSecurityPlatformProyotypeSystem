

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

#include "icdbapi.h"

int main()
{
	int 		ret = 0;
	int 		bounds = 10;
	char* 		dbName = "orcl";
	char*	 	dbUser = "scott";
	char* 		dbPswd = "aa";
	
	char		mysql[2048] = {0};
	
	ICDBHandle	handle = NULL;
	
	//初始化数据库链接池
	ret = IC_DBApi_PoolInit( bounds,  dbName, dbUser, dbPswd);
	if (ret != 0)
	{
		printf("func  IC_DBApi_PoolInit() err:%d \n", ret);
		return ret;
	}

	//获取连接
	ret = IC_DBApi_ConnGet(&handle, 0, 0);
	if (ret != 0)
	{
		printf("func  IC_DBApi_ConnGet() err:%d \n", ret);
		return ret;
	}
	
	//开事物
	ret = IC_DBApi_BeginTran(handle);
	if (ret != 0)
	{
		printf("func  IC_DBApi_ConnGet() err:%d \n", ret);
		return ret;
	}
	
	//执行insert语言
	strcpy(mysql, "insert into dept(deptno, dname, loc) values(31, '31中文dname', '31中文loc')");
	
	//执行sql语言
	ret = IC_DBApi_ExecNSelSql(handle, mysql);
	if (ret != 0)
	{
		printf("func  IC_DBApi_ExecNSelSql() err:%d \n", ret);
		return ret;
	}
	
	if (ret == 0)
	{
		ret = IC_DBApi_Commit( handle);
	}
	else
	{
		ret =  IC_DBApi_Rollback( handle);
	}
	
	IC_DBApi_ConnFree( handle, 1); //1代表连接有效 不需要锻炼修复
	
	
	IC_DBApi_PoolFree();
	
	printf("hello....\n");
	return 0;	
}