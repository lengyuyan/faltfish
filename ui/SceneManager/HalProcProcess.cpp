#include "StdAfx.h"
#include "HalProcProcess.h"
#include "SceneAcqImage.h"

HalProcProcess::HalProcProcess(void)
{
		BuildInstance();
}

HalProcProcess::HalProcProcess(CString ConfigPath,int ProcID):VisionProcBase(ConfigPath,ProcID)
{
		BuildInstance();
}

HalProcProcess::~HalProcProcess(void)
{
	m_HalCommomPosSet.CloseSettingDlg();
}


void HalProcProcess::BuildInstance(void)
{
	CString szTemp;
	m_ProcTypeId = HALPROC_PROCESS;  
	m_HalCommomPosSet.m_ProcProcess = 0;
	m_HalCommomPosSet.GetContainer(this);
	ReadProcConfig(m_ProcNo);

}


void HalProcProcess::InitInstance(CWnd* pParentWnd)
{	
	CRect rtS;
	if (NULL == pParentWnd)
	{
		return;
	}
	m_pParentWnd = pParentWnd;

	assert(pParentWnd->IsKindOf( RUNTIME_CLASS(CWnd)));
	m_pParentWnd->GetClientRect(&rtS);
	m_HalCommomPosSet.Create(IDD_HALPROC_NPROCESS, m_pParentWnd);
	m_HalCommomPosSet.SetWindowPos(NULL,rtS.left,rtS.top,rtS.right,rtS.bottom,SWP_NOZORDER);
	m_HalCommomPosSet.ShowWindow(SW_SHOW);
	m_HalCommomPosSet.UpdateUI();


	m_SceneInitialCount = g_SceneInitialCount;

}

void HalProcProcess::DeleteSelf()
{
	delete this;

}


void HalProcProcess::HideConfigWnd()
{
	if(g_SceneInitialCount==m_SceneInitialCount)
	{
		m_HalCommomPosSet.ShowWindow(SW_HIDE);
	}
}

void HalProcProcess::ShowConfigWnd()
{
	if(g_SceneInitialCount==m_SceneInitialCount)
		m_HalCommomPosSet.ShowWindow(SW_SHOW);
}


void HalProcProcess::SaveProcConfig(int ProcIndex)
{
	VisionProcBase::SaveProcConfig(ProcIndex);
	//����������

	m_HalCommomPosSet.SaveProcConfig(m_ConfigPath,ProcIndex);

}

void HalProcProcess::ReadProcConfig(int ProcIndex)
{
	VisionProcBase::ReadProcConfig(ProcIndex);
	//����������


	m_HalCommomPosSet.ReadProcConfig(m_ConfigPath,ProcIndex);

}

bool HalProcProcess::CommonProcess(string pathName,vector<double>&inparam,vector<double>&outparam,int display,int& error,string &hmessage)
{
	int ImageIndex = g_SceneList.GetSceneBySceneID(GetSceneId())->GetProcImageIndex(GetProcId());
	SceneAcqImage* pAcqImage = dynamic_cast<SceneAcqImage*>(g_SceneList.GetSceneBySceneID(GetSceneId())->GetProcByTypeIndex(IMAGE_ACQISITION,ImageIndex));
	int CamIndex = pAcqImage->m_CamIndex; //�������
	long winId = -1;

	if(g_SceneInitialCount==m_SceneInitialCount&&g_SceneInitialCount!=0)
	{
		winId = g_SceneDlgHwinId;
	}else
	{
		SYSTEM_PARAM pSysParam;
		GetSysParam(&pSysParam);
		vector<CStatic *>* pImageWnd = ((vector<CStatic *>*)(pSysParam.MainImageWndVec));
		if (CamIndex < pImageWnd->size())
		{
			winId = (long)(pImageWnd->at(CamIndex)->m_hWnd);//��ʾ�������Ӧ�Ĵ���
		}else
		{
			winId = (long)(pImageWnd->at(0)->m_hWnd);//��ʾ�ڵ�һ����
		}
	} 

	return HalconProcedureProcess(winId,GetSceneId(),ImageIndex,pathName,inparam,outparam,display,error,hmessage);
	
}

BOOL HalProcProcess::VisionProcRun(CHECK_RESULT& ProcRes)
{
	bool ResRun = false;
	//double RotPhi,MoveX,MoveY;
	SYSTEM_PARAM sysParam;
	GetSysParam(&sysParam);
	SCENE_PKG config;
	GetSceneParam(GetSceneId(),&config);
	int AxisIndex = config.AxisIndex;
	if (AxisIndex<0)
	{
		AfxMessageBox("��������ô���");
		return false;
	}
	int imageIndex = (g_SceneList.GetSceneBySceneID(GetSceneId()))->GetProcImageIndex(GetProcId());
	SceneAcqImage* pAcqImage = dynamic_cast<SceneAcqImage*>(g_SceneList.GetSceneBySceneID(GetSceneId())->GetProcByTypeIndex(IMAGE_ACQISITION,imageIndex));
	int CamIndex = pAcqImage->m_CamIndex; //�������

	//3 �����ú�������
	if (m_HalCommomPosSet.VisionProcRun(ProcRes))
	{
		ProcRes.isGeneralProcess = HALPROC_PROCESS;
		ResRun = true;
	}else
	{
		ProcRes.err = HALPROC_PROCESS;
		ResRun = false;
	}

	//4������Ҫ�ı��ֵ�����������С�
	//��ʾ���
	vector<CStatic *>* pImageWnd = ((vector<CStatic *>*)(sysParam.MainImageWndVec));
	long HalconWinID = -1;
	if(-1 == g_SceneDlgHwinId)
	{
		FindHalconWinID((int)(pImageWnd->at(CamIndex)->m_hWnd),HalconWinID);
	}
	else
	{
		FindHalconWinID(g_SceneDlgHwinId,HalconWinID);
	}
	
	if (ResRun)
	{
		SetVisionColorHeight(HalconWinID,"green");
		ShowVisionString(HalconWinID," Hal����OK");

	}else 
	{
		SetVisionColorHeight(HalconWinID,"red");
		ShowVisionString(HalconWinID," Hal����NG");
	}

	//���ֻ�������̣��򷵻�OK
	if (sysParam.IsSimulate)
	{
		ResRun = true;
		
	}

	return ResRun;

}
