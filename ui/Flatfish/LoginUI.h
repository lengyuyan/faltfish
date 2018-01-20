#pragma once
#include "list"
#include "afxwin.h"
using namespace std;

struct USER_LOGIN_DATA 
{
    int iLevel;            //�û�����          
    char szName[16];       //�û���
    char szPassword[16];   //����
};

// CLogin �Ի���

class CLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLogin();

// �Ի�������
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();

private:
    list<USER_LOGIN_DATA> m_LoginList;
public:
    CComboBox m_ComboName;
};
