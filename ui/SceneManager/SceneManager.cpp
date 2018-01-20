// SceneManager.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#pragma once

#include "stdafx.h"
#include "SceneManager.h"
#include "SceneDebugDlg.h"


//������õĴ���ѡ��
#include "SceneAcqImage.h"    //ͼ��ɼ�����
#include "ScenePrePro.h"   //ͼ��Ԥ����
#include "SceneMatchModel.h"  //ƥ�䴦��
#include "HalProcPosition.h"   //halconͨ�ö�λ����
#include "HalProcProcess.h"   //halconͨ�ö�λ����

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CSceneManagerApp

BEGIN_MESSAGE_MAP(CSceneManagerApp, CWinApp)
END_MESSAGE_MAP()


// CSceneManagerApp ����


CSceneManagerApp::CSceneManagerApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSceneManagerApp ����

CSceneManagerApp theApp;


//CArray<CArray<VisionProcBase*>*> g_SceneList;
//CArray<ProcList*> g_SceneList;

// CSceneManagerApp ��ʼ��

BOOL CSceneManagerApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}



extern "C" void PASCAL EXPORT ShowSceneDebugDlg(int iLevels)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (1)
	{
		 CSceneDebugDlg dlg;
		 dlg.m_LoginLevels = iLevels;
		 dlg.DoModal();
	}else
	{
		CSceneDebugDlg* pdlg = new CSceneDebugDlg;
		pdlg->Create(IDD_DIALOG_SCENE_DEBUG);
		pdlg->m_LoginLevels = 3;
		pdlg->ShowWindow(TRUE);
	}


}

void BuildProc(VisionProcBase** ppProcBase,CString configPath,int ProcType,int SceneID,int ProcIdBase)
{
	*ppProcBase = NULL;
	switch (ProcType)
	{
	case  IMAGE_READ://VisionProcBase::READ_IMAGE:
		{		

		}
		break;
	case MAIN_PRO:
		{

		}
		break;
	case MODEL_PRO:
		{
			SceneMatchModel*  pMatchModel = new SceneMatchModel(configPath,SceneID*SCENE_PROC_ID_FACTOR + ProcIdBase);//m_pFunSettingWnd
			*ppProcBase = pMatchModel;
		}
		break;
	case THRE_PRO:
		{

		}
		break;
	case IMAGE_ACQISITION:   //ͼ��ɼ�
		{
			//�������,���캯���Ĳ���Ϊ:����·��,procID=����ID*10000+procIDBase(��������е�����)
			SceneAcqImage* pAcqImage = new SceneAcqImage(configPath,SceneID*SCENE_PROC_ID_FACTOR + ProcIdBase); 
			//����ID�����ƣ���Ϊ�˷���Ӵ���ID���Ƴ�����ID SceneID��
			*ppProcBase = pAcqImage;
		}
		break;
	case IMAGE_PREPROCESS:   //ͼ��Ԥ����
		{
			//�������,���캯���Ĳ���Ϊ:����·��,procID=����ID*10000+procIDBase(��������е�����)
			ScenePrePro* pPreImage = new ScenePrePro(configPath,SceneID*SCENE_PROC_ID_FACTOR + ProcIdBase); 
			*ppProcBase = pPreImage;
		}
		break;
		
	case CONTOUR_POSITION:
		{
			ScenePositon* pPosition = new ScenePositon(configPath,SceneID*SCENE_PROC_ID_FACTOR + ProcIdBase);
			*ppProcBase = pPosition;
		}
		break;
	case HALPROC_POSITIONG:
		{
			HalProcPosition* pHalPosition  =  new HalProcPosition(configPath,SceneID*SCENE_PROC_ID_FACTOR + ProcIdBase)  ;
			*ppProcBase = pHalPosition;
		}
		break;
	case HALPROC_PROCESS:
		{
			HalProcProcess* pHalPosition  =  new HalProcProcess(configPath,SceneID*SCENE_PROC_ID_FACTOR + ProcIdBase)  ;
			*ppProcBase = pHalPosition;
		}
		break;
	default:
		break;
	}


}

//�ѳ�������Ķ�����������
extern "C"  void PASCAL EXPORT IniSceneData(const char* SceneConfigPath ,int SceneID)
{
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());
	 //1,Ϊ�ó���������������������з���ÿ������Ķ����ַ������������󵫲���ʼ�����ڡ�
	  ProcList* pScene= new ProcList;
	  pScene->SetSceneID(SceneID);
	  //������ֵ
	  char ProcIndexName[256]={0};
	  char stText[256]={0};

	 int pkgNum =  GetPrivateProfileInt("scene", "pkgnum", 0, SceneConfigPath);
	 GetPrivateProfileString("scene","NextSceneRotAngle","",stText,sizeof(stText),SceneConfigPath);
	 pScene->SetNextRotAngle(atof(stText));
	 pScene->SetRelaSceneID(GetPrivateProfileInt("scene", "LastSceneId", -1, SceneConfigPath));
	 pScene->SetNextSceneID(GetPrivateProfileInt("scene", "NextSceneId", -1, SceneConfigPath));

	 VisionProcBase* pVisionProc = NULL;
	 for (int i = 0 ; i <pkgNum ;i++)
	 {
		 pVisionProc = NULL;
		 sprintf(ProcIndexName,"%d",i);//�����ļ����[0]��ʼ��
		 int ProcType = GetPrivateProfileInt(ProcIndexName, "ProcType", -1, SceneConfigPath);

		  BuildProc(&pVisionProc,SceneConfigPath,ProcType,SceneID,i);


		if( NULL != pVisionProc)
		{
			 pScene->AddProc(pVisionProc);	  
		}

	 }
	  if (0<pScene->GetProcCount())
	  {
		  g_SceneList.AddScene(pScene);
	  }
	  
}

void  PASCAL EXPORT  InitGrabWay2Sceneimage(int SceneID,bool sendflag)
{
	//�ҵ����ն����õ����ղ�����
	//g_SceneList.
	 VisionProcBase* pVisionProc = NULL;
	ProcList* pScene =  g_SceneList.GetSceneBySceneID(SceneID);
	
	if (!pScene)
	{
		return;
	}
	for (int i=0;i<pScene->GetProcCount();i++)
	{
		if(pScene->GetProcType(i) == IMAGE_ACQISITION ) 
		{
			pVisionProc = pScene->GetProcByIndex(i);
			break;  //ֻץ�����еĵ�һ��������Ϊ�ɼ���
		}
	}
	if (!pVisionProc)
	{
		return;
	}

	//���ù���������պ���,��ʼ������û�������ع�ȣ�������ȫ������һ�����պ��������棬���԰����õ��ع����ý�ȥ
	//InitGrabSceneimage1(((SceneAcqImage*)pVisionProc)->m_CamIndex,SceneID);


	((SceneAcqImage*)pVisionProc)->ProcGrabImage(sendflag);


}


BOOL  SceneVisionProcess(int iSceneID,CHECK_RESULT& ProcRes)
{
	ProcList* pScene = g_SceneList.GetSceneBySceneID(iSceneID);
	if (pScene)
	{
		if (pScene->RunSceneSingle(ProcRes))//ÿ������������и�RunSceneSingle������˳��ִ��,ʧ���˾Ͳ���ִ�к��������
		{
			//������ȷ��ɺ�ĺ���������������ڳ�����ÿһ��������Ӻ���Ӧ�޸ģ�
			return TRUE;
		}else
		{
			return FALSE;
		}
		
	}else
	{
		//ProcRes.err = -1;
		return FALSE;
	}

}




