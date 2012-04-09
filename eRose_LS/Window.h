#pragma once

#include "stdafx.h"

extern class Window *wnd;
extern class Window *wnd2;
extern class Window *cfgwnd;

class Window
{
public:
	Window()
	{
		hWindow = NULL;
		ghInstance = NULL;
		CmdShow = 0;
		listset = false;
		tabctr = false;
		editsset = false;
		configset = false;
		tie.mask = TCIF_TEXT | TCIF_IMAGE;
		tie.iImage = -1;
		tie.pszText = "Connector";
		tie2.mask = TCIF_TEXT | TCIF_IMAGE;
		tie2.iImage = -1;
		tie2.pszText = "Worldserver";
		tie3.mask = TCIF_TEXT | TCIF_IMAGE;
		tie3.iImage = -1;
		tie3.pszText = "Blocked IP";
	}
	~Window()
	{
	}

	HWND hWindow;
	HINSTANCE ghInstance;
	int CmdShow;
	bool listset;
	bool tabctr;
	bool editsset;
	bool configset;
	TCITEM tie;
	TCITEM tie2;
	TCITEM tie3;
};