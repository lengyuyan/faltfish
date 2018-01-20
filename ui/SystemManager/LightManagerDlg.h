#pragma once
#include "afxwin.h"


// LightManagerDlg �Ի���

class LightManagerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LightManagerDlg)

public:
	LightManagerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~LightManagerDlg();

// �Ի�������
	enum { IDD = IDD_LIGHTDEBUG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonOpLight();
    virtual BOOL OnInitDialog();

protected:
    void InitLightComBox();
public:
    CComboBox m_CommList;
};
