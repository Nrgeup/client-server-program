
// UDPServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUDPServerApp:
// �йش����ʵ�֣������ UDPServer.cpp
//

class CUDPServerApp : public CWinApp
{
public:
	CUDPServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CUDPServerApp theApp;