#pragma once

#include "stdafx.h"
#include "Window.h"

extern class CConfig *cfg;

class CConfig
{
public:
	CConfig(void);
	~CConfig(void);
	bool ReadIni();
	bool WriteIni();
	char *ip;
	char *password;
	int serverport;
	int clientport;
	int minright;
	int limituser;
};