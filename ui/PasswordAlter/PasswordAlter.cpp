
// PasswordAlter.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "PasswordAlter.h"
#include "PasswordAlterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPasswordAlterApp

BEGIN_MESSAGE_MAP(CPasswordAlterApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPasswordAlterApp ����

CPasswordAlterApp::CPasswordAlterApp()
{
	// ֧����������������
	//m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CPasswordAlterApp ����

CPasswordAlterApp theApp;


// CPasswordAlterApp ��ʼ��


/*
BOOL CPasswordAlterApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CPasswordAlterDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}*/

BOOL CPasswordAlterApp::InitInstance()
{
    CWinApp::InitInstance();

    return TRUE;
}


extern "C" int PASCAL EXPORT outdlg(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    int iLevels = 0;
    CPasswordAlterDlg dlg;
    iLevels = dlg.DoModal();

    return iLevels;
}

extern "C" bool PASCAL EXPORT ShowModifyPWDlg(int iLevels)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CPasswordAlterDlg dlg;
    iLevels = dlg.DoModal();

    return true;
}