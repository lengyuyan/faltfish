
// PasswordAlterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PasswordAlter.h"
#include "PasswordAlterDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include "common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
databag userbag[2] = {0};
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPasswordAlterDlg �Ի���




CPasswordAlterDlg::CPasswordAlterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPasswordAlterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPasswordAlterDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_USERNAME, m_CobUserName);
}

BEGIN_MESSAGE_MAP(CPasswordAlterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_AFFIRM, &CPasswordAlterDlg::OnBnClickedButtonAffirm)
    ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CPasswordAlterDlg::OnBnClickedButtonCancel)
END_MESSAGE_MAP()


// CPasswordAlterDlg ��Ϣ�������

BOOL CPasswordAlterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
    SetBackgroundColor(BACKGROUND_COLOR_M);
    this->SetWindowText("�����޸�");
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    Init(); 

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPasswordAlterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPasswordAlterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPasswordAlterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPasswordAlterDlg::Init()
{
    m_CobUserName.InsertString(0,"admin");
    m_CobUserName.InsertString(1,"engineer");

    m_CobUserName.SetCurSel(0);
}

void CPasswordAlterDlg::OnBnClickedButtonAffirm()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    //��ȡ����ѡ����û�
    CString str;
    m_CobUserName.GetLBText(m_CobUserName.GetCurSel(),str);

    //��ȡ���벢����
    if(!ReadPassword(str))
    {
        return;
    }

    //�Ƚ����������������Ƿ���ͬ�����Ҹ�������
   if(!WritePasswordComp_Upd(str))
   {
       return;
   }

    ofstream outfile("password.dat",ios::binary);
    if (!outfile)
    {
        MessageBox("Open error!");
        return;
    }
    for(int i=0;i<2;i++)
    {
        int iS = sizeof(userbag[i]);
        outfile.write((char*)&userbag[i],sizeof(userbag[i]));
    }
    outfile.close( );

    MessageBox("�޸�����ɹ�");
    OnOK();
}

bool CPasswordAlterDlg::WritePasswordComp_Upd(CString Username)
{
    CString newpasswordstr;
    CString renewpasswordstr;
    CEdit *pEditCurrentPassword = (CEdit *)GetDlgItem(IDC_EDIT_CURRENT_PASSWORD);
    CEdit *pEditRecurrentPassword = (CEdit *)GetDlgItem(IDC_EDIT_RECURRENT_PASSWORD);
    pEditCurrentPassword->GetWindowText(newpasswordstr);
    pEditRecurrentPassword->GetWindowText(renewpasswordstr);
    if (0 != strcmp(newpasswordstr,renewpasswordstr))
    {
        MessageBox("�����������벻һ��");
        pEditRecurrentPassword->SetFocus();
        pEditRecurrentPassword->SetSel(0,-1);
        return false;
    }

    for (int i=0; i<2; i++) 
    {
        if (0 == strcmp(userbag[i].UserName,Username))
        {

            char Out[64] = {0};
            char *In =  (char *)(LPCTSTR)newpasswordstr;
            Execution_3DES(Out,In,newpasswordstr.GetLength(),"0123", strlen("0123"), true);

            memcpy(userbag[i].UserSecreat,Out,newpasswordstr.GetLength()+7);
            userbag[i].EncryptStrLen = newpasswordstr.GetLength();
        }
    }
    return true;
}

bool CPasswordAlterDlg::ReadPassword(CString Username)
{
    ifstream infile("password.dat",ios::binary);
    if (!infile)
    {
        //���password�ļ���ʧ�����½�һ��password.dat
        //Ĭ��    admin   tod8888
        //        engineer  Tod_123
        WritePasswordDat();
    }
    for(int i=0; i<2; i++)
    {
        infile.read((char*)&userbag[i],sizeof(userbag[i]));
    }
    infile.close();

    CString oldpasswordstr;
    CEdit *pEditOriginalPassword = (CEdit *)GetDlgItem(IDC_EDIT_ORIGINAL_PASSWORD);
    pEditOriginalPassword->GetWindowText(oldpasswordstr);
    for (int i=0; i<2; i++)
    {
        if (0 == strcmp(userbag[i].UserName,Username))
        {
            char Out[64] = {0};
            Execution_3DES(Out,userbag[i].UserSecreat,userbag[i].EncryptStrLen,"0123", strlen("0123"), false);
            strcpy_s(userbag[i].UserSecreat,64,Out);
            if (0 != strcmp(userbag[i].UserSecreat,oldpasswordstr))
            {
                MessageBox("ԭʼ�����������");
                pEditOriginalPassword->SetFocus();
                pEditOriginalPassword->SetSel(0,-1);
                return false;
            }
        }
    }
    return true;
}

void CPasswordAlterDlg::WritePasswordDat()
{
    char Out_admin[64] = {0};
    char Out_enginee[64] = {0};
    Execution_3DES(Out_admin,"tod8888",strlen("tod8888"),"0123", strlen("0123"), true);
    Execution_3DES(Out_enginee,"Tod_123",strlen("Tod_123"),"0123", strlen("0123"), true);

    strcpy(userbag[0].UserName, "admin");
    memcpy(userbag[0].UserSecreat,Out_admin,strlen("tod8888")+7);
    userbag[0].EncryptStrLen = strlen("tod8888");

    strcpy(userbag[1].UserName, "engineer");
    memcpy(userbag[1].UserSecreat,Out_enginee,strlen("Tod_123")+7);
    userbag[1].EncryptStrLen = strlen("Tod_123");


    ofstream outfile("password.dat",ios::binary);
    for(int i=0;i<2;i++)
    {
        outfile.write((char*)&userbag[i],sizeof(userbag[i]));
    }
    outfile.close( );
}

void CPasswordAlterDlg::OnBnClickedButtonCancel()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    //OnCancel();
    OnOK();
}
