
// ImageIntefaceTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CImageIntefaceTestApp:
// �йش����ʵ�֣������ ImageIntefaceTest.cpp
//

class CImageIntefaceTestApp : public CWinApp
{
public:
	CImageIntefaceTestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CImageIntefaceTestApp theApp;