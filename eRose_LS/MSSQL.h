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
	HANDLE sqlenv;
	HANDLE sqlconn;
};