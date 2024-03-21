
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		


class CSerialCommuincationApp : public CWinApp
{
public:
	CSerialCommuincationApp();

public:
	virtual BOOL InitInstance();


	DECLARE_MESSAGE_MAP()
};

extern CSerialCommuincationApp theApp;
