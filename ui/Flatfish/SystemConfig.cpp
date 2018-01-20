// SystemConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Flatfish.h"
#include "SystemConfig.h"
#include "afxdialogex.h"
#include "iManagerCtrl.h"
#include "DataType.h"
#include "list"
#include <string>
#include "SceneDebugDlg.h"
#include "map"

using namespace std;
#define  VALUE_NUM   64
#define  NO_PITCH    -1
//map<CString, CString> SceneNameSave;
//map<CString, CString> StationSave;
list<SceneName> SceneNameSave;
static bool LandType = true;

SystemConfig *SystemConfig::m_pInstance = NULL;
IMPLEMENT_DYNAMIC(SystemConfig, CDialogEx)

	SystemConfig::SystemConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(SystemConfig::IDD, pParent)
{
	 SceneChoose = NO_PITCH;
	 CameraChoose = NO_PITCH;
	 reCameraChoose = NO_PITCH;
	 DoPortChoose = NO_PITCH;
	 SortPortItem = NO_PITCH;     
	 SortPortSubItem = NO_PITCH;
     m_pInstance = this;
}

SystemConfig::~SystemConfig()
{
}

void SystemConfig::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_SCENEMODIFY, m_Scene);
    DDX_Control(pDX, IDC_LIST_CAMERACURRENT, m_Camera);
    DDX_Control(pDX, IDC_LIST_CAMERAMODIFY, m_Cameralist);
    DDX_Control(pDX, IDC_LIST_DOPORT, m_DoPort);
    DDX_Control(pDX, IDC_LIST_SERIALPORT, m_SortPort);
    DDX_Control(pDX, IDC_CHECK_LANDTYPE, m_CheckLandType);
    DDX_Control(pDX, IDC_EDIT_MODIFYDISPLAY, m_SceneModiEdit);
    DDX_Control(pDX, IDC_EDIT_LIGHTNUMMODIFY, m_PortModiEdit);
}

BEGIN_MESSAGE_MAP(SystemConfig, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SCENEMODIFY, &SystemConfig::OnClickListSceneModify)
	ON_BN_CLICKED(IDC_BUTTON_ADDSCENE, &SystemConfig::OnBnClickedButtonAddscene)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &SystemConfig::OnBnClickedButtonDeletescene)
	ON_BN_CLICKED(IDC_BUTTON_UP, &SystemConfig::OnBnClickedButtonUpscene)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, &SystemConfig::OnBnClickedButtonDownscene)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CAMERACURRENT, &SystemConfig::OnClickListCameraCurrent)
	ON_BN_CLICKED(IDC_BUTTON_ADDCAMERA, &SystemConfig::OnBnClickedAddCamera)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CAMERAMODIFY, &SystemConfig::OnClickListCameraModify)
	ON_BN_CLICKED(IDC_BUTTON_UPCAMERA, &SystemConfig::OnBnClickedButtonUpcamera)
	ON_BN_CLICKED(IDC_BUTTON_DOWNCAMERA, &SystemConfig::OnBnClickedButtonDowncamera)
	ON_BN_CLICKED(IDC_BUTTON_CAMERALISTOK, &SystemConfig::OnBnClickedButtonCameralistInstall)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CAMERACURRENT, &SystemConfig::OnDblclkListCameraCurrent)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CAMERAMODIFY, &SystemConfig::OnDblclkListCameraModify)
	ON_BN_CLICKED(IDC_BUTTON_SCENESAVE, &SystemConfig::OnBnClickedButtonScenesave)
    ON_BN_CLICKED(IDC_BUTTON_CAMERASEAVE, &SystemConfig::OnBnClickedButtonCameraseave)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DOPORT, &SystemConfig::OnDblclkListDoport)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DOPORT, &SystemConfig::OnClickListDoport)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SERIALPORT, &SystemConfig::OnDblclkListSerialport)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SERIALPORT, &SystemConfig::OnClickListSerialport)
	ON_BN_CLICKED(IDC_BUTTON_UPSORTPORT, &SystemConfig::OnBnClickedButtonUpsortport)
	ON_BN_CLICKED(IDC_BUTTON_DOWNSORTPORT, &SystemConfig::OnBnClickedButtonDownsortport)
	ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BUTTON_RESETDISPLAY, &SystemConfig::OnBnClickedButtonResetdisplay)
    ON_BN_CLICKED(IDC_BUTTON_ALTERSCENENAME, &SystemConfig::OnBnClickedButtonAlterscenename)
    ON_BN_CLICKED(IDC_BUTTON_UPDATACAMERA, &SystemConfig::OnBnClickedButtonUpdatacamera)
    ON_BN_CLICKED(IDC_BUTTON_ADDPORT, &SystemConfig::OnBnClickedButtonAddport)
    ON_BN_CLICKED(IDC_BUTTON_LIGHTNUMMODIFY, &SystemConfig::OnBnClickedButtonLightnummodify)
    ON_BN_CLICKED(IDC_BUTTON_UPDATAPORT, &SystemConfig::OnBnClickedButtonUpdataport)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_CHECK_LANDTYPE, &SystemConfig::OnBnClickedCheckLandtype)
    ON_BN_CLICKED(IDC_BUTTON_SAVEPATH, &SystemConfig::OnBnClickedButtonSavepath)
END_MESSAGE_MAP()


// SystemConfig ��Ϣ�������
BOOL SystemConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    SetWindowText("ϵͳ�������ý���");
    SetBackgroundColor(BACKGROUND_COLOR_M);
    //::ShowWindow(::GetDlgItem(m_hWnd,IDC_LIST_DOPORT),SW_HIDE);
    //::ShowWindow(::GetDlgItem(m_hWnd,IDC_LIST_SERIALPORT),SW_HIDE);
    //::ShowWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_UPSORTPORT),SW_HIDE);
    //::ShowWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_DOWNSORTPORT),SW_HIDE);
    //::ShowWindow(::GetDlgItem(m_hWnd,IDC_STATIC_PORT),SW_HIDE);
    register_notify(MODULE_SYSTEMCONFIG_UI, OnConfigMessage);


	CString str;
	SYSTEM_PARAM   Sparam;
	GetSysParam(&Sparam);

	str.Format("%d", Sparam.iWindowNum);
	//GetDlgItem(IDC_EDIT_WORKWINDOW)->SetWindowText(szBuf);
	SetDlgItemText(IDC_EDIT_WORKWINDOW, str); //�������ڸ���

	str.Format("%d", Sparam.iSceneNum);
	SetDlgItemText(IDC_EDIT_SCENENUM, str);   //��Ч��������

	str.Format("%d", Sparam.iCameraNum);
	SetDlgItemText(IDC_EDIT_CAMERANUM, str);//�������

	str.Format("%d", Sparam.iLightNum);
	SetDlgItemText(IDC_EDIT_LIGHTNUM, str); //��Դ����

	SetDlgItemText(IDC_EDIT_IMAGESAVEPATH, Sparam.ImageSavePath.c_str()); //ͼƬ����·��

	SetDlgItemText(IDC_IPADDRESS_SERVERIP, Sparam.stSocketParam.strServerIPAdd.c_str()); //����ͨ�Ų������ã�IP

	str.Format("%d", Sparam.stSocketParam.ServerPort);
	SetDlgItemText(IDC_EDIT_PORT, str);//����ͨ�Ų������ã��˿�

    SetDlgItemText(IDC_IPADDRESS_CLIENTIP,Sparam.stSocketParam.strClientIPAdd.c_str()); //����ͨ�Ų������ã�IP

    str.Format("%d", Sparam.stSocketParam.ClientPort);
    SetDlgItemText(IDC_EDIT_CLIENTPORT, str);//����ͨ�Ų������ã��˿�

	InitAllListctrl();
	GetDlgItem(IDC_EDIT_SCENENUM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_CAMERANUM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_LIGHTNUM)->EnableWindow(FALSE); 
    SetTimer(0, LOGIN_TIME_OUT, 0);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE

}


int WINAPI SystemConfig::OnConfigMessage( unsigned int wParam, void *lParam, SYSTEM_MSG_TYPE msgType)
{
    switch (wParam)
    {
    case MSG_LANDTYPE_INFORMATION:
        {
            LandType = *(bool *)lParam;
            if(LandType)
            {
              m_pInstance->m_CheckLandType.SetCheck(1);
            }
        }
        break;
    default:
        {
            
        }
        break;
    }
    return 0;
}

void SystemConfig::InitAllListctrl()
{
    CRect rt;
	CString str;

	InitListctrl(&m_Scene);//�����޸ĵ�ListCtrl��ʼ��
    m_Scene.GetWindowRect(rt);
    m_Scene.InsertColumn(0,"��������" , 0, (rt.Width() - 4)/4, 10);
    m_Scene.InsertColumn(1,"��������" , 0, (rt.Width() - 4)/2, 10);
    m_Scene.InsertColumn(2,"��������" , 0, (rt.Width() - 4)/4, 10);


    InitListctrl(&m_SortPort);
	m_SortPort.GetWindowRect(rt);
    m_SortPort.InsertColumn(0,"����" , 0, (rt.Width() - 4)/3, 10);
    m_SortPort.InsertColumn(1,"��������" , 0, (rt.Width() - 4)/3, 10);
    m_SortPort.InsertColumn(2,"��Դͨ����" , 0, (rt.Width() - 4)/3, 10);

    InitListctrl(&m_DoPort);
    m_DoPort.GetWindowRect(rt);
    m_DoPort.InsertColumn(0,"����" , 0, rt.Width() - 4, 10);


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
	GetCameraIDList(cameralist);
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
	//m_DoPort��ʼ��
	CString strSerialList;
	string strSerialPort[256];
	int k=0;
#ifdef _TESTPORT
	strSerialList = "COM1.COM2.COM3.COM4.COM5.COM6";	
#else
    strSerialList = GetPortNum();
#endif
    if(0!=strcmp(strSerialList,""))
    {
        for(int i=0; i<strSerialList.GetLength(); i++)
        {
            if(strSerialList[i] == '.')
            {
                k++;
            }
            else
            {
                strSerialPort[k].push_back(strSerialList[i]);
            }
        }
        for(int i=0; i<k+1; i++)
        {
            int iRow = m_DoPort.InsertItem(i , "");
            m_DoPort.SetItemText(iRow, 0, strSerialPort[i].c_str());
        }
    }
	//********************************************************************************
	//m_SortPort��ʼ��
	for (int i=0; i<Sparam.iLightNum; i++)
	{
		int iRow = m_SortPort.InsertItem(i , "");
        str.Format("%d", i);
		m_SortPort.SetItemText(iRow, 0, str);
		m_SortPort.SetItemText(iRow, 1, (Sparam.strLightInfo[i].PortName).c_str());
        str.Format("%d", Sparam.strLightInfo[i].Channel);
        m_SortPort.SetItemText(iRow, 2, str);
	}
    m_SortPort.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED );
    SortPortItem = 0;
	//*********************************************************************************
	GetDlgItem(IDC_BUTTON_UP)->EnableWindow(FALSE);
}




void SystemConfig::InitListctrl(CListCtrl *Listctrl)
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



CString SystemConfig::GetPortNum()
{
	CString strPortName;
	string strSerialList;
	HKEY hKey;
	LPCTSTR data_Set = "HARDWARE\\DEVICEMAP\\SERIALCOMM\\";
	//LPCTSTR data_Set="HARDWARE\\DEVICEMAP\\PARALLEL PORTS\\";  //����
	LONG ret0 = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_Set, 0, KEY_READ, &hKey);
	if (ret0 != ERROR_SUCCESS)
	{
		return FALSE;
	}
	char Name[25];
	UCHAR szPortName[25];
	LONG Status;
	DWORD dwIndex = 0;
	DWORD dwName;
	DWORD dwSizeofPortName;
	DWORD type;
	dwName = sizeof(Name);
	dwSizeofPortName = sizeof(szPortName);
	do 
	{
        Status = RegEnumValue(hKey,dwIndex++,Name,&dwName,NULL,&type,szPortName,&dwSizeofPortName);
        if ((Status == ERROR_SUCCESS)||(Status == ERROR_MORE_DATA))
        {
            HANDLE hPort = ::CreateFile(CString(szPortName), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
            strPortName.Format("%s",szPortName);
            if (!JudgeLightConnectStatus(strPortName.GetBuffer()))
            {
                strSerialList = strSerialList + strPortName.GetBuffer() + ".";
                CloseHandle(hPort);
            }
        }
	} while ((Status == ERROR_SUCCESS)||(Status == ERROR_MORE_DATA));
	RegCloseKey(hKey);

	strPortName.Format("%s", strSerialList.c_str());
	return strPortName;
}



void SystemConfig::OnClickListSceneModify(NMHDR *pNMHDR, LRESULT *pResult)
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


void SystemConfig::OnBnClickedButtonAddscene()
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
	GetCurrentDirectory(512, OldCurrentPath);
    strcpy(CreateCurrentPath,OldCurrentPath);
	strcat_s(CreateCurrentPath,sizeof(CreateCurrentPath), "\\");
	strcat_s(CreateCurrentPath,sizeof(CreateCurrentPath), strP);
	if( CreateDirectory(CreateCurrentPath, NULL) )
	{
        //���ļ����������Ҫ��ģ��
        char IniCurrentPath[512] = {0};
        char IniTargetPath[512] = {0};
        char HvCurrentPath[512] = {0};   
        char HvTargetPath[512] = {0}; 
        //ini�ļ����ƶ�
        strcpy(IniCurrentPath,OldCurrentPath);
        strcat_s(IniCurrentPath,sizeof(IniCurrentPath), "\\demo\\config.ini");
        strcpy(IniTargetPath,CreateCurrentPath);
	    strcat_s(IniTargetPath,sizeof(IniTargetPath), "\\config.ini");
        CopyFile(IniCurrentPath,IniTargetPath,false);

        //hv���ƶ�
        strcpy(HvTargetPath,CreateCurrentPath);
        strcat_s(HvTargetPath,sizeof(HvTargetPath), "\\hdvp");
        CreateDirectory(HvTargetPath, NULL);
        strcat_s(HvTargetPath,sizeof(HvTargetPath), "\\process.hdvp");
        strcpy(HvCurrentPath,OldCurrentPath);
        strcat_s(HvCurrentPath,sizeof(HvCurrentPath), "\\demo\\hdvp\\process.hdvp");
        CopyFile(HvCurrentPath,HvTargetPath,false);
	}

}


void SystemConfig::AddScenePross()
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
	 
	SCENE_PKG pDatAdd;

	pDatAdd.sceneID = m_Scene.GetItemCount()-1;

	memset(pDatAdd.szSceneName, 0, SCENE_SIZE);
	strcpy(pDatAdd.szSceneName, str);
	
	//pDatAdd.iCountImage=1;
	SetSceneParam(&pDatAdd);

	////�������ṹ���ͼƬ�������ó�ʼ��
	//SCENE_CONFIG pDatam;
	//GetSceneParam(m_Scene.GetItemCount()-1,&pDatam);
	//pDatam.vecImageProInfo[0].iLineCount=5;
	//pDatam.vecImageProInfo[0].iCamareNo=5;
	//pDatam.vecImageProInfo[0].iCircleCount=5;
	//pDatam.vecImageProInfo[0].iLightNo=5;
	//SetSceneParam(&pDatam);


	//SYSTEM_PARAM   Sparam;
	//GetSysParam(&Sparam);
	//Sparam.iSceneNum-=1;
	//SetSceneParam(&Sparam);

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





void SystemConfig::OnBnClickedButtonDeletescene()
{
   /*     CString  strp;
        //ȡ�õ���ĳ�����
        strp=m_Scene.GetItemText(SceneItem,SceneSubItem);
		char szCurrentPath[512] = {0};
		GetCurrentDirectory(512, szCurrentPath);
		strcat_s(szCurrentPath,sizeof(szCurrentPath), "\\");
		strcat_s(szCurrentPath,sizeof(szCurrentPath),strp);
		DeleteDirFile(szCurrentPath);
		RemoveDirectory(szCurrentPath);*/

		if((SceneChoose == m_Scene.GetItemCount()-1) && (0 != SceneChoose))
		{
			m_Scene.DeleteItem(m_Scene.GetItemCount()-1);  //ֱ��ɾ��CListCtrl�е����һ��
			m_Scene.SetItemState(m_Scene.GetItemCount()-1, LVIS_SELECTED, LVIS_SELECTED );
			SceneChoose = m_Scene.GetItemCount()-1;
		}
		else if(1 == m_Scene.GetItemCount())
		{
			m_Scene.DeleteItem(0);

			//GetDlgItem(IDC_BUTTON_DELETEIMAGENUM)->EnableWindow(FALSE);
			//GetDlgItem(IDC_BUTTON_ADDIMAGENUM)->EnableWindow(FALSE);
			//GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
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






void SystemConfig::OnBnClickedButtonUpscene()
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


void SystemConfig::OnBnClickedButtonDownscene()
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


void SystemConfig::OnClickListCameraCurrent(NMHDR *pNMHDR, LRESULT *pResult)
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


void SystemConfig::OnBnClickedAddCamera()
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


void SystemConfig::OnClickListCameraModify(NMHDR *pNMHDR, LRESULT *pResult)
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


void SystemConfig::Deletecamera()
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



void SystemConfig::OnBnClickedButtonUpcamera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str = m_Cameralist.GetItemText(reCameraChoose, 1);//ȡ��ѡ���е�����
	CString str_change = m_Cameralist.GetItemText(--reCameraChoose, 1);
	
	m_Cameralist.SetItemText(reCameraChoose, 1, str);
	m_Cameralist.SetItemText(++reCameraChoose, 1, str_change);

	m_Cameralist.SetItemState(--reCameraChoose, LVIS_SELECTED, LVIS_SELECTED );
	GetDlgItem(IDC_BUTTON_CAMERALISTOK)->EnableWindow(TRUE);
}



void SystemConfig::OnBnClickedButtonDowncamera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString str = m_Cameralist.GetItemText(reCameraChoose, 1);//ȡ��ѡ���е�����
	CString str_change = m_Cameralist.GetItemText(++reCameraChoose, 1);

	m_Cameralist.SetItemText(--reCameraChoose, 1, str_change);
	m_Cameralist.SetItemText(++reCameraChoose, 1, str);
	m_Cameralist.SetItemState(reCameraChoose, LVIS_SELECTED, LVIS_SELECTED );

	GetDlgItem(IDC_BUTTON_CAMERALISTOK)->EnableWindow(TRUE);
}


void SystemConfig::OnBnClickedButtonCameralistInstall()
{   
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SYSTEM_PARAM param;
	GetSysParam(&param);
	for(int q=0; q<VALUE_NUM; q++)
	{ 
		param.pstrCamareGUID[q] = "";    
	}
	for(int i=0; i<m_Cameralist.GetItemCount(); i++)
	{
		param.pstrCamareGUID[i] = m_Cameralist.GetItemText(i, 1);//ȡ��ѡ���е�����
	}
	param.iCameraNum = m_Cameralist.GetItemCount();
    SetSysParam(&param);
	GetDlgItem(IDC_BUTTON_CAMERALISTOK)->EnableWindow(FALSE);
}


void SystemConfig::OnBnClickedButtonCameraseave()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    SYSTEM_PARAM paramm;
    GetSysParam(&paramm);
    for(int q=0; q<VALUE_NUM; q++)
    { 
        paramm.pstrCamareGUID[q] = "";    
    }
    for(int i=0; i<m_Cameralist.GetItemCount(); i++)
    {

        paramm.pstrCamareGUID[i] = m_Cameralist.GetItemText(i, 1);//ȡ��ѡ���е�����
    }
    paramm.iCameraNum = m_Cameralist.GetItemCount();
    SaveSysParam(&paramm);
}


void SystemConfig::SceneInstall()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SceneModifyInstall();
	SortPortInstall();
}



void SystemConfig::SortPortInstall()
{
	SYSTEM_PARAM pDataPort;
	GetSysParam(&pDataPort);
	for (int i=0; i<16; i++)
	{
		pDataPort.strLightInfo[i].PortName = "";
	}
	for(int i=0; i<m_SortPort.GetItemCount(); i++)
	{
		pDataPort.strLightInfo[i].PortName = m_SortPort.GetItemText(i, 1);
        pDataPort.strLightInfo[i].Channel = atoi(m_SortPort.GetItemText(i, 2));
	}
    pDataPort.iLightNum = m_SortPort.GetItemCount();
	SetSysParam(&pDataPort);
}



void SystemConfig::SceneModifyInstall()
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
	else if(Lnum<VALUE_NUM && Lnum>0)
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

        for(int j=0; j < VALUE_NUM ; j++)
        {
            Getsceneparam.szSceneName[j] = 0;
        }
        Getsceneparam.stationID = atoi(m_Scene.GetItemText(i,2).GetBuffer());
        strcpy(Getsceneparam.szSceneName, m_Scene.GetItemText(i,1).GetBuffer());

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


void SystemConfig::OnBnClickedButtonScenesave()
{
    SceneSave();
	SortPortSave();
}


void SystemConfig::SortPortSave()
{
	SYSTEM_PARAM pDataPort;
	GetSysParam(&pDataPort);
	for (int i=0; i<16; i++)
	{
		pDataPort.strLightInfo[i].PortName = "";
	}
	for(int i=0; i<m_SortPort.GetItemCount(); i++)
	{
		pDataPort.strLightInfo[i].PortName = m_SortPort.GetItemText(i,1);
        pDataPort.strLightInfo[i].Channel = atoi(m_SortPort.GetItemText(i,2));
	}
    pDataPort.iLightNum = m_SortPort.GetItemCount();
	SaveSysParam(&pDataPort);
}



void SystemConfig::SceneSave()
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
            //str=m_Scene.GetItemText(SceneChoose,1);
            char szCurrentPath[512] = {0};
            char szCurrentPathmy[512] = {0};
            GetCurrentDirectory(512, szCurrentPath);
            GetCurrentDirectory(512, szCurrentPathmy);
            strcat_s(szCurrentPath,sizeof(szCurrentPath), "\\");
            strcat_s(szCurrentPath,sizeof(szCurrentPath),it->Oldname);
            strcat_s(szCurrentPathmy,sizeof(szCurrentPath), "\\");
            strcat_s(szCurrentPathmy,sizeof(szCurrentPath),it->Updataname);
            rename(szCurrentPath,szCurrentPathmy);
        }
    }
    SceneNameSave.clear();
    //str=m_Scene.GetItemText(SceneChoose,1);
    //char szCurrentPath[512] = {0};
    //char szCurrentPathmy[512]={0};
    //GetCurrentDirectory(512, szCurrentPath);
    //GetCurrentDirectory(512, szCurrentPathmy);
    //strcat_s(szCurrentPath,sizeof(szCurrentPath), "\\");
    //strcat_s(szCurrentPath,sizeof(szCurrentPath),strFilename);
    //strcat_s(szCurrentPathmy,sizeof(szCurrentPath), "\\");
    //strcat_s(szCurrentPathmy,sizeof(szCurrentPath),str);
    //rename(szCurrentPath,szCurrentPathmy);

	//����һЩSys�������õĸ���
	GetDlgItemText(IDC_EDIT_WORKWINDOW,str_port);
	Lnum = atoi(str_port);
	if (str_port == "")
	{
		MessageBox("�������ڵĸ�������Ϊ��");
	}
	else if(Lnum<VALUE_NUM && Lnum>=0)
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
	SaveSysParam(&pScenenum);

    //**************************************************************
	//���������ṹ�����
	SYSTEM_PARAM pDatam;
	GetSysParam(&pDatam);
	for(int i=0; i<m_Scene.GetItemCount(); i++)
	{
		pDatam.pstrSceneName[i] = m_Scene.GetItemText(i,1).GetBuffer();
	}
	pDatam.iSceneNum = m_Scene.GetItemCount();
	SaveSysParam(&pDatam);  
	//**************************************************************
	//���������ͳ���������ȡ����

    SCENE_PKG  Getsceneparam;

    for (int i = 0; i < m_Scene.GetItemCount(); i++)
    {
        GetSceneParam(i, &Getsceneparam);//��ȡ�ṹ������

        memset(Getsceneparam.szSceneName, 0, SCENE_SIZE);
        Getsceneparam.stationID = atoi(m_Scene.GetItemText(i,2));
        strcpy(Getsceneparam.szSceneName, m_Scene.GetItemText(i,1).GetBuffer());
        SaveSceneParam(i,&Getsceneparam);
    }
 }


void SystemConfig::OnDblclkListCameraModify(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
   Deletecamera();
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




void SystemConfig::OnDblclkListCameraCurrent(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    OnBnClickedAddCamera();
    *pResult = 0;
}




bool SystemConfig::DeleteDirFile(char* psDirName)
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



void SystemConfig::OnClickListDoport(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != NO_PITCH)
	{
		DoPortChoose = pNMListView->iItem;
	}
	*pResult = 0;
}



void SystemConfig::OnClickListSerialport(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != NO_PITCH)
	{
        CString str;
		SortPortItem = pNMListView->iItem;     //ѡ����
		SortPortSubItem = pNMListView->iSubItem;    //ѡ����
        if (2 == SortPortSubItem)
        {
            str = m_SortPort.GetItemText(SortPortItem, SortPortSubItem);
            SetDlgItemText(IDC_EDIT_LIGHTNUMMODIFY, str);
            m_PortModiEdit.SetFocus();
            m_PortModiEdit.SetSel(0, -1);
        }
	}
	if((1 == m_SortPort.GetItemCount()) || (0 == m_SortPort.GetItemCount()))
	{
		GetDlgItem(IDC_BUTTON_UPSORTPORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DOWNSORTPORT)->EnableWindow(FALSE);
	}
	else if(SortPortItem == m_SortPort.GetItemCount()-1)
	{
		GetDlgItem(IDC_BUTTON_UPSORTPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DOWNSORTPORT)->EnableWindow(FALSE);
	}
	else if(0 == SortPortItem)
	{
		GetDlgItem(IDC_BUTTON_UPSORTPORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DOWNSORTPORT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_UPSORTPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DOWNSORTPORT)->EnableWindow(TRUE);
	}
	*pResult = 0;
}




void SystemConfig::OnDblclkListDoport(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    OnBnClickedButtonAddport();
    *pResult = 0;
}


void SystemConfig::OnDblclkListSerialport(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    int iEquit;
    iEquit = SortPortItem;
    m_SortPort.DeleteItem(SortPortItem);
    CString str;
    for (int i = 0; i<m_SortPort.GetItemCount(); i++)
    {
        str.Format("%d", i);
        m_SortPort.SetItemText(i, 0, str);
    }
    SortPortItem = iEquit;
    m_SortPort.SetItemState(SortPortItem, LVIS_SELECTED, LVIS_SELECTED );

	*pResult = 0;
}


void SystemConfig::OnBnClickedButtonUpsortport()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int chanl = SortPortItem;
    CString strcurrent = m_SortPort.GetItemText(SortPortItem, 1);
    CString strnext = m_SortPort.GetItemText(--SortPortItem, 1);
    m_SortPort.SetItemText(SortPortItem, 1, strcurrent);
    m_SortPort.SetItemText(++SortPortItem, 1, strnext);

    strcurrent = m_SortPort.GetItemText(chanl, 2);
    strnext = m_SortPort.GetItemText(--chanl, 2);
    m_SortPort.SetItemText(chanl, 2, strcurrent);
    m_SortPort.SetItemText(++chanl, 2, strnext);

    m_SortPort.SetItemState(--SortPortItem, LVIS_SELECTED, LVIS_SELECTED );



    if(0 == SortPortItem)
    {
        GetDlgItem(IDC_BUTTON_UPSORTPORT)->EnableWindow(FALSE);
    }
    GetDlgItem(IDC_BUTTON_DOWNSORTPORT)->EnableWindow(TRUE);
}


void SystemConfig::OnBnClickedButtonDownsortport()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int chanl = SortPortItem;
    CString strcurrent = m_SortPort.GetItemText(SortPortItem, 1);
    CString strnext = m_SortPort.GetItemText(++SortPortItem, 1);
    m_SortPort.SetItemText(--SortPortItem, 1, strnext);
    m_SortPort.SetItemText(++SortPortItem, 1, strcurrent);

    strcurrent = m_SortPort.GetItemText(chanl, 2);
    strnext = m_SortPort.GetItemText(++chanl, 2);
    m_SortPort.SetItemText(--chanl, 2, strnext);
    m_SortPort.SetItemText(++chanl, 2, strcurrent);

    m_SortPort.SetItemState(SortPortItem, LVIS_SELECTED, LVIS_SELECTED );
    if(SortPortItem == m_SortPort.GetItemCount()-1)
    {
        GetDlgItem(IDC_BUTTON_DOWNSORTPORT)->EnableWindow(FALSE);
    }
    GetDlgItem(IDC_BUTTON_UPSORTPORT)->EnableWindow(TRUE);
}


void SystemConfig::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		CDialogEx::OnClose();
}


void SystemConfig::OnBnClickedButtonResetdisplay()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    SceneInstall();
    msg_send(MSG_RESET_PARAMETER, 0, MODULE_SCENE_MAIN_UI); 
}



void SystemConfig::OnBnClickedButtonAlterscenename()
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


void SystemConfig::OnBnClickedButtonUpdatacamera()
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
	GetCameraIDList(cameralist);
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


void SystemConfig::OnBnClickedButtonAddport()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (m_DoPort.GetItemCount() <= 0)
    {
        MessageBox("�޿��ô���");
    }
    else
    {
        CString strDoport;
        CString str;
        strDoport = m_DoPort.GetItemText(DoPortChoose, 0);
        int InsertLine = m_SortPort.GetItemCount();
        int row = m_SortPort.InsertItem(InsertLine, "");
        str.Format("%d", InsertLine);
        m_SortPort.SetItemText(row, 0, str);
        m_SortPort.SetItemText(row, 1, strDoport);
        m_SortPort.SetItemText(row, 2, "8");

        //�ж��������Ƿ��Ѵ���
        for(int i=0; i<m_SortPort.GetItemCount()-1; i++)
        {
            CString cstr = m_SortPort.GetItemText(i, 1);
            if(0 == strcmp(cstr, strDoport))
            {
                MessageBox("�Ѵ���ѡ����");
                m_SortPort.DeleteItem(m_SortPort.GetItemCount()-1);
            }
        }
    }
}


void SystemConfig::OnBnClickedButtonLightnummodify()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString str;
    GetDlgItemText(IDC_EDIT_LIGHTNUMMODIFY,str);
    if (2 == SortPortSubItem)
    {
        m_SortPort.SetItemText(SortPortItem, SortPortSubItem, str);
    }
}


void SystemConfig::OnBnClickedButtonUpdataport()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    //m_DoPort��ʼ��
    m_DoPort.DeleteAllItems();
    CString strSerialList;
    string strSerialPort[256];
    int k = 0;
#ifdef _TESTPORT
    strSerialList = "COM6.COM5.COM4.COM3.COM2.COM1";	
#else
    strSerialList = GetPortNum();
#endif
    if (0 != strcmp(strSerialList,""))
    {

        for(int i=0; i<strSerialList.GetLength(); i++)
        {
            if(strSerialList[i] == '.')
            {
                k++;
            }
            else
            {
                strSerialPort[k].push_back(strSerialList[i]);
            }
        }
        for(int i=0 ; i<k+1 ; i++)
        {
            int iRow = m_DoPort.InsertItem(i ,"");
            m_DoPort.SetItemText(iRow, 0, strSerialPort[i].c_str());
        }
    }
}


BOOL SystemConfig::DestroyWindow()
{
    // TODO: �ڴ����ר�ô����/����û���
    int iType = 0;
    msg_post(RECV_UI_LOGIN_MSG,&iType,sizeof(int),MODULE_SCENE_MAIN_SDK);

    unregister_notify(MODULE_SYSTEMCONFIG_UI);
    return CDialogEx::DestroyWindow();
}


void SystemConfig::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    OnOK();
    CDialogEx::OnTimer(nIDEvent);
}


BOOL SystemConfig::PreTranslateMessage(MSG* pMsg)
{
    // TODO: �ڴ����ר�ô����/����û���
    if (pMsg->message == WM_LBUTTONDOWN || VK_RETURN == pMsg->wParam)
    {
        KillTimer(0);
        SetTimer(0, LOGIN_TIME_OUT,0);
    }

    return CDialogEx::PreTranslateMessage(pMsg);
}


void SystemConfig::OnBnClickedCheckLandtype()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int status = m_CheckLandType.GetCheck();
    if (1 == status)
    {
        LandType = true;
    }
    else
    {
        LandType = false;
    }
    msg_post(MSG_LANDTYPE_INFORMATION, (void *)&LandType, sizeof(bool), MODULE_SCENE_MAIN_UI);
}


void SystemConfig::OnBnClickedButtonSavepath()
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
    Sparam.ImageSavePath = szDisplayName;
    SaveSysParam(&Sparam);
}
