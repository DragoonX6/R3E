#pragma once

#include "stdafx.h"
#include "Window.h"

class Thread: Window
{
public:
	Thread(LPTHREAD_START_ROUTINE t)
	{
		m_hThread = CreateThread(0,0, t, 0, 0, &m_dwThreadId); // create a new thread and start our file creation process
		if(!m_hThread) // if something goes wrong
		{
			MessageBox(hWindow, "Could not create thread", "Error", MB_OK); // messagebox says all
			CloseHandle(m_hThread); // stop the process
		}
	}
	~Thread()
	{
		TerminateThread(m_hThread, dwTexitcode); //close the Thread
	}
private:
	DWORD m_dwThreadId;
	DWORD dwTexitcode;
	HANDLE m_hThread;
};