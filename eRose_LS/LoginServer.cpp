#include "LoginServer.h"


CLoginServer::CLoginServer(void)
{
	isRunning = false;
}


CLoginServer::~CLoginServer(void)
{
}

DWORD WINAPI CLoginServer::Run()
{
	return NULL;
}