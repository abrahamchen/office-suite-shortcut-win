
// SpyTool.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSpyToolApp:
// See SpyTool.cpp for the implementation of this class
//

class CSpyToolApp : public CWinApp
{
public:
	CSpyToolApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnMyHelp();

	DECLARE_MESSAGE_MAP()
};

extern CSpyToolApp theApp;