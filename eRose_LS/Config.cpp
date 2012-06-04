#include "Config.h"


CConfig::CConfig(void)
{
	ip = new char[16];
	memset(ip, 0, sizeof(ip));
	password  = new char[1024];
	memset(password, 0, sizeof(password));
	serverport = 0;
	clientport = 0;
	minright = 0;
	limituser = 0;
}


CConfig::~CConfig(void)
{
	SAFE_DELETE_ARRAY(ip);
}

bool CConfig::ReadIni()
{
	WIN32_FIND_DATA find;
	FindFirstFile("./eRose_LS.ini", &find);
	if(GetLastError() == ERROR_FILE_NOT_FOUND)
	{
		return false;
	}
	GetPrivateProfileString("Config", "DBIP", NULL, ip, 16, "./eRose_LS.ini");
	serverport = GetPrivateProfileInt("Config", "ServerListenPort", NULL, "./eRose_LS.ini");
	clientport = GetPrivateProfileInt("Config", "ClientListenPort", NULL, "./eRose_LS.ini");
	minright = GetPrivateProfileInt("Config", "LoginRight", NULL, "./eRose_LS.ini");
	limituser = GetPrivateProfileInt("Config", "LimitUserCNT", NULL, "./eRose_LS.ini");
	GetPrivateProfileString("Config", "DBPassword", NULL, password, 1024, "./eRose_LS.ini");

	SetDlgItemText(cfgwnd->hWindow, IDC_IPADDRESS1, ip);
	SetDlgItemInt(cfgwnd->hWindow, IDC_EDIT2, serverport, false);
	SetDlgItemInt(cfgwnd->hWindow, IDC_EDIT3, minright, false);
	SetDlgItemInt(cfgwnd->hWindow, IDC_EDIT4, clientport, false);
	SetDlgItemInt(cfgwnd->hWindow, IDC_EDIT5, limituser, false);
	SetDlgItemText(cfgwnd->hWindow, IDC_EDIT1, password);

	return true;
}

bool CConfig::WriteIni()
{
		GetDlgItemText(cfgwnd->hWindow, IDC_IPADDRESS1, ip, 16);
		serverport = GetDlgItemInt(cfgwnd->hWindow, IDC_EDIT2, NULL, false);
		minright = GetDlgItemInt(cfgwnd->hWindow, IDC_EDIT3, NULL, false);
		clientport = GetDlgItemInt(cfgwnd->hWindow, IDC_EDIT4, NULL, false);
		limituser = GetDlgItemInt(cfgwnd->hWindow, IDC_EDIT5, NULL, false);
		GetDlgItemText(cfgwnd->hWindow, IDC_EDIT1, password, 1024);

		if(!WritePrivateProfileString("Config", "DBIP", ip, "./eRose_LS.ini"))
		{
			MessageBox(NULL, "Error writing to Erose_LS.ini", "Error", MB_ICONERROR | MB_OK);
			return false;
		}
		char tempstr[1024];
		memset(tempstr, 0, 1024);
		_itoa_s(serverport, tempstr, 10);
		WritePrivateProfileString("Config", "ServerListenPort", tempstr, "./eRose_LS.ini");
		memset(tempstr, 0, 1024);
		_itoa_s(minright, tempstr, 10);
		WritePrivateProfileString("Config", "LoginRight", tempstr, "./eRose_LS.ini");
		memset(tempstr, 0, 1024);
		_itoa_s(clientport, tempstr, 10);
		WritePrivateProfileString("Config", "ClientListenPort", tempstr, "./eRose_LS.ini");
		memset(tempstr, 0, 1024);
		_itoa_s(limituser, tempstr, 10);
		WritePrivateProfileString("Config", "LimitUserCNT", tempstr, "./eRose_LS.ini");
		WritePrivateProfileString("Config", "DBPassword", password, "./eRose_LS.ini");
		
	return true;
}