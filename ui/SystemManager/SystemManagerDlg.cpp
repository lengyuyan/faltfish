// SystemManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "sysapi.h"//�����˻�ȡ·��
#include "SystemManager.h"
#include "SystemManagerDlg.h"
#include "afxdialogex.h"
#include "iManagerCtrl.h"
#include "DataType.h"
#include "LightManagerDlg.h"
#include "TcpIpProctocol.h"
#include "SysProductSet.h"
#include "SysCamCalConfig.h"
#include "SysAxisToolRelation.h"
#include "list"
#include "FuncDefine.h"
#include <string>
#include "map"
using namespace std;

#define  NO_PITCH    -1

list<SceneName> SceneNameSave;

CSystemManagerDlg *CSystemManagerDlg::m_pInstance = NULL;

// CSystemManagerDlg �Ի���
typedef void (cdecl ShowDebugDlg)(int);
typedef int (cdecl outdlg)(void);

IMPLEMENT_DYNAMIC(CSystemManagerDlg, CDialogEx)

CSystemManagerDlg::CSystemManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSystemManagerDlg::IDD, pParent)
{
    m_LoginLevels = 0;
    SceneChoose = NO_PITCH;
    CameraChoose = NO_PITCH;
    reCameraChoose = NO_PITCH;
    m_pInstance = this;
    m_hPasswordDLL = NULL;
}

CSystemManagerDlg::~CSystemManagerDlg()
{
}

void CSystemManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SCENEMODIFY, m_Scene);
	DDX_Control(pDX, IDC_LIST_CAMERACURRENT, m_Camera);
	DDX_Control(pDX, IDC_LIST_CAMERAMODIFY, m_Cameralist);
	DDX_Control(pDX, IDC_CHECK_LANDTYPE, m_CheckLandType);
	DDX_Control(pDX, IDC_EDIT_MODIFYDISPLAY, m_SceneModiEdit);
	DDX_Control(pDX, IDC_CHECK_IS_USER_COMM, m_AutoOperLine);
	DDX_Control(pDX, IDC_EDIT_AXISNUM, m_AxisNum);

	DDX_Control(pDX, IDC_SIMULATE_WORK, m_WorkSimulate);
}


BEGIN_MESSAGE_MAP(CSystemManagerDlg, CDialogEx)
    ON_NOTIFY(NM_CLICK, IDC_LIST_SCENEMODIFY, &CSystemManagerDlg::OnClickListSceneModify)
    ON_BN_CLICKED(IDC_BUTTON_ADDSCENE, &CSystemManagerDlg::OnBnClickedButtonAddscene)
    ON_BN_CLICKED(IDC_BUTTON_DELETE, &CSystemManagerDlg::OnBnClickedButtonDeletescene)
    ON_BN_CLICKED(IDC_BUTTON_UP, &CSystemManagerDlg::OnBnClickedButtonUpscene)
    ON_BN_CLICKED(IDC_BUTTON_DOWN, &CSystemManagerDlg::OnBnClickedButtonDownscene)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CAMERACURRENT, &CSystemManagerDlg::OnClickListCameraCurrent)
    ON_BN_CLICKED(IDC_BUTTON_ADDCAMERA, &CSystemManagerDlg::OnBnClickedAddCamera)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CAMERAMODIFY, &CSystemManagerDlg::OnClickListCameraModify)
    ON_BN_CLICKED(IDC_BUTTON_UPCAMERA, &CSystemManagerDlg::OnBnClickedButtonUpcamera)
    ON_BN_CLICKED(IDC_BUTTON_DOWNCAMERA, &CSystemManagerDlg::OnBnClickedButtonDowncamera)
    ON_BN_CLICKED(IDC_BUTTON_CAMERALISTOK, &CSystemManagerDlg::OnBnClickedButtonCameralistInstall)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_CAMERACURRENT, &CSystemManagerDlg::OnDblclkListCameraCurrent)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_CAMERAMODIFY, &CSystemManagerDlg::OnDblclkListCameraModify)
    ON_BN_CLICKED(IDC_BUTTON_SCENESAVE, &CSystemManagerDlg::OnBnClickedButtonScenesave)
    ON_BN_CLICKED(IDC_BUTTON_CAMERASEAVE, &CSystemManagerDlg::OnBnClickedButtonCameraseave)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BUTTON_RESETDISPLAY, &CSystemManagerDlg::OnBnClickedButtonResetdisplay)
    ON_BN_CLICKED(IDC_BUTTON_ALTERSCENENAME, &CSystemManagerDlg::OnBnClickedButtonAlterscenename)
    ON_BN_CLICKED(IDC_BUTTON_UPDATACAMERA, &CSystemManagerDlg::OnBnClickedButtonUpdatacamera)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_CHECK_LANDTYPE, &CSystemManagerDlg::OnBnClickedCheckLandtype)
    ON_BN_CLICKED(IDC_BUTTON_SAVEPATH, &CSystemManagerDlg::OnBnClickedButtonSavepath)
    ON_BN_CLICKED(IDC_BUTTON_LIGHTNUMMODIFY, &CSystemManagerDlg::OnBnClickedButtonLightnummodify)
    ON_BN_CLICKED(IDC_CHECK_IS_USER_COMM, &CSystemManagerDlg::OnBnClickedCheckIsUserComm)
    ON_BN_CLICKED(IDC_BUTTON_PASSWORDALTER, &CSystemManagerDlg::OnBnClickedButtonPasswordalter)
	ON_BN_CLICKED(IDC_SYSSETDLG_IPPROTOCOL, &CSystemManagerDlg::OnBnClickedSyssetdlgIpprotocol)
	ON_BN_CLICKED(IDC_SYSCONFIG_CALCAM, &CSystemManagerDlg::OnBnClickedSysconfigCalcam)
	ON_BN_CLICKED(IDC_SYSSETTING_PRODUCT, &CSystemManagerDlg::OnBnClickedSyssettingProduct)
	ON_BN_CLICKED(IDC_SYS_AXISTOOL, &CSystemManagerDlg::OnBnClickedSysAxistool)
	ON_BN_CLICKED(IDC_SIMULATE_WORK, &CSystemManagerDlg::OnBnClickedSimulateWork)
END_MESSAGE_MAP()


// CSystemManagerDlg ��Ϣ�������
BOOL CSystemManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    SetWindowText("ϵͳ�������ý���");
    SetBackgroundColor(BACKGROUND_COLOR_M);
    register_notify(MODULE_SYSTEMCONFIG_UI, OnConfigMessage,this);


	CString str;
	//SYSTEM_PARAM   Sparam;
	//GetSysParam(&Sparam);
	SYSTEM_PARAM* pSparam =(SYSTEM_PARAM*)GetSysParamPoint();

	str.Format("%d", pSparam->iWindowNum);
	//GetDlgItem(IDC_EDIT_WORKWINDOW)->SetWindowText(szBuf);
	SetDlgItemText(IDC_EDIT_WORKWINDOW, str); //�������ڸ���

	str.Format("%d", pSparam->iSceneNum);
	SetDlgItemText(IDC_EDIT_SCENENUM, str);   //��Ч��������

	str.Format("%d", pSparam->iCameraNum);
	SetDlgItemText(IDC_EDIT_CAMERANUM, str);//�������

	str.Format("%d", pSparam->iLightNum);
	SetDlgItemText(IDC_EDIT_LIGHTNUM, str); //��Դ����

	str.Format("%d",  pSparam->iAxisNum);
	SetDlgItemText(IDC_EDIT_AXISNUM, str); //�����

	SetDlgItemText(IDC_EDIT_IMAGESAVEPATH,  pSparam->ImageSavePath); //ͼƬ����·��

	SetDlgItemText(IDC_IPADDRESS_SERVERIP,  pSparam->stSocketParam.strServerIPAdd.c_str()); //����ͨ�Ų������ã�IP

	str.Format("%d",  pSparam->stSocketParam.ServerPort);
	SetDlgItemText(IDC_EDIT_PORT, str);//����ͨ�Ų������ã��˿�

    SetDlgItemText(IDC_IPADDRESS_CLIENTIP, pSparam->stSocketParam.strClientIPAdd.c_str()); //����ͨ�Ų������ã�IP

    str.Format("%d",  pSparam->stSocketParam.ClientPort);
    SetDlgItemText(IDC_EDIT_CLIENTPORT, str);//����ͨ�Ų������ã��˿�

	InitAllListctrl();
    InitPasswordDll();
	GetDlgItem(IDC_EDIT_SCENENUM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_CAMERANUM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_LIGHTNUM)->EnableWindow(FALSE); 
    SetTimer(0, LOGIN_TIME_OUT, 0);
    
	 pSparam->IsSimulate?m_WorkSimulate.SetCheck(1):m_WorkSimulate.SetCheck(0);

    if( pSparam->bLandType)
    {
        m_pInstance->m_CheckLandType.SetCheck(1);
    }
    if (0 <  pSparam->iAutoOL)
    {
        m_AutoOperLine.SetCheck(1);
    }
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE

}


void CSystemManagerDlg::InitPasswordDll()
{
    char szPath[512] = {0};
    get_module_path((LPSTR)szPath);
    CString strDll;
    strDll = szPath;
    strDll = strDll + "lib\\PasswordAlter.dll";
    m_hPasswordDLL = LoadLibrary(strDll);
}


BOOL CSystemManagerDlg::DestroyWindow()
{
    // TODO: �ڴ����ר�ô����/����û���
    int iType = 0;
    msg_post(RECV_UI_LOGIN_MSG,&iType,sizeof(int),MODULE_SCENE_MAIN_SDK);

    unregister_notify(MODULE_SYSTEMCONFIG_UI,OnConfigMessage);
    return CDialogEx::DestroyWindow();
}
BOOL CSystemManagerDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: �ڴ����ר�ô����/����û���
    if (pMsg->message == WM_LBUTTONDOWN || VK_RETURN == pMsg->wParam)
    {
        KillTimer(0);
        SetTimer(0, LOGIN_TIME_OUT,0);
    }

    return CDialogEx::PreTranslateMessage(pMsg);
}

void CSystemManagerDlg::OnClickListSceneModify(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    CString  str;

    if(pNMListView->iItem != NO_PITCH)
    {
        SceneItem = pNMListView->iItem;
        SceneSubItem = pNMListView->iSubItem;

        if (0 != SceneSubItem)
        {
            str =  m_Scene.GetItemText(SceneItem, SceneSubItem);
            SetDlgItemText(IDC_EDIT_MODIFYDISPLAY, str);
          //  GetDlgItem(IDC_EDIT_MODIFYDISPLAY)->SetFocus();
            m_SceneModiEdit.SetFocus();
            m_SceneModiEdit.SetSel(0, -1);
        }
        GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_UP)->EnableWindow(TRUE);

        SceneChoose = pNMListView->iItem;

        if((pNMListView->iItem == m_Scene.GetItemCount()-1) && (0 == pNMListView->iItem))
        {
            GetDlgItem(IDC_BUTTON_UP)->EnableWindow(FALSE);
            GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(FALSE);
        }

        else if(pNMListView->iItem == m_Scene.GetItemCount()-1)
        {
            GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(FALSE);
            GetDlgItem(IDC_BUTTON_UP)->EnableWindow(TRUE);
        }
        else if(0 == pNMListView->iItem)
        {
            GetDlgItem(IDC_BUTTON_UP)->EnableWindow(FALSE);
            GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(TRUE);
        }
    }

	*pResult = 0;

}

void CSystemManagerDlg::OnBnClickedButtonAddscene()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    AddScenePross();
    CString str = m_Scene.GetItemText(SceneChoose, 1);
    SetDlgItemText(IDC_EDIT_MODIFYDISPLAY, str);
    m_SceneModiEdit.SetFocus();
    m_SceneModiEdit.SetSel(0, -1);
    SceneSubItem = 1;
    //***********************************************************
	//�����ļ���
    CString strP;
    //ȡ���½��ĳ�����
    strP = m_Scene.GetItemText(SceneChoose, 1);
	char OldCurrentPath[512] = {0};
    char CreateCurrentPath[512] = {0};
    get_module_path((LPSTR)OldCurrentPath);
    strcpy_s(CreateCurrentPath,512,OldCurrentPath);
	//strcat_s(CreateCurrentPath,sizeof(CreateCurrentPath), "\\");
	strcat_s(CreateCurrentPath,sizeof(CreateCurrentPath), strP);
	if( CreateDirectory(CreateCurrentPath, NULL) )
	{
        //���ļ����������Ҫ��ģ��
        char IniCurrentPath[512] = {0};
        char IniTargetPath[512] = {0};
        char HvCurrentPath[512] = {0};   
        char HvTargetPath[512] = {0}; 
        //ini�ļ����ƶ�
        strcpy_s(IniCurrentPath,512,OldCurrentPath);
        strcat_s(IniCurrentPath,sizeof(IniCurrentPath), "\\demo\\config.ini");
        strcpy_s(IniTargetPath,512,CreateCurrentPath);
	    strcat_s(IniTargetPath,sizeof(IniTargetPath), "\\config.ini");
        CopyFile(IniCurrentPath,IniTargetPath,false);

        //hv���ƶ�
        strcpy_s(HvTargetPath,512,CreateCurrentPath);
        strcat_s(HvTargetPath,sizeof(HvTargetPath), "\\hdvp");
        CreateDirectory(HvTargetPath, NULL);
        strcat_s(HvTargetPath,sizeof(HvTargetPath), "\\process.hdvp");
        strcpy_s(HvCurrentPath,512, OldCurrentPath);
        strcat_s(HvCurrentPath,sizeof(HvCurrentPath), "\\demo\\hdvp\\process.hdvp");
        CopyFile(HvCurrentPath,HvTargetPath,false);
	}

}


void CSystemManagerDlg::AddScenePross()
{
    CString str;
	int i = m_Scene.GetItemCount();
	int iRow = m_Scene.InsertItem(i , "");
	str.Format("%d", i);
	m_Scene.SetItemText(iRow, 0, str);

	m_Scene.SetItemState(m_Scene.GetItemCount()-1, LVIS_SELECTED , LVIS_SELECTED );  
	SceneChoose = m_Scene.GetItemCount()-1;

    str.Format("scene%d", i);
    m_Scene.SetItemText(SceneChoose, 1, str);
    m_Scene.SetItemText(SceneChoose, 2, "0");
	//����һ���ṹ��
	 
	SCENE_PKG AddSceneData;
    SCENE_DEVICE_INFO  InitDeviceData;
    VIRSION_DATA_PKG   *pInitVirsionData = new VIRSION_DATA_PKG[2];
    unsigned int *pDataType = new unsigned int[2];
    
    AddSceneData.sceneID = m_Scene.GetItemCount()-1;
    AddSceneData.vecInfo.push_back(InitDeviceData);
    AddSceneData.vecDataType.push_back(pDataType);
    AddSceneData.vecBody.push_back(pInitVirsionData);

	memset(AddSceneData.szSceneName, 0, SCENE_SIZE);
	strcpy_s(AddSceneData.szSceneName,SCENE_SIZE, str);
	
	//pDatAdd.imageCount=1;
	SetSceneParam(&AddSceneData);

	if (1 == m_Scene.GetItemCount())
	{
		GetDlgItem(IDC_BUTTON_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_UP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(FALSE);
	}

}





void CSystemManagerDlg::OnBnClickedButtonDeletescene()
{
    if((SceneChoose == m_Scene.GetItemCount()-1) && (0 != SceneChoose))
    {
        m_Scene.DeleteItem(m_Scene.GetItemCount()-1);  //ֱ��ɾ��CListCtrl�е����һ��
        m_Scene.SetItemState(m_Scene.GetItemCount()-1, LVIS_SELECTED, LVIS_SELECTED );
        SceneChoose = m_Scene.GetItemCount()-1;
    }
    else if(1 == m_Scene.GetItemCount())
    {
        m_Scene.DeleteItem(0);
    }
    else
    {
        m_Scene.DeleteItem(SceneChoose);
        for(int i=SceneChoose; i<m_Scene.GetItemCount(); i++)
        {
            SCENE_PKG pData;
            GetSceneParam(i+1, &pData);
            --pData.sceneID;
            SetSceneParam(&pData);
        }

        int iEquit;
        iEquit = SceneChoose;
        CString str;
        for (int i=0; i<m_Scene.GetItemCount(); i++)
        {
            str.Format("%d", i);
            m_Scene.SetItemText(i, 0, str);
        }
        SceneChoose = iEquit;
        m_Scene.SetItemState(SceneChoose, LVIS_SELECTED , LVIS_SELECTED );
    }

}






void CSystemManagerDlg::OnBnClickedButtonUpscene()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


    int ChangeValue;
    SCENE_PKG pDataq;
    SCENE_PKG pDatah;
    GetSceneParam(SceneChoose, &pDataq);
    GetSceneParam(--SceneChoose, &pDatah);
    ChangeValue = pDatah.sceneID;
    pDatah.sceneID = pDataq.sceneID;
    pDataq.sceneID = ChangeValue;
    SetSceneParam(&pDataq);
    SetSceneParam(&pDatah);

    int icheck = ++SceneChoose;
    //ʵʱ��ʾ���Ͻ�������ѡ���еĳ�����������	
    CString str = m_Scene.GetItemText(SceneChoose, 1);//ȡ��ѡ���е�����
    CString str_change = m_Scene.GetItemText(--SceneChoose, 1);
    m_Scene.SetItemText(SceneChoose, 1, str);
    m_Scene.SetItemText(++SceneChoose, 1, str_change);

    //ʵʱ��ʾ���Ͻ�������ѡ���еĴ�������������	
    str = m_Scene.GetItemText(icheck, 2);//ȡ��ѡ���е�����
    str_change = m_Scene.GetItemText(--icheck, 2);
    m_Scene.SetItemText(icheck, 2, str);
    m_Scene.SetItemText(++icheck, 2, str_change);

    m_Scene.SetItemState(--SceneChoose, LVIS_SELECTED, LVIS_SELECTED );
    if(SceneChoose != m_Scene.GetItemCount()-1)
    {
        GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(TRUE);
    }
    if(0 == SceneChoose)
    {
        GetDlgItem(IDC_BUTTON_UP)->EnableWindow(FALSE);
    }
}

void CSystemManagerDlg::OnBnClickedButtonDownscene()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    int ChangeValue;
    SCENE_PKG pDataq;
    SCENE_PKG pDatah;
    GetSceneParam(SceneChoose, &pDataq);
    GetSceneParam(++SceneChoose, &pDatah);
    ChangeValue = pDatah.sceneID;
    pDatah.sceneID = pDataq.sceneID;
    pDataq.sceneID = ChangeValue;
    SetSceneParam(&pDataq);
    SetSceneParam(&pDatah);

    int icheck = --SceneChoose;
    //���������Ľ���
    CString str = m_Scene.GetItemText(SceneChoose, 1);//ȡ��ѡ���е�����
    CString str_change = m_Scene.GetItemText(++SceneChoose, 1);
    m_Scene.SetItemText(--SceneChoose, 1, str_change);
    m_Scene.SetItemText(++SceneChoose, 1, str);

    //���������Ľ���
    str = m_Scene.GetItemText(icheck, 2);//ȡ��ѡ���е�����
    str_change = m_Scene.GetItemText(++icheck, 2);
    m_Scene.SetItemText(--icheck, 2, str_change);
    m_Scene.SetItemText(++icheck, 2, str);

    m_Scene.SetItemState(SceneChoose, LVIS_SELECTED, LVIS_SELECTED );
    if(0 != SceneChoose)
    {
        GetDlgItem(IDC_BUTTON_UP)->EnableWindow(TRUE);
    }
    if(SceneChoose == m_Scene.GetItemCount()-1)
    {
        GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(FALSE);
    }

    //ʵʱ��ʾ���½�������ѡ���е�����
}
void CSystemManagerDlg::OnBnClickedButtonScenesave()
{
    SceneSave();
    SetSysParam(NULL);//����
	//CDialogEx::OnOK();
}

void CSystemManagerDlg::SceneSave()
{
	CString str_port;
	long Lnum;

	SYSTEM_PARAM  pScenenum;
	GetSysParam(&pScenenum);

    //�ļ���������
    if (!SceneNameSave.empty())
    {
        list<SceneName>::iterator it;
        //CString str;
        for (it = SceneNameSave.begin() ; it != SceneNameSave.end(); it++)
        {
            char szCurrentPath[512] = {0};
            char szCurrentPathmy[512] = {0};
            get_module_path((LPSTR)szCurrentPath);
            get_module_path((LPSTR)szCurrentPathmy);
            strcat_s(szCurrentPath,sizeof(szCurrentPath),it->Oldname);
            strcat_s(szCurrentPathmy,sizeof(szCurrentPath),it->Updataname);
            rename(szCurrentPath,szCurrentPathmy);
            
            //�ṹ���У��������ĸ���
        }
    }
    SceneNameSave.clear();

	//����һЩSys�������õĸ���
	GetDlgItemText(IDC_EDIT_WORKWINDOW,str_port);
	Lnum = atoi(str_port);
	if (str_port == "")
	{
		MessageBox("�������ڵĸ�������Ϊ��");
	}
	else if(Lnum<CAMERA_NUMBER && Lnum>=0)
	{
		pScenenum.iWindowNum = Lnum;//�������ڵĸ���
	}
	else
	{
		MessageBox("�������ڵĸ������ô���");
	}

	GetDlgItemText(IDC_EDIT_AXISNUM,str_port);
	pScenenum.iAxisNum = atoi(str_port);


	//****************************************************************
    //�����
	CString strIP;
	BYTE IP0,IP1,IP2,IP3;
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_SERVERIP))->GetAddress(IP0, IP1, IP2, IP3);
	strIP.Format(_T("%d.%d.%d.%d"),IP0, IP1, IP2, IP3);
	pScenenum.stSocketParam.strServerIPAdd = strIP;//����ͨ�Ų������ã�IP
	//*******************************************************************
	GetDlgItemText(IDC_EDIT_SERVERPORT,str_port);
	Lnum = atoi(str_port);
	if (str_port == "")
	{
		MessageBox("�˿ںŲ���Ϊ��");
	}
	else if(Lnum<65536 && Lnum>=0)
	{
		pScenenum.stSocketParam.ServerPort = atoi(str_port);//����ͨ�Ų������ã��˿�
	}
	else
	{
		MessageBox("�˿ں����ô���");
    }
    //****************************************************************
    //�ͻ���
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_CLIENTIP))->GetAddress(IP0, IP1, IP2, IP3);
	strIP.Format(_T("%d.%d.%d.%d"),IP0, IP1, IP2, IP3);
	pScenenum.stSocketParam.strClientIPAdd = strIP;//����ͨ�Ų������ã�IP
	//*******************************************************************
	GetDlgItemText(IDC_EDIT_CLIENTPORT,str_port);
	Lnum = atoi(str_port);
	if (str_port == "")
	{
		MessageBox("�˿ںŲ���Ϊ��");
	}
	else if(Lnum<65536 && Lnum>=0)
	{
		pScenenum.stSocketParam.ClientPort = atoi(str_port);//����ͨ�Ų������ã��˿�
	}
	else
	{
		MessageBox("�˿ں����ô���");
	}

    //**************************************************************
	//���������ṹ�����
	for(int i=0; i<m_Scene.GetItemCount(); i++)
	{
		pScenenum.pstrSceneName[i] = m_Scene.GetItemText(i,1).GetBuffer();
	}
	pScenenum.iSceneNum = m_Scene.GetItemCount();
    //**************************************************************
    SaveSysParam(&pScenenum);  
	//���������ͳ���������ȡ����

    SCENE_PKG  Getsceneparam;

    for (int i = 0; i < m_Scene.GetItemCount(); i++)
    {
        GetSceneParam(i, &Getsceneparam);//��ȡ�ṹ������

        memset(Getsceneparam.szSceneName, 0, SCENE_SIZE);
        Getsceneparam.stationID = atoi(m_Scene.GetItemText(i,2));
        strcpy_s(Getsceneparam.szSceneName, SCENE_SIZE, m_Scene.GetItemText(i,1).GetBuffer());
        SaveSceneParam(i,&Getsceneparam);
    }
 }


//camera
void CSystemManagerDlg::OnClickListCameraCurrent(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	GetDlgItem(IDC_BUTTON_ADDCAMERA)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_UPCAMERA)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DOWNCAMERA)->EnableWindow(FALSE);

	if(pNMListView->iItem != NO_PITCH)
	{ 
		CameraChoose = pNMListView->iItem;
	}
	*pResult = 0;
}


void CSystemManagerDlg::OnBnClickedAddCamera()
{

    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (m_Camera.GetItemCount() <= 0)
    {
        MessageBox("�޿������");
    }
    else
    {
        GetDlgItem(IDC_BUTTON_CAMERALISTOK)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_UPCAMERA)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_DOWNCAMERA)->EnableWindow(FALSE);

        CString str = m_Camera.GetItemText(CameraChoose, 0);//ȡ��ѡ���е�����
        CString strLindex;
        int iLindex = m_Cameralist.GetItemCount();
        strLindex.Format("%d", iLindex);
        m_Cameralist.SetItemText(m_Cameralist.InsertItem(iLindex ,""), 0, strLindex);
        m_Cameralist.SetItemText(iLindex, 1, str);

        for(int i=0; i<m_Cameralist.GetItemCount()-1; i++)
        {
            CString cstr = m_Cameralist.GetItemText(i, 1);
            if(0 == strcmp(cstr, str))
            {
                MessageBox("�Ѵ���ѡ����");
                m_Cameralist.DeleteItem(m_Cameralist.GetItemCount()-1);
            }
        }
        m_Cameralist.SetItemState(m_Cameralist.GetItemCount()-1, LVIS_SELECTED , LVIS_SELECTED );
        reCameraChoose = m_Cameralist.GetItemCount()-1;
    }
}


void CSystemManagerDlg::OnClickListCameraModify(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	GetDlgItem(IDC_BUTTON_ADDCAMERA)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UPCAMERA)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DOWNCAMERA)->EnableWindow(TRUE);

	if(pNMListView->iItem != NO_PITCH)
	{ 
		reCameraChoose = pNMListView->iItem;
	}
	if((pNMListView->iItem == m_Cameralist.GetItemCount()-1) && (0==pNMListView->iItem))
	{
		GetDlgItem(IDC_BUTTON_DOWNCAMERA)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UPCAMERA)->EnableWindow(FALSE);
	}
	else if(pNMListView->iItem == m_Cameralist.GetItemCount()-1)
	{
		GetDlgItem(IDC_BUTTON_DOWNCAMERA)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UPCAMERA)->EnableWindow(TRUE);
	}
	else if(0 == pNMListView->iItem)
	{
		GetDlgItem(IDC_BUTTON_UPCAMERA)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DOWNCAMERA)->EnableWindow(TRUE);
	}
	*pResult = 0;
}


void CSystemManagerDlg::Deletecamera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    int errornum = reCameraChoose;
	m_Cameralist.DeleteItem(reCameraChoose);     ///////?�������Camerachangenum�Զ���һ��
    reCameraChoose = errornum;

    //ɾ����ѡ����һ��
    if(1 == m_Cameralist.GetItemCount())
    {
        m_Cameralist.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED );
        reCameraChoose = 0;
    }

    else if(reCameraChoose == m_Cameralist.GetItemCount())
    {
        m_Cameralist.SetItemState(m_Cameralist.GetItemCount()-1, LVIS_SELECTED, LVIS_SELECTED );
        reCameraChoose = m_Cameralist.GetItemCount()-1;
    }
    else
    {
        m_Cameralist.SetItemState(reCameraChoose, LVIS_SELECTED, LVIS_SELECTED );
    }

	GetDlgItem(IDC_BUTTON_CAMERALISTOK)->EnableWindow(TRUE);
}



void CSystemManagerDlg::OnBnClickedButtonUpcamera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str = m_Cameralist.GetItemText(reCameraChoose, 1);//ȡ��ѡ���е�����
	CString str_change = m_Cameralist.GetItemText(--reCameraChoose, 1);
	
	m_Cameralist.SetItemText(reCameraChoose, 1, str);
	m_Cameralist.SetItemText(++reCameraChoose, 1, str_change);

	m_Cameralist.SetItemState(--reCameraChoose, LVIS_SELECTED, LVIS_SELECTED );
	GetDlgItem(IDC_BUTTON_CAMERALISTOK)->EnableWindow(TRUE);
}



void CSystemManagerDlg::OnBnClickedButtonDowncamera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString str = m_Cameralist.GetItemText(reCameraChoose, 1);//ȡ��ѡ���е�����
	CString str_change = m_Cameralist.GetItemText(++reCameraChoose, 1);

	m_Cameralist.SetItemText(--reCameraChoose, 1, str_change);
	m_Cameralist.SetItemText(++reCameraChoose, 1, str);
	m_Cameralist.SetItemState(reCameraChoose, LVIS_SELECTED, LVIS_SELECTED );

	GetDlgItem(IDC_BUTTON_CAMERALISTOK)->EnableWindow(TRUE);
}


void CSystemManagerDlg::OnBnClickedButtonCameralistInstall()
{   
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char szText[256] ={0};
	string szCamName;
	SYSTEM_PARAM *param = NULL;
	param = (SYSTEM_PARAM *)GetSysParamPoint();
	for(int q=0; q<CAMERA_NUMBER; q++)
	{ 
		param->pstrCamareGUID[q] = "";    //
	}
	for(int i=0; i<m_Cameralist.GetItemCount(); i++)
	{
		memset(szText,0,sizeof(szText));
	    memcpy(szText,m_Cameralist.GetItemText(i, 1),sizeof(szText));
		szCamName = szText;
		param->pstrCamareGUID[i] = szCamName;//ȡ��ѡ���е�����
	}
	param->iCameraNum = m_Cameralist.GetItemCount();
    SetSysParam(param);
	GetDlgItem(IDC_BUTTON_CAMERALISTOK)->EnableWindow(FALSE);
	SaveSysParam(param);
}

void CSystemManagerDlg::OnDblclkListCameraModify(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    Deletecamera();
	return;
    int iEquit;
    iEquit = reCameraChoose;
    CString str;
    for (int i=0; i<m_Cameralist.GetItemCount(); i++)
    {
        str.Format("%d", i);
        m_Cameralist.SetItemText(i, 0, str);
    }
    reCameraChoose = iEquit;
    m_Cameralist.SetItemState(reCameraChoose, LVIS_SELECTED, LVIS_SELECTED );
    *pResult = 0;
}

void CSystemManagerDlg::OnDblclkListCameraCurrent(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    OnBnClickedAddCamera();
    *pResult = 0;
}

void CSystemManagerDlg::OnBnClickedButtonCameraseave()  //�ĳ��������
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������  
	//for (int i = 0;i<m_Cameralist.GetItemCount();i++)
	//{
		m_Cameralist.DeleteAllItems();
		reCameraChoose = -1;
	//}

}

void CSystemManagerDlg::OnBnClickedButtonUpdatacamera()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    //****************************************************************
	//���¶�ȡ������к�
	list<string> cameralist;
	list<string>::iterator it;
#ifdef _TEST
	cameralist.push_back("99998");
	cameralist.push_back("99999");
	cameralist.push_back("99996");
	cameralist.push_back("99992");
	cameralist.push_back("99993");
#else
	SYSTEM_PARAM sysParam;

	GetSysParam(&sysParam);

	GetCameraIDList(cameralist,sysParam.iCameraDllNum,sysParam.pstrCameraDll);

#endif
    m_Camera.DeleteAllItems();
	for (it = cameralist.begin(); it!= cameralist.end(); it++) 
	{
		int k = 0;
		int iRow = m_Camera.InsertItem(k, "");
		m_Camera.SetItemText(iRow, 0, (*it).c_str());
		k++;
	}
}

void CSystemManagerDlg::OnBnClickedButtonResetdisplay()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ʱ��֪���ô�
    ////SceneInstall();
    ////msg_send(MSG_RESET_PARAMETER, 0, MODULE_SCENE_MAIN_UI); 
}

void CSystemManagerDlg::OnBnClickedButtonAlterscenename()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    CString str;
    CString qstr;
    bool qbool = true;
    GetDlgItemText(IDC_EDIT_MODIFYDISPLAY,str);
    if (str == "")
    {
        MessageBox("����������");
    }
    else
    {    
        for (int i = 0; i < m_Scene.GetItemCount(); i++)
        {
            if (str == m_Scene.GetItemText(i,1))
            {
                MessageBox("�˳��������Ѵ���");
                qbool = false;
            }
        }
        if (1 == SceneSubItem && qbool)
        {
            SceneName param;
            param.Oldname = m_Scene.GetItemText(SceneChoose,SceneSubItem);
            param.Updataname = str;
            SceneNameSave.push_back(param);
            m_Scene.SetItemText(SceneChoose,SceneSubItem,str);
        }
        else if (2 == SceneSubItem)
        {
            SYSTEM_PARAM   Sparam;
            GetSysParam(&Sparam);
            if (atoi(str) < Sparam.iSceneNum)
            {
                 m_Scene.SetItemText(SceneChoose,SceneSubItem,str);
            }
            else
            {
                MessageBox("����������");
            }
        }
    }

}

void CSystemManagerDlg::OnBnClickedCheckLandtype()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int status = m_CheckLandType.GetCheck();
    bool LandType = true;
    SYSTEM_PARAM   Sparam;
    GetSysParam(&Sparam);
    if (1 == status)
    {
        LandType = true;
    }
    else
    {
        LandType = false;
    }
    Sparam.bLandType = LandType;
    SetSysParam(&Sparam);
}

void CSystemManagerDlg::OnBnClickedButtonSavepath()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    //ѡ���ļ���·������
    BROWSEINFO  bi;
    bi.hwndOwner=NULL;
    bi.pidlRoot=NULL;
    bi.pszDisplayName=NULL;
    bi.lpszTitle=NULL;
    bi.ulFlags=0;
    bi.lpfn =NULL;
    bi.iImage =0;
    LPCITEMIDLIST pidl=SHBrowseForFolder(&bi);
    if(!pidl)
    {
        return;
    }
    TCHAR  szDisplayName[255];
    SHGetPathFromIDList(pidl,szDisplayName);
    SetDlgItemText(IDC_EDIT_IMAGESAVEPATH,szDisplayName);
    
    //��ѡ����ļ���·�����б���
    SYSTEM_PARAM   Sparam;
    GetSysParam(&Sparam);
    strcpy_s(Sparam.ImageSavePath, 256, szDisplayName);
    SaveSysParam(&Sparam);
}


void CSystemManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    OnOK();
    CDialogEx::OnTimer(nIDEvent);
}

void CSystemManagerDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnClose();
}
//////////////////////////////////////////////////////////////

void CSystemManagerDlg::SceneInstall()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SceneModifyInstall();
	//SortPortInstall();
}

void CSystemManagerDlg::SceneModifyInstall()
{
	CString str_port;
	long Lnum;

	SYSTEM_PARAM  pScenenum;
	GetSysParam(&pScenenum);

	//����һЩSys�������õĸ���
	GetDlgItemText(IDC_EDIT_WORKWINDOW, str_port);
	Lnum = atoi(str_port);
	if (str_port == "")
	{
		MessageBox("�������ڵĸ�������Ϊ��");
	}
	else if(Lnum<CAMERA_NUMBER && Lnum>0)
	{
		pScenenum.iWindowNum = Lnum;//�������ڵĸ���
	}
	else
	{
		MessageBox("�������ڵĸ������ô���");
	}
	//****************************************************************
    //�����
	CString strIP;
	BYTE IP0,IP1,IP2,IP3;
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_SERVERIP))->GetAddress(IP0, IP1, IP2,IP3);
	strIP.Format(_T("%d.%d.%d.%d"),IP0, IP1, IP2, IP3);
	pScenenum.stSocketParam.strServerIPAdd = strIP;//����ͨ�Ų������ã�IP
	//*******************************************************************
	GetDlgItemText(IDC_EDIT_SERVERPORT, str_port);
	Lnum = atoi(str_port);
	if (str_port == "")
	{
		MessageBox("�˿ں����ò���Ϊ��");
	}
	else if(Lnum<=65535 && Lnum>=0)
	{
		pScenenum.stSocketParam.ServerPort = atoi(str_port);//����ͨ�Ų������ã��˿�
	}
	else
	{
        MessageBox("�˿ں����ô���");
    }
    //*******************************************************************
    //�ͻ���
    ((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_CLIENTIP))->GetAddress(IP0, IP1, IP2, IP3);
    strIP.Format(_T("%d.%d.%d.%d"), IP0, IP1, IP2, IP3);
    pScenenum.stSocketParam.strClientIPAdd = strIP;//����ͨ�Ų������ã�IP

    GetDlgItemText(IDC_EDIT_CLIENTPORT, str_port);
    Lnum = atoi(str_port);
    if (str_port == "")
    {
        MessageBox("�˿ں����ò���Ϊ��");
    }
    else if(Lnum<=65535 && Lnum>=0)
    {
		pScenenum.stSocketParam.ClientPort = atoi(str_port);//����ͨ�Ų������ã��˿�
	}
	else
	{
		MessageBox("�˿ں����ô���");
	}
	SetSysParam(&pScenenum);
	//**************************************************************
	//���������ͳ���������ȡ����

    SCENE_PKG  Getsceneparam;
    for (int i = 0; i<m_Scene.GetItemCount(); i++)
    {
        GetSceneParam(i, &Getsceneparam);//��ȡ�ṹ������

        for(int j=0; j < SCENE_SIZE ; j++)
        {
            Getsceneparam.szSceneName[j] = 0;
        }
        Getsceneparam.stationID = atoi(m_Scene.GetItemText(i,2).GetBuffer());
        strcpy_s(Getsceneparam.szSceneName, SCENE_SIZE,m_Scene.GetItemText(i,1).GetBuffer());

        SetSceneParam(&Getsceneparam);
    }
    
	//**************************************************************
	//���������ṹ�����

	SYSTEM_PARAM pDatam;
	GetSysParam(&pDatam);
	for(int i=0; i<m_Scene.GetItemCount(); i++)
	{
		pDatam.pstrSceneName[i] = m_Scene.GetItemText(i,1).GetBuffer();
	}
	pDatam.iSceneNum = m_Scene.GetItemCount();
	SetSysParam(&pDatam);    
}


bool CSystemManagerDlg::DeleteDirFile(char* psDirName)
{
	CFileFind FileFinder;
	char psFindName[1024] = {0};
	sprintf_s(psFindName,sizeof(psFindName),"%s\\*.*",psDirName);
	BOOL IsFinded = FileFinder.FindFile(psFindName);
	while(IsFinded)
	{
		IsFinded = FileFinder.FindNextFile();
		if(!FileFinder.IsDots())
		{
			char NameBuf[1024] = {0};
			char NextNameBuf[1024] = {0};
			strcpy_s(NameBuf, sizeof(NameBuf), FileFinder.GetFileName());
			sprintf_s(NextNameBuf, sizeof(NextNameBuf), "%s\\%s",psDirName,NameBuf);
			if(FileFinder.IsDirectory() && !FileFinder.IsDots())//�鿴�Ƿ���Ŀ¼
			{
				DeleteDirFile(NextNameBuf);
			}
			else
			{
				DeleteFile(NextNameBuf); 
			}
			RemoveDirectory(NextNameBuf);
		}
	}
	return true;
}

int WINAPI CSystemManagerDlg::OnConfigMessage( unsigned int wParam, void *lParam, void* otherparam)
{
    switch (wParam)
    {
    case MSG_LANDTYPE_INFORMATION:
        {
        }
        break;
    default:
        {
            
        }
        break;
    }
    return 0;
}

void CSystemManagerDlg::OnBnClickedButtonLightnummodify()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CLightDebugDlg dlg;
    dlg.DoModal();
}

void CSystemManagerDlg::InitAllListctrl()
{
    CRect rt;
	CString str;

	InitListctrl(&m_Scene);//�����޸ĵ�ListCtrl��ʼ��
    m_Scene.GetWindowRect(rt);
    m_Scene.InsertColumn(0,"��������" , 0, (rt.Width() - 4)/4, 10);
    m_Scene.InsertColumn(1,"��������" , 0, (rt.Width() - 4)/2, 10);
    m_Scene.InsertColumn(2,"��������" , 0, (rt.Width() - 4)/4, 10); 

	InitListctrl(&m_Camera);//�����ʼ����ListCtrl��ʼ��
    m_Camera.GetWindowRect(rt);
    m_Camera.InsertColumn(0,"���" , 0, rt.Width() - 4, 10);


	InitListctrl(&m_Cameralist);//���������ListCtrl��ʼ��
    m_Cameralist.GetWindowRect(rt);
    m_Cameralist.InsertColumn(0,"����" , 0, (rt.Width() - 4)/2, 10);
    m_Cameralist.InsertColumn(1,"���ID" , 0, (rt.Width() - 4)/2, 10);


	//***************************************************************
	//�������޸ĵ����ݳ�ʼ����ȥ
	SYSTEM_PARAM   Sparam;
	GetSysParam(&Sparam);

        SCENE_PKG pData;
		for(int i=0; i<Sparam.iSceneNum ; i++)
		{
            GetSceneParam(i, &pData);
			int iRow = m_Scene.InsertItem(i , "");
			str.Format("%d", i);
			m_Scene.SetItemText(iRow, 0, str);
            m_Scene.SetItemText(iRow, 1, (Sparam.pstrSceneName[i]).c_str());
            str.Format("%d", pData.stationID);
            m_Scene.SetItemText(iRow, 2, str);
		}
		//��ʼ�������޸�
		m_Scene.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED );
		SceneChoose = 0;
	//****************************************************************
	//����������ID�ȳ�ʼ����ȥ
	list<string> cameralist;
	list<string>::iterator it;
#ifdef _TEST
	cameralist.push_back("77778");
	cameralist.push_back("77779");
	cameralist.push_back("77776");
	cameralist.push_back("77772");
	cameralist.push_back("77773");
#else
	SYSTEM_PARAM sysParam;

	GetSysParam(&sysParam);

	GetCameraIDList(cameralist,sysParam.iCameraDllNum,sysParam.pstrCameraDll);
#endif
	for (it = cameralist.begin(); it!= cameralist.end(); it++) 
	{
		int k = 0;
		int iRow = m_Camera.InsertItem(k , "");
		m_Camera.SetItemText(iRow, 0, (*it).c_str());
		k++;
	}
	//***************************************************************
	//��m_Cameralist�г�ʼ��
	for(int i=0; i<Sparam.iCameraNum; i++)
	{
		int iRow = m_Cameralist.InsertItem(i , "");
		m_Cameralist.SetItemText(iRow, 1, (Sparam.pstrCamareGUID[i]).c_str());
        str.Format("%d", i);
		m_Cameralist.SetItemText(iRow, 0, str);
	}
    m_Cameralist.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED );
    reCameraChoose = 0;
	//****************************************************************
	//********************************************************************************
    GetDlgItem(IDC_BUTTON_UP)->EnableWindow(FALSE);
}

void CSystemManagerDlg::InitListctrl(CListCtrl *Listctrl)
{
    CRect rt;
    Listctrl->GetWindowRect(rt);
    DWORD LdwStyle = Listctrl->GetExtendedStyle();
    LdwStyle = Listctrl->GetExtendedStyle();
    LdwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
    LdwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
    Listctrl->SetExtendedStyle(LdwStyle); //������չ���
    Listctrl->ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL);//����ĳһ��ѡ��
}

void CSystemManagerDlg::OnBnClickedCheckIsUserComm()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int iCheck = m_AutoOperLine.GetCheck();
    SYSTEM_PARAM   Sparam;
    GetSysParam(&Sparam);
    Sparam.iAutoOL = iCheck;
    SetSysParam(&Sparam);
}


void CSystemManagerDlg::OnBnClickedButtonPasswordalter()
{
    //TODO: �ڴ���ӿؼ�֪ͨ����������
    ShowDebugDlg *pShow;
    if (NULL != m_hPasswordDLL)
    {
        pShow = (ShowDebugDlg *)GetProcAddress(m_hPasswordDLL, "outdlg");
        if (NULL != pShow)
        {
            pShow(2);
        }
    }
}


void CSystemManagerDlg::OnBnClickedSyssetdlgIpprotocol()
{
	// TODO: Add your control notification handler code here
	TcpIpProctocol* ProctocolSet = new TcpIpProctocol;
	ProctocolSet->Create(IDD_SYS_WEBPROCTOCOL,this);
}


void  CSystemManagerDlg::SetCommunitProctocol(const PROCTOCOLDATA&  SetProData)
{
	//PROCTOCOLDATA     SetProData;
	//sprintf_s(SetProData.CalRevHead,"%s",CalRevHead);
	//sprintf_s(SetProData.WorkRevHead,"%s",WorkRevHead);
	//sprintf_s(SetProData.CalSendHead,"%s",CalSendHead);
	//sprintf_s(SetProData.WorkSendHead,"%s",WorkSendHead);
	//sprintf_s(SetProData.IPDivid,"%s",IPDivid);
	//sprintf_s(SetProData.IPEnd,"%s",IPEnd);
	//SetProData.MakeEndable();
	SetWebProctocol(SetProData);	
}

void CSystemManagerDlg::GetCommunitProctocol(PROCTOCOLDATA&  SetProData)
{
	GetWebProctocol(SetProData);
}

void CSystemManagerDlg::OnBnClickedSysconfigCalcam()
{
	// TODO: Add your control notification handler code here
	SysCamCalConfig* pCamCalConfig = new SysCamCalConfig;

	pCamCalConfig->Create(IDD_CAMCAL_DLG,this);
	

}
void CSystemManagerDlg::GetCamCalData(int CamIndex,int AxiIndex)
{
	//��δʵ�֡���Ҫ��������궨���˻����档


}



//��Ʒ���趨���������ڻ���Ʒʱһ���л�
void CSystemManagerDlg::SetSysProductMap(void* pMapData)
{
	SetProductMap(pMapData);
	
}


void CSystemManagerDlg::GetSysProductMap(void* pMapData)
{
	GetProductMap(pMapData);
}


void CSystemManagerDlg::SetCurProductName(string ProductName)
{
	SetSysCurProductName(ProductName);

}



void CSystemManagerDlg::OnBnClickedSyssettingProduct()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SysProductSet* pProductSet;
	pProductSet = new SysProductSet;
	pProductSet->Create(IDD_PRODUCT_CONFIG,this);
	//pProductSet->DoModal();

}


void CSystemManagerDlg::OnBnClickedSysAxistool()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	SysAxisToolRelation* pAxisToolRel;
	pAxisToolRel = new SysAxisToolRelation;

	pAxisToolRel->Create(IDD_SYS_AXIS_TOOL,this);

}


void CSystemManagerDlg::OnBnClickedSimulateWork()
{
	// TODO: Add your control notification handler code here
	int CheckState = m_WorkSimulate.GetCheck();
	 SYSTEM_PARAM* pSysParam = (SYSTEM_PARAM*)GetSysParamPoint();
    if (0 == CheckState)
    {
		//m_WorkSimulate.SetCheck(0);
		pSysParam->IsSimulate = false;	
    }else
	{
		//m_WorkSimulate.SetCheck(1);
		pSysParam->IsSimulate = true;
	}


}
