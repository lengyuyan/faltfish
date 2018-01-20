// SEBtn.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Flatfish.h"
#include "SEBtn.h"
#include "afxdialogex.h"
#include "iManagerCtrl.h"


// CSEBtn �Ի���
#define    RING_WIDTH_INDEX  5
#define    SMOOTH_INDEX      7
#define    THRESHOLD_INDEX   8
#define    ROIWIDTH_INDEX    9
#define    IPOSNUM_INDEX     10

IMPLEMENT_DYNAMIC(CSEBtn, CDialogEx)

CSEBtn::CSEBtn(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSEBtn::IDD, pParent)
{
	m_iCurSceneID = 0;
	m_iCurIndex = 0;
	m_winID = 0;
}

CSEBtn::~CSEBtn()
{
}

void CSEBtn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSEBtn, CDialogEx)

	//�Զ�����Ϣ
	ON_MESSAGE(BTN_ADD_SUB, &CSEBtn::OnBtnMessage)

	ON_BN_CLICKED(IDC_BUTTON_SEB_ADD, &CSEBtn::OnClickedButtonSebAdd)
	ON_BN_CLICKED(IDC_BUTTON_SEB_SUB, &CSEBtn::OnClickedButtonSebSub)
	ON_EN_CHANGE(IDC_EDIT_SEB, &CSEBtn::OnChangeEditSeb)
END_MESSAGE_MAP()

void CSEBtn::getInterfaceSetData( INTERFACE_SET_DATA interfaceSetData )
{
	m_interfaceSetData = interfaceSetData;
	CString str;
	GetDlgItem(IDC_STATIC_SEB)->SetWindowText(interfaceSetData.BtnName.c_str());
	str.Format("%.2f", interfaceSetData.InitValue);
	GetDlgItem(IDC_EDIT_SEB)->SetWindowText(str);
}


// CSEBtn ��Ϣ�������


void CSEBtn::OnClickedButtonSebAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SendMessage(BTN_ADD_SUB, BTN_ADD);
}


void CSEBtn::OnClickedButtonSebSub()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SendMessage(BTN_ADD_SUB, BTN_SUB);
}

void CSEBtn::UpdateUI( int iSceneID, int index, CWnd *hROI, int revise /*= 0*/ )
{
	m_iCurSceneID = iSceneID;
	m_iCurIndex = index;
	if (1 == revise)
	{
		SendMessage(BTN_ADD_SUB, BTN_INIT);
	}
	else if (0 == revise)
	{
		SendMessage(BTN_ADD_SUB, -1);
	}
	else if (-1 == revise)
	{
		SendMessage(BTN_ADD_SUB, BTN_DELETE);
	}
}

void CSEBtn::SetDisplayWindowID( int winID )
{
	m_winID = winID;
}

LRESULT CSEBtn::OnBtnMessage( WPARAM wParam, LPARAM lParam )
{
	CString str;
	double dValue = 0.0;
	bool insert = false;

	GetDlgItem(IDC_EDIT_SEB)->GetWindowText(str);	//��ȡ��ǰ��ʾ��ֵ
	dValue = atof(str.GetBuffer());
	
	switch (wParam)
	{
	case BTN_ADD:
		dValue += m_interfaceSetData.ChangeRate;
		break;
	case BTN_SUB:
		dValue -= m_interfaceSetData.ChangeRate;
		break;
	case BTN_INIT:
		insert = true;
		break;
	}
	if (INT_TYPE == m_interfaceSetData.ItemType)
	{
		str.Format("%.0f",dValue);
	}
	else if (DOUBLE_TYPE == m_interfaceSetData.ItemType)
	{
		str.Format("%.2f",dValue);
	}

	GetDlgItem(IDC_EDIT_SEB)->SetWindowText(str);

	UpdateItem(str, (BTN_ALGORITHM)wParam, insert);
	return 0;
}

void CSEBtn::UpdateItem( CString str, BTN_ALGORITHM btnAlgorithm, bool insert /*= false*/ )
{
	vector<VIRSION_DATA_PKG *>::iterator it;
	vector<unsigned int *>::iterator itType;
	SCENE_PKG pConfig;				//�������ļ��ж�ȡͼƬ�й�roi��Ϣ
	int insertPos = 0;
	GetSceneParam(m_iCurSceneID, &pConfig);
	//GetSceneParam(m_interfaceSetData.CurSceneID, &pConfig);
	VIRSION_DATA_PKG *pTmp = NULL;
	unsigned int *pTmpType = NULL;
	//��ȡԭ������
	for (it = pConfig.vecBody.begin(), itType = pConfig.vecDataType.begin(); it != pConfig.vecBody.end(); it++, itType++)
	{
		pTmp = *it;
		pTmpType = *itType;
		if ((pTmp->iValue % 10000) == m_interfaceSetData.iT)
		{
			break;
		}
		++insertPos;
	}
	if (insertPos >= pConfig.pkgCount)
	{
		return;
	}

	int iROISize = 0;
	if (BTN_INIT == btnAlgorithm)	//��ʼ��
	{
		iROISize = pTmp[1].iValue + 1;
	}
	else if (BTN_DELETE == btnAlgorithm)
	{
		iROISize = 0;
	}
	else
	{
		iROISize = pTmp[1].iValue;
	}
	//����ԭ�����ݷ���ռ�
	//int iT = m_iCurSceneID * 10000 + MORE_PRO * 100 + m_interfaceSetData.index;
	unsigned int iFlag = ROI_TYPE * 100 + 1;
	int iSize = VIRSION_DATA_PKG_HEADER_SIZE + iROISize;
	VIRSION_DATA_PKG *pInfo =  new VIRSION_DATA_PKG[iSize];
	unsigned int *pType = new unsigned int[iSize];
	memset(pInfo, 0, iSize * sizeof(VIRSION_DATA_PKG));
	memset(pType, 0, iSize * sizeof(unsigned int));

	//��������
	for (int indexT = 0; indexT < iSize; ++indexT)
	{
		//pTmp����
		if (1 == indexT)	//����numb
        {
            if (insert)
            {
                pInfo[indexT].iValue = pTmp[indexT].iValue + 1;
            }
			else if (BTN_DELETE == btnAlgorithm)
			{
				pInfo[indexT].iValue  = 0;
			}
			else if (pTmp[indexT].iValue < m_iCurIndex + 1)
			{
				pInfo[indexT].iValue = m_iCurIndex + 1;
			}
			else if (pTmp[indexT].iValue >= m_iCurIndex + 1)
			{
				pInfo[indexT].iValue = pTmp[indexT].iValue;
			}
			pType[indexT] = pTmpType[indexT];
		}
		else if (m_interfaceSetData.nIndex + VIRSION_DATA_PKG_HEADER_SIZE == indexT)	//�޸�ֵ
		{
			if (INT_TYPE == m_interfaceSetData.ItemType)
			{
				pInfo[indexT].iValue = atoi(str);
			}
			else if (DOUBLE_TYPE == m_interfaceSetData.ItemType)
			{
				pInfo[indexT].dValue = atof(str);
			}
			m_interfaceSetData.InitValue = atof(str);
			//pType����
			if (INT_TYPE == m_interfaceSetData.ItemType)		//int����
			{
				pType[indexT] = 0;
			} 
			else if (DOUBLE_TYPE == m_interfaceSetData.ItemType)	//double����
			{
				pType[indexT] = iFlag;
			}
		}
		else
        {
            pType[indexT] = pTmpType[indexT];
            if (INT_TYPE == pType[indexT])
            {
                pInfo[indexT].iValue = pTmp[indexT].iValue;
            }
            else
            {
			    pInfo[indexT].dValue = pTmp[indexT].dValue;
            }
		}
	}

	//����ParamItem
	UpdateParamItem(m_iCurSceneID, pInfo, pType, &m_interfaceSetData, iSize, insertPos, insert);
}


BOOL CSEBtn::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetBackgroundColor(BACKGROUND_COLOR_M);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CSEBtn::OnChangeEditSeb()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strTemp;
	bool point = false;
	CEdit* edit1 = ((CEdit*)(GetDlgItem(IDC_EDIT_SEB)));
	edit1->GetWindowText(strTemp);
	int len = strTemp.GetLength();
	for (int i = 0; i < len; ++i) 
	{
		if (strTemp.GetAt(i) < '0' || strTemp.GetAt(i) > '9')
		{
			if (strTemp.GetAt(i) != '.' && strTemp.GetAt(i) != '-' || point)
			{   
				strTemp = strTemp.Left(i);
				edit1->SetWindowText(strTemp);
				edit1->SetSel(i, i, TRUE);
				point = false;
				return;
			}
			if (strTemp.GetAt(i) == '.')
			{
				point = true;
				if (i == 0)
				{
					strTemp = "0.";
					edit1->SetWindowText(strTemp);
					i = strTemp.GetLength();
					edit1->SetSel(i, i, TRUE);
				}
			}
		}
	}
	return;
}

int CSEBtn::CurSceneID()
{
	return m_interfaceSetData.CurSceneID;//m_iCurSceneID;
}

string CSEBtn::BtnType()
{
	return m_interfaceSetData.BtnType;
}
