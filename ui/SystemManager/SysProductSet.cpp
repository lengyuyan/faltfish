// SysProductSet.cpp : implementation file
//

#include "stdafx.h"
#include "SystemManager.h"
#include "SysProductSet.h"
#include "afxdialogex.h"
#include "SystemManagerDlg.h"

// SysProductSet dialog

IMPLEMENT_DYNAMIC(SysProductSet, CDialogEx)

SysProductSet::SysProductSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(SysProductSet::IDD, pParent)
{
	m_SelProductIndex = -1;
	m_SelMapIndex = -1;
}

SysProductSet::~SysProductSet()
{
}

void SysProductSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRODUCTSET_NAME, m_EProductName);
	DDX_Control(pDX, IDC_PRODUCTSET_LIST, m_LBNameList);
	DDX_Control(pDX, IDC_PRODUCTSET_SCENEMAP, m_MapList);
}


BEGIN_MESSAGE_MAP(SysProductSet, CDialogEx)
	ON_BN_CLICKED(IDC_PRODUCTSET_ADDPRODUCT, &SysProductSet::OnBnClickedProductsetAddproduct)
	ON_BN_CLICKED(IDC_PRODUCTSET_ADDMAP, &SysProductSet::OnBnClickedProductsetAddmap)
	ON_EN_CHANGE(IDC_PRODUCTSET_NAME, &SysProductSet::OnEnChangeProductsetName)
	ON_BN_CLICKED(IDC_PRODUCTSET_DELPRODUCT, &SysProductSet::OnBnClickedProductsetDelproduct)
	ON_LBN_SELCHANGE(IDC_PRODUCTSET_LIST, &SysProductSet::OnLbnSelchangeProductsetList)
	ON_BN_CLICKED(IDC_PRODUCTSET_SAVE, &SysProductSet::OnBnClickedProductsetSave)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_PRODUCTSET_SCENEMAP, &SysProductSet::OnLvnEndlabeleditProductsetScenemap)
	ON_NOTIFY(NM_DBLCLK, IDC_PRODUCTSET_SCENEMAP, &SysProductSet::OnNMDblclkProductsetScenemap)
	ON_NOTIFY(NM_CLICK, IDC_PRODUCTSET_SCENEMAP, &SysProductSet::OnNMClickProductsetScenemap)
	ON_NOTIFY(LVN_KEYDOWN, IDC_PRODUCTSET_SCENEMAP, &SysProductSet::OnLvnKeydownProductsetScenemap)
	ON_BN_CLICKED(IDC_PRODUCTSET_DELMAP, &SysProductSet::OnBnClickedProductsetDelmap)
END_MESSAGE_MAP()


// SysProductSet message handlers
BOOL SysProductSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	

	DWORD LdwStyle = m_MapList.GetExtendedStyle();
	LdwStyle =       m_MapList.GetExtendedStyle();
	LdwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	LdwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	m_MapList.SetExtendedStyle(LdwStyle); //������չ���
	//Listctrl->ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL);//����ĳһ��ѡ��


	//��ʼ����Ʒ�б�
	 //PMAPDATA ProductMap;
	 CSystemManagerDlg* pParent = dynamic_cast<CSystemManagerDlg*>(GetParent());
	 pParent->GetSysProductMap(&m_ProductMap);
	 PMAPDATA::iterator it;
	 for (it = m_ProductMap.begin();it!=m_ProductMap.end();it++)
	 {
		m_LBNameList.AddString((it->first).c_str());

	 }

	//��ʼ��ӳ���б�
	RECT MapRect;
	m_MapList.GetClientRect(&MapRect);
	m_MapList.InsertColumn(0,"ͨ�ų�����");
	m_MapList.SetColumnWidth(0,MapRect.right/2);
	m_MapList.InsertColumn(1,"��Ʒʵ�ʳ�����");
	m_MapList.SetColumnWidth(1,MapRect.right/2);

	pCommonEdit = NULL;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}



void SysProductSet::UpdateMapList(void)
{
	PMAPDATA::iterator it;
	SCENEMAP::iterator mapIt;
	char szSceneNo[256]={0};
	char szMapNo[256]={0};
	CString ProductName;

	m_LBNameList.GetText(m_SelProductIndex,ProductName);
	it = m_ProductMap.find(ProductName.GetBuffer());
	if (it != m_ProductMap.end())
	{
		//����ѡ�еĲ�Ʒ��MAP״̬
		m_MapList.DeleteAllItems();
		int i = 0;
		for (mapIt = it->second.begin();mapIt!=it->second.end();mapIt++,i++)
		{
			memset(szSceneNo,0,sizeof(szSceneNo));
			memset(szMapNo,0,sizeof(szMapNo));
			sprintf_s(szSceneNo,"%d",mapIt->first);
			sprintf_s(szMapNo,"%d",mapIt->second);
			m_MapList.InsertItem(i,szSceneNo,0);
			m_MapList.SetItemText(i,0,szSceneNo);
			m_MapList.SetItemText(i,1,szMapNo);

		}
	}else  
	{
		//����������Ӳ�Ʒ����MAP��ʾ�հס�
		m_MapList.DeleteAllItems();
	}


}


//���Ӳ�Ʒ
void SysProductSet::OnBnClickedProductsetAddproduct()
{
	// TODO: Add your control notification handler code here
	//m_LBNameList.AddString(m_ProductName);
	char szText[256] ={0};
	sprintf_s(szText,"�Ƿ�����²�Ʒ:%s",m_ProductName);
	if (0 >= m_ProductName.GetLength())
	{
		AfxMessageBox("��Ʒ���ֲ���Ϊ��");
		return;
	}
	if(0 < m_LBNameList.FindString(0,m_ProductName))
	{
		AfxMessageBox("��Ʒ�����ظ�������������");
		return;
	}


	if( 1 == MessageBoxA(szText,m_ProductName,MB_OKCANCEL))
	{
		m_LBNameList.InsertString(++m_SelProductIndex,m_ProductName);
		//m_SelProductIndex = m_LBNameList.GetCount()-1;
		m_LBNameList.SetCurSel(m_SelProductIndex);	

		UpdateMapList();	
	}

}

//����ӳ��
void SysProductSet::OnBnClickedProductsetAddmap()
{
	// TODO: Add your control notification handler code here

	PMAPDATA::iterator it;
	SCENEMAP::iterator mapIt;
	SCENEMAP PSceneMap;

	char szSceneNo[256]={0};
	//char szMapNo[256]={0};
	if(m_SelProductIndex<0)
		return;

	memset(szSceneNo,0,sizeof(szSceneNo));
	//memset(szMapNo,0,sizeof(szMapNo));
	int CurNo = m_MapList.GetItemCount();

	sprintf_s(szSceneNo,"%d",CurNo);
	//sprintf_s(szMapNo,"%d",mapIt->second);
	m_MapList.InsertItem(CurNo,szSceneNo,0);
	m_MapList.SetItemText(CurNo,0,szSceneNo);
	//int m_MapList.GetItemText(CurNo,)
	m_MapList.SetItemText(CurNo,1,"-1");

	//���µ�map���ݷ��뵽��Ʒ��ӳ�������С�
	CString ProductName;
	//m_LBNameList.GetWindowTextA(ProductName);


	m_LBNameList.GetText(m_SelProductIndex,ProductName);

	int strIndex = m_LBNameList.FindString(0,ProductName);
	
	it = m_ProductMap.find(ProductName.GetBuffer());
	if (it == m_ProductMap.end())
	{
		//û�иò�Ʒ��ӳ�䣬������ӳ��
		if (strIndex >= 0)
		{
			PSceneMap.insert(make_pair(CurNo,-1));
		    m_ProductMap.insert(make_pair(ProductName.GetBuffer(),PSceneMap));
		}

	}else 
	{
		//�Ѿ��иò�Ʒ��ӳ�䣬����ӳ�������
		it->second.insert(make_pair(CurNo,-1));

	}
	UpdateMapList();

}


void SysProductSet::OnEnChangeProductsetName()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	m_EProductName.GetWindowTextA(m_ProductName);

}


void SysProductSet::OnBnClickedProductsetDelproduct()
{
	// TODO: Add your control notification handler code here
	CString strName;
	PMAPDATA::iterator it;
	if (m_SelProductIndex > -1)
	{
		m_LBNameList.GetText(m_SelProductIndex,strName);
		m_LBNameList.DeleteString(m_SelProductIndex);

		it = m_ProductMap.find(strName.GetBuffer());
		if (it!= m_ProductMap.end())
		{
			m_ProductMap.erase(m_ProductMap.find(strName.GetBuffer()));

		}
	}

	if (m_SelProductIndex>m_LBNameList.GetCount()-1)
	{
		m_SelProductIndex = m_LBNameList.GetCount()-1;
	
	}

    //�����趨����
	if (m_SelProductIndex > -1)
	{
		m_LBNameList.SetCurSel(m_SelProductIndex);
	}

	UpdateMapList();


}


void SysProductSet::OnLbnSelchangeProductsetList()
{
	// TODO: Add your control notification handler code here
	CSystemManagerDlg* pParent = dynamic_cast<CSystemManagerDlg*>(GetParent());
	CString strName;
	 m_SelProductIndex = m_LBNameList.GetCurSel();
	 UpdateMapList();

	 //����ѡ�в�Ʒ
	 m_LBNameList.GetText(m_SelProductIndex,strName);
	 pParent->SetCurProductName(strName.GetBuffer());
	

}


void SysProductSet::OnBnClickedProductsetSave()
{
	// TODO: Add your control notification handler code here
	CSystemManagerDlg* pParent = dynamic_cast<CSystemManagerDlg*>(GetParent());

	//�������
	//PMAPDATA ProductMap;

	//���ò���
	pParent->SetSysProductMap(&m_ProductMap);

	CDialogEx::OnOK();

}


void SysProductSet::OnLvnEndlabeleditProductsetScenemap(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������




	*pResult = 0;
}


void SysProductSet::OnNMDblclkProductsetScenemap(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_MapRowIndex = pNMItemActivate->iItem;
	m_MapColIndex = pNMItemActivate->iSubItem;
	CRect SubRect;
	//�༭��Ӧ����������
	if (m_MapRowIndex>=0&&m_MapColIndex>0)  //��0�е�ͨ�ų����Ų�������ģ���
	{
		//m_MapList.GetColumnWidth(ColIndex)  ;
		m_MapList.GetSubItemRect(m_MapRowIndex,m_MapColIndex,0,SubRect);
		if (pCommonEdit)
		{
			delete	pCommonEdit;
			pCommonEdit = NULL;
		}

	    pCommonEdit = new CEdit;
		pCommonEdit->Create(WS_CHILD|WS_VISIBLE|WS_BORDER,SubRect,&m_MapList,COMMONEDITID);

	}

	*pResult = 0;
}


void SysProductSet::OnNMClickProductsetScenemap(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
     m_SelMapIndex = pNMItemActivate->iItem;


	*pResult = 0;
}


void SysProductSet::OnLvnKeydownProductsetScenemap(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	WORD vkKeydown =  pLVKeyDow->wVKey;
	if (VK_RETURN == vkKeydown||VK_ESCAPE == vkKeydown)
	{
	    delete	pCommonEdit;

	}

	*pResult = 0;
}

 void SysProductSet::OnOK()
{
	 CString str;
	 if (NULL == pCommonEdit)
	 {
		 return;
	 }
	 pCommonEdit->GetWindowTextA(str);
	 m_MapList.SetItemText(m_MapRowIndex,m_MapColIndex,str);
	 //���趨�����ַ���ӳ����
	 CString strName;
	 PMAPDATA::iterator it;
	 if (m_SelProductIndex > -1)
	 {
		 m_LBNameList.GetText(m_SelProductIndex,strName);
		 it = m_ProductMap.find(strName.GetBuffer());
		 if (it!= m_ProductMap.end())
		 {
			it->second.find(m_MapRowIndex)->second = atoi(str);
		     
		 }
	 }
	 delete	pCommonEdit;
	 pCommonEdit = NULL;


	return;

	CDialogEx::OnOK();
}


 void SysProductSet::OnBnClickedProductsetDelmap()
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 PMAPDATA::iterator it;
	 SCENEMAP::iterator mapIt;
	 SCENEMAP PSceneMap;

	 char szSceneNo[256]={0};
	 //char szMapNo[256]={0};
	 if(m_SelProductIndex<0)
		 return;

	 memset(szSceneNo,0,sizeof(szSceneNo));
	 //memset(szMapNo,0,sizeof(szMapNo));
	 int CurNo = m_MapList.GetItemCount();
	 if (CurNo<1)
	 {
		 return;
	 }

	 //sprintf_s(szSceneNo,"%d",CurNo);
	 //m_MapList.DeleteItem(CurNo-1);


	 //sprintf_s(szMapNo,"%d",mapIt->second);
	// m_MapList.InsertItem(CurNo,szSceneNo,0);
	// m_MapList.SetItemText(CurNo,0,szSceneNo);
	 //int m_MapList.GetItemText(CurNo,)
	// m_MapList.SetItemText(CurNo,1,"-1");

	 //���µ�map���ݷ��뵽��Ʒ��ӳ�������С�
	 CString ProductName;
	 //m_LBNameList.GetWindowTextA(ProductName);


	 m_LBNameList.GetText(m_SelProductIndex,ProductName);

	 int strIndex = m_LBNameList.FindString(0,ProductName);

	 it = m_ProductMap.find(ProductName.GetBuffer());
	 if (it == m_ProductMap.end())
	 {
		 //û�иò�Ʒ��ӳ�䣬��ʲô��������
		 return;		

	 }else 
	 {
		 //�Ѿ��иò�Ʒ��ӳ�䣬���ȥ���һ��ӳ��

		 it->second.erase(CurNo-1);

	 }
	 UpdateMapList();

 }
