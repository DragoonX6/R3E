#pragma once

#include "stdafx.h"
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>

class MSSQL
{
public:
	MSSQL(void);
	~MSSQL(void);
	bool Connect(SQLCHAR* dsn, SQLCHAR *user, SQLCHAR* password);
	void Disconnect();
	SQLRETURN ExecuteQuery(SQLCHAR* Query);
	void ReadData(SQLCHAR* Buffer, int size, SQLCHAR* Query);
private:
	HANDLE sqlenv, sqlconn, sqlstatement;
};