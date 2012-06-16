#pragma once

#include "stdafx.h"
//#include "MSSQL.h"

extern class CLoginServer *LS;

class CLoginServer
{
public:
	CLoginServer(void);
	~CLoginServer(void);
	int Run();
	void Stop();
	int HandleEvent(NetworkEvent* evt);
private:
	bool isRunning;
	MSSQL *sql;
};