// LightManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LightManagerDlg.h"
#include "afxdialogex.h"


// LightManagerDlg �Ի���

IMPLEMENT_DYNAMIC(LightManagerDlg, CDialogEx)

LightManagerDlg::LightManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(LightManagerDlg::IDD, pParent)
{

}

LightManagerDlg::~LightManagerDlg()
{
}

void LightManagerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_CTRL_INDEX, m_CommList);
}


BEGIN_MESSAGE_MAP(LightManagerDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_OP_LIGHT, &LightManagerDlg::OnBnClickedButtonOpLight)
END_MESSAGE_MAP()


// LightManagerDlg ��Ϣ�������


void LightManagerDlg::OnBnClickedButtonOpLight()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
}


BOOL LightManagerDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    InitLightComBox();

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void LightManagerDlg::InitLightComBox()
{
    HKEY hKey; 
    int  index = 0;
    if(::RegOpenKeyEx( HKEY_LOCAL_MACHINE, "Hardware\\DeviceMap\\SerialComm", 
        NULL,
        KEY_READ,
        &hKey ) == ERROR_SUCCESS ) //�򿪴���ע����Ӧ�ļ�ֵ 
    {
        int i=0; 
        char portName[256],commName[256];
        DWORD dwLong,dwSize;
        while(1)
        { 
            dwLong = dwSize = sizeof(portName); 
            if( ::RegEnumValue( hKey, i, portName, &dwLong,NULL,NULL, (PUCHAR)commName, &dwSize ) == ERROR_NO_MORE_ITEMS )// ö�ٴ��� 
            {
                break; 
            }

            m_CommList.AddString( commName ); // commName���Ǵ������� 

            i++; 
        } 

        if( m_CommList.GetCount() == 0 ) 
        { 
            //::AfxMessageBox("����ö��ʧ��!!!"); 

        } 
        RegCloseKey(hKey);
        m_CommList.SetCurSel(0);
    } 
}