// Login.h : Login DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLoginApp
// �йش���ʵ�ֵ���Ϣ������� Login.cpp
//

class CLoginApp : public CWinApp
{
public:
	CLoginApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
