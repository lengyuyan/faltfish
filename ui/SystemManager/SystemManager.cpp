// SystemManager.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "SystemManager.h"
#include "SystemManagerDlg.h"

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

// CSystemManagerApp

BEGIN_MESSAGE_MAP(CSystemManagerApp, CWinApp)
END_MESSAGE_MAP()


// CSystemManagerApp ����

CSystemManagerApp::CSystemManagerApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSystemManagerApp ����

CSystemManagerApp theApp;


// CSystemManagerApp ��ʼ��

BOOL CSystemManagerApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

extern "C" void PASCAL EXPORT ShowSettingDebugDlg(int iLevels)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CSystemManagerDlg dlg;
    dlg.m_LoginLevels = iLevels;
    dlg.DoModal();
}