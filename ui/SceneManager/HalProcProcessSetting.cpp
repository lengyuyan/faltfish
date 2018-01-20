// HalProcProcessSetting.cpp : implementation file
//

#include "stdafx.h"
#include "SceneManager.h"
#include "HalProcProcessSetting.h"
#include "HalProcProcess.h"
#include "afxdialogex.h"
#include "common.h"

// HalProcPosSetting dialog

IMPLEMENT_DYNAMIC(HalProcProcessSetting, CDialogEx)

HalProcProcessSetting::HalProcProcessSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(HalProcProcessSetting::IDD, pParent)
{
	
	m_pContainer = NULL;
	m_ProcProcess = 0;
	m_error = 0;
	
}

HalProcProcessSetting::~HalProcProcessSetting()
{
}

void HalProcProcessSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);



	DDX_Control(pDX, IDC_LIST_INPUTPARAM, m_L_INPUT);
	DDX_Control(pDX, IDC_LIST_OUTPARAM, m_list_outparam);
}


BEGIN_MESSAGE_MAP(HalProcProcessSetting, CDialogEx)

	ON_BN_CLICKED(IDC_ADDINPUT, &HalProcProcessSetting::OnBnClickedAddinput)
	ON_BN_CLICKED(IDC_DELINPUT, &HalProcProcessSetting::OnBnClickedDelinput)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_INPUTPARAM, &HalProcProcessSetting::OnLvnItemchangedListInputparam)
	ON_NOTIFY(NM_CLICK, IDC_LIST_INPUTPARAM, &HalProcProcessSetting::OnNMClickListInputparam)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_INPUTPARAM, &HalProcProcessSetting::OnNMDblclkListInputparam)

	ON_BN_CLICKED(IDC_PROCSETTINGP, &HalProcProcessSetting::OnBnClickedProcsettingp)
	ON_BN_CLICKED(IDC_LOADPROC, &HalProcProcessSetting::OnBnClickedLoadproc)
	ON_BN_CLICKED(IDC_PROCTESTP, &HalProcProcessSetting::OnBnClickedProctestp)
END_MESSAGE_MAP()


// HalProcPosSetting message handlers
void HalProcProcessSetting::GetContainer(void* pContainer)
{
	m_pContainer = pContainer;

}

void  HalProcProcessSetting::UpdateUI(void)
{

	//������Ϣ
	CString mess = hmessage.c_str();
	char err[10] = {0};
	itoa(m_error,err,10);
	mess += " Ecode:";
	mess+=err;
	GetDlgItem(IDC_HALPROC_PROCPMESSAGE)->SetWindowText(mess);
	//��ʾHalProcedure·��
	GetDlgItem(IDC_HALPROC_PROCPATHP)->SetWindowText(m_ProcdurePath);

}

void HalProcProcessSetting::CloseSettingDlg()
{
	OnOK();
}








void HalProcProcessSetting::SaveProcConfig(CString ConfigPath,int ProcIndex)
{
	char szValue[256]={0};
	char szName[256] ={0};
	CString ProcIndexName,strTemp;
	ProcIndexName.Format("%d",ProcIndex);

	//�������ļ��Ĳ�����������ù���ӿڲ�����Ϊ�˼򻯴�������ֱ�������������

	//�������������ƶ���

	sprintf_s(szValue,"%d",m_Inputnum);
	WritePrivateProfileString(ProcIndexName, "Inputnum",szValue, ConfigPath);
	sprintf_s(szValue,"%d",m_Isdisplay);
	WritePrivateProfileString(ProcIndexName, "Isdisplay",szValue, ConfigPath);

	//����������ֵ
	for (int i = 0 ;i<m_Inputnum;i++)
	{
		sprintf_s(szName,"InputParam%d",i);
		sprintf_s(szValue,"%0.12lf",m_InParam[i]);
		WritePrivateProfileString(ProcIndexName,szName,szValue, ConfigPath);
	}

	//��ŵ��ù���·��������
	sprintf_s(szName,"HalProcedurePath");
	WritePrivateProfileString(ProcIndexName, szName,m_ProcdurePath, ConfigPath);

}

void HalProcProcessSetting::ReadProcConfig(CString ConfigPath,int ProcIndex)
{
	char szValue[256]={0};
	char szName[256] ={0};
	CString ProcIndexName,strTemp;
	ProcIndexName.Format("%d",ProcIndex);
	double inputtemp =0.0;

	m_Inputnum = 0;
	m_Isdisplay = 0;
	m_InParam.clear();

	//��ȡ�����������
	m_Inputnum= GetPrivateProfileInt(ProcIndexName,"Inputnum",0,ConfigPath);
	//��ȡ�Ƿ���ʾ�ڴ���
	m_Isdisplay = GetPrivateProfileInt(ProcIndexName,"Isdisplay",0,ConfigPath);


	//��ȡ�������ֵ
	for (int i = 0 ;i<m_Inputnum;i++)
	{
		sprintf_s(szName,"InputParam%d",i);
		GetPrivateProfileString(ProcIndexName,szName,"-1.0",szValue,sizeof(szValue),ConfigPath);
		inputtemp = atof(szValue);
		m_InParam.push_back(inputtemp);

	}
	//��ȡ���ù���·��������
	sprintf_s(szName,"HalProcedurePath");
	GetPrivateProfileString(ProcIndexName,szName,"",szValue,sizeof(szValue),ConfigPath);
	m_ProcdurePath = szValue;

}

BOOL HalProcProcessSetting::VisionProcRun(CHECK_RESULT& ProcRes)
{
	//�����Ӿ����еĺ���������X,Y,phi
	CString strTemp;
	bool res = false;
	int i =0;
	m_OutParam.clear();
	try
	{
		res = ((HalProcProcess*)m_pContainer)->CommonProcess(m_ProcdurePath.GetString(),m_InParam,m_OutParam,m_Isdisplay,m_error,hmessage);
	}
	catch (...)
	{
		LOG_ERR("halcon�������!");
		res = false;
	}
	int outnum = 0;
	outnum = m_OutParam.size();
	outnum= outnum>RESULT_NUMBER?RESULT_NUMBER:outnum;
	
	//��һ��ֻ��ʾ���ν����������,��error����ʾ
	int result = m_error=0?1:m_error;
	sprintf(ProcRes.szValue[i],"%d",result);

	for (i = 0;i<outnum;i++)
	{
		sprintf(ProcRes.szValue[i+1],"%0.4lf",m_OutParam[i]);
	}
	ProcRes.iCount= outnum +1;//��һ��ֵΪ�˴δ���Ľ��

	//����
	//sprintf(ProcRes.szValue[i],"%d",1);

	//for (i = 0;i<98;i++)
	//{
	//	sprintf(ProcRes.szValue[i+1],"%0.4lf",1.1);
	//}
	//ProcRes.iCount= 98 +1;//��һ��ֵΪ�˴δ���Ľ��
	return res;
}



void HalProcProcessSetting::OnBnClickedProctestp()
{
	// TODO: Add your control notification handler code here
	bool res = false;
	CString str;
	int wid = 0;
	RECT rect;
	double PosX = 0.0,PosY = 0.0,PosPhi = 0.0;
	m_OutParam.clear();
	try
	{
		((HalProcProcess*)m_pContainer)->CommonProcess(m_ProcdurePath.GetString(),m_InParam,m_OutParam,m_Isdisplay,m_error,hmessage);
		 res = true;
	}
	catch (...)
	{
		GetDlgItem(IDC_HALPROC_PROCPMESSAGE)->SetWindowText(hmessage.c_str());
		res = false;
	}

	m_list_outparam.DeleteAllItems();
	if (res)
	{
		m_list_outparam.SetUpdateItemModel(FALSE);	
		m_list_outparam.GetWindowRect(&rect);
		wid = rect.right - rect.left;
		m_list_outparam.DeleteAllItems();
		m_list_outparam.InsertColumn(0, _T("1"));
		m_list_outparam.InsertColumn(1, _T("2"));
		m_list_outparam.InsertColumn(2, _T("3"));
		m_list_outparam.InsertColumn(3, _T("4"));
		m_list_outparam.SetColumnWidth(0, wid / 4);
		m_list_outparam.SetColumnWidth(1, wid / 4);
		m_list_outparam.SetColumnWidth(2, wid / 4);
		m_list_outparam.SetColumnWidth(3, wid / 4);
		m_list_outparam.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		//��������
		m_list_outparam.DeleteAllItems();
		m_list_outparam.SetEditCell(0xffff, 0);
		m_list_outparam.SetEditCell(0xffff, 1);
		m_list_outparam.SetEditCell(0xffff, 2);
		m_list_outparam.SetEditCell(0xffff, 3);
		int count = m_OutParam.size();
		int jrow = 0;
		if (count > 0)
		{
			for (int i = 0; i < count;jrow++ )
			{
				if (i<count)
				{
					if (i%4 == 0)
					{
						m_list_outparam.InsertItem(jrow, str); //����һ����¼
					}
					

					str.Format("%.4lf", m_OutParam[i]); 				
					m_list_outparam.SetItemText(jrow, i, str);
					i++;
				}
				if (i<count)
				{
					//m_list_outparam.InsertItem(i, str); //����һ����¼
					str.Format("%.4lf", m_OutParam[i]); 				
					m_list_outparam.SetItemText(jrow, i, str);
					i++;
				}
				if (i<count)
				{
					//m_list_outparam.InsertItem(i, str); //����һ����¼
					str.Format("%.4lf", m_OutParam[i]); 				
					m_list_outparam.SetItemText(jrow, i, str);
					i++;
				}
				if (i<count)
				{
					//m_list_outparam.InsertItem(i, str); //����һ����¼
					str.Format("%.3lf", m_OutParam[i]); 				
					m_list_outparam.SetItemText(jrow, i, str);
					i++;
				}
			}
		}

	}
	//������Ϣ
	CString mess = hmessage.c_str();
	char err[10] = {0};
	itoa(m_error,err,10);
	mess += ",Ecode:";
	mess+=err;
	GetDlgItem(IDC_HALPROC_PROCPMESSAGE)->SetWindowText(mess);


}
void HalProcProcessSetting::InsertNewRow(int row)
{
	m_L_INPUT.InsertItem(row, "");
	m_L_INPUT.SetItemText(row, 0, "*");
}

void HalProcProcessSetting::InitVarList()
{
	//��ʼ������
	RECT rect;
	CString str;
	int count = 0;
	int wid = 0;
	InitInputList();

	//�������ÿ�
	m_list_outparam.SetUpdateItemModel(FALSE);	
	m_list_outparam.GetWindowRect(&rect);
	wid = rect.right - rect.left;
	m_list_outparam.DeleteAllItems();
	m_list_outparam.InsertColumn(0, _T("1"));
	m_list_outparam.InsertColumn(1, _T("2"));
	m_list_outparam.InsertColumn(2, _T("3"));
	m_list_outparam.InsertColumn(3, _T("4"));
	m_list_outparam.SetColumnWidth(0, wid / 4);
	m_list_outparam.SetColumnWidth(1, wid / 4);
	m_list_outparam.SetColumnWidth(2, wid / 4);
	m_list_outparam.SetColumnWidth(3, wid / 4);
	m_list_outparam.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//��������
	//m_list_outparam.DeleteAllItems();
	m_list_outparam.SetEditCell(0xffff,0);
	m_list_outparam.SetEditCell(0xffff, 1);
	m_list_outparam.SetEditCell(0xffff, 2);
	m_list_outparam.SetEditCell(0xffff, 3);

	count = m_OutParam.size();
	int jrow = 0;
	if (count > 0)
	{
		for (int i = 0; i < count; jrow++)
		{
			if (i<count)
			{
				if (i%4==0)
				{
					m_list_outparam.InsertItem(jrow, str); //����һ����¼
				}
				
				str.Format("%.3lf", m_OutParam[i]); 				
				m_list_outparam.SetItemText(jrow, i, str);
				i++;
			}
			if (i<count)
			{
				str.Format("%.3lf", m_OutParam[i]); 				
				m_list_outparam.SetItemText(jrow, i, str);
				i++;
			}
			if (i<count)
			{
				str.Format("%.3lf", m_OutParam[i]); 				
				m_list_outparam.SetItemText(jrow, i, str);
				i++;
			}
			if (i<count)
			{
				str.Format("%.3lf", m_OutParam[i]); 				
				m_list_outparam.SetItemText(jrow, i, str);
				i++;
			}
		}
	}
}
BOOL HalProcProcessSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitVarList();
	
	return TRUE;
}

void HalProcProcessSetting::InitInputList()
{
	DWORD listStyle = m_L_INPUT.GetExStyle();
	listStyle = listStyle|LVS_EX_GRIDLINES;
	listStyle = listStyle|LVS_EX_FULLROWSELECT;
	m_L_INPUT.SetExtendedStyle(listStyle);

	RECT ListRect;
	int margin = 30;
	m_L_INPUT.GetWindowRect(&ListRect);
	m_L_INPUT.InsertColumn(0,"����");
	m_L_INPUT.SetColumnWidth(0,(ListRect.right-ListRect.left)/2);
	m_L_INPUT.InsertColumn(1,"����ֵ");
	m_L_INPUT.SetColumnWidth(1,(ListRect.right-ListRect.left)/2);

	//��ʾ����
	char szText[256] = {0};
	m_L_INPUT.DeleteAllItems();
	m_SelMoveRow = -1;
	m_SelMoveCol = -1;
	for (int i= 0;i<m_Inputnum;i++)
	{
		sprintf_s(szText,"%d",i);
		m_L_INPUT.InsertItem(i,szText);
		//m_L_INPUT.SetItemText(i,0,szText);
		memset(szText,0,sizeof(szText));
		sprintf_s(szText,"%0.4lf",m_InParam[i]);
		m_L_INPUT.SetItemText(i,1,szText);

	}	
	pCommonEdit = NULL;

}

void HalProcProcessSetting::OnBnClickedAddinput()
{
	char szText[256] = {0};
	int ListRowIndex = m_SelMoveRow+1;//
	for (int i = ListRowIndex;i<m_L_INPUT.GetItemCount();i++)
	{
		sprintf_s(szText,"%d",i+1);
		m_L_INPUT.SetItemText(i,0,szText);
	}

	sprintf_s(szText,"%d",ListRowIndex);
	m_L_INPUT.InsertItem(ListRowIndex,szText);
	m_L_INPUT.SetItemText(ListRowIndex,1,"0.0");
}


void HalProcProcessSetting::OnBnClickedDelinput()
{
	if (m_SelMoveRow<0)
	{
		return;
	}
	char szText[64] ={0};

	for (int i = m_SelMoveRow+1;i<m_L_INPUT.GetItemCount();i++)
	{
		sprintf_s(szText,"%d",i-1);
		m_L_INPUT.SetItemText(i,0,szText);
	}
	m_L_INPUT.DeleteItem(m_SelMoveRow);
	//ɾ����ȥ��ѡ�񽹵��С�
	m_SelMoveRow = -1;
	m_SelMoveCol = -1;
}


void HalProcProcessSetting::OnLvnItemchangedListInputparam(NMHDR *pNMHDR, LRESULT *pResult)//û�õ�
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void HalProcProcessSetting::OnNMClickListInputparam(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_SelMoveRow = pNMItemActivate->iItem;
	m_SelMoveCol = pNMItemActivate->iSubItem;
	*pResult = 0;
}


void HalProcProcessSetting::OnNMDblclkListInputparam(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_SelMoveRow = pNMItemActivate->iItem;
	m_SelMoveCol = pNMItemActivate->iSubItem;
	CRect SubRect;
	//�༭��Ӧ����������
	if (m_SelMoveRow>=0&&m_SelMoveCol>0)  //
	{
		//m_MapList.GetColumnWidth(ColIndex)  ;
		m_L_INPUT.GetSubItemRect(m_SelMoveRow,m_SelMoveCol,0,SubRect);
		if (pCommonEdit)
		{
			delete	pCommonEdit;
			pCommonEdit = NULL;
		}

		pCommonEdit = new CEdit; //�����س������ڴ�й©
		pCommonEdit->Create(WS_CHILD|WS_VISIBLE|WS_BORDER,SubRect,&m_L_INPUT,MOVEDATAEDITID);
		pCommonEdit->SetWindowText(m_L_INPUT.GetItemText(m_SelMoveRow,m_SelMoveCol));

	}

	*pResult = 0;
}
void HalProcProcessSetting::OnOK()
{
	CString str;
	if (NULL == pCommonEdit)
	{
		return;
	}
	SYSTEM_PARAM* pSysParam = (SYSTEM_PARAM*)GetSysParamPoint();
	pCommonEdit->GetWindowText(str);
	m_L_INPUT.SetItemText(m_SelMoveRow,m_SelMoveCol,str);

	//���趨�����ַ���
	//int MoveData = atoi(m_CalMoveConfig.GetItemText(m_SelMoveRow,m_SelMoveCol));
	//switch (m_SelMoveCol)
	//{
	//case 1:
	//	pSysParam->CalMotorX[m_CamIndex][m_AxiIndex][m_ToolIndex][m_SelMoveRow]= MoveData;
	//	break;
	//case 2:
	//	pSysParam->CalMotorY[m_CamIndex][m_AxiIndex][m_ToolIndex][m_SelMoveRow]= MoveData;
	//	break;
	//case 3:
	//	pSysParam->CalMotorPhi[m_CamIndex][m_AxiIndex][m_ToolIndex][m_SelMoveRow]= MoveData;
	//	break;
	//}

	delete	pCommonEdit;
	pCommonEdit = NULL;
}

void HalProcProcessSetting::OnBnClickedProcsettingp()
{
	//��������������,Ȼ�����ָ��ȥ
	CString str;
	int count =0;
	char tempchar[32] ={0};
	double dtemp =0.0;
	
	//�����������,Ϊ�ָ���
	//	m_L_INPUT.UpdateItemText();  //update
	count = m_L_INPUT.GetItemCount();
	m_InParam.clear(); //�����������
	for (int i=0;i<count;i++)
	{
		
		str = m_L_INPUT.GetItemText(i, 1);//��ȡ��2��
		strcpy_s(tempchar, m_L_INPUT.GetItemText(i, 1));
		int ty = get_str_type(tempchar);
		if (EMSTR_DOUBLE != ty && EMSTR_INT != ty)
		{
			AfxMessageBox("�����������������!");
			m_Inputnum = 0;
			m_InParam.clear();
			return;
		}
		dtemp = atof(tempchar);
		m_InParam.push_back(dtemp);
		m_Inputnum = m_InParam.size();
	}

	GetDlgItem(IDC_HALPROC_PROCPATHP)->GetWindowText(m_ProcdurePath);
}


void HalProcProcessSetting::OnBnClickedLoadproc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strFilePath;
	const char pszFilter[] = _T("(*.*)|*.*|*.hdvp|*.hdpl");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,pszFilter, this);

	if(IDOK == dlg.DoModal())
	{
		strFilePath = dlg.GetPathName();
		if (0 == strFilePath.GetLength())
		{
			return;
		}
		strFilePath.Replace('\\','/');
		m_ProcdurePath = strFilePath;
		GetDlgItem(IDC_HALPROC_PROCPATHP)->SetWindowText(m_ProcdurePath);
	}
}



