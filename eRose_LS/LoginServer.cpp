#include "LoginServer.h"


CLoginServer::CLoginServer(void)
{
	isRunning = false;
}


CLoginServer::~CLoginServer(void)
{
}

int CLoginServer::Run()
{
	isRunning = true;
	sql->Connect((SQLCHAR*)"seven_ORA", (SQLCHAR*)cfg->user, (SQLCHAR*)cfg->password);
	while(isRunning)
	{
		// Receive packet function
		// dispatch packet function
	}
	return 0;
}

void CLoginServer::Stop()
{
	isRunning = false;
}