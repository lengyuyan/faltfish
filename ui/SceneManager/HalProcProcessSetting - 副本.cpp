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
	ON_BN_CLICKED(IDC_HALPROC_LOADPATH, &HalProcProcessSetting::OnBnClickedHalprocLoadpath)
	ON_BN_CLICKED(IDC_HALPROC_TEST, &HalProcProcessSetting::OnBnClickedHalprocTest)
	ON_BN_CLICKED(IDC_HALPROC_COMMIT, &HalProcProcessSetting::OnBnClickedHalprocCommit)
	//�ֶ�����
	ON_MESSAGE(MLSM_ITEMCHANGEDEX, &HalProcProcessSetting::OnMsgChangeItem)
	
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





void HalProcProcessSetting::OnBnClickedHalprocCommit()
{
	CString str;
	int count =0;
	char tempchar[32] ={0};
	double dtemp =0.0;
	m_InParam.clear(); //�����������
	//�����������,Ϊ�ָ���
	m_L_INPUT.UpdateItemText();  //update
	count = m_L_INPUT.GetItemCount();
	m_Inputnum = count;
	for (int i=0;i<count;i++)
	{
		str = m_L_INPUT.GetItemText(i, 1);//��ȡ��2��
		strcpy_s(tempchar, m_L_INPUT.GetItemText(i, 1));
		if (EMSTR_DOUBLE != get_str_type(tempchar) && EMSTR_INT != get_str_type(tempchar));
		{
			AfxMessageBox("�����������������!");
			m_Inputnum = 0;
			m_InParam.clear();
			return;
		}
		dtemp = atof(tempchar);
		m_InParam.push_back(dtemp);
	}

	GetDlgItem(IDC_PROCSETTINGP)->GetWindowText(m_ProcdurePath);
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
	double PosX = 0.0,PosY = 0.0,PosPhi = 0.0;

	try
	{
		res = ((HalProcProcess*)m_pContainer)->CommonProcess(m_ProcdurePath.GetString(),m_InParam,m_OutParam,m_Isdisplay,m_error,hmessage);
	}
	catch (...)
	{

		res = false;
	}

	if(res)
	{

	}else
	{
		PosX = 0.0;
		PosY = 0.0;
		PosPhi = 0.0;

	}

	int BeginIndex = ProcRes.iCount;
	ProcRes.iCount += 3;
	// �Ƿ���Ҫ��ƥ���ֵ���
	int ResCount = ProcRes.iCount; 

	sprintf(ProcRes.szValue[BeginIndex],"%0.5lf",PosX);
	sprintf(ProcRes.szValue[BeginIndex+1],"%0.5lf",PosY);
	sprintf(ProcRes.szValue[BeginIndex+2],"%0.5lf",PosPhi);

	return res;
	////strTemp.Format("%ld",)
	//GetDlgItem(IDC_HALCOMMPOS_DISX)->SetWindowTextA(strTemp);
	////strTemp.Format("%ld",)
	//GetDlgItem(IDC_HALCOMMPOS_DISY)->SetWindowTextA(strTemp);
	////strTemp.Format("%ld",)
	//GetDlgItem(IDC_HALCOMMPOS_DISPHI)->SetWindowTextA(strTemp);

}



void HalProcProcessSetting::OnBnClickedHalprocLoadpath()
{
	// TODO: Add your control notification handler code here
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
		GetDlgItem(IDC_LOADPROC)->SetWindowTextA(m_ProcdurePath);
	}


}


void HalProcProcessSetting::OnBnClickedHalprocTest()
{
	// TODO: Add your control notification handler code here
	bool res = false;
	CString str;
	int wid = 0;
	RECT rect;
	double PosX = 0.0,PosY = 0.0,PosPhi = 0.0;
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
		m_list_outparam.InsertColumn(0, _T("3"));
		m_list_outparam.InsertColumn(1, _T("4"));
		m_list_outparam.SetColumnWidth(0, wid / 4);
		m_list_outparam.SetColumnWidth(1, wid / 4);
		m_list_outparam.SetColumnWidth(2, wid / 4);
		m_list_outparam.SetColumnWidth(3, wid / 4);
		m_list_outparam.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		//��������
		m_list_outparam.DeleteAllItems();
		m_list_outparam.SetEditCell(0xffff, 1);
		m_list_outparam.SetEditCell(0xffff, 2);
		m_list_outparam.SetEditCell(0xffff, 3);
		int count = m_OutParam.size();
		if (count > 0)
		{
			for (int i = 0; i < count; )
			{
				if (i<count)
				{
					m_list_outparam.InsertItem(i, str); //����һ����¼
					str.Format("%.3lf", m_OutParam[i]); 				
					m_list_outparam.SetItemText(i, i, str);
					i++;
				}
				if (i<count)
				{
					m_list_outparam.InsertItem(i, str); //����һ����¼
					str.Format("%.3lf", m_OutParam[i+1]); 				
					m_list_outparam.SetItemText(i, i+1, str);
					i++;
				}
				if (i<count)
				{
					m_list_outparam.InsertItem(i, str); //����һ����¼
					str.Format("%.3lf", m_OutParam[i+2]); 				
					m_list_outparam.SetItemText(i, i+2, str);
					i++;
				}
				if (i<count)
				{
					m_list_outparam.InsertItem(i, str); //����һ����¼
					str.Format("%.3lf", m_OutParam[i+3]); 				
					m_list_outparam.SetItemText(i, i+3, str);
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
LRESULT HalProcProcessSetting::OnMsgChangeItem(WPARAM wParam, LPARAM lParam)
{
	int row = (int)(wParam & 0xFFFF);
	int col = (int)(wParam>>16);

	if (m_L_INPUT.m_hWnd == (HWND)lParam)
	{
		EditVarList(row, col);
	}

	return 0;
}
void HalProcProcessSetting::EditVarList(int row, int col)
{
	CString str;
	int ret = 0;
	int count = 0;
	char temp[32] ={0};
	char *p = NULL;
	int realcount = m_Inputnum;


	str = m_L_INPUT.GetEditText();
	str.Trim(" ");
	//if (str.IsEmpty())
	//{
	//	return;
	//}
	p = str.GetBuffer();
	if ((EMSTR_DOUBLE != get_str_type(p) && EMSTR_INT != get_str_type(p)) && FALSE == str.IsEmpty())
	{
		AfxMessageBox("����������");
		return ;
	}

	m_L_INPUT.UpdateItemText();  //update
	count = m_L_INPUT.GetItemCount();
	if (count>m_Inputnum+1)
	{
		AfxMessageBox("�밴��˳�������");
		return ;
		
	}
	if (str.IsEmpty())
	{

	}
	double dtemp = atof(p);
	if (row>=realcount)
	{
		m_InParam.push_back(dtemp);
		m_Inputnum++;
	}
	else if (count == realcount+1 && str.IsEmpty())
	{
		m_InParam.pop_back();
		m_Inputnum--;
		m_L_INPUT.DeleteItem(realcount);
		m_L_INPUT.DeleteItem(realcount-1);
		//str.Format("%d", realcount);
		//m_L_INPUT.SetItemText(row, 0, str);
		InsertNewRow(realcount-1);
		m_L_INPUT.SetEditCell(realcount-1,1);//����


	}
	else
	{
		m_InParam[row] = dtemp;
	}

	 if (row == count - 1 )//��������һ��
	 {	
		 str.Format("%d", count);
		 m_L_INPUT.SetItemText(row, 0, str);
		 InsertNewRow(count);
		 m_L_INPUT.SetEditCell(count,1);
	 }



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
	m_L_INPUT.SetUpdateItemModel(FALSE);	
	m_L_INPUT.GetWindowRect(&rect);
	wid = rect.right - rect.left;
	m_L_INPUT.InsertColumn(0, _T("����"));
	m_L_INPUT.InsertColumn(1, _T("ֵ"));
	m_L_INPUT.SetColumnWidth(0, wid / 2);
	m_L_INPUT.SetColumnWidth(1, wid / 2);
	m_L_INPUT.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//��������
	m_L_INPUT.DeleteAllItems();
	m_L_INPUT.SetEditCell(0xffff, 1);
	m_L_INPUT.SetEditCell(0xffff, 2);
	m_L_INPUT.SetEditCell(0xffff, 3);
	if (m_Inputnum > 0)
	{
		for (int i = 0; i < m_Inputnum; i++)
		{
			str.Format("%d", i+1); 
			m_L_INPUT.InsertItem(i, str); //����һ����¼
			m_L_INPUT.SetItemText(i, 0, str);
			str.Format("%.3lf", m_InParam[i]);
			m_L_INPUT.SetItemText(i, 1, str);
			//m_L_INPUT.SetEditCell(i, 1);
		}

	}
	//��������
	m_L_INPUT.InsertItem(m_Inputnum, "");
	m_L_INPUT.SetItemText(m_Inputnum, 0, "*");
	m_L_INPUT.SetItemText(m_Inputnum, 1, "");
	m_L_INPUT.SetEditCell(m_Inputnum-1, 1);

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
	m_list_outparam.SetEditCell(0xffff, 1);
	m_list_outparam.SetEditCell(0xffff, 2);
	m_list_outparam.SetEditCell(0xffff, 3);
	m_list_outparam.SetEditCell(0xffff, 4);

	count = m_OutParam.size();
	if (count > 0)
	{
		for (int i = 0; i < count; )
		{
			if (i<count)
			{
				m_list_outparam.InsertItem(i, str); //����һ����¼
				str.Format("%.3lf", m_OutParam[i]); 				
				m_list_outparam.SetItemText(i, i, str);
				i++;
			}
			if (i<count)
			{
				m_list_outparam.InsertItem(i, str); //����һ����¼
				str.Format("%.3lf", m_OutParam[i+1]); 				
				m_list_outparam.SetItemText(i, i+1, str);
				i++;
			}
			if (i<count)
			{
				m_list_outparam.InsertItem(i, str); //����һ����¼
				str.Format("%.3lf", m_OutParam[i+2]); 				
				m_list_outparam.SetItemText(i, i+2, str);
				i++;
			}
			if (i<count)
			{
				m_list_outparam.InsertItem(i, str); //����һ����¼
				str.Format("%.3lf", m_OutParam[i+3]); 				
				m_list_outparam.SetItemText(i, i+3, str);
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









