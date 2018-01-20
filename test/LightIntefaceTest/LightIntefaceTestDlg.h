
// LightIntefaceTestDlg.h : ͷ�ļ�
//

#pragma once
#include "iLightCtrl.h"
#include "afxwin.h"
#include "afxcmn.h"

// CLightIntefaceTestDlg �Ի���
class CLightIntefaceTestDlg : public CDialogEx
{
// ����
public:
	CLightIntefaceTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_LIGHTINTEFACETEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cb_com;
	CComboBox m_cb_index;
	CSliderCtrl m_slider_value;
	afx_msg void OnBnClickedOk();
	CComboBox m_cb_lighttype;
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonClose();
	void InitLight();

	string m_name;
	int m_index;
	LIGHTCTL_TYPE m_ltype;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonGetlightV();
};
