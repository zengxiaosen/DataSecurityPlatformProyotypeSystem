#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <pthread.h>

#include "keymng_dbop.h"
#include "keymngserverop.h"
#include "keymnglog.h"
#include "icdbapi.h"

//读keysn
int KeyMngsvr_DBOp_GenKeyID(void *dbhdl, int *keyid){
    int ret = 0;
    int ikeysn;

    ICDBRow row;
    ICDBField field[7];
    char mysql1[1024];
    char mysql2[1024];

    memset(field, 0, sizeof(field));
    memset(mysql1, 0, sizeof(mysql1));
    memset(mysql2, 0, sizeof(mysql2));

    if(dbhdl == NULL || keyid == NULL){
        ret = -1;
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "func KeyMngsvr_DBOp_GenKeyID() err");
        goto END;
    }

    sprintf(mysql1, "select ikeysn from SECMNG.KEYSN for update ");//使用sql锁 for update

    //读取序列号累加器
    field[0].cont = (char *)&ikeysn;

    row.field = field;
    row.fieldCount = 1;
    //累加器加1
    sprintf(mysql2, "update SECMNG.KEYSN set ikeysn = ikeysn + 1");

    ret = IC_DBApi_ExecNSelSql(dbhdl, mysql1, &row);
    if(ret != 0){
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "func IC_DBApi_ExecSelSql() err,\n sql:%s", mysql1);
        goto END;
    }

    ret = IC_DBApi_ExecNSelSql(dbhdl, mysql2);//执行非select 语句
    if(ret != 0){
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "func IC_DBApi_ExecNSelSql() err,\n sql:%s", mysql1);
        goto END;
    }
    *keyid = ikeysn;
END:
    return ret;

}

//写网点信息到数据库
int KeyMngsvr_DBOp_WriteSecKey(void *dbhdl, NodeSHMInfo *pNodeInfo){
    int ret = 0;
    char mysql[2048] = {0};
    char optime[24] = {0};
    char tmpseckey2[1024];
    int tmplen = 1024;

    char buf2[1024];

    memset(tmpseckey2, 0, sizeof(tmpseckey2));
    memset(mysql, 0, sizeof(mysql));

    //获取当前操作时间
    ret = IC_DBApi_GetDBTime(dbhdl, optime);
    if(ret != 0){
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "func IC_DBApi_GetDBTime() err");
        goto END;
    }

    //varchar2 4096 //ora9
    ret = IC_DBApi_Der2Pem(pNodeInfo->seckey, sizeof(pNodeInfo->seckey), tmpseckey2, &tmplen);
    if(ret != 0){
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "func IC_DBApi_Der2Pem() err");
        goto END;
    }

    printf("tmpseckey:%s \n", tmpseckey2);
    printf("tmplen:%d \n", tmplen);

    KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[2], ret, "\n tmpseckey===>%s \n tmplen===>%d\n", tmpseckey2, tmplen);

    //组织sql语句
    sprintf(mysql, "Insert Into SECMNG.SECKEYINFO(clientid, serverid, keyid, createtime, state, seckey) \
                    values ('%s', '%s', %d, '%s', %d, '%s') ", pNodeInfo->clientId, pNodeInfo->serverId, \
                    pNodeInfo->seckeyid, optime, 0, tmpseckey2 );
    
    //执行非select sql语句
    ret = IC_DBApi_ExecNSelSql(dbhdl, mysql);
    if(ret != 0){
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "func IC_DBApi_ExecNSelSql() err \n sql===>%s\n", mysql);
        goto END;
    }
END:
    return ret;
}