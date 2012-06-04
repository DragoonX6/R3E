#include "stdafx.h"
#include "Thread.h"
#include "Window.h"
#include "LoginServer.h"
#include "Config.h"

Window *wnd;
Window *wnd2;
Window *cfgwnd;
CLoginServer *LS;
CConfig *cfg;

DWORD WINAPI Run()
{
	return LS->Run();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	wnd = new Window(); // init our window object
	wnd2 = new Window(); // idem
	cfgwnd = new Window(); // idem
	cfg = new CConfig(); // init our config object
	InitCommonControls();
	MSG msg;
	wnd->ghInstance = hInstance; // init the window instance
	wnd2->ghInstance = hInstance; // idem
	cfgwnd->ghInstance = hInstance; // idem
	wnd->CmdShow = nCmdShow;
	wnd2->CmdShow = nCmdShow;
	cfgwnd->CmdShow = nCmdShow;
	wnd->hWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, MainDlg); // create our main dialog
	if(!wnd->hWindow) // if hwindow == NULL
	{
		char buf[100];
		sprintf_s(buf, "Error 0x%x", GetLastError()); // copy the last error into the buffer
		MessageBox (0, buf, "CreateDialog", MB_ICONEXCLAMATION | MB_OK); // display it in a message box
		//LOG(buf);
        return -1; // stop the application
    }

	ShowWindow(wnd->hWindow, wnd->CmdShow); // show the dialog
	UpdateWindow(wnd->hWindow); // update the window

	int status;
	while ((status = GetMessage(&msg,NULL,0,0)) !=0) // message loop
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;

}

// main dialog message handler
BOOL CALLBACK MainDlg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		{
			return FALSE;
		}break;
	case WM_COMMAND:
		{
			switch(wParam) // TODO add code for the other buttons
			{
			case WM_DESTROY: // the red X button closes our dialog and exits // TODO add server stopping code
				{
					closedialog(wnd->hWindow);
					PostQuitMessage(0);
				}break;
			case ID_SYSTEM_EXIT: // the menu button Exit also closes our dialog and exits // TODO add server stopping code
				{
					closedialog(wnd->hWindow);
					PostQuitMessage(0);
				}break;
			case ID_HELP_ABOUT:
				{
						wnd2->hWindow = CreateDialog(wnd2->ghInstance, MAKEINTRESOURCE(IDD_DIALOG2), wnd->hWindow, dlg2proc); //create our about window
						if(!wnd2->hWindow)
						{
							char buf[100];
							sprintf_s(buf, "Error 0x%x", GetLastError()); // copy the last error into the buffer
							MessageBox (0, buf, "Error creating about window", MB_ICONEXCLAMATION | MB_OK); // display it in a message box
						}
						SetDlgItemText(wnd2->hWindow, IDC_EDIT1, "\t\t\teRose Login Server\r\n\t\t\tWritten by Dragoon\r\n\t\t         rafael_andreas@hotmail.com\r\n"); // lazy way of centering the text
						ShowWindow(wnd2->hWindow, wnd2->CmdShow); //show it
						UpdateWindow(wnd2->hWindow); // update it
						
				}break;
			}
		}break;
	case WM_NOTIFY:
		{ // we init our stuffs here instead of in the WM_INITDIALOG case, because it is creating the dialog, means it's not there
		  // the result is no text or anything in our control
			if(!wnd->listset) // init the columns for our listings (fucking long IMO)
			{
				LVCOLUMN lvc;
				lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
				lvc.iSubItem	= 0;
				lvc.pszText		= "Account";
				lvc.cx			= 100;
				lvc.fmt			= LVCFMT_LEFT;
				ListView_InsertColumn(GetDlgItem(wnd->hWindow, IDC_LIST1), 0, &lvc);
				lvc.iSubItem	= 1;
				lvc.pszText		= "IP";
				lvc.cx			= 100;
				lvc.fmt			= LVCFMT_LEFT;
				ListView_InsertColumn(GetDlgItem(wnd->hWindow, IDC_LIST1), 1, &lvc);
				lvc.iSubItem	= 2;
				lvc.pszText		= "Connected Time";
				lvc.cx			= 100;
				lvc.fmt			= LVCFMT_LEFT;
				ListView_InsertColumn(GetDlgItem(wnd->hWindow, IDC_LIST1), 2, &lvc);
				LVCOLUMN lvc2;
				lvc2.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
				lvc2.iSubItem	= 0;
				lvc2.pszText	= "Name";
				lvc2.cx			= 100;
				lvc2.fmt		= LVCFMT_LEFT;
				ListView_InsertColumn(GetDlgItem(wnd->hWindow, IDC_LIST2), 0, &lvc2);
				lvc2.iSubItem	= 1;
				lvc2.pszText	= "IP";
				lvc2.cx			= 100;
				lvc2.fmt		= LVCFMT_LEFT;
				ListView_InsertColumn(GetDlgItem(wnd->hWindow, IDC_LIST2), 1, &lvc2);
				lvc2.iSubItem	= 2;
				lvc2.pszText	= "Port";
				lvc2.cx			= 72;
				lvc2.fmt		= LVCFMT_LEFT;
				ListView_InsertColumn(GetDlgItem(wnd->hWindow, IDC_LIST2), 2, &lvc2);
				lvc2.iSubItem	= 3;
				lvc2.pszText	= "Channels";
				lvc2.cx			= 100;
				lvc2.fmt		= LVCFMT_LEFT;
				ListView_InsertColumn(GetDlgItem(wnd->hWindow, IDC_LIST2), 3, &lvc2);
				lvc2.iSubItem	= 4;
				lvc2.pszText	= "Users";
				lvc2.cx			= 100;
				lvc2.fmt		= LVCFMT_LEFT;
				ListView_InsertColumn(GetDlgItem(wnd->hWindow, IDC_LIST2), 4, &lvc2);
				lvc2.iSubItem	= 5;
				lvc2.pszText	= "Max users";
				lvc2.cx			= 100;
				lvc2.fmt		= LVCFMT_LEFT;
				ListView_InsertColumn(GetDlgItem(wnd->hWindow, IDC_LIST2), 5, &lvc2);
				lvc2.iSubItem	= 6;
				lvc2.pszText	= "Connected Time";
				lvc2.cx			= 100;
				lvc2.fmt		= LVCFMT_LEFT;
				ListView_InsertColumn(GetDlgItem(wnd->hWindow, IDC_LIST2), 6, &lvc2);
				LVCOLUMN lvc3;
				lvc3.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
				lvc3.iSubItem	= 0;
				lvc3.pszText	= "IP";
				lvc3.cx			= 100;
				lvc3.fmt		= LVCFMT_LEFT;
				ListView_InsertColumn(GetDlgItem(wnd->hWindow, IDC_LIST3), 0, &lvc3);
				lvc3.iSubItem	= 1;
				lvc3.pszText	= "Start";
				lvc3.cx			= 100;
				lvc3.fmt		= LVCFMT_LEFT;
				ListView_InsertColumn(GetDlgItem(wnd->hWindow, IDC_LIST3), 1, &lvc3);
				lvc3.iSubItem	= 2;
				lvc3.pszText	= "End";
				lvc3.cx			= 100;
				lvc3.fmt		= LVCFMT_LEFT;
				ListView_InsertColumn(GetDlgItem(wnd->hWindow, IDC_LIST3), 2, &lvc3);
				wnd->listset = true; // this is to stop recreating the stuff every time it calls WM_NOTIFY
			}
			if(!wnd->tabctr) // init the tabs
			{ 
				if (TabCtrl_InsertItem(GetDlgItem(wnd->hWindow, IDC_TAB1), 0, &wnd->tie) == -1)
				{
					return NULL;
				}
				if (TabCtrl_InsertItem(GetDlgItem(wnd->hWindow, IDC_TAB1), 1, &wnd->tie2) == -1)
				{
					return NULL;
				}
				if (TabCtrl_InsertItem(GetDlgItem(wnd->hWindow, IDC_TAB1), 2, &wnd->tie3) == -1)
				{
					return NULL;
				}
				wnd->tabctr = true;
			}
			if(!wnd->configset)
			{
				cfgwnd->hWindow = CreateDialog(cfgwnd->ghInstance, MAKEINTRESOURCE(IDD_DIALOG3), wnd->hWindow, cfgproc);
				if(!cfgwnd->hWindow)
				{
					char buf[100];
					sprintf_s(buf, "Error 0x%x", GetLastError()); // copy the last error into the buffer
					MessageBox (0, buf, "Error creating config window", MB_ICONEXCLAMATION | MB_OK); // display it in a message box
					closedialog(cfgwnd->hWindow);
					closedialog(wnd->hWindow);
					PostQuitMessage(0);
				}
				ShowWindow(cfgwnd->hWindow, cfgwnd->CmdShow); //show it
				UpdateWindow(cfgwnd->hWindow); // update it
				wnd->configset = true;
			}
			switch(((LPNMHDR)lParam)->code)
			{
			case TCN_SELCHANGING:
				{
					return FALSE; // return false to allow the tab to change
				}break;
			case TCN_SELCHANGE:
				{
					int ipage = TabCtrl_GetCurSel(GetDlgItem(wnd->hWindow, IDC_TAB1));
					if(ipage == 0)
					{
						ShowWindow(GetDlgItem(wnd->hWindow, IDC_LIST1), 1);
						ShowWindow(GetDlgItem(wnd->hWindow, IDC_LIST2), 0);
						ShowWindow(GetDlgItem(wnd->hWindow, IDC_LIST3), 0);
					}
					else if(ipage == 1)
					{
						ShowWindow(GetDlgItem(wnd->hWindow, IDC_LIST1), 0);
						ShowWindow(GetDlgItem(wnd->hWindow, IDC_LIST2), 1);
						ShowWindow(GetDlgItem(wnd->hWindow, IDC_LIST3), 0);
					}
					else if(ipage == 2)
					{
						ShowWindow(GetDlgItem(wnd->hWindow, IDC_LIST1), 0);
						ShowWindow(GetDlgItem(wnd->hWindow, IDC_LIST2), 0);
						ShowWindow(GetDlgItem(wnd->hWindow, IDC_LIST3), 1);
					}
				}break;
			}
		}break;
	}
	return FALSE;
}

// about dialog message handler
BOOL CALLBACK dlg2proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			case WM_DESTROY:
				{
					closedialog(wnd2->hWindow);
				}
			case IDOK:
				{
					closedialog(wnd2->hWindow);
				}break;
			}
		}break;
	}
	return FALSE;
}

// config dialog message handler
BOOL CALLBACK cfgproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		{
			bool ret = cfg->ReadIni();
			if(!ret)
			{
				closedialog(cfgwnd->hWindow);
				closedialog(wnd->hWindow);
				MessageBox(wnd2->hWindow, "Erose_LS.ini not found!", "Error", MB_ICONERROR | MB_OK);
				PostQuitMessage(0);
			}
			return TRUE;
		}break;
	case WM_COMMAND:
		{
			switch(wParam)
			{
			case IDOK:
				{
					cfg->WriteIni();
					closedialog(cfgwnd->hWindow);
				}break;
			}
		}break;
	}
	return FALSE;
}

int closedialog(HWND h) // close dialog code
{
	EndDialog(h, 0); // close the dialog
	return 0;
}