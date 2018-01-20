
// PasswordAlterDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

struct databag
{
    char UserName[64];
    char UserSecreat[64];
    long EncryptStrLen;
};
// CPasswordAlterDlg �Ի���
class CPasswordAlterDlg : public CDialogEx
{
// ����
public:
	CPasswordAlterDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PASSWORDALTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
public:
    void Init();
    bool ReadPassword(CString Username);
    bool WritePasswordComp_Upd(CString Username);
    void WritePasswordDat();
private:
    //databag userbag[2];
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
    afx_msg void OnBnClickedButtonAffirm();
    CComboBox m_CobUserName;
    afx_msg void OnBnClickedButtonCancel();
};
