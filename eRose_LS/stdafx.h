#pragma once

#include <stdio.h>
#include <Windows.h>
#include <CommCtrl.h>
//#include <tchar.h>
#include <string.h>
#include "resource.h"
#include "MSSQL.h"
#include "..\RECommon\SafeDelete.hpp"
#include "..\RNE\NetworkEvent.hpp"
#include "Thread.h"
#include "Window.h"
#include "LoginServer.h"
#include "Config.h"

BOOL CALLBACK MainDlg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK dlg2proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK cfgproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

DWORD WINAPI Run();

int closedialog(HWND h);

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif