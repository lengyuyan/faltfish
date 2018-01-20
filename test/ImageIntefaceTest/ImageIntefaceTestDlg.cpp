
// ImageIntefaceTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageIntefaceTest.h"
#include "ImageIntefaceTestDlg.h"
#include "afxdialogex.h"
#include "iImageCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

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


// CImageIntefaceTestDlg �Ի���




CImageIntefaceTestDlg::CImageIntefaceTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageIntefaceTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_imageWidth=2599;
	m_imageHeight = 1950;
}

void CImageIntefaceTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_IMAGEWIND_FIRST, m_static_windfirst);
	DDX_Control(pDX, IDC_STATIC_IMAGEWIND_SECOND, m_static_windsecond);
	DDX_Control(pDX, IDC_SLIDER_EXT, m_slider_exm);
	DDX_Control(pDX, IDC_SLIDER_GAIN, m_slider_gain);
}

BEGIN_MESSAGE_MAP(CImageIntefaceTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_GETIMAGE_FIRSTR, &CImageIntefaceTestDlg::OnBnClickedButtonGetimageFirstr)
	ON_BN_CLICKED(IDC_BUTTON_GETIMAGE_FIRST, &CImageIntefaceTestDlg::OnBnClickedButtonGetimageFirst)
	ON_BN_CLICKED(IDC_BUTTON_VEDIO_FIRST, &CImageIntefaceTestDlg::OnBnClickedButtonVedioFirst)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_EXT, &CImageIntefaceTestDlg::OnNMCustomdrawSliderExt)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CImageIntefaceTestDlg ��Ϣ�������

BOOL CImageIntefaceTestDlg::OnInitDialog()
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

	m_slider_exm.SetRange(0,2000);
	m_slider_exm.SetTicFreq(1);

	m_slider_gain.SetRange(8,48);
	m_slider_gain.SetTicFreq(1);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	gen_image_const(&m_hImage,"byte",m_imageWidth,m_imageHeight);
	get_image_size(m_hImage, &m_imageWidth, &m_imageHeight);
	set_window_attr("background_color","black");
	open_window(0,0,m_imageWidth,m_imageHeight,(int)m_static_windfirst.m_hWnd,"","visible",&m_diswin_first);
	set_part((HTuple)m_diswin_first, 0,0,m_imageWidth -1, m_imageHeight-1);

	open_window(0,0,m_imageWidth,m_imageHeight,(int)m_static_windfirst.m_hWnd,"","visible",&m_diswin_second);
	set_part((HTuple)m_diswin_second, 0,0, m_imageHeight-1, m_imageWidth-1);

	imageInstance(CAMARA_TYPE_AVT_1394);
	list<string> guidlist;
	getGUIDList(guidlist);
	m_list_str_it = guidlist.begin();
	m_name = m_list_str_it->c_str();
	int res = openCamera(m_list_str_it->c_str());

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CImageIntefaceTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CImageIntefaceTestDlg::OnPaint()
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
HCURSOR CImageIntefaceTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CImageIntefaceTestDlg::OnBnClickedButtonGetimageFirstr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	unsigned char *ptr = NULL;
	char typ[128];
	long w,h;
	get_image_pointer1(m_hImage, (Hlong *)&ptr, typ, &w, &h);
	getImage(m_name,(void *)ptr);
	disp_obj(m_hImage,m_diswin_first);
}


void CImageIntefaceTestDlg::OnBnClickedButtonGetimageFirst()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	getImage(m_list_str_it->c_str(),&m_hImage);
	disp_obj(m_hImage,m_diswin_second);
}

void VedioCallback(void *context)
{
	CImageIntefaceTestDlg * m_this = (CImageIntefaceTestDlg*)context;
	disp_obj(m_this->m_hImage,m_this->m_diswin_first);
}

void CImageIntefaceTestDlg::OnBnClickedButtonVedioFirst()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	unsigned char *ptr = NULL;
	char typ[128];
	long w,h;
	get_image_pointer1(m_hImage, (Hlong *)&ptr, typ, &w, &h);
	callback = VedioCallback;
	startSnap(m_name,ptr,callback,this);
}

void CImageIntefaceTestDlg::OnNMCustomdrawSliderExt(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	*pResult = 0;
}


void CImageIntefaceTestDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar; 
	CString strText; 
	unsigned int val=0;

	//��� SLIDER �ؼ����� 
	switch( pSlider->GetDlgCtrlID() ) 
	{ 
	case IDC_SLIDER_EXT:
		val = pSlider->GetPos();
		strText.Format( _T("%d"), val ); 
		SetDlgItemText( IDC_STATIC_EXM_DIS, strText ); 
		setExposureTime(m_name,val);		
		break; 
	 case IDC_SLIDER_GAIN: 
		  val = pSlider->GetPos();
		 strText.Format( _T("%d"), val ); 
		 SetDlgItemText( IDC_STATIC_GAIN_DIS, strText ); 
		 setGain(m_name,val);		 
		 break;
	} 

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
