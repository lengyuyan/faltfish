// SetSceneDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Flatfish.h"
#include "SetPicindexDlg.h"
#include "afxdialogex.h"
#include "DataType.h"


// CSetSceneDlg �Ի���

IMPLEMENT_DYNAMIC(CSetPicindexDlg, CDialogEx)

	CSetPicindexDlg::CSetPicindexDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetPicindexDlg::IDD, pParent)
{
	m_iscene = 0;
}

CSetPicindexDlg::~CSetPicindexDlg()
{
}

void CSetPicindexDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetPicindexDlg, CDialogEx)
	ON_MESSAGE(SET_SCENE, &CSetPicindexDlg::OnSetScene)
END_MESSAGE_MAP()


// CSetSceneDlg ��Ϣ�������


BOOL CSetPicindexDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	switch(pMsg->message)
	{
	case WM_LBUTTONUP:
		{
			UINT id = GetWindowLong(pMsg->hwnd, GWL_ID);
			if (id >= PIC_ID && id < (PIC_ID+10))
			{

				HWND hwnd = ::FindWindowA(NULL,"SceneDlg");
				if (NULL == hwnd)
				{
					return FALSE;
				}
				//GetDlgItem(id)->GetWindowText(m_strTemp);
				::SendMessageW(hwnd,SET_PICINDEX,NULL,(LPARAM)(id - PIC_ID - 1));
				CSetPicindexDlg::OnOK();
			}

		}
		break;
	default:
		break;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CSetPicindexDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED);//ȥ��������

	int  iNum = 0;
    int  iRow = 0;
    int  iCol = 0;
	int  xb = 0;
	int  yb = 0;
	int  xe = 0;
	int  ye = 0;
	int  iWidth = 0;
	int  iHeight = 0;

	int iscene = 0;
	int m = 0;
	m_iIdBegin = PIC_ID;
	SCENE_CONFIG config;
	GetSceneParam(m_iscene,&config);
	iNum =config.iCountImage;
	iCol = sqrt((float)iNum);
	if (iNum > iCol*iCol)
	{
		iCol++;
    }
    iRow = iCol - (iCol * iCol - iNum)/iCol;
	CRect rt;
	CRect rtPic;
	GetClientRect(rt);


    SYSTEM_PARAM sysPara;
    GetSysParam(&sysPara);
	iWidth = ((rt.Width() - 5)/iCol)-5;
	iHeight = ((rt.Height() - 5)/iRow)-5;
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
			// ����pic�ؼ�
			m_iIdBegin = m_iIdBegin+1;
			m_pButton = new CButton();
			CString str;
            str = sysPara.pstrSceneName[iscene].c_str();
			//str.Format("%d",iscene);
			m_pButton->Create(str,WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,CRect(xb,yb,xe,ye), this,m_iIdBegin);
			iscene++;
			
		}
		if (m < 2)
		{
			GetDlgItem(PIC_ID+1)->MoveWindow(5,5,m_iWidth/3,m_iHeight/3);
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


LRESULT CSetPicindexDlg::OnSetScene( WPARAM wParam, LPARAM lParam )//�����ı���Ϣ
{
	m_iscene = (int)lParam;
	return 1;
}