// SetSceneDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SceneManager.h"
#include "SetSceneDlg.h"
#include "afxdialogex.h"
#include "DataType.h"


// CSetSceneDlg �Ի���

IMPLEMENT_DYNAMIC(CSetSceneDlg, CDialogEx)

CSetSceneDlg::CSetSceneDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetSceneDlg::IDD, pParent)
{
    m_iCurSceneID = 0;
    m_pButton = NULL;
}

CSetSceneDlg::~CSetSceneDlg()
{
    if (NULL != m_pButton)
    {
        delete []m_pButton;
        m_pButton = NULL;
    }
}

void CSetSceneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetSceneDlg, CDialogEx)
	
    ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CSetSceneDlg ��Ϣ�������


BOOL CSetSceneDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch(pMsg->message)
	{
	case WM_LBUTTONUP:
		{
			UINT id = GetWindowLong(pMsg->hwnd, GWL_ID);
			if (id >= BTN_ID && id < (BTN_ID+ SCENE_SIZE - 1))
			{

				SYSTEM_PARAM *config = NULL;
				config = (SYSTEM_PARAM *)GetSysParamPoint();
				CString strTest = config->pstrSceneName[m_iCurSceneID].c_str();
				HWND hwnd = ::FindWindowA(NULL,config->pstrSceneName[m_iCurSceneID].c_str());

				if (NULL == hwnd)
				{
					return FALSE;
				}
				m_iCurSceneID = id - BTN_ID - 1;
				//�������Դ��ڷ����л���Ϣ
				::SendMessageW(hwnd,SET_SCENE,NULL,(LPARAM)(m_iCurSceneID)/*m_strTemp.GetBuffer()*/);
				CSetSceneDlg::OnOK();
			}
		}
		break;
	case WM_MOUSEMOVE:
		if (m_tooltip.GetSafeHwnd() != NULL)
		{
			m_tooltip.RelayEvent(pMsg);
		}
		break;
    case WA_INACTIVE:
    case WM_KILLFOCUS:
    case WM_MOUSEACTIVATE:
    //case WM_NCMOUSEMOVE:
    case WM_CAPTURECHANGED:
        {
            ShowWindow(SW_HIDE);
        }
	default:
		break;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CSetSceneDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_tooltip.Create(this);  // ����ToolTipCtrl
	m_tooltip.SetTipBkColor(RGB(128, 255, 255)); // ������ʾ�ı�������ɫ
	m_tooltip.SetTipTextColor(RGB(255, 0, 0)); // ������ʾ�ı�������ɫ
	m_tooltip.Activate(TRUE);  // ����ToolTipCtrl

	int  iNum = 0;
    int  iRow = 0;
    int  iCol = 0;
	int  xb = 0;
	int  yb = 0;
	int  xe = 0;
	int  ye = 0;
	int  iWidth = 0;
	int  iHeight = 0;

    SetBackgroundColor(BACKGROUND_COLOR_M);
	int m = 0;
	m_iIdBegin = BTN_ID;//������ťʱ����ָ����ťidΪ3000

	SYSTEM_PARAM *config = NULL;
	config = (SYSTEM_PARAM *)GetSysParamPoint();
	iNum =config->iSceneNum +1;
	iCol = (int)sqrt((float)iNum); //����ǰ�ø����ͣ� ������Ϊintȡ��
	if (iNum > iCol*iCol)
	{
		iCol++;
    }
    iRow = iCol - (iCol * iCol - iNum)/iCol;
	CRect rt;
	CRect rtPic;
	GetClientRect(rt);
	
	
	iWidth = ((rt.Width() - 5)/iCol)-5;
	iHeight = ((rt.Height() - 5)/iRow)-5;
    int iscene = 0;

	SYSTEM_PARAM *sysPara = NULL;
	sysPara = (SYSTEM_PARAM *)GetSysParamPoint();
    m_pButton = new CButton[iNum];
	for (int i = 0; i < iRow; i++)
	{
		
		for ( int j = 0; j < iCol; j++)
		{
			m++;
			if (m > iNum)
			{
				break;
			}
			xb = j*iWidth + 10;
			yb = i*iHeight + 10;
			xe = (j+1)*iWidth;
			ye = (i+1)*iHeight;
			
			m_iHeight = ye+10;
			m_iWidth = rt.Width();

			m_iIdBegin = m_iIdBegin+1;
			CString str;
			
			if (m == iNum)
			{
				str = "�½�����";
				//m_iIdBegin =  ?  //�Ƿ���Ҫ�趨һ��������ID��ʶ���½�������

			}else
			{
			   str = sysPara->pstrSceneName[iscene].c_str();
			}
			m_pButton[iscene].Create(str,WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,CRect(xb,yb,xe,ye), this,m_iIdBegin);
			m_tooltip.AddTool(GetDlgItem(m_iIdBegin), str);
			iscene++;
		}
		if (m < 2)
		{
			GetDlgItem(BTN_ID+1)->MoveWindow(5,5,m_iWidth/3,m_iHeight/3);
			MoveWindow(0,0,m_iWidth/3+15,m_iHeight/3+15);
		}
		else
		{
			MoveWindow(0,0,m_iWidth,m_iHeight);
		}
	}




	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CSetSceneDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    CDialogEx::OnActivate(nState, pWndOther, bMinimized);
    if(nState == WA_INACTIVE)
    {
        ShowWindow(SW_HIDE);
    }
    // TODO: �ڴ˴������Ϣ����������
}
