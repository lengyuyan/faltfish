#pragma once
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"

#include "EditListCtrlEx.h"

// HalProcProcessSetting dialog

class HalProcProcessSetting : public CDialogEx
{
	DECLARE_DYNAMIC(HalProcProcessSetting)

public:
	HalProcProcessSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~HalProcProcessSetting();

// Dialog Data
	enum { IDD = IDD_HALPROC_NPROCESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

#define  INPUTPARAMNUM 12
public:

	void GetContainer(void* pContainer);
	void UpdateUI(void);
	void CloseSettingDlg();

	int m_ProcProcess;
	void* m_pContainer;

	void SaveProcConfig(CString ConfigPath,int ProcIndex);
	void ReadProcConfig(CString ConfigPath,int ProcIndex);
	BOOL VisionProcRun(CHECK_RESULT& ProcRes);

	//halcon Procdure�������
	//�������
	int m_Inputnum;
	int m_Isdisplay;
	vector<double>m_InParam;
	//�������
	int m_error;
	string hmessage;
	vector<double>m_OutParam;
	CString m_ProcdurePath;


	#define MOVEDATAEDITID  10001
	CListCtrl  m_L_INPUT;
	int m_SelMoveRow;
	int m_SelMoveCol;
	CEdit* pCommonEdit;


	CEditListCtrlEx m_list_outparam;
	void InitVarList();   
	void InsertNewRow(int row);
	virtual BOOL OnInitDialog();
	//����5������������
	void InitInputList();	
	afx_msg void OnBnClickedAddinput();//��������
	afx_msg void OnBnClickedDelinput();//ɾ������
	afx_msg void OnLvnItemchangedListInputparam(NMHDR *pNMHDR, LRESULT *pResult);//����
	afx_msg void OnNMClickListInputparam(NMHDR *pNMHDR, LRESULT *pResult);//�����У�ѡ��
	afx_msg void OnNMDblclkListInputparam(NMHDR *pNMHDR, LRESULT *pResult);//˫����׼���ı�ֵ,����һ��edit��������
	virtual void OnOK();//�س���Ӧ


	//3����Ӧ��ť���趨�����أ�����
	afx_msg void OnBnClickedProcsettingp();
	afx_msg void OnBnClickedLoadproc();
	afx_msg void OnBnClickedProctestp();
};
