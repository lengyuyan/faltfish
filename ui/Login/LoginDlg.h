#pragma once
#include <list>
using namespace std;

struct USER_LOGIN_DATA 
{
    int iLevel;            //�û�����          
    char szName[64];       //�û���
    char szPassword[64];   //����
};

// CLoginDlg �Ի���

struct databag
{
    char UserName[64];
    char UserSecreat[64];
    long EncryptStrLen;
};

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLoginDlg();

    // �Ի�������
    enum { IDD = IDD_DIALOG_LOGIN };
public:
    void InitGetDat();
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();

private:
    list<USER_LOGIN_DATA> m_LoginList;
public:
    CComboBox m_ComboName;
    afx_msg void OnBnClickedCancel();

};
