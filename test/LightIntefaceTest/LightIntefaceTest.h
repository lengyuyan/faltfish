
// LightIntefaceTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLightIntefaceTestApp:
// �йش����ʵ�֣������ LightIntefaceTest.cpp
//

class CLightIntefaceTestApp : public CWinApp
{
public:
	CLightIntefaceTestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLightIntefaceTestApp theApp;