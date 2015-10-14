
// trick17.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Ctrick17App:
// See trick17.cpp for the implementation of this class
//

class Ctrick17App : public CWinApp
{
public:
	Ctrick17App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Ctrick17App theApp;