// SceneThresholdSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Flatfish.h"
#include "SceneThresholdSetting.h"
#include "afxdialogex.h"
#include "iManagerCtrl.h"

#define  EDIT_ITEM_NAME(X) IDC_EDIT##X


// CSceneThresholdSetting �Ի���

IMPLEMENT_DYNAMIC(CSceneThresholdSetting, CDialogEx)

CSceneThresholdSetting::CSceneThresholdSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSceneThresholdSetting::IDD, pParent)
{
    m_iCurSceneID = 0;
    m_iCurIndex = 0;
}

CSceneThresholdSetting::~CSceneThresholdSetting()
{
}

void CSceneThresholdSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSceneThresholdSetting, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_SETTING, &CSceneThresholdSetting::OnBnClickedButtonSetting)
    ON_BN_CLICKED(IDC_BUTTON_ADD, &CSceneThresholdSetting::OnBnClickedButtonAdd)
    ON_BN_CLICKED(IDC_BUTTON_SUB, &CSceneThresholdSetting::OnBnClickedButtonSub)
END_MESSAGE_MAP()


// CSceneThresholdSetting ��Ϣ�������

void CSceneThresholdSetting::UpdateUI(int iSceneID, int index)
{
    char szBuf[BUF_SIZE] = {0};
    int iCount = 0;
    SCENE_PKG config;  //�������ļ��ж�ȡͼƬ�й�roi��Ϣ
    GetSceneParam(iSceneID,&config);
    m_iCurSceneID = iSceneID;
    m_iCurIndex = index;

    SetBackgroundColor(BACKGROUND_COLOR_M);
    memset(szBuf, 0, BUF_SIZE);
    int iPos = IFindParamData(&config, THRE_PRO, m_iCurIndex);
    if (iPos >= 0)
    {
        int index = FindSpecTypePos(config.vecBody[iPos], config.vecDataType[iPos], THRE_TYPE);
        iCount = config.vecBody[iPos][index].iValue;
    }
    sprintf(szBuf, "%d", iCount);
    GetDlgItem(IDC_EDIT1)->SetWindowText(szBuf);
    
}

void CSceneThresholdSetting::OnBnClickedButtonSetting()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    SCENE_PKG config;  //�������ļ��ж�ȡͼƬ�й�roi��Ϣ
    GetSceneParam(m_iCurSceneID,&config);

    int iPos = IFindParamData(&config, THRE_PRO, m_iCurIndex);

    CString strBuf;
    GetDlgItem(IDC_EDIT1)->GetWindowText(strBuf);
    if (iPos >= 0)
    {
        int index = FindSpecTypePos(config.vecBody[iPos], config.vecDataType[iPos], THRE_TYPE);
        config.vecBody[iPos][index].iValue = atoi(strBuf);
    }
    SetSceneParam(&config);
}

void CSceneThresholdSetting::SetDisplayWindowID(int winID)
{
    m_winID = winID;
}

void CSceneThresholdSetting::OnBnClickedButtonAdd()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString str;
    SCENE_PKG config;
    GetDlgItem(IDC_EDIT1)->GetWindowText(str);
    int iValue = atoi(str);
    iValue++;
    if (iValue >255 || iValue < 0 || m_iCurIndex >= SCENE_SIZE)
    {
        return;
    }
    str.Format("%d",iValue);
    GetDlgItem(IDC_EDIT1)->SetWindowText(str);
    GetSceneParam(m_iCurSceneID, &config);
    int iPos = IFindParamData(&config, THRE_PRO, m_iCurIndex);
    if (iPos >= 0)
    {
        int index = FindSpecTypePos(config.vecBody[iPos], config.vecDataType[iPos], THRE_TYPE);
        config.vecBody[iPos][index].iValue = iValue;
    }
    SetSceneParam(&config);
    VisionDebugThreshold(m_winID, m_iCurSceneID, m_iCurIndex);
}


void CSceneThresholdSetting::OnBnClickedButtonSub()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString str;
    SCENE_PKG config;
    GetDlgItem(IDC_EDIT1)->GetWindowText(str);
    int iValue = atoi(str);
    iValue--;
    if (iValue >255 || iValue < 0 || m_iCurIndex >= SCENE_SIZE)
    {
        return;
    }
    str.Format("%d",iValue);
    GetDlgItem(IDC_EDIT1)->SetWindowText(str);
    GetSceneParam(m_iCurSceneID, &config);
    int iPos = IFindParamData(&config, THRE_PRO, m_iCurIndex);
    if (iPos >= 0)
    {
        int index = FindSpecTypePos(config.vecBody[iPos], config.vecDataType[iPos], THRE_TYPE);
        config.vecBody[iPos][index].iValue = iValue;
    }
    SetSceneParam(&config);
    VisionDebugThreshold(m_winID, m_iCurSceneID, m_iCurIndex);
}


BOOL CSceneThresholdSetting::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN ) 
	{
        if (pMsg->hwnd==GetDlgItem(IDC_EDIT1)->m_hWnd)
        {
            CString str;
            SCENE_PKG config;
            GetDlgItemText(IDC_EDIT1,str);
            GetSceneParam(m_iCurSceneID, &config);
            int iPos = IFindParamData(&config, THRE_PRO, m_iCurIndex);
            if (iPos >= 0)
            {
                int index = FindSpecTypePos(config.vecBody[iPos], config.vecDataType[iPos], THRE_TYPE);
                config.vecBody[iPos][index].iValue = atoi(str);
            }
            SetSceneParam(&config);
            VisionDebugThreshold(m_winID, m_iCurSceneID, m_iCurIndex);
            return true;
        }

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
