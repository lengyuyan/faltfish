// LightDebugDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SystemManager.h"
#include "LightDebugDlg.h"
#include "afxdialogex.h"
#include "iManagerCtrl.h"

#define  NO_PITCH    -1
// CLightDebugDlg �Ի���

IMPLEMENT_DYNAMIC(CLightDebugDlg, CDialogEx)

CLightDebugDlg::CLightDebugDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLightDebugDlg::IDD, pParent)
    , m_LightValue(0)
{
    m_DoPortChoose = NO_PITCH;
    m_SortPortItem = NO_PITCH;     
    m_SortPortSubItem = NO_PITCH;
    m_OpenStatus = 0;
}

CLightDebugDlg::~CLightDebugDlg()
{
}

void CLightDebugDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_CTRL_INDEX, m_CommList);
    DDX_Control(pDX, IDC_SPIN_VALUE_CTRL, m_SpinValue);
    DDX_Control(pDX, IDC_SLIDER_LIGHT_VALUE, m_SliderChanl);
    DDX_Text(pDX, IDC_EDIT_LIGHT_VALUE, m_LightValue);
    DDX_Control(pDX, IDC_LIST_DOPORT, m_DoPort);
    DDX_Control(pDX, IDC_LIST_SERIALPORT, m_SortPort);
    DDX_Control(pDX, IDC_EDIT_LIGHTNUMMODIFY, m_PortModiEdit);
}


BEGIN_MESSAGE_MAP(CLightDebugDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_OP_LIGHT, &CLightDebugDlg::OnBnClickedButtonOpLight)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_LIGHT_VALUE, &CLightDebugDlg::OnNMCustomdrawSliderLightValue)
    ON_BN_CLICKED(IDC_BUTTON_SET_LIGHT, &CLightDebugDlg::OnBnClickedButtonSetLight)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_DOPORT, &CLightDebugDlg::OnDblclkListDoport)
    ON_NOTIFY(NM_CLICK, IDC_LIST_DOPORT, &CLightDebugDlg::OnClickListDoport)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_SERIALPORT, &CLightDebugDlg::OnDblclkListSerialport)
    ON_NOTIFY(NM_CLICK, IDC_LIST_SERIALPORT, &CLightDebugDlg::OnClickListSerialport)
    ON_BN_CLICKED(IDC_BUTTON_UPSORTPORT, &CLightDebugDlg::OnBnClickedButtonUpsortport)
    ON_BN_CLICKED(IDC_BUTTON_DOWNSORTPORT, &CLightDebugDlg::OnBnClickedButtonDownsortport)
    ON_BN_CLICKED(IDC_BUTTON_ADDPORT, &CLightDebugDlg::OnBnClickedButtonAddport)
    ON_BN_CLICKED(IDC_BUTTON_LIGHTNUMMODIFY, &CLightDebugDlg::OnBnClickedButtonLightnummodify)
    ON_BN_CLICKED(IDC_BUTTON_DELETE_PORT, &CLightDebugDlg::OnBnClickedButtonDeletePort)
    ON_BN_CLICKED(IDC_BUTTON_UPDATAPORT, &CLightDebugDlg::OnBnClickedButtonUpdataport)
    ON_BN_CLICKED(IDC_BUTTON_RESET_LIGHT, &CLightDebugDlg::OnBnClickedButtonResetLight)
    ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL_INDEX, &CLightDebugDlg::OnCbnSelchangeComboChannelIndex)
END_MESSAGE_MAP()


void CLightDebugDlg::OnBnClickedButtonOpLight()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    CString strCom;
    CString strErr;
    CString strBufInfo;
    GetDlgItem(IDC_COMBO_CTRL_INDEX)->GetWindowText(strCom);
    int index = ((CComboBox *)GetDlgItem(IDC_COMBO_CHANNEL_INDEX))->GetCurSel();
    m_OpenStatus = m_OpenStatus % 2;
    int iRet = OpratorLight(strCom.GetBuffer(), index, m_LightValue, m_OpenStatus); //���ù�Դ����
    if (LIGHTCTL_SUCCESS != iRet)
    {
        strErr.Format("����ʧ�ܣ� ������Ϊ%d", iRet);
    }
    else
    {
        strErr.Format("�����ɹ�", iRet);
        if (m_OpenStatus == 0)
        {
            GetDlgItem(IDC_BUTTON_OP_LIGHT)->SetWindowText("�رմ���");
        }
        else
        {
            GetDlgItem(IDC_BUTTON_OP_LIGHT)->SetWindowText("�򿪴���");
        }
    }
    m_OpenStatus++;
    GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(strErr);
}


BOOL CLightDebugDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetBackgroundColor(BACKGROUND_COLOR_M);
    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    InitLightComBox();
    InitAllListctrl();
    SortShowBut();

    CComboBox *pLightType = (CComboBox *)GetDlgItem(IDC_COMBO_LIGHT_TYPE);
    pLightType->AddString("NDPS");
    pLightType->AddString("OPT");
	//pLightType->AddString("OPTPV");
    pLightType->SetCurSel(0);

    pLightType = (CComboBox *)GetDlgItem(IDC_COMBO_CHANNEL_INDEX);
    char szChannel[64] = {0};
    for (int i = 0; i < 8; )
    {
        i++;
        sprintf_s(szChannel,"%d", i);
        pLightType->AddString(szChannel);
    }
    pLightType->SetCurSel(0);

    m_SpinValue.SetBuddy( GetDlgItem(IDC_EDIT_LIGHT_VALUE) );        
    m_SpinValue.SetRange( 0, 255 );
    m_LightValue = 200;

    m_SliderChanl.SetRange(0, 255);
    m_SliderChanl.SetTicFreq(5);
    m_SliderChanl.SetPos(m_LightValue);

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}
// CLightDebugDlg ��Ϣ�������

void CLightDebugDlg::InitLightComBox()
{
    HKEY hKey; 
    int  index = 0;
    if(::RegOpenKeyEx( HKEY_LOCAL_MACHINE, "Hardware\\DeviceMap\\SerialComm", 
        NULL,
        KEY_READ,
        &hKey ) == ERROR_SUCCESS ) //�򿪴���ע����Ӧ�ļ�ֵ 
    {
        int i=0; 
        char portName[256],commName[256];
        DWORD dwLong,dwSize;
        while(1)
        { 
            dwLong = dwSize = sizeof(portName); 
            if( ::RegEnumValue( hKey, i, portName, &dwLong,NULL,NULL, (PUCHAR)commName, &dwSize ) == ERROR_NO_MORE_ITEMS )// ö�ٴ��� 
            {
                break; 
            }

            m_CommList.AddString( commName ); // commName���Ǵ������� 

            i++; 
        } 

        if( m_CommList.GetCount() == 0 ) 
        { 
            //::AfxMessageBox("����ö��ʧ��!!!"); 

        } 
        RegCloseKey(hKey);
        m_CommList.SetCurSel(0);
    } 
}

void CLightDebugDlg::OnNMCustomdrawSliderLightValue(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    char szBuf[64] = {0};
    int iValue = m_SliderChanl.GetPos();
    sprintf_s(szBuf, "%d", iValue);
    GetDlgItem(IDC_EDIT_LIGHT_VALUE)->SetWindowText(szBuf);
    *pResult = 0;
    CString strCom;
    CString strErr;
    GetDlgItem(IDC_COMBO_CTRL_INDEX)->GetWindowText(strCom);
    int index = ((CComboBox *)GetDlgItem(IDC_COMBO_CHANNEL_INDEX))->GetCurSel();
    int iRet = OpratorLight(strCom.GetBuffer(), index, iValue, 2); //���ù�Դ����
    if (LIGHTCTL_SUCCESS != iRet)
    {
        strErr.Format("�򿪹�Դʧ�ܣ� ������Ϊ%d", iRet);
    }
    else
    {
        strErr.Format("�򿪹�Դ�ɹ�", iRet);
    }
    GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(strErr);
}


void CLightDebugDlg::OnBnClickedButtonSetLight()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    CString strCom;
    CString strErr;
    GetDlgItem(IDC_COMBO_CTRL_INDEX)->GetWindowText(strCom);
    int index = ((CComboBox *)GetDlgItem(IDC_COMBO_CHANNEL_INDEX))->GetCurSel();
    int iRet = OpratorLight(strCom.GetBuffer(), index, m_LightValue, 2); //���ù�Դ����
    if (LIGHTCTL_SUCCESS != iRet)
    {
        strErr.Format("�򿪹�Դʧ�ܣ� ������Ϊ%d", iRet);
    }
    else
    {
        strErr.Format("�򿪹�Դ�ɹ�", iRet);
    }
    GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(strErr);
}

void CLightDebugDlg::OnDblclkListDoport(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    OnBnClickedButtonAddport();
    *pResult = 0;
}

void CLightDebugDlg::OnClickListDoport(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    if(pNMListView->iItem != NO_PITCH)
    {
        m_DoPortChoose = pNMListView->iItem;
    }
    *pResult = 0;
}

void CLightDebugDlg::OnBnClickedButtonAddport()
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
        if (m_DoPortChoose < 0)
        {
            MessageBox("δѡ��������");
            return;
        }
        strDoport = m_DoPort.GetItemText(m_DoPortChoose, 0);
        int InsertLine = m_SortPort.GetItemCount();
        int row = m_SortPort.InsertItem(InsertLine, "");
        str.Format("%d", InsertLine);
        m_SortPort.SetItemText(row, 0, str);
        m_SortPort.SetItemText(row, 1, strDoport);
        m_SortPort.SetItemText(row, 2, "4");
        m_SortPort.SetItemText(row, 3, "NDPS");

        //�ж��������Ƿ��Ѵ���
        for(int i=0; i<m_SortPort.GetItemCount()-1; i++)
        {
            CString cstr = m_SortPort.GetItemText(i, 1);
            if(0 == strcmp(cstr, strDoport))
            {
                MessageBox("�Ѵ���ѡ����");
                m_SortPort.DeleteItem(m_SortPort.GetItemCount()-1);
                return;
            }
        }
        SortShowBut();

        
        SortPortSave();
        //���������ڴ�
        SYSTEM_PARAM* para;
		para = (SYSTEM_PARAM*)GetSysParamPoint();
        CtrlComm(&para->strLightInfo[row], true, para->strLightType);  //�޸�ǰ�ͷŵ�ǰ����
    }

}
void CLightDebugDlg::OnDblclkListSerialport(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    //int iEquit;
    //iEquit = m_SortPortItem;
    //m_SortPort.DeleteItem(m_SortPortItem);
    //CString str;
    //for (int i = 0; i<m_SortPort.GetItemCount(); i++)
    //{
    //    str.Format("%d", i);
    //    m_SortPort.SetItemText(i, 0, str);
    //}
    //m_SortPortItem = iEquit;
    //m_SortPort.SetItemState(m_SortPortItem, LVIS_SELECTED, LVIS_SELECTED );

    *pResult = 0;
}

void CLightDebugDlg::OnClickListSerialport(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    if(pNMListView->iItem != NO_PITCH)
    {
        CString str;
        m_SortPortItem = pNMListView->iItem;     //ѡ����
        m_SortPortSubItem = pNMListView->iSubItem;    //ѡ����
        if (2 == m_SortPortSubItem)
        {
            str = m_SortPort.GetItemText(m_SortPortItem, m_SortPortSubItem);
            SetDlgItemText(IDC_EDIT_LIGHTNUMMODIFY, str);
            m_PortModiEdit.SetFocus();
            m_PortModiEdit.SetSel(0, -1);
        }

        SYSTEM_PARAM*   Sparam;
	     Sparam = (SYSTEM_PARAM*)GetSysParamPoint();
        ((CComboBox *)GetDlgItem(IDC_COMBO_LIGHT_TYPE))->SetCurSel(Sparam->strLightInfo[m_SortPortItem].strLightType);
        ((CComboBox *)GetDlgItem(IDC_COMBO_CTRL_INDEX))->SetWindowText(Sparam->strLightInfo[m_SortPortItem].PortName.c_str());
    }
    SortShowBut();
    
    *pResult = 0;
}


void CLightDebugDlg::OnBnClickedButtonUpsortport()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int chanl = m_SortPortItem;
    CString strcurrent = m_SortPort.GetItemText(chanl, 1);
    CString strnext = m_SortPort.GetItemText(--chanl, 1);
    m_SortPort.SetItemText(chanl, 1, strcurrent);
    m_SortPort.SetItemText(++chanl, 1, strnext);

    chanl = m_SortPortItem;
    strcurrent = m_SortPort.GetItemText(chanl, 2);
    strnext = m_SortPort.GetItemText(--chanl, 2);
    m_SortPort.SetItemText(chanl, 2, strcurrent);
    m_SortPort.SetItemText(++chanl, 2, strnext);

    chanl = m_SortPortItem;
    strcurrent = m_SortPort.GetItemText(chanl, 3);
    strnext = m_SortPort.GetItemText(--chanl, 3);
    m_SortPort.SetItemText(chanl, 3, strcurrent);
    m_SortPort.SetItemText(++chanl, 3, strnext);

    m_SortPort.SetItemState(--m_SortPortItem, LVIS_SELECTED, LVIS_SELECTED );



    if(0 == m_SortPortItem)
    {
        GetDlgItem(IDC_BUTTON_UPSORTPORT)->EnableWindow(FALSE);
    }
    GetDlgItem(IDC_BUTTON_DOWNSORTPORT)->EnableWindow(TRUE);

    SortPortSave();
}


void CLightDebugDlg::OnBnClickedButtonDownsortport()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int chanl = m_SortPortItem;
    CString strcurrent = m_SortPort.GetItemText(chanl, 1);
    CString strnext = m_SortPort.GetItemText(++chanl, 1);
    m_SortPort.SetItemText(--chanl, 1, strnext);
    m_SortPort.SetItemText(++chanl, 1, strcurrent);

    chanl = m_SortPortItem;
    strcurrent = m_SortPort.GetItemText(chanl, 2);
    strnext = m_SortPort.GetItemText(++chanl, 2);
    m_SortPort.SetItemText(--chanl, 2, strnext);
    m_SortPort.SetItemText(++chanl, 2, strcurrent);

    chanl = m_SortPortItem;
    strcurrent = m_SortPort.GetItemText(chanl, 3);
    strnext = m_SortPort.GetItemText(++chanl, 3);
    m_SortPort.SetItemText(--chanl, 3, strnext);
    m_SortPort.SetItemText(++chanl, 3, strcurrent);

    m_SortPortItem++;
    m_SortPort.SetItemState(m_SortPortItem, LVIS_SELECTED, LVIS_SELECTED );
    if(m_SortPortItem == m_SortPort.GetItemCount()-1)
    {
        GetDlgItem(IDC_BUTTON_DOWNSORTPORT)->EnableWindow(FALSE);
    }
    GetDlgItem(IDC_BUTTON_UPSORTPORT)->EnableWindow(TRUE);

    SortPortSave();
}

void CLightDebugDlg::OnBnClickedButtonUpdataport()
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

void CLightDebugDlg::OnBnClickedButtonLightnummodify()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString str;
    GetDlgItemText(IDC_EDIT_LIGHTNUMMODIFY,str);
    if (2 == m_SortPortSubItem)
    {
        m_SortPort.SetItemText(m_SortPortItem, m_SortPortSubItem, str);
    }
    int iSelType = ((CComboBox *)GetDlgItem(IDC_COMBO_LIGHT_TYPE))->GetCurSel();
    LIGHTCTL_TYPE enLightType = LIGHTCTL_TYPE_NDPS;
    switch(iSelType)
    {
    case 0:
        {
            str.Format("%s", "NDPS");
            enLightType = LIGHTCTL_TYPE_NDPS;
        }
        break;
    case 1:
        {
            str.Format("%s", "OPT");
            enLightType = LIGHTCTL_TYPE_OPT;
		}
		break;
		/*case 2:
		{
		str.Format("%s", "OPTPV");
		enLightType = LIGHTCTL_TYPE_OPT_PV;
		}
		break;*/
    default:
        str.Format("%s", "NDPS");
        break;
    }
    m_SortPort.SetItemText(m_SortPortItem, 3, str);
    
    //���õ�ǰ���� 
    LightInfo light;
    light.PortName = m_SortPort.GetItemText(m_SortPortItem, 1);
    CtrlComm(&light, false);  //�޸�ǰ�ͷŵ�ǰ����

    SortPortSave();
    SYSTEM_PARAM *para;
	para = (SYSTEM_PARAM*)GetSysParamPoint();
    para->strLightInfo[m_SortPortItem].strLightType = enLightType;
    CtrlComm(&para->strLightInfo[m_SortPortItem], true, para->strLightType);  //�޸�ǰ�ͷŵ�ǰ����
    SetSysParam(para);
}

void CLightDebugDlg::SortPortInstall()
{
    SYSTEM_PARAM *pDataPort;
	pDataPort = (SYSTEM_PARAM*)GetSysParamPoint();
    for (int i=0; i<16; i++)
    {
        pDataPort->strLightInfo[i].PortName = "";
    }
    for(int i=0; i<m_SortPort.GetItemCount(); i++)
    {
        pDataPort->strLightInfo[i].PortName = m_SortPort.GetItemText(i, 1);
        pDataPort->strLightInfo[i].Channel = atoi(m_SortPort.GetItemText(i, 2));
    }
    pDataPort->iLightNum = m_SortPort.GetItemCount();
    SetSysParam(pDataPort);
}

void CLightDebugDlg::InitAllListctrl()
{
    CRect rt;
	CString str;
    
    InitListctrl(&m_SortPort);
	m_SortPort.GetWindowRect(rt);
    m_SortPort.InsertColumn(0,"����" , 0, (rt.Width() - 4)/4, 10);
    m_SortPort.InsertColumn(1,"��������" , 0, (rt.Width() - 4)/4, 10);
    m_SortPort.InsertColumn(2,"��Դͨ����" , 0, (rt.Width() - 4)/4, 10);
    m_SortPort.InsertColumn(3,"��Դ����" , 0, (rt.Width() - 4)/4, 10);

    InitListctrl(&m_DoPort);
    m_DoPort.GetWindowRect(rt);
    m_DoPort.InsertColumn(0,"����" , 0, rt.Width() - 4, 10);
    //***************************************************************
	//�������޸ĵ����ݳ�ʼ����ȥ
	SYSTEM_PARAM*   Sparam;
	Sparam = (SYSTEM_PARAM* ) GetSysParamPoint();
	//****************************************************************
	//***************************************************************
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
        for(int i=0; i<k; i++)
        {
            int iRow = m_DoPort.InsertItem(i , "");
            m_DoPort.SetItemText(iRow, 0, strSerialPort[i].c_str());
        }
    }
	//********************************************************************************
	//m_SortPort��ʼ��
	for (int i=0; i<Sparam->iLightNum; i++)
	{
		int iRow = m_SortPort.InsertItem(i , "");
        str.Format("%d", i);
		m_SortPort.SetItemText(iRow, 0, str);
		m_SortPort.SetItemText(iRow, 1, (Sparam->strLightInfo[i].PortName).c_str());
        str.Format("%d", Sparam->strLightInfo[i].Channel);
        m_SortPort.SetItemText(iRow, 2, str);
        switch(Sparam->strLightInfo[i].strLightType)
        {
        case 0:
            {
                str.Format("%s", "NDPS");
            }
            break;
        case 1:
            {
                str.Format("%s", "OPT");
            }
            break;
        default:
            str.Format("%s", "NDPS");
            break;
        }
        m_SortPort.SetItemText(iRow, 3, str);
	}
    m_SortPort.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED );
    m_SortPortItem = 0;
	//*********************************************************************************
	//
}

void CLightDebugDlg::InitListctrl(CListCtrl *Listctrl)
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

void CLightDebugDlg::SortPortSave()
{
    SYSTEM_PARAM* pDataPort;

	pDataPort = (SYSTEM_PARAM* ) GetSysParamPoint();
    for (int i=0; i<16; i++)
    {
        pDataPort->strLightInfo[i].PortName = "";
    }
    for(int i=0; i<m_SortPort.GetItemCount(); i++)
    {
        pDataPort->strLightInfo[i].PortName = m_SortPort.GetItemText(i,1);
        pDataPort->strLightInfo[i].Channel = atoi(m_SortPort.GetItemText(i,2));
        //pDataPort.strLightInfo[i].LightType = atoi(m_SortPort.GetItemText(i,3));
    }
    pDataPort->iLightNum = m_SortPort.GetItemCount();
    SetSysParam(&pDataPort);
}

CString CLightDebugDlg::GetPortNum()
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
           // if (!JudgeLightConnectStatus(strPortName.GetBuffer()))
           // {
                strSerialList = strSerialList + strPortName.GetBuffer() + ".";
                CloseHandle(hPort);
           // }
        }
    } while ((Status == ERROR_SUCCESS)||(Status == ERROR_MORE_DATA));
    RegCloseKey(hKey);

    strPortName.Format("%s", strSerialList.c_str());
    return strPortName;
}

void CLightDebugDlg::OnBnClickedButtonDeletePort()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int iEquit;
    iEquit = m_SortPortItem;
    //�ͷŴ���
    LightInfo light;
    light.PortName = m_SortPort.GetItemText(m_SortPortItem, 1);
    CtrlComm(&light, false);  //�޸�ǰ�ͷŵ�ǰ����
    m_SortPort.DeleteItem(m_SortPortItem);

    CString str;
    for (int i = 0; i<m_SortPort.GetItemCount(); i++)
    {
        str.Format("%d", i);
        m_SortPort.SetItemText(i, 0, str);
    }
    m_SortPortItem = iEquit;
    m_SortPort.SetItemState(m_SortPortItem, LVIS_SELECTED, LVIS_SELECTED );
    SortShowBut();

    SortPortSave();
}
void CLightDebugDlg::SortShowBut()
{
    if((1 == m_SortPort.GetItemCount()) || (0 == m_SortPort.GetItemCount()))
    {
        GetDlgItem(IDC_BUTTON_UPSORTPORT)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_DOWNSORTPORT)->EnableWindow(FALSE);
    }
    else if(m_SortPortItem == m_SortPort.GetItemCount()-1)
    {
        GetDlgItem(IDC_BUTTON_UPSORTPORT)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_DOWNSORTPORT)->EnableWindow(FALSE);
    }
    else if(0 == m_SortPortItem)
    {
        GetDlgItem(IDC_BUTTON_UPSORTPORT)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_DOWNSORTPORT)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_BUTTON_UPSORTPORT)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_DOWNSORTPORT)->EnableWindow(TRUE);
    }
}

void CLightDebugDlg::OnBnClickedButtonResetLight()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    //���õ�ǰ���� 
    LightInfo light;
    light.PortName = m_SortPort.GetItemText(m_SortPortItem, 1);
    CtrlComm(&light, false);  //�޸�ǰ�ͷŵ�ǰ����

    SYSTEM_PARAM *para = NULL;
	para = (SYSTEM_PARAM* ) GetSysParamPoint();
    CtrlComm(&para->strLightInfo[m_SortPortItem], true, para->strLightType);  //�޸�ǰ�ͷŵ�ǰ����
}



void CLightDebugDlg::OnCbnSelchangeComboChannelIndex()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString strCom;
    GetDlgItem(IDC_BUTTON_OP_LIGHT)->SetWindowText("�򿪴���");
    m_OpenStatus = 0;
    int index = ((CComboBox *)GetDlgItem(IDC_COMBO_CHANNEL_INDEX))->GetCurSel();
    int iRet = OpratorLight(strCom.GetBuffer(), index, m_LightValue, m_OpenStatus + 1); //���ù�Դ����

}
