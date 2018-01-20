// ScenePositionSettin.cpp : ʵ���ļ�
//
#pragma once

#include "stdafx.h"
#include "SceneManager.h"
#include "ScenePositionSettin.h"
#include "ScenePositon.h"
#include "afxdialogex.h"


// ScenePositionSettin �Ի���

IMPLEMENT_DYNAMIC(ScenePositionSettin, CDialogEx)

ScenePositionSettin::ScenePositionSettin(CWnd* pParent /*=NULL*/)
	: CDialogEx(ScenePositionSettin::IDD, pParent)
{

	m_MatchIndex = -1;


}

ScenePositionSettin::~ScenePositionSettin()
{
}

void ScenePositionSettin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCENEPOSITION_COORDINATEWAY, m_CoordinateType);
	DDX_Control(pDX, IDC_SCENEPOSITION_ORIGIN_X, m_OriginalX);
	DDX_Control(pDX, IDC_SCENEPOSITION_ORIGIN_Y, m_OriginalY);
	DDX_Control(pDX, IDC_SCENEPOSITION_ORIGIN_A, m_OriginalA);
	DDX_Control(pDX, IDC_SCENEPOSITION_SELMATCH, m_CMatchSel);
}


BEGIN_MESSAGE_MAP(ScenePositionSettin, CDialogEx)
	ON_BN_CLICKED(IDC_SCENEPOSITION_COMMIT, &ScenePositionSettin::OnBnClickedScenepositionCommit)
	ON_BN_CLICKED(IDC_SCENEDLG_POSITION_SEARCHCONTOUR, &ScenePositionSettin::OnBnClickedScenedlgPositionSearchcontour)
	ON_CBN_SELCHANGE(IDC_SCENEPOSITION_COORDINATEWAY, &ScenePositionSettin::OnCbnSelchangeScenepositionCoordinateway)
	ON_BN_CLICKED(IDC_SCENEPOSITION_CANCEL, &ScenePositionSettin::OnBnClickedScenepositionCancel)
	ON_CBN_DROPDOWN(IDC_SCENEPOSITION_SELMATCH, &ScenePositionSettin::OnCbnDropdownScenepositionSelmatch)
	ON_CBN_SELCHANGE(IDC_SCENEPOSITION_SELMATCH, &ScenePositionSettin::OnCbnSelchangeScenepositionSelmatch)
END_MESSAGE_MAP()


// ScenePositionSettin ��Ϣ�������
void ScenePositionSettin::CloseSettingDlg()
{
	OnOK();
}



void ScenePositionSettin::OnCbnSelchangeScenepositionCoordinateway()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DWORD_PTR TypeDate =   m_CoordinateType.GetItemData(m_CoordinateType.GetCurSel());
	m_CoordType =(coordinateType)TypeDate;

}

void ScenePositionSettin::OnCbnDropdownScenepositionSelmatch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strTemp;
	int CurProcIndex = ((ScenePositon*)m_Pcontainer)->GetProcIndexById(((ScenePositon*)m_Pcontainer)->GetProcId());
	for (int i = 0; i<m_CMatchSel.GetCount();i++)
	{
		m_CMatchSel.DeleteString(0);
	}
	
	for (int i =0,k = 0; i<=CurProcIndex;i++)
	{
		 if( MODEL_PRO == ((ScenePositon*)m_Pcontainer)->GetCurScene()->GetProcType(i))
		 {
			 strTemp.Format("��%d��ƥ��_���%d",k,i);
			 m_CMatchSel.InsertString(k,strTemp);
			 m_CMatchSel.SetItemData(k,k);  //��������ֵΪ ƥ�����
			 k++;
		 }
	}


}


void ScenePositionSettin::OnCbnSelchangeScenepositionSelmatch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_CMatchSel.GetCurSel()>=0)
	{
	     m_MatchIndex = m_CMatchSel.GetItemData(m_CMatchSel.GetCurSel());
	}else
	{
		 m_MatchIndex = -1;
	}
}



void ScenePositionSettin::UpdateUI()
{
	m_CoordinateType.InsertString(0,"�㵽��");
	m_CoordinateType.SetItemData(0,Point_To_Point);
	m_CoordinateType.InsertString(0,"�㵽ֱ��");
	m_CoordinateType.SetItemData(0,Point_To_Line);
	m_CoordinateType.InsertString(0,"ֱ�ߵ�ֱ��");
	m_CoordinateType.SetItemData(0,Line_To_Line);
	
	//��ǰģʽ��Ϊ�㵽��ģʽ��
	for (int i = 0 ; i<m_CoordinateType.GetCount(); i++)
	{
		if (Point_To_Point == m_CoordinateType.GetItemData(i))
		{
		    m_CoordinateType.SetCurSel(i);
			m_CoordType = Point_To_Point;
		}
	}
	m_OriginalX.SetWindowTextA("0.0");
	m_OriginalY.SetWindowTextA("0.0");
	m_OriginalA.SetWindowTextA("0.0");


}

void  ScenePositionSettin::GetContainer(void* Pcontainer)   //�����ĳ�ʼ���������������д
{
	m_Pcontainer = Pcontainer;
	m_iCurSceneID = ((ScenePositon*)Pcontainer)->GetSceneId();
	//m_iCurIndex = ((ScenePositon*)Pcontainer)->get

}

void ScenePositionSettin::SetDisplayWindowID(int winID)
{
	m_winID = winID;
}

void ScenePositionSettin::SetCoordinate()
{
	double K,phi,B;
	int ElemNum = m_CoordinatePoint.GetCount();
	
	switch (m_CoordType)
	{
	case Point_To_Point:
		{
			if (ElemNum<2)
			{
				AfxMessageBox("��-��ϵȱ�ٵ�");
				return;
			}
			//ԭ��Ϊ��һ���㣬���ݵڶ���������㷨������Ƕȣ���ֵ��
			
			MyFunClass::Point2ToLine(m_CoordinatePoint.GetAt(ElemNum-2).x,m_CoordinatePoint.GetAt(ElemNum-2).y,
				        m_CoordinatePoint.GetAt(ElemNum-1).x, m_CoordinatePoint.GetAt(ElemNum-1).y,K,phi,B);	
			m_Coodinate.OriginalX = m_CoordinatePoint.GetAt(ElemNum-2).x;
			m_Coodinate.OriginalY = m_CoordinatePoint.GetAt(ElemNum-2).y;
			m_Coodinate.OriginalA = phi;
			m_CoordinatePoint.RemoveAll();
		}
		break;
	case Point_To_Line:
		{
			if (ElemNum<1)
			{
				AfxMessageBox("��-��ϵȱ�ٵ����");
				return;
			}
		}
		break;


	}

}


void ScenePositionSettin::ReadProcConfig(CString ConfigPath,int ProcIndex)
{
	char szValue[256]={0};
	CString ProcIndexName,strTemp;
	ProcIndexName.Format("%d",ProcIndex);
	m_CoordType = (coordinateType)GetPrivateProfileInt(ProcIndexName,"CoordinateType",-1,ConfigPath);
	m_MatchIndex = GetPrivateProfileInt(ProcIndexName,"MatchIndex",-1,ConfigPath);



}
void ScenePositionSettin::SaveProcConfig(CString ConfigPath,int ProcIndex)
{
	char szValue[256]={0};
	char szName[256] ={0};
	CString ProcIndexName,strTemp;
	ProcIndexName.Format("%d",ProcIndex);
	//�洢ģ������
	memset(szName,0,sizeof(szName));
	memset(szValue,0,sizeof(szValue));
	sprintf_s(szValue,"%d",m_CoordType);
	WritePrivateProfileStringA(ProcIndexName, "CoordinateType",szValue, ConfigPath);

	memset(szValue,0,sizeof(szValue));
	sprintf_s(szValue,"%d",m_MatchIndex);
	WritePrivateProfileStringA(ProcIndexName, "MatchIndex",szValue, ConfigPath);
	// ��Ż�ͼID


}
BOOL ScenePositionSettin::VisionProcRun( CHECK_RESULT& ProcRes)
{
	//��ѡ���ƥ��Ŵ����Ӿ����������������ȶ����ͼ�η���任������ϣ����ƥ���Ϊ��������Чֵ������ֱ����϶�λ��
	//if (m_MatchIndex >= 0)
	double row,col,Rii;

    ((ScenePositon*)m_Pcontainer)->PosFitCircle(m_MatchIndex,row,col,Rii);
	
	//�� ��ϵ����ݱ任������������ϵ��

	  //......


	return TRUE;
}


void ScenePositionSettin::OnBnClickedScenedlgPositionSearchcontour()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	 double row,col,Rii;
	 char szValue[256]={0};
	 ((ScenePositon*)m_Pcontainer)->AddCurShapeId();

	if( !((ScenePositon*)m_Pcontainer)->PosFitCircle(m_MatchIndex,row,col,Rii))
	{
		AfxMessageBox("����ƥ��ʧ��");
		return;
	} 
	 //��Բ�ķ���m_CoordinatePoint������,����ʹ��ʱ��Ϊ��ȫ���ֻʹ���������Ԫ�ء�
	 DoublePoint p1(col,row);
	 m_CoordinatePoint.Add(p1);
	 memset(szValue,0,sizeof(szValue));
	 sprintf_s(szValue,"%.6lf",col);
	  m_OriginalX.SetWindowTextA(szValue);
	 memset(szValue,0,sizeof(szValue));
	 sprintf_s(szValue,"%.6lf",row);

	 m_OriginalY.SetWindowTextA(szValue);



}

void ScenePositionSettin::OnBnClickedScenepositionCommit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetCoordinate();

}


void ScenePositionSettin::OnBnClickedScenepositionCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	((ScenePositon*)m_Pcontainer)->DelCoordinateShapeId();
	m_CoordinatePoint.RemoveAll();


}


