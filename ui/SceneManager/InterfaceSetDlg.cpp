// InterfaceSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SceneManager.h"
#include "InterfaceSetDlg.h"
#include "afxdialogex.h"
#include "iManagerCtrl.h"


// CInterfaceSetDlg �Ի���

IMPLEMENT_DYNAMIC(CInterfaceSetDlg, CDialogEx)

CInterfaceSetDlg::CInterfaceSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInterfaceSetDlg::IDD, pParent)
{
	m_staticBtnType		= new CStatic;
	m_staticBtnName		= new CStatic;
	m_staticItemType	= new CStatic;
	m_staticItemNumb	= new CStatic;
	m_staticInitValue	= new CStatic;
	m_staticChangeRate	= new CStatic;
	m_staticOptionNumb	= new CStatic;
	m_cboBtnType		= new CComboBox;
	m_cboItemType		= new CComboBox;
	m_cboOptionNumb		= new CComboBox;
	m_editBtnName		= new CEdit;
	m_editItemNumb		= new CEdit;
	m_editInitValue		= new CEdit;
	m_changeRate1		= new CButton;
	m_changeRate2		= new CButton;
	m_changeRate3		= new CButton;
	m_iCurSceneID		= 0;
	m_nItem				= 0;
	m_nCIndex			= 0;
	m_iT				= 0;
	m_staticOption1		= new CStatic;
	m_staticOption2		= new CStatic;
	m_staticOption3		= new CStatic;
	m_staticOption4		= new CStatic;
	m_staticOption5		= new CStatic;
	m_staticOption6		= new CStatic;
	m_staticOption7		= new CStatic;
	m_staticOption8		= new CStatic;
	m_editOption1		= new CEdit;
	m_editOption2		= new CEdit;
	m_editOption3		= new CEdit;
	m_editOption4		= new CEdit;
	m_editOption5		= new CEdit;
	m_editOption6		= new CEdit;
	m_editOption7		= new CEdit;
	m_editOption8		= new CEdit;
}

CInterfaceSetDlg::~CInterfaceSetDlg()
{
	delete m_staticBtnType;
	delete m_staticBtnName;
	delete m_staticItemType;
	delete m_staticItemNumb;
	delete m_staticInitValue;
	delete m_staticChangeRate;
	delete m_staticOptionNumb;
	delete m_cboBtnType;
	delete m_cboItemType;
	delete m_cboOptionNumb;
	delete m_editBtnName;
	delete m_editItemNumb;
	delete m_editInitValue;
	delete m_changeRate1;
	delete m_changeRate2;
	delete m_changeRate3;
	delete m_staticOption1;
	delete m_staticOption2;
	delete m_staticOption3;
	delete m_staticOption4;
	delete m_staticOption5;
	delete m_staticOption6;
	delete m_staticOption7;
	delete m_staticOption8;
	delete m_editOption1;
	delete m_editOption2;
	delete m_editOption3;
	delete m_editOption4;
	delete m_editOption5;
	delete m_editOption6;
	delete m_editOption7;
	delete m_editOption8;
}

void CInterfaceSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInterfaceSetDlg, CDialogEx)
	ON_WM_PARENTNOTIFY()
	ON_CBN_SELCHANGE(IDC_COMBO_BTN_TYPE, &CInterfaceSetDlg::OnSelchangeComboBtnType)
	ON_CBN_SELCHANGE(IDC_COMBO_ITEM_TYPE, &CInterfaceSetDlg::OnSelchangeComboRoiType)
	ON_EN_SETFOCUS(IDC_EDIT_BTN_NAME, &CInterfaceSetDlg::OnSetfocusEditBtnName)
	ON_CBN_SELCHANGE(IDC_COMBO_OPTION_NUMB, &CInterfaceSetDlg::OnSelchangeComboOptionNumb)
END_MESSAGE_MAP()


// CInterfaceSetDlg ��Ϣ�������


BOOL CInterfaceSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	init_btn_info();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CInterfaceSetDlg::init_btn_info(void)
{
	//�������
	int top_x = 0;
	int top_y = 0;
	int end_x = 80;
	int end_y = 20;
	int height = 40;
	int width = 80;
	
	//��ѡ��
	m_staticBtnType->Create("�ؼ����ͣ�", WS_CHILD | WS_VISIBLE | SS_LEFT,
		CRect(top_x, top_y, end_x, end_y), this, IDC_STATIC_BTN_TYPE);
	top_x = end_x;
	end_x += width + 20;
	m_cboBtnType->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_NOINTEGRALHEIGHT, 
		CRect(top_x, top_y, end_x, end_y + 80), this, IDC_COMBO_BTN_TYPE);
	top_y += height;
	end_y += height;
	top_x = 0;
	end_x = width;

	m_staticItemNumb->Create("Item��ţ�", WS_CHILD | WS_VISIBLE | SS_LEFT,
		CRect(top_x, top_y, end_x, end_y), this, IDC_STATIC_ITEM_NUMB);
	top_x = end_x;
	end_x += width + 20;
	m_editItemNumb->Create(ES_MULTILINE | ES_READONLY | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, 
		CRect(top_x, top_y, end_x, end_y), this, IDC_EDIT_ITEM_NUMB);
	top_y += height;
	end_y += height;
	top_x = 0;
	end_x = width;

	m_staticBtnName->Create("�ؼ�����", WS_CHILD | WS_VISIBLE | SS_LEFT,
		CRect(top_x, top_y, end_x, end_y), this, IDC_STATIC_BTN_NAME);
	top_x = end_x;
	end_x += width + 20;
	m_editBtnName->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, 
		CRect(top_x, top_y, end_x, end_y), this, IDC_EDIT_BTN_NAME);
	m_editBtnName->SetLimitText(12);
	top_y += height;
	end_y += height;
	top_x = 0;
	end_x = width;


	//��ѡ��
	//SEBtn
    int tmp_height = top_y;
    int tmp_width = end_x;

	m_staticItemType->Create("Item���ͣ�", WS_CHILD | WS_VISIBLE | SS_LEFT,
		CRect(top_x, top_y, end_x, end_y), this, IDC_STATIC_ITEM_TYPE);
	top_x = end_x;
	end_x += width + 20;
	m_cboItemType->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_NOINTEGRALHEIGHT, 
		CRect(top_x, top_y, end_x, end_y + 80), this, IDC_COMBO_ITEM_TYPE);
	top_y += height;
	end_y += height;
	top_x = 0;
	end_x = width;

	m_staticInitValue->Create("��ʼֵ��", WS_CHILD | WS_VISIBLE | SS_LEFT,
		CRect(top_x, top_y, end_x, end_y), this, IDC_STATIC_INIT_VALUE);
	top_x = end_x;
	end_x += width + 20;
	m_editInitValue->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, 
		CRect(top_x, top_y, end_x, end_y), this, IDC_EDIT_INIT_VALUE);
	m_editInitValue->SetLimitText(10);
	top_y += height;
	end_y += height;
	top_x = 0;
	end_x = width;

	m_staticChangeRate->Create("�仯�ʣ�", WS_CHILD | WS_VISIBLE | SS_LEFT,
		CRect(top_x, top_y, end_x, end_y), this, IDC_STATIC_CHANGE_RATE);
	top_x = end_x;
	end_x += width + 20;
	m_changeRate1->Create("0.01", WS_CHILD | WS_VISIBLE | WS_GROUP | BS_AUTORADIOBUTTON, 
		CRect(top_x, top_y, end_x, end_y), this, IDC_BTN_CHANGE_RATE1);
	top_y += height;
	end_y += height;
	m_changeRate2->Create("0.1", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 
		CRect(top_x, top_y, end_x, end_y), this, IDC_BTN_CHANGE_RATE2);
	top_y += height;
	end_y += height;
	m_changeRate3->Create("1", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 
		CRect(top_x, top_y, end_x, end_y), this, IDC_BTN_CHANGE_RATE3);
	top_y += height;
	end_y += height;
	top_x = 0;
	end_x = width;

	//SRBtn
	top_y = tmp_height;
	end_x = tmp_width;
	end_y = top_y + height;

	m_staticOptionNumb->Create("ѡ�������", WS_CHILD | WS_VISIBLE | SS_LEFT,
		CRect(top_x, top_y, end_x, end_y), this, IDC_STATIC_OPTION_NUMB);
	top_x = end_x;
	end_x += width + 20;
	m_cboOptionNumb->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_NOINTEGRALHEIGHT, 
		CRect(top_x, top_y, end_x, end_y + 80), this, IDC_COMBO_OPTION_NUMB);
	top_y += height;
	end_y += height;
	top_x = 0;
	end_x = width;

	end_x = top_x + 30;
	end_y = top_y + 20;
	m_staticOption1->Create("1��", WS_CHILD | WS_VISIBLE | SS_LEFT,
		CRect(top_x, top_y, end_x, end_y), this, IDC_STATIC_OPTION_NUMB);
	top_x = end_x;
	end_x += 60;
	m_editOption1->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, 
		CRect(top_x, top_y, end_x, end_y), this, IDC_EDIT_INIT_VALUE);
	m_editOption1->SetLimitText(6);
	top_x = end_x + 5;
	end_x = top_x + 30;
	m_staticOption2->Create("2��", WS_CHILD | WS_VISIBLE | SS_LEFT,
		CRect(top_x, top_y, end_x, end_y), this, IDC_STATIC_OPTION_NUMB);
	top_x = end_x;
	end_x += 60;
	m_editOption2->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, 
		CRect(top_x, top_y, end_x, end_y), this, IDC_EDIT_INIT_VALUE);
	m_editOption2->SetLimitText(6);
	top_y += height;
	end_y += height;
	top_x = 0;
	end_x = top_x + 30;

	m_staticOption3->Create("3��", WS_CHILD | WS_VISIBLE | SS_LEFT,
		CRect(top_x, top_y, end_x, end_y), this, IDC_STATIC_OPTION_NUMB);
	top_x = end_x;
	end_x += 60;
	m_editOption3->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, 
		CRect(top_x, top_y, end_x, end_y), this, IDC_EDIT_INIT_VALUE);
	m_editOption3->SetLimitText(6);
	top_x = end_x + 5;
	end_x = top_x + 30;
	m_staticOption4->Create("4��", WS_CHILD | WS_VISIBLE | SS_LEFT,
		CRect(top_x, top_y, end_x, end_y), this, IDC_STATIC_OPTION_NUMB);
	top_x = end_x;
	end_x += 60;
	m_editOption4->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, 
		CRect(top_x, top_y, end_x, end_y), this, IDC_EDIT_INIT_VALUE);
	m_editOption4->SetLimitText(6);
	top_y += height;
	end_y += height;
	top_x = 0;
	end_x = top_x + 30;

	m_staticOption5->Create("5��", WS_CHILD | WS_VISIBLE | SS_LEFT,
		CRect(top_x, top_y, end_x, end_y), this, IDC_STATIC_OPTION_NUMB);
	top_x = end_x;
	end_x += 60;
	m_editOption5->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, 
		CRect(top_x, top_y, end_x, end_y), this, IDC_EDIT_INIT_VALUE);
	m_editOption5->SetLimitText(6);
	top_x = end_x + 5;
	end_x = top_x + 30;
	m_staticOption6->Create("6��", WS_CHILD | WS_VISIBLE | SS_LEFT,
		CRect(top_x, top_y, end_x, end_y), this, IDC_STATIC_OPTION_NUMB);
	top_x = end_x;
	end_x += 60;
	m_editOption6->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, 
		CRect(top_x, top_y, end_x, end_y), this, IDC_EDIT_INIT_VALUE);
	m_editOption6->SetLimitText(6);
	top_y += height;
	end_y += height;
	top_x = 0;
	end_x = top_x + 30;

	m_staticOption7->Create("7��", WS_CHILD | WS_VISIBLE | SS_LEFT,
		CRect(top_x, top_y, end_x, end_y), this, IDC_STATIC_OPTION_NUMB);
	top_x = end_x;
	end_x += 60;
	m_editOption7->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, 
		CRect(top_x, top_y, end_x, end_y), this, IDC_EDIT_INIT_VALUE);
	m_editOption7->SetLimitText(6);
	top_x = end_x + 5;
	end_x = top_x + 30;
	m_staticOption8->Create("8��", WS_CHILD | WS_VISIBLE | SS_LEFT,
		CRect(top_x, top_y, end_x, end_y), this, IDC_STATIC_OPTION_NUMB);
	top_x = end_x;
	end_x += 60;
	m_editOption8->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, 
		CRect(top_x, top_y, end_x, end_y), this, IDC_EDIT_INIT_VALUE);
	m_editOption8->SetLimitText(6);
	top_y += height;
	end_y += height;
	top_x = 0;
	end_x = width;

	//��ʼ������
	UpdataUI();
	
	return;
}


void CInterfaceSetDlg::OnSelchangeComboBtnType()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdataUI(m_cboBtnType->GetCurSel());
}


void CInterfaceSetDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	CString str;
	TCHAR szBuf[1024];
	LVITEM lvi;
	lvi.iSubItem = 0;
	lvi.mask = LVIF_TEXT;
	lvi.pszText = szBuf;
	lvi.cchTextMax = 1024;

	//���ԭ������
    m_interfaceSetData.clear();

	//��ȡ����
	m_cboBtnType->GetLBText(m_cboBtnType->GetCurSel(), str);
	m_interfaceSetData.BtnType = str;
	
	m_editItemNumb->GetWindowText(str);
	m_interfaceSetData.ItemNumb = atoi(str);
	
	switch (m_cboBtnType->GetCurSel())
	{
	case 0:
		m_cboItemType->GetWindowText(str);
		if (!strcmp("int", str))
		{
			m_interfaceSetData.ItemType = INT_TYPE;
		} 
		else if (!strcmp("double", str))
		{
			m_interfaceSetData.ItemType = DOUBLE_TYPE;
		}
		m_editBtnName->GetWindowText(str);
		m_interfaceSetData.BtnName = str;
		m_editInitValue->GetWindowText(str);
		m_interfaceSetData.InitValue = atof(str);
		if (m_changeRate1->GetCheck())
		{
			m_changeRate1->GetWindowText(str);
		} 
		else if (m_changeRate2->GetCheck())
		{
			m_changeRate2->GetWindowText(str);
		}
		else if (m_changeRate3->GetCheck())
		{
			m_changeRate3->GetWindowText(str);
		}
		m_interfaceSetData.ChangeRate = atof(str);
		break;
    case 1:
        m_editBtnName->GetWindowText(str);
        m_interfaceSetData.BtnName = str;
		m_cboOptionNumb->GetWindowText(str);
		m_interfaceSetData.OptionNumb = atoi(str);
		for(int i = 0; i < 8; ++i)
		{
			switch (i)
			{
			case 0:
				m_editOption1->GetWindowText(str);
				break;
			case 1:
				m_editOption2->GetWindowText(str);
				break;
			case 2:
				m_editOption3->GetWindowText(str);
				break;
			case 3:
				m_editOption4->GetWindowText(str);
				break;
			case 4:
				m_editOption5->GetWindowText(str);
				break;
			case 5:
				m_editOption6->GetWindowText(str);
				break;
			case 6:
				m_editOption7->GetWindowText(str);
				break;
			case 7:
				m_editOption8->GetWindowText(str);
				break;
			}
			strcpy_s(m_interfaceSetData.OptionName[i], 16,str);
		}
		break;
	}

	CDialogEx::OnOK();
}

void CInterfaceSetDlg::UpdataUI( int index /*= 0*/ )
{
	CString str;
	int tmp_index = 0;

	//����UI�ؼ�
	m_cboBtnType->ResetContent();
	m_cboItemType->ResetContent();
	m_cboOptionNumb->ResetContent();

	m_cboBtnType->AddString("SEBtn");
	m_cboBtnType->AddString("SRBtn");

	SCENE_PKG config;  //�������ļ��ж�ȡͼƬ�й�roi��Ϣ
	GetSceneParam(m_iCurSceneID, &config);

	int it = 0;
	int iT = 0;
	int iType = 0;
	m_nItem = 0;
	//m_nCIndex = 0;

	for (int i = 0; i < config.vecBody.size(); ++i)
	{
		it = config.vecBody[i][0].iValue;
		iT = it % 10000;
		iType = iT / 100;
		if (m_iT == iT)
		{
			m_nItem = config.vecBody[i][1].iValue;
			break;
		}
	}

	for (int i = 0; i <= m_nItem; ++i)
	{
	str.Format("%d", i);
	m_editItemNumb->SetWindowText(str);
	}

	switch (index)
	{
	case 0:
		{
			m_cboItemType->AddString("double");
			m_cboItemType->AddString("int");
			showWindowSEBtn(SW_SHOW);
			showWindowSRBtn(SW_HIDE);
		}
		break;
	case 1:
		{
			for (int i = 0; i < 8; ++i)
			{
				str.Format("%d", i + 1);
				m_cboOptionNumb->AddString(str);
			}
			m_editOption1->SetWindowText("name1");
			m_editOption2->SetWindowText("name2");
			m_editOption3->SetWindowText("name3");
			m_editOption4->SetWindowText("name4");
			m_editOption5->SetWindowText("name5");
			m_editOption6->SetWindowText("name6");
			m_editOption7->SetWindowText("name7");
			m_editOption8->SetWindowText("name8");

			m_editOption1->EnableWindow(TRUE);
			m_editOption2->EnableWindow(FALSE);
			m_editOption3->EnableWindow(FALSE);
			m_editOption4->EnableWindow(FALSE);
			m_editOption5->EnableWindow(FALSE);
			m_editOption6->EnableWindow(FALSE);
			m_editOption7->EnableWindow(FALSE);
			m_editOption8->EnableWindow(FALSE);

			showWindowSRBtn(SW_SHOW);
			showWindowSEBtn(SW_HIDE);
		}
		break;
	}

	//����Ĭ��ѡ��
	m_cboBtnType->SetCurSel(index);
	m_cboItemType->SetCurSel(0);
	m_cboOptionNumb->SetCurSel(0);
	m_editBtnName->SetWindowText("������ؼ���");
	m_editInitValue->SetWindowText("0");
	m_changeRate1->SetCheck(BST_UNCHECKED);
	m_changeRate2->SetCheck(BST_CHECKED);
	m_changeRate3->SetCheck(BST_UNCHECKED);
}

INTERFACE_SET_DATA CInterfaceSetDlg::showWindow( CInterfaceSetDlg &m_InterfaceSet, int iSceneID, int iT, int nCIndex /*= 0*/ )
{
	m_iCurSceneID = iSceneID;
	m_iT = iT;
	m_nCIndex = nCIndex;
	m_InterfaceSet.DoModal();

	return m_interfaceSetData;
}


void CInterfaceSetDlg::OnCancel()
{
    // TODO: �ڴ����ר�ô����/����û���
    m_interfaceSetData.clear();

	CDialogEx::OnCancel();
}


void CInterfaceSetDlg::showWindowSEBtn( int index )
{
	m_staticItemType->ShowWindow(index);
	m_cboItemType->ShowWindow(index);
	m_staticInitValue->ShowWindow(index);
	m_editInitValue->ShowWindow(index);
	m_staticChangeRate->ShowWindow(index);
	m_changeRate1->ShowWindow(index);
	m_changeRate2->ShowWindow(index);
	m_changeRate3->ShowWindow(index);
}

void CInterfaceSetDlg::showWindowSRBtn( int index )
{
	m_staticOptionNumb->ShowWindow(index);
	m_cboOptionNumb->ShowWindow(index);
	m_staticOption1->ShowWindow(index);
	m_staticOption2->ShowWindow(index);
	m_staticOption3->ShowWindow(index);
	m_staticOption4->ShowWindow(index);
	m_staticOption5->ShowWindow(index);
	m_staticOption6->ShowWindow(index);
	m_staticOption7->ShowWindow(index);
	m_staticOption8->ShowWindow(index);
	m_editOption1->ShowWindow(index);
	m_editOption2->ShowWindow(index);
	m_editOption3->ShowWindow(index);
	m_editOption4->ShowWindow(index);
	m_editOption5->ShowWindow(index);
	m_editOption6->ShowWindow(index);
	m_editOption7->ShowWindow(index);
	m_editOption8->ShowWindow(index);
}


void CInterfaceSetDlg::OnSelchangeComboRoiType()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	m_cboItemType->GetWindowText(str);
	if (!strcmp("double", str))
	{
		m_changeRate1->ShowWindow(SW_SHOW);
		m_changeRate2->SetWindowText("0.1");
		m_changeRate3->SetWindowText("1");
	}
	else if (!strcmp("int", str))
	{
		m_changeRate1->ShowWindow(SW_HIDE);
		m_changeRate2->SetWindowText("1");
		m_changeRate3->SetWindowText("10");
	}
	if (m_changeRate1->GetCheck())
	{
		m_changeRate1->SetCheck(BST_UNCHECKED);
		m_changeRate2->SetCheck(BST_CHECKED);
		m_changeRate3->SetCheck(BST_UNCHECKED);
	}
	m_editBtnName->GetWindowText(str);
	m_editBtnName->SetFocus();
	m_editBtnName->SetSel(0, str.GetLength());
}


void CInterfaceSetDlg::OnSetfocusEditBtnName()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	m_editBtnName->GetWindowText(str);
	m_editBtnName->SetFocus();
	m_editBtnName->SetSel(0, str.GetLength());
}


void CInterfaceSetDlg::OnSelchangeComboOptionNumb()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str = "";
	m_cboOptionNumb->GetWindowText(str);
	m_editOption1->EnableWindow(FALSE);
	m_editOption2->EnableWindow(FALSE);
	m_editOption3->EnableWindow(FALSE);
	m_editOption4->EnableWindow(FALSE);
	m_editOption5->EnableWindow(FALSE);
	m_editOption6->EnableWindow(FALSE);
	m_editOption7->EnableWindow(FALSE);
	m_editOption8->EnableWindow(FALSE);
	switch (atoi(str))
	{
	case 8:
		m_editOption8->EnableWindow(TRUE);
	case 7:
		m_editOption7->EnableWindow(TRUE);
	case 6:
		m_editOption6->EnableWindow(TRUE);
	case 5:
		m_editOption5->EnableWindow(TRUE);
	case 4:
		m_editOption4->EnableWindow(TRUE);
	case 3:
		m_editOption3->EnableWindow(TRUE);
	case 2:
		m_editOption2->EnableWindow(TRUE);
	case 1:
		m_editOption1->EnableWindow(TRUE);
	}
}


