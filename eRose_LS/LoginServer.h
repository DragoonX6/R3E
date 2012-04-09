#pragma once

#include "stdafx.h"
#include "MSSQL.h"

extern class CLoginServer *LS;

class CLoginServer: MSSQL
{
public:
	CLoginServer(void);
	~CLoginServer(void);
	DWORD WINAPI Run();
	bool isRunning;
	MSSQL *sql;
};