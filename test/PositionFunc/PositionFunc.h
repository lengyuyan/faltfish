// PositionFunc.h : PositionFunc DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPositionFuncApp
// �йش���ʵ�ֵ���Ϣ������� PositionFunc.cpp
//

class CPositionFuncApp : public CWinApp
{
public:
	CPositionFuncApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
