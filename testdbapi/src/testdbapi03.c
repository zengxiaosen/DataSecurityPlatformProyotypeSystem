

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

/*
[it01@localhost testdbapi]$ ./testdbapi03
func IC_DBApi_PoolInit() err:17010 
*/

/*

ERROR:
ORA-28000: the account is locked
*/

/*
[2016.01.28 15:04:26] [DEBUG] [SUCCESS] Leave Sql_Check [src/icdbsqlcheck.c] [108]
[2016.01.28 15:04:26] [ERROR] [ERRNO is -1] IC_DBApi_ExecNSelSql EXECUTE IMMEDIATE Error [./src/icdbapi.c] [1669]
[2016.01.28 15:04:26] [ERROR] [ERRNO is -1] Sql is [insert into dept(deptno, dname, loc) values(94, '94name', '94loc') ] [./src/icdbapi.c] [1670]
*/

// oerr ora 1

/*
[it01@localhost testdbapi]$ oerr ora 1
00001, 00000, "unique constraint (%s.%s) violated"
// *Cause: An UPDATE or INSERT statement attempted to insert a duplicate key.
//         For Trusted Oracle configured in DBMS MAC mode, you may see
//         this message if a duplicate entry exists at a different level.
// *Action: Either remove the unique restriction or do not insert the key.
[it01@localhost testdbapi]$ 
*/

void main03_1()
{
	int 	ret = 0;
	int 	bounds = 10;
	char	* dbName = "orcl";
	char	* dbUser = "scott";
	char	* dbPswd = "aa";
	
	char	mysql[2048] = {0};
	
	ICDBHandle handle = NULL;
	//初始化链接池
	ret = IC_DBApi_PoolInit(bounds,  dbName, dbUser, dbPswd);
	if (ret != 0)
	{
		printf("func IC_DBApi_PoolInit() err:%d \n", ret);
		return; 
	}
	printf("func IC_DBApi_PoolInit() ok");
	
	//获取连接
	ret = IC_DBApi_ConnGet(&handle, 0, 0 );
	if (ret != 0)
	{
		printf("func IC_DBApi_ConnGet() err:%d \n", ret);
		goto End;
	}
	
	//事物: 一个或者多个sql语句的集合 要么都成功要么都失败
	//oracle的事情 和 mysql事物的机制是不一样 
	//oracle默认打开 insert sql ，insert sql， 
	//mysql 默认自动提交的 sql sql sql

	ret = IC_DBApi_BeginTran(handle);
	if (ret != 0)
	{
		IC_DBApi_ConnFree(handle, 1); //tag==1 连接ok 
		printf("func IC_DBApi_BeginTran() err:%d \n", ret);
		goto End;
	}
	strcpy(mysql, "insert into dept(deptno, dname, loc) values(96, '95name中文d', '95loc中文dd') ");
	
	
	//执行非select 
	ret = IC_DBApi_ExecNSelSql(handle, mysql);
	if (ret != 0 )
	{
		IC_DBApi_ConnFree(handle, 1); //tag==1 连接ok 
		printf("func IC_DBApi_ExecNSelSql() err:%d \n", ret);
		goto End;
	}
	
	if (ret == 0)
	{
		IC_DBApi_Commit(handle);
	}
	else
	{
		IC_DBApi_Rollback(handle);
	}
	//释放连接
	IC_DBApi_ConnFree(handle, 1); //tag==1 连接ok 
	
End:
	IC_DBApi_PoolFree();

	printf("heloo...\n");

}

void main()
{
	
	int 		ret = 0;
	int 		bounds = 1;
	char		*dbName = "orcl";
	char		*dbUser = "scott";
	char		*dbPswd = "aa";
	
	int 		deptno;
	char 		dname[24];
	char  		loc[24];
	
	
	ICDBRow   	row;
	
	ICDBField   field[4];
	memset(field, 0, sizeof(ICDBField) *4 );
	memset(&row, 0, sizeof(ICDBRow) );
	
	field[0].cont = (char *)&deptno;
	field[0].contLen = 4;
	
	field[1].cont = (char *)dname;
	field[1].contLen = 24;
	
	
	field[2].cont = (char *)loc;
	field[2].contLen = 24;
	
	row.field = field;
	row.fieldCount = 3;
	
	
	ICDBCursor 	cursor;
	char	 	mysqlbuf[2048] = {0};
	
	ICDBHandle 	handle = NULL;

	
	strcpy(mysqlbuf, "select deptno, dname, loc from dept");
	

	ret = IC_DBApi_PoolInit(10, dbName, dbUser, dbPswd);
	if (ret != 0)
	{
		printf("func IC_DBApi_PoolInit() err:%d \n", ret);
		return;
	}
		
	
	
	ret =  IC_DBApi_ConnGet(&handle, 0, 0);
	if (ret != 0)
	{
		printf("func IC_DBApi_PoolInit() err:%d \n", ret);
		return ;
	}
	
	
	//游标是为一次查询而生
	ret = IC_DBApi_OpenCursor(handle, mysqlbuf, 3, &cursor);
	if (ret != 0)
	{
		printf("func IC_DBApi_PoolInit() err:%d \n", ret);
		return ;
	}
	
	//aaa bbb  cccc
	//1111 222 3333
	//zzzz  zzz2  zzz3
	printf("\n");
	while (1)
	{
		ret = IC_DBApi_FetchByCursor(handle, cursor, &row);
		if (ret == 100)
		{
			break;
		}
		else if (ret == 0)
		{
			printf("%d\t%s\t%s\n", deptno, dname, loc);
		}
		else
		{
			printf("失败\n");
			break;
		}
	}
	
	IC_DBApi_CloseCursor(handle, cursor);	
	

	if (ret != 0)
	{
		if (ret == IC_DB_CONNECTION_ERR)
		{
			ret = IC_DBApi_ConnFree(handle, 0);//告诉底层库,进行锻炼修复
		}
		else
		{
			ret = IC_DBApi_ConnFree(handle, 1);
		}
	}
	else
	{
		IC_DBApi_ConnFree(handle, 1); //有效连接
	}	
	
	IC_DBApi_PoolFree();
	printf("hello....\n");	
}



