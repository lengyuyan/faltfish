// SysAxisToolRelation.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SystemManager.h"
#include "SysAxisToolRelation.h"
#include "afxdialogex.h"
#include "SystemManagerDlg.h"
#include "iManagerCtrl.h"
#include "DataType.h"
// SysAxisToolRelation �Ի���

IMPLEMENT_DYNAMIC(SysAxisToolRelation, CDialogEx)

SysAxisToolRelation::SysAxisToolRelation(CWnd* pParent /*=NULL*/)
	: CDialogEx(SysAxisToolRelation::IDD, pParent)
{

}

SysAxisToolRelation::~SysAxisToolRelation()
{
}

void SysAxisToolRelation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SYS_AXISTOOL_METRIX, m_AxisToolList);

}


BEGIN_MESSAGE_MAP(SysAxisToolRelation, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SYS_AXISTOOL_METRIX, &SysAxisToolRelation::OnLvnItemchangedSysAxistoolMetrix)
	ON_BN_CLICKED(IDC_SYSAXISTOOL_SAVE, &SysAxisToolRelation::OnBnClickedSysaxistoolSave)
	ON_NOTIFY(NM_DBLCLK, IDC_SYS_AXISTOOL_METRIX, &SysAxisToolRelation::OnNMDblclkSysAxistoolMetrix)
END_MESSAGE_MAP()


// SysAxisToolRelation ��Ϣ�������


void SysAxisToolRelation::OnLvnItemchangedSysAxistoolMetrix(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

BOOL SysAxisToolRelation::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	DWORD ExtSty = m_AxisToolList.GetExStyle();
	ExtSty = ExtSty|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT;
	m_AxisToolList.SetExtendedStyle(ExtSty);


	//��ʼ��ͷ
	RECT ListRect;
	m_AxisToolList.GetClientRect(&ListRect);
	m_AxisToolList.InsertColumn(0,"�����");
	m_AxisToolList.SetColumnWidth(0,ListRect.right/6);
	m_AxisToolList.InsertColumn(1,"Tool���");
	m_AxisToolList.SetColumnWidth(1,ListRect.right/6);
	m_AxisToolList.InsertColumn(2,"AinT_X");
	m_AxisToolList.SetColumnWidth(2,ListRect.right/6);
	m_AxisToolList.InsertColumn(3,"AinT_Y");
	m_AxisToolList.SetColumnWidth(3,ListRect.right/6);
	m_AxisToolList.InsertColumn(4,"X_Adjust");
	m_AxisToolList.SetColumnWidth(4,ListRect.right/6);
	m_AxisToolList.InsertColumn(5,"Y_Adjust");
	m_AxisToolList.SetColumnWidth(5,ListRect.right/6);

	pCommonEdit = NULL;

	//��ʾ����
	UpdataUI();


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


void SysAxisToolRelation::UpdataUI()
{
	  m_AxisToolList.DeleteAllItems();
	//CSystemManagerDlg* pParent = dynamic_cast<CSystemManagerDlg*>(GetParent());
	 //int ListRowIndex = 0;
	 SYSTEM_PARAM* pSysParam = ( SYSTEM_PARAM*)GetSysParamPoint();
	 char szText[64]={0};
	 for (int i = 0, ListRowIndex =0;i <pSysParam->iAxisNum;i++)
	 {
		 for (int j = 0;j<pSysParam->iToolNum;j++, ListRowIndex++)
		 {

			 sprintf_s(szText,"%d",ListRowIndex);
			 m_AxisToolList.InsertItem(ListRowIndex,szText);

			 sprintf_s(szText,"%d",i);
			 m_AxisToolList.SetItemText(ListRowIndex,0,szText);

			 sprintf_s(szText,"%d",j);
			 m_AxisToolList.SetItemText(ListRowIndex,1,szText);

			 sprintf_s(szText,"%lf",pSysParam->RotX[i][j]);
			 m_AxisToolList.SetItemText(ListRowIndex,2,szText);

			 sprintf_s(szText,"%lf",pSysParam->RotY[i][j]);
			 m_AxisToolList.SetItemText(ListRowIndex,3,szText);

			 sprintf_s(szText,"%lf",pSysParam->RotX_Adjust[i][j]);
			 m_AxisToolList.SetItemText(ListRowIndex,4,szText);

			 sprintf_s(szText,"%lf",pSysParam->RotY_Adjust[i][j]);
			 m_AxisToolList.SetItemText(ListRowIndex,5,szText);

		 }
	 }
}

void SysAxisToolRelation::OnBnClickedSysaxistoolSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SYSTEM_PARAM* pSysParam = ( SYSTEM_PARAM*)GetSysParamPoint();
	char szText[64]={0};
	int ToolNum = pSysParam->iToolNum;
	for (int i = 0;i <pSysParam->iAxisNum;i++)
	{
		for (int j = 0;j<ToolNum;j++)
		{
			m_AxisToolList.GetItemText(i*ToolNum+j,4,szText,sizeof(szText));
			pSysParam->RotX_Adjust[i][j] = atof(szText);

			m_AxisToolList.GetItemText(i*ToolNum+j,5,szText,sizeof(szText));
			pSysParam->RotY_Adjust[i][j] = atof(szText);

			//��������������浽�����ļ���
		    SaveSysRotAxleAdjust(i,j,pSysParam->RotX_Adjust[i][j],pSysParam->RotY_Adjust[i][j]);

		}
	}




}


void SysAxisToolRelation::OnNMDblclkSysAxistoolMetrix(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	m_ListRowIndex = pNMItemActivate->iItem;
	m_ListColIndex = pNMItemActivate->iSubItem;
	CRect SubRect;
	//�༭��Ӧ����������
	if (m_ListRowIndex>=0&&m_ListColIndex>=4)  //��0�е�ͨ�ų����Ų�������ģ���
	{
		//m_MapList.GetColumnWidth(ColIndex)  ;
		m_AxisToolList.GetSubItemRect(m_ListRowIndex,m_ListColIndex,0,SubRect);
		if (pCommonEdit)
		{
			delete	pCommonEdit;
			pCommonEdit = NULL;
		}

		pCommonEdit = new CEdit;
		pCommonEdit->Create(WS_CHILD|WS_VISIBLE|WS_BORDER,SubRect,&m_AxisToolList,COMMONEDITID);

		
		pCommonEdit->SetWindowTextA(m_AxisToolList.GetItemText(m_ListRowIndex,m_ListColIndex));

	}

	*pResult = 0;
}

void SysAxisToolRelation::OnOK()
{
	CString str;
	if (NULL == pCommonEdit)
	{
		return;
	}
	pCommonEdit->GetWindowTextA(str);
	m_AxisToolList.SetItemText(m_ListRowIndex,m_ListColIndex,str);
	//���趨�����ַ���
	int AxisIndex = atoi(m_AxisToolList.GetItemText(m_ListRowIndex,0));
	int ToolIndex = atoi(m_AxisToolList.GetItemText(m_ListRowIndex,1));

	//CString strName;

	SYSTEM_PARAM* pSysParam = (SYSTEM_PARAM*)GetSysParamPoint();
	pSysParam->RotX_Adjust[AxisIndex][ToolIndex] = atof(m_AxisToolList.GetItemText(m_ListRowIndex,4));
	pSysParam->RotY_Adjust[AxisIndex][ToolIndex] = atof(m_AxisToolList.GetItemText(m_ListRowIndex,5));
	
	delete	pCommonEdit;
	pCommonEdit = NULL;
	

	return;

	CDialogEx::OnOK();
}

