// SystemManager.h : SystemManager DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSystemManagerApp
// �йش���ʵ�ֵ���Ϣ������� SystemManager.cpp
//

class CSystemManagerApp : public CWinApp
{
public:
	CSystemManagerApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
