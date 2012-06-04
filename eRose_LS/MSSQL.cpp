#include "MSSQL.h"

MSSQL::MSSQL(void) :sqlenv(SQL_NULL_HANDLE), sqlconn(SQL_NULL_HANDLE)
{
}


MSSQL::~MSSQL(void)
{
}

bool MSSQL::Connect(SQLCHAR* dsn, SQLCHAR *user, SQLCHAR* password)
{
	SQLRETURN ret;
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlenv);
	if(ret == SQL_ERROR || ret == SQL_INVALID_HANDLE)
		return false;
	ret = SQLSetEnvAttr(&sqlenv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	if(ret == SQL_ERROR || ret == SQL_INVALID_HANDLE)
		return false;
	ret = SQLAllocHandle(SQL_HANDLE_DBC, sqlenv, &sqlconn);
	if(ret == SQL_ERROR || ret == SQL_INVALID_HANDLE)
		return false;
	ret = SQLConnect(sqlconn, dsn,SQL_NTS,user,SQL_NTS, password, SQL_NTS);
	if(ret == SQL_ERROR || ret == SQL_INVALID_HANDLE)
		return false;
	return true;
}