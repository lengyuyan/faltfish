// Login.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "Login.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CLoginApp

BEGIN_MESSAGE_MAP(CLoginApp, CWinApp)
END_MESSAGE_MAP()


// CLoginApp ����

CLoginApp::CLoginApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CLoginApp ����

CLoginApp theApp;


// CLoginApp ��ʼ��

BOOL CLoginApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


extern "C" int PASCAL EXPORT ShowLoginDlg(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    int iLevels = 0;
    CLoginDlg dlg;
    iLevels = dlg.DoModal();

    return iLevels;
}

extern "C" bool PASCAL EXPORT ShowModifyPWDlg(int iLevels)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CLoginDlg dlg;
    iLevels = dlg.DoModal();

    return true;
}