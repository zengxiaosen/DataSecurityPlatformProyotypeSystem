

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

//从数据库中拿数据
// scott 从

int main()
{
	int 		ret = 0;
	int 		bounds = 10;
	char* 		dbName = "orcl";
	char*	 	dbUser = "scott";
	char* 		dbPswd = "aa";
	
	char		mysql[2048] = {0};

	int deptno;
	char dname[24];
	char loc[24];
	char mysql[2048] = {0};
	ICDBCursor cursor = NULL;
	ICDBRow row;
	ICDBField field[4];

	memset(&row, 0, sizeof(ICDBRow));
	memset(field, 0, sizeof(ICDBField)*4);

	field[0].cont = (char *)&deptno;
	field[0].contLen = 4;
	field[1].cont = (char *)dname;
	field[1].contLen = 24;
	field[2].cont = (char *)loc;
	field[2].contLen = 24;

	row.field = field; // 建立关联
	row.fieldCount = 3;

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
		goto End;
	}
	
	//执行insert语言
	//strcpy(mysql, "insert into dept(deptno, dname, loc) values(31, '31中文dname', '31中文loc')");
	strcpy(mysql, "select deptno, dname, loc from dept");
	ret = IC_DBApi_OpenCursor(handle, mysql, 3, &cursor);
	if (ret != 0)
	{
		printf("func  IC_DBApi_OpenCursor() err:%d \n", ret);
		//return ret;
		goto End;
	}

	//获取游标所指向内存空间的数据
	while(1){
		//fetch into 一行一行的获取数据
		ret = IC_DBApi_FetchByCursor(handle, cursor, &row);
		if(ret == 100){
			//没有数据
			break;
		}else if(ret != 0){
			goto End;
		}
		printf("%d\t%s\t%s\n", deptno, dname, loc);
	}
End:
	if(cursor) IC_DBApi_CloseCursor(handle, cursor);


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