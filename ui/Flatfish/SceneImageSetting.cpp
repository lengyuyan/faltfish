// SceneImageSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Flatfish.h"
#include "SceneImageSetting.h"
#include "afxdialogex.h"
#include "iManagerCtrl.h"


// CSceneImageSetting �Ի���

IMPLEMENT_DYNAMIC(CSceneImageSetting, CDialogEx)

CSceneImageSetting::CSceneImageSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSceneImageSetting::IDD, pParent)
{
    m_iCurSceneID = 0;
    m_iCurIndex = 0;

}

CSceneImageSetting::~CSceneImageSetting()
{
}

void CSceneImageSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSceneImageSetting, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_SETTING, &CSceneImageSetting::OnBnClickedButtonSetting)
    ON_BN_CLICKED(IDC_BTN_LIGHTADD, &CSceneImageSetting::OnBnClickedBtnLightadd)
    ON_BN_CLICKED(IDC_BTN_LIGHTMINUS, &CSceneImageSetting::OnBnClickedBtnLightminus)
END_MESSAGE_MAP()


// CSceneImageSetting ��Ϣ�������

void CSceneImageSetting::UpdateUI(int iSceneID, int index)
{
    m_iCurSceneID = iSceneID;
    m_iCurIndex = index;
    CString strBuf;
    SCENE_PKG config;  //�������ļ��ж�ȡͼƬ�й�roi��Ϣ
    SetBackgroundColor(BACKGROUND_COLOR_M);
    GetSceneParam(iSceneID,&config);
    if (index >= config.vecInfo.size())
    {
        AfxMessageBox("����������Χ");
        return;
    }
    strBuf.Format("%d", config.vecInfo[index].CameraIndex);
    GetDlgItem(IDC_EDIT1)->SetWindowText(strBuf);
    strBuf.Format("%d", config.vecInfo[index].LightIndex);
    GetDlgItem(IDC_EDIT2)->SetWindowText(strBuf);
    strBuf.Format("%d", config.vecInfo[index].CameraShutter);
    GetDlgItem(IDC_EDIT_SHUTTER)->SetWindowText(strBuf);
    
}

void CSceneImageSetting::OnBnClickedButtonSetting()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString strBuf;
    SCENE_PKG config;  //�������ļ��ж�ȡͼƬ�й�roi��Ϣ
    GetSceneParam(m_iCurSceneID,&config);
    if (m_iCurIndex >= config.vecInfo.size())
    {
        AfxMessageBox("����������Χ");
        return;
    }
    GetDlgItem(IDC_EDIT1)->GetWindowText(strBuf);
    config.vecInfo[m_iCurIndex].CameraIndex = atoi(strBuf);
    GetDlgItem(IDC_EDIT2)->GetWindowText(strBuf);
    config.vecInfo[m_iCurIndex].LightIndex = atoi(strBuf);
    GetDlgItem(IDC_EDIT_SHUTTER)->GetWindowText(strBuf);
    config.vecInfo[m_iCurIndex].CameraShutter = atoi(strBuf);
    SetSceneParam(&config);
}

void CSceneImageSetting::SetDisplayWindowID(int winID)
{
    m_winID = winID;
}

BOOL CSceneImageSetting::PreTranslateMessage(MSG* pMsg)
{
    // TODO: �ڴ����ר�ô����/����û���

    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN ) 
    {
        CString strBuf;
        SCENE_PKG config;  //�������ļ��ж�ȡͼƬ�й�roi��Ϣ
        GetSceneParam(m_iCurSceneID,&config);

        if (m_iCurIndex >= config.vecInfo.size())
        {
            AfxMessageBox("����������Χ");
            return false;
        }
        if (pMsg->hwnd==GetDlgItem(IDC_EDIT1)->m_hWnd)
        {
            GetDlgItem(IDC_EDIT1)->GetWindowText(strBuf);
            config.vecInfo[m_iCurIndex].CameraIndex = atoi(strBuf);
        }
        else if (pMsg->hwnd==GetDlgItem(IDC_EDIT2)->m_hWnd)
        {
            GetDlgItem(IDC_EDIT2)->GetWindowText(strBuf);
            config.vecInfo[m_iCurIndex].LightIndex = atoi(strBuf);
        }
        else if (pMsg->hwnd==GetDlgItem(IDC_EDIT_SHUTTER)->m_hWnd)
        {
            int  iValue = 0;
            GetDlgItem(IDC_EDIT_SHUTTER)->GetWindowText(strBuf);
            iValue = atoi(strBuf.GetBuffer());
            if (iValue >4095 || m_iCurIndex >= config.imageCount)
            {
                return false;
            }
            config.vecInfo[m_iCurIndex].CameraShutter = iValue;
        }
        SetSceneParam(&config);
        return true;
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}


void CSceneImageSetting::OnBnClickedBtnLightadd()
{
        // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString str;
    SCENE_PKG config;
    int iValue = 0;
    GetSceneParam(m_iCurSceneID, &config);
    GetDlgItem(IDC_EDIT_SHUTTER)->GetWindowText(str);
    iValue = atoi(str.GetBuffer());
    iValue = iValue + 1;
    if (iValue >4095 || m_iCurIndex >= config.imageCount)
    {
        AfxMessageBox("δ֪�쳣");
        return;
    }
    if (!config.vecInfo.empty())
    {
        if (m_iCurIndex >= config.vecInfo.size())
        {
            AfxMessageBox("����������Χ");
            return;
        }
        config.vecInfo[m_iCurIndex].CameraShutter = iValue;
    }
    str.Format("%d",iValue);
    GetDlgItem(IDC_EDIT_SHUTTER)->SetWindowText(str);
    SetSceneParam(&config); 
}


void CSceneImageSetting::OnBnClickedBtnLightminus()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString str;
    SCENE_PKG config;
    int iValue = 0;
    GetSceneParam(m_iCurSceneID, &config);
    GetDlgItem(IDC_EDIT_SHUTTER)->GetWindowText(str);
    iValue = atoi(str.GetBuffer());
    iValue = iValue - 1;
    if (iValue >4095 || m_iCurIndex >= config.imageCount)
    {
        return;
    }
    int iPos = IFindParamData(&config, MODEL_PRO, m_iCurIndex);
    if (iPos >= 0)
    {
        if (m_iCurIndex >= config.vecInfo.size())
        {
            AfxMessageBox("����������Χ");
            return;
        }
        config.vecInfo[m_iCurIndex].CameraShutter = iValue;
    }
    str.Format("%d",iValue);
    GetDlgItem(IDC_EDIT_SHUTTER)->SetWindowText(str);
    SetSceneParam(&config);
}
