
// FlatFish.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "FlatFish.h"
#include "FlatFishDlg.h"
//#include "DeviceInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFlatFishApp

BEGIN_MESSAGE_MAP(CFlatFishApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

#define FLATFISH_APP_ONLY_ONE "Flatfish Application version 2.0"

// CFlatFishApp ����

CFlatFishApp::CFlatFishApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CFlatFishApp ����

CFlatFishApp theApp;


// CFlatFishApp ��ʼ��

BOOL CFlatFishApp::InitInstance()
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
    /*_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);*/
    //_crtBreakAlloc = 1592;

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
	SetRegistryKey(_T("Flatfish"));
	init_common();
    HANDLE OnlyOne = OpenMutex(MUTEX_ALL_ACCESS, FALSE, FLATFISH_APP_ONLY_ONE); ////��ʵ������
    if(NULL == OnlyOne)
    {
        CreateMutex(NULL, FALSE, FLATFISH_APP_ONLY_ONE);
    }
    else
    {
        LOG_ERR("flatfish already run...");
		uninit_common();
        exit(0);
    }
	

    INT_PTR nResponse = IDOK;
    if (CheckLicenseValid())
    {
        CFlatFishDlg dlg;
        m_pMainWnd = &dlg;
        nResponse = dlg.DoModal();
    }
    else
    {
        ;
    }
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

	 uninit_common();

	return FALSE;
}


bool CFlatFishApp::CheckLicenseValid()
{
    char szPath[256] = {0};
    return true;
}