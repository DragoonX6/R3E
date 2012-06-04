#include "stdafx.hpp"

Index *idx;

HINSTANCE ghInstance;
HWND hWindow;

// the main code
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)

{
	MSG msg;
	ghInstance = hInstance;
	hWindow = CreateDialog(ghInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, MainDlg);
	if (hWindow == 0) {
		char buf[100];
		sprintf(buf, "Error 0x%x", GetLastError());
		MessageBox(0, buf, "CreateDialog", MB_ICONEXCLAMATION | MB_OK);
		return 1;
	}

	int status = 0;
	while((status = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if(status == -1)
		{
			return -1;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

// dialog message handler
BOOL CALLBACK MainDlg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		{
			return TRUE;
		}break;
	case WM_COMMAND:
		{
			switch(wParam)
			{
			case IDOK:
				{
					idx = new Index();
					
				}
			}
		}break;
	case WM_NOTIFY:
		{
			
		}break;
	}
	return FALSE;
}