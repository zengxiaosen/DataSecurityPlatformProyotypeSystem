

// secmng_globvar.h
#ifndef SECMNG_GLOB_VAR_H_
#define SECMNG_GLOB_VAR_H_

#include"stdafx.h"

//数据源全局变量
CString	g_dbSource, g_dbUse, g_dbpw;

CDatabase	myDB;
CDatabase	*g_pDB = NULL;

CString		g_SecMngIniName = "secmngadmin.ini";


#endif