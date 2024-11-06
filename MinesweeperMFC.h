
// MinesweeperMFC.h : main header file for the MinesweeperMFC application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMinesweeperMFCApp:
// See MinesweeperMFC.cpp for the implementation of this class
//

class CMinesweeperMFCApp : public CWinApp
{
public:
	CMinesweeperMFCApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMinesweeperMFCApp theApp;
