#pragma once
#include "resource.h"

// ScenePreProSetting �Ի���

class ScenePreProSetting : public CDialogEx
{
	DECLARE_DYNAMIC(ScenePreProSetting)

public:
	ScenePreProSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ScenePreProSetting();

// �Ի�������
	enum { IDD = IDD_PROPRECESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	void UpdateUI(void);
	void GetContainer(void* pContainer);
	void CloseSettingDlg();
	void SaveProcConfig(CString ConfigPath,int ProcIndex);
	void ReadProcConfig(CString ConfigPath,int ProcIndex);
	BOOL VisionProcRun(CHECK_RESULT& ProcRes);

	//����
	int m_ProcProcess;
	void* m_pContainer;
	double m_Multiplication;
	double m_Addition;

	afx_msg void OnBnClickedBtSetting();
	afx_msg void OnBnClickedBtTestenhance();
};
