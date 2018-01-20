// SelectFunction.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SceneManager.h"
#include "SelectFunction.h"
#include "afxdialogex.h"

#include "iManagerCtrl.h"

// CSelectFunction �Ի���

IMPLEMENT_DYNAMIC(CSelectFunction, CDialogEx)

CSelectFunction::CSelectFunction(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectFunction::IDD, pParent)
{
    m_iCurSceneID = 0;
	m_ItemChange = FALSE;
}

CSelectFunction::~CSelectFunction()
{
}

void CSelectFunction::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCENEFUNC_TREE, m_FunTree);//ͼ������һ����


}


BEGIN_MESSAGE_MAP(CSelectFunction, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_IMAGE, &CSelectFunction::OnBnClickedButtonImage)
    ON_BN_CLICKED(IDC_BUTTON_MODEL, &CSelectFunction::OnBnClickedButtonModel)
    ON_BN_CLICKED(IDC_BUTTON_THRESHOLD, &CSelectFunction::OnBnClickedButtonThreshold)
    ON_BN_CLICKED(IDC_BUTTON_FIND_LINE, &CSelectFunction::OnBnClickedButtonFindLine)
    ON_BN_CLICKED(IDC_BUTTON_FIND_CIRCLE, &CSelectFunction::OnBnClickedButtonFindCircle)
    ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON_MORE, &CSelectFunction::OnBnClickedButtonMore)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_DBLCLK, IDC_SCENEFUNC_TREE, &CSelectFunction::OnNMDblclkScenefuncTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_SCENEFUNC_TREE, &CSelectFunction::OnTvnSelchangedScenefuncTree)
	ON_BN_CLICKED(IDC_SCENEFUN_POSITION, &CSelectFunction::OnBnClickedScenefunPosition)
END_MESSAGE_MAP()


// CSelectFunction ��Ϣ�������

BOOL CSelectFunction::OnInitDialog()
{
	CDialogEx::OnInitDialog();	
	// TODO:  Add extra initialization here
	CRect rt;
	GetClientRect(&rt);
	m_FunTree.MoveWindow(rt.left+5,rt.top+5,rt.right-10,rt.bottom-10);
	HTREEITEM TreeI1 =	m_FunTree.InsertItem(STR_FUN_IMAGE_READ);//,TVI_ROOT,TVI_LAST
	//m_FunTree.InsertItem("ͼ�����",0 ,TreeI1);	
	m_FunTree.InsertItem(STR_FUN_IMAGE_ACQ);
	m_FunTree.InsertItem(STR_FUN_IMAGE_ENHANCEMENT);
	
	m_FunTree.InsertItem(STR_FUN_MATCH);
	m_FunTree.InsertItem(STR_CONTOUR_POSITION);
	m_FunTree.InsertItem(STR_HALPROC_POSITION);
	m_FunTree.InsertItem(STR_HALPROC_PROCESS);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CSelectFunction::OnBnClickedButtonImage()//ͼ��ɼ�
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    NotifyAddFunction((LPARAM)MAIN_PRO);
}

void CSelectFunction::UpdateUI(int iSceneID, int index)
{
    SetBackgroundColor(BACKGROUND_COLOR_M);
    m_iCurSceneID = iSceneID;
	m_CurProcIndex = index; //�°���ûʲô��


	SCENE_PKG pSceneConfig;
	GetSceneParam(iSceneID,&pSceneConfig);
	CRect rt;
	GetClientRect(&rt);
	//m_FunTree.Create(WS_CHILD|WS_VISIBLE,rt,this,IDC_SCENEFUNC_TREE);

	if ( 0 == pSceneConfig.ProcProcess )
	{
		GetDlgItem(IDC_BUTTON_IMAGE)->ShowWindow(FALSE);//��Щ���Ǿɵ�
		GetDlgItem(IDC_BUTTON_MODEL)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THRESHOLD)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FIND_LINE)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FIND_CIRCLE)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MORE)->ShowWindow(FALSE);
		GetDlgItem(IDC_SCENEFUN_POSITION)->ShowWindow(FALSE);
		

		//�����µĽ���way2
		//m_FunTree.Create(WS_CHILD|WS_VISIBLE,rt,this,IDC_SCENEFUNC_TREE);
		
		m_FunTree.ShowWindow(TRUE);

	}else
	{
		int Itemleft = rt.left + 5;
		int Itemright  = rt.right - 5;
		int UseTop  = rt.top +5;
		int ItemTop = UseTop;
		int UseBottom = rt.bottom -5;
		int ItemInterval = (UseBottom - UseTop)/6;
		int ItemWidth = Itemright - Itemleft;
		int ItemHeight = ItemInterval - 10;
		m_FunTree.ShowWindow(FALSE);
	    GetDlgItem(IDC_BUTTON_IMAGE)->MoveWindow(Itemleft,ItemTop,ItemWidth,ItemHeight);
		ItemTop +=ItemInterval;
	    GetDlgItem(IDC_BUTTON_MODEL)->MoveWindow(Itemleft,ItemTop,ItemWidth,ItemHeight);
		ItemTop +=ItemInterval;
		GetDlgItem(IDC_BUTTON_THRESHOLD)->MoveWindow(Itemleft,ItemTop,ItemWidth,ItemHeight);
		ItemTop +=ItemInterval;
		GetDlgItem(IDC_SCENEFUN_POSITION)->MoveWindow(Itemleft,ItemTop,ItemWidth,ItemHeight);
		ItemTop +=ItemInterval;
		GetDlgItem(IDC_BUTTON_FIND_LINE)->MoveWindow(Itemleft,ItemTop,ItemWidth,ItemHeight);
		ItemTop +=ItemInterval;
		GetDlgItem(IDC_BUTTON_FIND_CIRCLE)->MoveWindow(Itemleft,ItemTop,ItemWidth,ItemHeight);
		ItemTop +=ItemInterval;
		GetDlgItem(IDC_BUTTON_MORE)->MoveWindow(Itemleft,ItemTop,ItemWidth,ItemHeight);
		ItemTop +=ItemInterval;

	}
	
}

void CSelectFunction::OnBnClickedButtonModel()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    NotifyAddFunction((LPARAM)MODEL_PRO);
}

void CSelectFunction::OnBnClickedScenefunPosition()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NotifyAddFunction((LPARAM)CONTOUR_POSITION);
}


void CSelectFunction::OnBnClickedButtonThreshold()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    NotifyAddFunction((LPARAM)THRE_PRO);
}


void CSelectFunction::OnBnClickedButtonFindLine()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    NotifyAddFunction((LPARAM)LINE_PRO);
}


void CSelectFunction::OnBnClickedButtonFindCircle()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    NotifyAddFunction((LPARAM)CIRCLE_PRO);
}


void CSelectFunction::NotifyAddFunction(LPARAM iType)
{
    CString strSceneName;
    SYSTEM_PARAM config;
    GetSysParam(&config);
    HWND hwnd = ::FindWindowA(NULL,config.pstrSceneName[m_iCurSceneID].c_str());
    if (NULL != hwnd)
    {
        //GetDlgItem(id)->GetWindowText(m_strTemp);
         ::SendMessageW(hwnd,ADD_FUNCTION_MSG,NULL,(LPARAM)(iType));
        CSelectFunction::OnOK();
    }
}

void CSelectFunction::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    CDialogEx::OnActivate(nState, pWndOther, bMinimized);

    // TODO: �ڴ˴������Ϣ����������
    if(nState == WA_INACTIVE)
    {
        ShowWindow(SW_HIDE);
    }
}


void CSelectFunction::OnBnClickedButtonMore()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NotifyAddFunction((LPARAM)MORE_PRO);
}


void CSelectFunction::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
	vector<VIRSION_DATA_PKG *>::iterator it;
	SCENE_PKG pConfig;				//�������ļ��ж�ȡͼƬ�й�roi��Ϣ
	GetSceneParam(m_iCurSceneID, &pConfig);
	VIRSION_DATA_PKG *pTmp = NULL;
	//���û���ð�ť
	for (it = pConfig.vecBody.begin(); it != pConfig.vecBody.end(); it++)
	{
		pTmp = *it;
		if ((pTmp->iValue % 10000)/100 == MORE_PRO)
		{
			GetDlgItem(IDC_BUTTON_MORE)->EnableWindow(FALSE);
			break;
		}
		else
		{
			GetDlgItem(IDC_BUTTON_MORE)->EnableWindow(TRUE);
		}
	}
}


void CSelectFunction::OnNMDblclkScenefuncTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	 
	 VisionProcBase* pProcObj = NULL;
	 if (STR_FUN_IMAGE_READ == m_ToolItemStr)
	 {
		// pProcObj = new;
	 } 
	 else if (STR_FUN_IMAGE_ACQ == m_ToolItemStr)
	 {
		   NotifyAddFunction((LPARAM)IMAGE_ACQISITION);
	 } 
	 else if (STR_FUN_IMAGE_ENHANCEMENT == m_ToolItemStr)
	 {
		 NotifyAddFunction((LPARAM)IMAGE_PREPROCESS);
	 } 	 
	 else if(STR_FUN_MATCH == m_ToolItemStr)
	 {
		   NotifyAddFunction((LPARAM)MODEL_PRO);

	 }else if (STR_CONTOUR_POSITION == m_ToolItemStr)
	 {
		   NotifyAddFunction((LPARAM)CONTOUR_POSITION);
	 }else if (STR_HALPROC_POSITION == m_ToolItemStr )
	 {
		 NotifyAddFunction((LPARAM)HALPROC_POSITIONG);

	 } else if (STR_HALPROC_PROCESS == m_ToolItemStr )
	 {
		 NotifyAddFunction((LPARAM)HALPROC_PROCESS);

	 }


	*pResult = 0;
}


void CSelectFunction::OnTvnSelchangedScenefuncTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_ToolItemStr = m_FunTree.GetItemText(pNMTreeView->itemNew.hItem);//ѡ�����ʲô�ַ�
	m_ItemChange = TRUE;

	*pResult = 0;
}



