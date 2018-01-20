
// FlatFishDlg.cpp : ʵ���ļ�
//

#pragma  once
#include "stdafx.h"
#include "FlatFish.h"
#include "FlatFishDlg.h"
//#include "afxdialogex.h"
#include "iManagerCtrl.h"
#include "DataType.h" 
#include "map"
#include "sysapi.h"//�����˻�ȡ·��

// CFlatFishDlg ��Ϣ�������
#include "assert.h"
//#include "HalconCpp.h"
//using namespace Halcon;
#include <complex>

#define UPDATE_STATUS_TIMER 1000       //ˢ��״̬��ʱ�䶨ʱ��
#define FLATFISH_STATUS_TIME 1000      //״̬��ʱ�䶨ʱ�����
#define  VISION_MANUAL_MSG       WM_USER + 1000    //ҵ����Ϣ

#define MAX_WIDTH   1024
#define MAX_HEIGHT  768

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CFlatFishDlg �Ի���

typedef void (WINAPI ShowDebugDlg)(int);
typedef int (WINAPI ShowLoginDlg)(void);
typedef void (WINAPI BuildScene)(const char*,int );//
typedef void  (WINAPI GrabWay2Image)(int ,bool);
//typedef BOOL (*pSceneProcess)(int,CHECK_RESULT&);

static UINT indicators[] =
{
    IDS_STRING_CONNECT_STATUS,        //�����������״̬    
    IDS_STRING_TIME,                  //ϵͳʱ��
};

#define FLATFISH_ERROR_BASECODE -1000//�����ģ�������
#define FLATFISH_STATUSPAN_NOTREADY FLATFISH_ERROR_BASECODE-1  //״̬��û��׼����

CFlatFishDlg *CFlatFishDlg::m_pInstance = NULL;
bool   CFlatFishDlg::isstatusready = false;

CFlatFishDlg::CFlatFishDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFlatFishDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pInstance = this;
    m_pIndicators = NULL;
    m_IndicatorsSize = 2;			//Ĭ��Ϊ2��Ϊ���������״̬����ʱ����ʾ
    m_hSceneDebugDLL = NULL;		//��������
    m_hSystemSettingDLL = NULL;		//ϵͳ����
    m_hLoginDLL = NULL;				//��½����
	m_IsSceneInit = false;			//�����Ƿ��ʼ��

}

void CFlatFishDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFlatFishDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

    //�Զ���ҵ����Ϣ
    ON_MESSAGE(VISION_MANUAL_MSG, &CFlatFishDlg::OnManualMessage)


    ON_BN_CLICKED(IDC_BUTTON_SHOW_SCENE_WIN, &CFlatFishDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFlatFishDlg::OnBnClickedButton2)
    ON_WM_SIZE()
    ON_WM_SHOWWINDOW()
    ON_WM_CTLCOLOR()
    ON_WM_TIMER()
END_MESSAGE_MAP()


BOOL CFlatFishDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	//SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	//SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	SetIcon(AfxGetApp()->LoadIcon(IDI_BIG_ICON), TRUE);
	SetIcon(AfxGetApp()->LoadIcon(IDI_BIG_ICON), FALSE);

	char szText[256] = {0};
	::GetWindowTextA(m_hWnd,szText,sizeof(szText));
	m_TitleName = szText;

    register_notify(MODULE_SCENE_MAIN_UI, OnSDKMessage,this);
    InitManager();  //iManagerCtrl���еĺ��� �����
    MoveWindow(0, 0, MAX_WIDTH, MAX_HEIGHT);  //�����ö�ȡ   
    SetBackgroundColor(BACKGROUND_COLOR_M);

	//���ؿ�
	initSettingDll();
	//��ʼ��UI����
	initLayout();
	// ����������������
	m_IsSceneInit = false;
	initSceneList();
	// UI����
	GrabSceneInit();
    initStatusBar(); 
    //��״̬����߳�
    OpenCheckStateThread();

    SYSTEM_PARAM* pSparam =(SYSTEM_PARAM*)GetSysParamPoint();

	PROCTOCOLDATA proctoData ;
	pSparam->stSocketParam.GetProctocolData(proctoData);
	proctoData.MakeEndable();
	
	SetWebProctocol(proctoData);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CFlatFishDlg::initSettingDll()
{
    char szPath[512] = {0};
    get_module_path((LPSTR)szPath);
    CString strDll;
    strDll = szPath;
    strDll = strDll + "SceneManager.dll";
    m_hSceneDebugDLL = LoadLibrary(strDll);
    strDll = szPath;
    strDll = strDll + "SystemManager.dll";
    m_hSystemSettingDLL = LoadLibrary(strDll);
    strDll = szPath;
    strDll = strDll + "Login.dll";
    m_hLoginDLL = LoadLibrary(strDll);
}
void  CFlatFishDlg::initSceneList()
{
	if (m_IsSceneInit)
	{
		return;
	}
	m_IsSceneInit = true;
	int SceneNum = GetSceneNum();
	BuildScene*  pBuildScene = NULL;

	pBuildScene = (BuildScene*)GetProcAddress(m_hSceneDebugDLL, "IniSceneData"); //


	for (int i = 0; i < SceneNum; i++)
	{		
		if( 0 == GetSceneProcType(i))   //�ж��Ƿ���0 == ProcProcess
		{
		     //����ÿ�����������е�ͼ����
			if (NULL != pBuildScene)	
			{		
		         pBuildScene(GetSceneConfigPath(GetSceneId(i)),GetSceneId(i));
			}

		}
	}
	
	GetSceneProcess((pSceneProcess*)GetProcAddress(m_hSceneDebugDLL, "SceneVisionProcess"));

}

void CFlatFishDlg::unLoadDll()
{
    if (NULL != m_hSceneDebugDLL)
    {
        FreeLibrary(m_hSceneDebugDLL);
        m_hSceneDebugDLL = NULL;
    }
    if (NULL != m_hSystemSettingDLL)
    {
        FreeLibrary(m_hSystemSettingDLL);
        m_hSystemSettingDLL = NULL;
    }
    if (NULL != m_hLoginDLL)
    {
        FreeLibrary(m_hLoginDLL);
        m_hLoginDLL = NULL;
    }
}
// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFlatFishDlg::OnPaint()
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
HCURSOR CFlatFishDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//���ܣ�������ע����Ϣ��,���ģ�����msg_post()�����������ڷ�����Ϣ,���ص��˺���
//wParam:ҵ����Ϣ����
//msgType:ϵͳ��Ϣ����
//lParam: ����
int WINAPI CFlatFishDlg::OnSDKMessage( unsigned int wParam, void *lParam,void* otherparam)
{
    if (NULL == m_pInstance)
    {
        return false;
    }
	switch (wParam)
	{
    case MSG_RESET_SYSTEM:
        {
            InitManager();
            ResetFinishedSendMsg();         //��������ϵͳ���غ���
            ::SendMessage(m_pInstance->m_hWnd,VISION_MANUAL_MSG, wParam, (LPARAM)lParam); //����ˢ�´���ͨ���Զ�����Ϣ����
        }
        break;
	case MSG_DIS_MAINWND_NAME:
	    {
			//char szText[256]={0};
            char szText1[256]={0};
			//::GetWindowTextA(m_pInstance->m_hWnd,szText,sizeof(szText));
			sprintf_s(szText1,"%s_��Ʒ:%s",m_pInstance->m_TitleName.GetBuffer(),lParam);
			::SetWindowTextA(m_pInstance->m_hWnd,szText1);

	    }
		break;
	case MSG_DISP_CLIB_INFO:
		{
			//char szText[256]={0};
			char szText1[256]={0};
			//::GetWindowTextA(m_pInstance->m_hWnd,szText,sizeof(szText));
			sprintf_s(szText1,"�궨����:%s",lParam);
			::SetWindowTextA(m_pInstance->m_hWnd,szText1);

		}

	 break;
    default:
        {
            ::SendMessage(m_pInstance->m_hWnd,VISION_MANUAL_MSG, wParam, (LPARAM)lParam); //����ˢ�´���ͨ���Զ���ҵ����Ϣ����
        }
        break;
    }
    return 0;
}
/**********************************************************************
����      : OnManualMessage
����      : ���������Ի���ҵ����Ϣ
�������  : WPARAM wParam:��Ϣ����
			LPARAM lParam ����Ϣ���� 
�������  : N/A
����ֵ    : 
��ע      : 
�޸ļ�¼��
����        �޸���               �޸�����
2016-08-16  guopengzhou          ������ͼƬ���ǲ�Ҫ����ͼƬ��
**********************************************************************/

LRESULT CFlatFishDlg::OnManualMessage( WPARAM wParam, LPARAM lParam )
{
    CListBox *pList = NULL;
    pList = (CListBox *)m_pInstance->GetDlgItem(IDC_LIST_RUN_INFO);
    switch (wParam)
    {
    case MSG_CLOSE_SYSTEM:
        {
            OnOK();
        }
        break;
    case MSG_DISP_IMAGE_DUMP:
        {	
            CString str;
            SYSTEM_PARAM *sysParam;
			SCENE_PKG pkg;
			char *buf = NULL;
			char path[MAX_PATH*2] = {0};
			int w = 0,h = 0,size = 0;
			sysParam = (SYSTEM_PARAM* )GetSysParamPoint();
            int id = *(int *)lParam;
            if (id < 0 || id >= sysParam->iSceneNum)
            {
                LOG_INFO("����ID��Ч");
                return -1;
            }

          //  str.Format("�Ӿ������������%d",id);
         //   pList->AddString(str.GetBuffer());
          
            GetSceneParam(id, &pkg);
            //DispObj((int)m_vecPicItem[pkg.stationID]->m_hWnd ,id,true);
			SaveImageDump((int)m_vecPicItem[pkg.stationID]->m_hWnd ,id,path);

			//buf = (char*)GetImageBufByFile(path,size,w,h);
			//buf = (char*)GetImageDump((int)m_pInstance->m_vecPicItem[pkg.stationID]->m_hWnd,0,w,h);			
			//SendImageDump(buf,size,w,h,id);
			return 0;
        }
        break;
    case MSG_DISP_IMAGE:
        {
            SCENE_PKG config;
            SYSTEM_PARAM* sysParam;
			sysParam = (SYSTEM_PARAM* )GetSysParamPoint();
            int SceneID = *(int *)lParam;
            if (SceneID < 0 || SceneID >= sysParam->iSceneNum)
            {
                LOG_INFO("����ID��Ч");
                return -1;
            }
            GetSceneParam(SceneID, &config);
            if (config.stationID >= m_pInstance->m_vecPicItem.size())
            {
                LOG_INFO("���ڲ�����");
                //���ڲ�����
                return -1;
            }
            DispObj((int)m_pInstance->m_vecPicItem[config.stationID]->m_hWnd, SceneID);
        }
        break;
    case MSG_RESET_SYSTEM:
        {
            pList->AddString("����ϵͳ");
            pList->SetCurSel(pList->GetCount() - 1); 
        }
        break;
    case MSG_DISP_WORKING_INFO:
        {
            char *pStr = (char *)lParam;
            pList->AddString(pStr);
        }
        break;
    case MSG_CONNECT_FAILED:
        {
			if (false == isstatusready) //����Ҫ����
			{
				LOG_ERR("״̬����û�г�ʼ���ɹ�,���ܽ�����Ϣ");
				return FLATFISH_STATUSPAN_NOTREADY;
			}
            m_statusBar.SetPaneText(0, "����ʧ��", 1); //״̬����û�г�ʼ���ɹ�,�ɹ�����ܴ���
        }
        break;
    case MSG_CONNECT_SUCCESS:
        {
			if (false == isstatusready) //����Ҫ����
			{
				LOG_ERR("״̬����û�г�ʼ���ɹ�,���ܽ�����Ϣ");
				return FLATFISH_STATUSPAN_NOTREADY;
			}
            m_statusBar.SetPaneText(0, "���ӳɹ�", 1);
        }
        break;
    case MSG_CONNECT_CAMERA_FAILED:
        {
			if (false == isstatusready) //����Ҫ����
			{
				LOG_ERR("״̬����û�г�ʼ���ɹ�,���ܽ�����Ϣ");
				return FLATFISH_STATUSPAN_NOTREADY;
			}
            int iCamera = *(int *)lParam + 1;
            char szBuf[128] = {0};
            sprintf_s(szBuf, "���%d����ʧ��", iCamera - 1);
            m_statusBar.SetPaneText(iCamera, szBuf, 1);
        }
        break;
    case MSG_CONNECT_CAMERA_SUCCESS:
        {
			if (false == isstatusready) //����Ҫ����
			{
				LOG_ERR("״̬����û�г�ʼ���ɹ�,���ܽ�����Ϣ");
				return FLATFISH_STATUSPAN_NOTREADY;
			}
            int iCamera = *(int *)lParam + 1;
            char szBuf[128] = {0};
            sprintf_s(szBuf, "���%d���ӳɹ�", iCamera - 1);
            m_statusBar.SetPaneText(iCamera, szBuf, 1);
        }
        break;
    case MSG_RESET_PARAMETER:
        {
            InitManager();
            initLayout();
            pList->AddString("���ò����ɹ�");
        }
        break;
    case MSG_LOGIN_SYSTEM:
        {
            m_LoginLevel = *(int *)lParam;
        }
        break;
    default:
        break;
    }
    pList->SetCurSel(pList->GetCount() - 1); 
    return 0;
}
void CFlatFishDlg::initLayout()
{
    int indexImage = 0;
    int  iNum = 0;
    int  iRow = 0;
    int  iCol = 0;
    int  xb = 0;
    int  yb = 0;
    int  xe = 0;
    int  ye = 0;
    int  iWidth = 0;
    int  iHeight = 0;

    int  iPicID = 5000;
    SYSTEM_PARAM *config;
	config = (SYSTEM_PARAM *)GetSysParamPoint();
    iNum = config->iWindowNum;

	
    iCol = (int)sqrt((float)iNum);
    if (iNum > iCol * iCol)
    {
        iCol++;
    }
	//����������,��������,���Լ�������
    iRow = iCol - (iCol * iCol - iNum)/iCol;
    CRect rt;
    CRect rtPic;
    GetClientRect(rt);
    rt.bottom -= 20;   //ȥ��״̬���߶�
    CWnd *pPic = GetDlgItem(IDC_STATIC_IMAGE);
    rtPic.left = rt.left+10;
    rtPic.top = rt.top+10;
    rtPic.bottom = rt.bottom - rt.Height()/5;
    rtPic.right = rt.right;
    pPic->MoveWindow(rtPic);
    iWidth = ((rtPic.Width() - 5)/iCol)-5;
    iHeight = ((rtPic.Height() - 5)/iRow)-5;
    CStatic *pstPic = NULL;

    //������Ϣ��ʾ��
    CRect rtList;
    //rtList.left = rtPic.right + 15;
    rtList.left = rt.left+10;
    rtList.top = rtPic.bottom + 5;
    rtList.right = rt.right - 10 + - rt.Width()/5;
    rtList.bottom = rt.bottom - 5;
    GetDlgItem(IDC_LIST_RUN_INFO)->MoveWindow(rtList);

    //��ť
    int HeightV = rt.Height()/5 / 9;
    //rtList.left = rtPic.right + 45;
    rtList.left = rtList.right + 45;
    rtList.top = rt.bottom - (rt.Height()/5) + HeightV;
    rtList.right = rt.right - 30;
    rtList.bottom = rt.bottom - rt.Height()/5 + 4 * HeightV;
    GetDlgItem(IDC_BUTTON_SHOW_SCENE_WIN)->MoveWindow(rtList);

    //rtList.left = rtPic.right + 45;
    rtList.left = rtList.left;
    rtList.top = rt.bottom - (rt.Height()/5) + 5 * HeightV;
    rtList.right = rt.right - 30;
    rtList.bottom = rt.bottom - rt.Height()/5 + 8 * HeightV;
    GetDlgItem(IDC_BUTTON2)->MoveWindow(rtList);

    if (iNum == m_vecPicItem.size())
    {
        return;
    }
    else
    {
        for (int index = 0; index < m_vecPicItem.size(); index++)
        {
            delete m_vecPicItem[index];
            m_vecPicItem[index]=NULL;
        }
        m_vecPicItem.clear();
    }

	m_LayRow = iRow;
	m_LayCol = iCol;
    for (int i = 0; i < iRow; i++)
    {
        for (int j = 0; j < iCol; j++)
        {
            if ((i+1) * (j+1) > iNum)
            {
				//m_LayRow = 1;
				//m_LayCol = 1;
                break;
            }
            xb = j*iWidth + 1;
            yb = i*iHeight + 1;
            xe = (j+1)*iWidth;
            ye = (i+1)*iHeight;
            // ����pic�ؼ�
            CRect rect;
            pstPic = new CStatic();
            pstPic->Create(NULL,WS_VISIBLE|SS_BITMAP|SS_BLACKFRAME|SS_CENTERIMAGE,CRect(xb,yb,xe,ye), GetDlgItem(IDC_STATIC_IMAGE), iPicID);
            pstPic->ShowWindow(SW_SHOW);
            pstPic->GetClientRect(rect);
            BindWindow((long)pstPic->m_hWnd,0, 0, xe-xb, ye - yb);//����ȥ����id,���,��Ⱥ͸߶�
			if (indexImage> config->iCameraNum-1)
			{
				indexImage = config->iCameraNum-1;
			}
			RestoreImagePart((long)pstPic->m_hWnd,indexImage, xe-xb, ye-yb,indexImage); //�����������������ʾ����
           // DispImage((long)pstPic->m_hWnd, indexImage,0);
            indexImage++;
            m_vecPicItem.push_back(pstPic);
        }
    }
	SYSTEM_PARAM* pSysParam = (SYSTEM_PARAM*)GetSysParamPoint();
	assert(pSysParam);
	pSysParam->MainImageWndVec = &m_vecPicItem;
}

void  CFlatFishDlg::GrabSceneInit()
{
	int SceneId = 0;
	SCENE_PKG pscene;
	bool sendflag = false;
	for (int i = 0; i < m_LayRow; i++)
	{
		for (int j = 0; j < m_LayCol; j++)
		{
			//step1 ���ݳ���ID�ҵ���Ӧ����
			GetSceneParam(SceneId, &pscene);   //indexImage ����SceneID;
			if ( 0 == pscene.ProcProcess)
			{
				//���ó�����̬���еĺ������Գ���IDΪ������������Ӧ�����е�ͼ��ɼ��������ɼ�ͼ��
				GrabWay2Image* pGrabWay2Image;
				pGrabWay2Image = (GrabWay2Image *)GetProcAddress(m_hSceneDebugDLL, "InitGrabWay2Sceneimage");
				pGrabWay2Image(SceneId,sendflag);			//�ɼ�����ʾ	
			}else
			{
				InitGrabSceneimage(SceneId); //�������漰��halocn��ͼ�񻺴�ָ��
			}
			//���������������ʾͼ��һ�������ʾһ�����ڣ���Ϊ���ͬʱֻ����һ��������ʾ��SceneId ��Ϊ �������
			// DispImage((long)m_vecPicItem.at(SceneId)->m_hWnd, SceneId,0);//���һ������Ϊһ�������еĵڼ���ͼ������
			 SceneId++;
		}
	}

}


void CFlatFishDlg::initStatusBar()
{
    m_statusBar.CreateEx(this, SBT_TOOLTIPS, CBRS_BOTTOM | WS_VISIBLE | WS_CHILD, AFX_IDW_STATUS_BAR); //����״̬��
    m_statusBar.GetStatusBarCtrl().SetMinHeight(20);//����״̬������С�߶�
    SYSTEM_PARAM *param;
   // GetSysParam(&param);
	param = (SYSTEM_PARAM *)GetSysParamPoint();
    m_IndicatorsSize += param->iCameraNum;
    m_IndicatorsSize += param->iLightNum;
    m_pIndicators = new UINT[m_IndicatorsSize];//״̬������Ϊ����͵ƹ���Ŀ
    m_pIndicators[0] = IDS_STRING_CONNECT_STATUS; //��һ��Ϊ��������״̬��
    int iBegin = 1;
    int iSize = param->iCameraNum + iBegin;//���״̬�ӵ�2��ʼ
    int j = 0;
    for (int i = iBegin; i < iSize; i++)
    {
        m_pIndicators[i] = IDS_STRING_CAMERA_0 + j; //����д����ע����״̬��ֻ��16�����,��Ϊֻ��IDS_STRING_CAMERA_0��15
        j++;
    }
    iBegin = iSize;
    iSize = iSize + param->iLightNum + 1; //���ž��ǵƹ��״̬,�ƹ�����ֻ��7��
    j = 0;
    for (int i = iBegin; i < iSize; i++)
    {
        m_pIndicators[i] = IDS_STRING_COMM_0 + j;
        j++;
    }
    m_pIndicators[m_IndicatorsSize - 1] = IDS_STRING_TIME; //���һ����ʱ��
    m_statusBar.SetIndicators(m_pIndicators, m_IndicatorsSize); //����״̬����Ŀ����������+���(���16)+�ƹ�(���8)+ʱ��
    m_statusBar.GetStatusBarCtrl().SetBkColor(BACKGROUND_COLOR_M);

    int iWidth = 0;
    CRect rt;
    GetClientRect(&rt);
    iWidth = rt.Width() / m_IndicatorsSize;
    for (int i = 0; i < m_IndicatorsSize - 1; i++)
    {
        m_statusBar.SetPaneInfo(i, m_pIndicators[i], SBPS_NORMAL, iWidth); //ƽ���ָ�״̬��

    }
    m_statusBar.SetPaneInfo(m_IndicatorsSize - 1, m_pIndicators[m_IndicatorsSize - 1], SBPS_STRETCH, 0);

    m_statusBar.GetStatusBarCtrl().SetBkColor(BACKGROUND_COLOR_BAR);//����״̬����ɫ
    //��Ļ�ϻ���״̬��ʱ��ָ�
    RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, IDS_STRING_TIME);

	isstatusready = true;//״̬��׼������,���Խ�����Ϣ��
    //��ʱˢ��ʱ��
    SetTimer(UPDATE_STATUS_TIMER, FLATFISH_STATUS_TIME, NULL);
	
}
 
void CFlatFishDlg::OnBnClickedButton2()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    SYSTEM_PARAM   *Sparam;
	Sparam = (SYSTEM_PARAM* )GetSysParamPoint();
    if (Sparam->bLandType)
    {
        m_LoginLevel = -1;
        ShowLoginDlg *pLoginDlg = NULL;
        if (NULL != m_hLoginDLL)
        {
            pLoginDlg = (ShowLoginDlg *)GetProcAddress(m_hLoginDLL, "ShowLoginDlg");//
            if (NULL != pLoginDlg)
            {
                m_LoginLevel = pLoginDlg();
            }
        }
    }
    else
    {
        m_LoginLevel = 2;
    }
	if (-1 <= m_LoginLevel)
	{
		 m_LoginLevel = 2;
	}
	 ////gpz
    if (1 < m_LoginLevel)
    {
       // msg_send(MSG_LOGIN_SYSTEM,&m_LoginLevel,MODULE_ALL, EVENT_LOGIN_MSG);
		msg_send(MSG_LOGIN_SYSTEM,&m_LoginLevel,sizeof(m_LoginLevel),MODULE_ALL);
        msg_post(RECV_UI_LOGIN_MSG,&m_LoginLevel,sizeof(int),MODULE_SCENE_MAIN_SDK);

        ShowDebugDlg *pShow;
        if (NULL != m_hSystemSettingDLL)
        {
            pShow = (ShowDebugDlg *)GetProcAddress(m_hSystemSettingDLL, "ShowSettingDebugDlg");
            if (NULL != pShow)
            {
                pShow(m_LoginLevel);
            }
        }
    }
    else if(0 <= m_LoginLevel)
    {
        AfxMessageBox("Ȩ�޲���, ���л�Ϊenginee");
    }
    else if (-1 == m_LoginLevel)
    {
        AfxMessageBox("�������");
    }
    else  //ȡ��
    {
        ;
    }
}
void CFlatFishDlg::OnBnClickedButton1()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    SYSTEM_PARAM*   Sparam;
	Sparam = (SYSTEM_PARAM* )GetSysParamPoint();
    if (Sparam->bLandType)
    {
        m_LoginLevel = -1;
        ShowLoginDlg *pLoginDlg = NULL;
        if (NULL != m_hLoginDLL)
        {
            pLoginDlg = (ShowLoginDlg *)GetProcAddress(m_hLoginDLL, "ShowLoginDlg");//
            if (NULL != pLoginDlg)
            {
                m_LoginLevel = pLoginDlg();
            }
        }
    }
    else
    {
        m_LoginLevel = 2;
    }
	if (-1 <= m_LoginLevel)
	{
		m_LoginLevel = 2;
	}	
    if (0 < m_LoginLevel)
    {
        msg_send(MSG_LOGIN_SYSTEM,&m_LoginLevel,sizeof(m_LoginLevel),MODULE_ALL);
        if (NULL != m_hSceneDebugDLL)
        {
            ShowDebugDlg *pShow;
            pShow = (ShowDebugDlg *)GetProcAddress(m_hSceneDebugDLL, "ShowSceneDebugDlg");
            if (NULL != pShow)
            {
                //msg_post(RECV_UI_LOGIN_MSG,&m_LoginLevel,sizeof(int),MODULE_SCENE_MAIN_SDK);
                pShow(m_LoginLevel);
            }
        }

    }
    else if(-4 == m_LoginLevel)
    {
        AfxMessageBox("Ȩ�޲���");
    }
    else if (-5 == m_LoginLevel)
    {
        AfxMessageBox("�������");
    }
    else  //ȡ��
    {
        ;
    }

}

void CFlatFishDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialogEx::OnShowWindow(bShow, nStatus);

    // TODO: �ڴ˴������Ϣ����������
    MoveWindow(0, 0, MAX_WIDTH, MAX_HEIGHT);
}

void CFlatFishDlg::OnOK()
{
    // TODO: �ڴ����ר�ô����/����û���

    /*if (IDNO == AfxMessageBox("ȷ��", MB_YESNO))
    {
        return;
    }*/
    CDialogEx::OnOK();
}

void CFlatFishDlg::OnCancel()
{
    // TODO: �ڴ����ר�ô����/����û���

    if (IDNO == AfxMessageBox("ȷ��", MB_YESNO))
    {
        return;
    }
    CDialogEx::OnCancel();
}


HBRUSH CFlatFishDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  �ڴ˸��� DC ���κ�����
    if(nCtlColor== IDC_LIST_RUN_INFO)
    {

        //hbr = CreateSolidBrush(RGB(255,0,255) );         //listbox������ɫ       
        //pDC->SetBkColor(RGB(255,0,255));
        //CRect rt;
        //GetWindowRect(&rt);
        //FillRect(*pDC, rt, hbr);
        return hbr;
    } 

    // TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
    return hbr;
}


BOOL CFlatFishDlg::DestroyWindow()
{
    // TODO: �ڴ����ר�ô����/����û���
    if (NULL != m_pIndicators)
    {
        delete m_pIndicators;
        m_pIndicators = NULL;
    }
    vector<CStatic *>::iterator it;
    CStatic *pTmp = NULL;
    for (it = m_vecPicItem.begin(); it != m_vecPicItem.end(); it++)
    {
        pTmp = *it;
        if (NULL != pTmp)
        {
            delete pTmp;
        }
    }
    UnInitManager();
    unregister_notify(MODULE_SCENE_MAIN_UI,OnSDKMessage);
    return CDialogEx::DestroyWindow();
}

void CFlatFishDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    switch (nIDEvent)
    {
    case UPDATE_STATUS_TIMER:
        {
            SYSTEMTIME st;
            char szBuf[64] = {0};
            GetLocalTime(&st);
            sprintf_s(szBuf, "%04d/%02d/%02d  %02d:%02d:%02d",st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
            m_statusBar.SetPaneText(m_IndicatorsSize-1, szBuf, 1);
        }
        break;
    }
    CDialogEx::OnTimer(nIDEvent);
}

void *CFlatFishDlg::GetImageBufByFile(char *path,int &size,int &x,int &h)
{
	CFileStatus fstatus; 
	CFile file; 
	LONG cb; 
	if (file.Open(path,CFile::modeRead)&&file.GetStatus(path,fstatus)&&((cb = (LONG)fstatus.m_size) != -1)) 
	{ 
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb); 
		LPVOID pvData = NULL; 
		if (hGlobal != NULL) 
		{ 
			pvData = GlobalLock(hGlobal);
			if (pvData != NULL) 
			{ 
				file.Read(pvData, cb); 
				GlobalUnlock(hGlobal); 
				size = cb;
				return pvData;
			}
		}
	}
	return NULL;
}


BOOL CFlatFishDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if(WM_KEYDOWN == pMsg->message && VK_ESCAPE == pMsg->wParam)
    {//����esc��
        return TRUE;
    }
    if(WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam)
    {//����enter
        return TRUE;
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
