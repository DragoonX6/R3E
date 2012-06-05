#include "MSSQL.h"

MSSQL::MSSQL(void)// :sqlenv(SQL_NULL_HANDLE), sqlconn(SQL_NULL_HANDLE)
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

void MSSQL::Disconnect()
{
	SQLFreeHandle(SQL_HANDLE_ENV, sqlenv);
	SQLFreeHandle(SQL_HANDLE_DBC, sqlconn);
	SQLFreeHandle(SQL_HANDLE_STMT, sqlstatement);
}

SQLRETURN MSSQL::ExecuteQuery(SQLCHAR* Query)
{
	SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, sqlconn, &sqlstatement);
	if(ret = SQL_ERROR || ret == SQL_INVALID_HANDLE)
		return ret;
	return SQLExecDirect(sqlstatement, Query, SQL_NTS);
}

void MSSQL::ReadData(SQLCHAR* Buffer, int size, SQLCHAR* Query)
{
	SQLCHAR retval[80000];
	memset(retval, 0, sizeof(retval)); // clear out buffer, we don't want it to be full of shit
	SQLINTEGER cbData = 0;
	SQLAllocHandle(SQL_HANDLE_STMT, sqlconn, &sqlstatement);
	SQLExecDirect(sqlstatement, Query, SQL_NTS);
	SQLFetch(sqlstatement); // Get the first result (there should only be one)
	SQLGetData(sqlstatement, 1, SQL_C_CHAR, retval, 80000, &cbData);
	memcpy_s(Buffer, size, retval, size);
}