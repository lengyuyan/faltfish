// PasswordAlter.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPasswordAlterApp:
// �йش����ʵ�֣������ PasswordAlter.cpp
//

class CPasswordAlterApp : public CWinApp
{
public:
	CPasswordAlterApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

//extern CPasswordAlterApp theApp;