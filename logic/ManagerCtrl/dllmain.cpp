// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣  //iManagerCtrl.cpp
#include "stdafx.h"
#include "ManagerCtrl.h"
#include "iManagerCtrl.h"
#include "iImageCtrl.h"
#include "iLightCtrl.h"
#include "iCommunication.h"
#include "iConfigManager.h"

bool InitManager()
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		pInstance->UnInitManager();
		Sleep(500);
		bRet = pInstance->InitManager();
	}
	return true;
}

bool UnInitManager()
{
	bool bRet = false;
	CManagerCtrl::ReleaseInstance();
	return true;
}



void GetSceneProcess(pSceneProcess* pProcess)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		pInstance->GetSceneProcess(pProcess);
	}

}


/**********************************************************************
����      : GetSceneNameInfo
����      : ��ȡ������Ϣ
�������  : N/A
�������  : iSceneNum ��������
pSceneName ���������б�
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool GetSceneNameInfo(int &iSceneNum, char **pSceneName)
{
	return true;
}

bool TriggerVisionProcess(int iSceneID, int iPercent)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->TriggerVisionProcess(iSceneID, iPercent);
	}
	return bRet;
}

/**********************************************************************
����      : SetSysParam
����      : �����Ӿ�����
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
pData    Ҫ����ֵ�Ľṹ��ָ��
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool SetSysParam(void *pData)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->SetSysParam(pData);
	}
	return bRet;
}

/**********************************************************************
����      : SaveSysParam
����      : ���²������Ӿ�����
�������  : N/A 
pData    Ҫ����ֵ�Ľṹ��ָ��
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool SaveSysParam(void *pData)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->SaveSysParam(pData);
	}
	return bRet;
}
/**********************************************************************
����      : GetSysParam
����      : ��ȡϵͳ����
�������  : N/A 
�������  : pData    Ҫ��ȡ�Ľṹ��ָ��
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool GetSysParam(void *pData)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->GetSysParam(pData);
	}
	return bRet;
}


void* GetSysParamPoint()
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		return pInstance->GetSysParamPoint();
	}
	return NULL;

}


bool SetSceneParam(void *pData)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->SetSceneParam(pData);
	}
	return bRet;
}
/**********************************************************************
����      : SaveSceneParam
����      : ���沢�����Ӿ�����
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
pData    Ҫ����ֵ�Ľṹ��ָ��
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool SaveSceneParam(int iSceneID, void *pData)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->SaveSceneParam(iSceneID, pData);
	}
	return bRet;
}

bool  GetSceneParam(int iSceneID, void *pData)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->GetSceneParam(iSceneID, pData);
	}
	return bRet;
}

void* GetSceneParamHandle(int iSceneID)
{
	return CManagerCtrl::GetSceneHandle(iSceneID);

}



EXPORT_C DLL_EXPORT int GetSceneNum()
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		return pInstance->GetSceneNum();
	}
	return -1;

}
EXPORT_C DLL_EXPORT char* GetSceneConfigName()
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		return pInstance->GetSceneName();
	}
	return NULL;
}

EXPORT_C DLL_EXPORT const char* GetSceneConfigPath(int SceneID)
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		return pInstance->GetSceneconfigPath(SceneID);
	}
	return NULL;

}

EXPORT_C DLL_EXPORT const char* GetScenePath(int SceneID)
{

	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		return pInstance->GetScenePath(SceneID);
	}
	return NULL;


}



EXPORT_C DLL_EXPORT int GetSceneProcType(int SceneID)
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		return 	pInstance->GetSceneProcType(SceneID);
	}
	return -1;

}

EXPORT_C DLL_EXPORT int GetSceneId(int SceneIndex)
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{

		return 	pInstance->GetSceneId(SceneIndex);

	}
	return -1;



}




EXPORT_C DLL_EXPORT char* GetSysConfigPath()
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		return pInstance->GetSysconfigPath();
	}
	return NULL;
}

void SetCamParam(int CamIndex,int shutterTime,int Gain)
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		pInstance->SetCamParam(CamIndex,shutterTime,Gain);
	}

}




int SceneGrabImage(int iSceneID,int ImageIndex,int CamIndex,int shutterTime,int Gain,int Interval,int LightIndex,int LightValue)
{

	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		return pInstance->SceneGrabImage(iSceneID, ImageIndex,CamIndex,shutterTime,Gain,Interval,LightIndex,LightValue);
	}
	return -1;
}

int SceneGrabFinished(int iSceneID, CHECK_RESULT *res,int iErrorCode)
{

	CManagerCtrl *pInstance = NULL;
	int TelSceneId = iSceneID;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		pInstance->ManagerGrabFinish(TelSceneId, res,iErrorCode);
		return 0;
	}
	return -1;

}




bool InitGrabSceneimage(int iSceneID, int index)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->InitGrabSceneimage(iSceneID, index);
	}
	return bRet;
}

bool InitGrabSceneimage1(int CamIndex ,int iSceneID,int ImageIndex)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->InitGrabSceneimage1( CamIndex , iSceneID, ImageIndex);
	}
	return bRet;

}


/**********************************************************************
����      : GetCameraIDList
����      : ��ȡ���GUID�б�
�������  : guidlist�� GUID�б�
�������  : N/A
����ֵ    : ���ؽ��
��ע      : 
**********************************************************************/
int GetCameraIDList(std::list<std::string> &guidlist,int dllnum,string dlllist[])
{
	return getGUIDList(guidlist,dllnum,dlllist);
}

bool SetDispWindow(int iSceneID, int iWinID)
{
	return true;
}

bool OpenContinousGrab(int iWindowID, int iSceneID, int index, unsigned int MsgID,int CameraIndex)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->OpenContinousGrab(iWindowID, iSceneID, index, MsgID,CameraIndex);
	}
	return bRet;
}
bool ColseContinousGrab(int iSceneID, int index,int CameraIndex)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->ColseContinousGrab(iSceneID, index,CameraIndex);
	}
	return bRet;
}

/**********************************************************************
����      : OpenWindow
����      : ��halcon������UI����
�������  : iWinID ���洰��ID
x,y,width,hight ���洰��λ�ô�С
�������  : 
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool BindWindow(long iWinID, int x, int y, int width, int hight, bool bDebug)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->BindWindow(iWinID, x, y, width, hight, bDebug);
	}
	return bRet;
}

bool SetDebugWindowID(int iSceneID, long iWinID)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->SetDebugWindowID(iSceneID, iWinID);
	}
	return bRet;
}
/**********************************************************************
����      : ClearWindow
����      : �����Ӧ��������Ϣ
�������  : iWinID ���洰��ID
�������  : 
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool ClearWindow(long iWinID)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->ClearWindow(iWinID);
	}
	return bRet;
}

/**********************************************************************
����      : ReadImage
����      : ���ر���ͼƬ
�������  : iSceneID  �������
pFileName �ļ�����
index ��ǰ�����еĵڼ���ͼ�񣨴��ڶ���ͼ������ܸ���index���ֲ�ͬ��Դ���ͼ������
�������  : 
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool ReadImage(const char *pFileName, int iSceneID, int index)
{
	bool bRet;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	bRet = pInstance->ReadImage(iSceneID, pFileName, index);
	return bRet;
}

/**********************************************************************
����      : DispImage
����      : ��ʾָ��ͼƬ��ָ��������
�������  : iWinID ���洰��ID
iSceneID  �������
index     ͼ����ţ�����ʹ��
�������  : 
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool DispImage(long iWinID, int iSceneID, int index)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->DispImage(iWinID, iSceneID, index);
	}
	return bRet;
}


bool FindHalconWinID(int iWinID, long& HalconWinID)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->FindHalconWinID( iWinID,HalconWinID);
	}
	return bRet;


}

bool ShowVisionString(long winID,char* StrContent,int topRow,int topCol )
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->ShowVisionString(winID,StrContent,topRow,topCol);
	}
	return bRet;


}
bool SetVisionColorHeight(long hwinID,char* strColor,int FontHeight)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->SetVisionColorHeight(hwinID,strColor,FontHeight);
	}
	return bRet;


}


/**********************************************************************
����      : DispObj
����      : ��ʾ������
�������  : iWinID ���洰��ID
iSceneID  �������
bIsShowTmp ���Բ�����������ʾ�м����
�������  : 
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool DispObj(int iWinID, int iSceneID, bool bIsShowTmp)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->DispObj(iWinID, iSceneID, bIsShowTmp);
	}
	return bRet;
}

bool SaveImage(int iSceneID, int index, string SNstr)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->SaveImage(iSceneID,index, SNstr);
	}
	return bRet;
}

EXPORT_C DLL_EXPORT bool SaveImageDump( const long winID, int iSceneID,char* outpath )
{
	bool bRet = false;   
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->SaveImageDump(winID, iSceneID,outpath);
	}
	return bRet;
}

EXPORT_C DLL_EXPORT bool SendImageDump( void *buff, int size,int w,int h,int iSceneID )
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		SendImage(iSceneID,buff,size,w,h);
	}
	return true;
}

EXPORT_C DLL_EXPORT void* GetImageDump( int winID, int sence,double &width,double &heigh )
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		return pInstance->GetImageDump(winID,sence,width,heigh);
	}
	return NULL;
}
/**********************************************************************
����      : DispROI
����      : ��Ϊ����ROIģʽ
�������  : iWinID ����ID
iSceneID  ����ID�����ݸ�ID��ȡ����
index ��ǰ�����еĵڼ���ROI
iType, 0,Ϊ����ͼ������, 1Ϊģ������2Ϊ��ֱ�ߣ� 3Ϊ��Բ�� ����Ϊö������
�������  : 
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool DispROI(int iWinID, int iSceneID, int index, PROCESS_TYPE iType)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->DispROI(iWinID, iSceneID, index, iType);
	}
	return bRet;
}

/**********************************************************************
����      : DrawROI
����      : ��Ϊ����ROIģʽ
�������  : iSceneID  �������
index ��ǰ�����еĵڼ���ROI
iType, 0,Ϊ����ͼ������, 1Ϊģ������2Ϊ��ֱ�ߣ� 3Ϊ��Բ�� ����Ϊö������
�������  : 
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool DrawROI(int iWinID, int iSceneID, int index, PROCESS_TYPE iType)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->DrawROI(iWinID, iSceneID, index, iType);
	}
	return bRet;
}

/**********************************************************************
����      : DrawROI
����      : ֱ�ӻ���ROI
�������  : iSceneID  �������
iType, ��ֱ�ߣ� ��Բ�� 
�������  : 
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/

bool OnDrawROI(int iWinID, int iSceneID,PROCESS_TYPE iType,RoiCombType CombType,vector<int>* pRoiIdList)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->OnDrawROI(iWinID, iSceneID, iType,CombType,pRoiIdList);
	}
	return bRet;
}

// int  GetLastShapeId(int iSceneID)
//{
//	CManagerCtrl *pInstance = NULL;
//	pInstance = CManagerCtrl::GetInstance();
//	if (NULL != pInstance)
//	{
//		return  pInstance->GetLastShapeId(iSceneID);
//	}
//	
//
//}

void  GenShape(int SceneID,const ST_ROI_INFO& roi,ShapeType shapeflag,int ShapeId,int CamIndex)
{

	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		pInstance->GenShape(SceneID, roi, shapeflag,ShapeId,CamIndex);
	}

}


int GetCurShapeId(int iSceneID)
{

	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		return pInstance->GetCurShapeId(iSceneID);
	}
	return -1;

}

bool   GetRoiParam(int sceneID,int ShapeId,ST_ROI_INFO* pRoi)
{

	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->GetRoiParam(sceneID,ShapeId,pRoi);
	}
	return bRet;


}


void OnShowAllRoi(int iWinID, int iSceneID,const vector<int>* const pRoiDdList)
{

	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		pInstance->OnShowAllRoi(iWinID, iSceneID,pRoiDdList);
	}

}







/**********************************************************************
����      : CreateVisionModel
����      : ����ģ��
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CreateVisionModel(int iWinID,int iSceneID, int index)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->CreateVisionModel(iWinID,iSceneID, index);
	}
	return bRet;
}

///**********************************************************************
//����      : CreateWay2VisionModel
//����      : ����ģ��
//�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
//�������  : N/A
//����ֵ    : �ɹ���ʧ��
//��ע      : 
//**********************************************************************/
bool CreateWay2VisionModel(int iWinID,int iSceneID,int RoiID,char* modePath, int Nevels,double AngleStart,double AngleExtent,
	int Contrast,int index)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->CreateVisionModel(iWinID,iSceneID,RoiID, modePath,Nevels,AngleStart,AngleExtent,Contrast,index);
	}
	return bRet;

}





///**********************************************************************
//����      : UpdateVisionModel
//����      : ����ģ��
//�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
//�������  : N/A
//����ֵ    : �ɹ���ʧ��
//��ע      : 
//**********************************************************************/
//bool UpdateVisionModel(int iSceneID, int index, int iType)
//{
//    bool bRet = false;
//    CManagerCtrl *pInstance = NULL;
//    pInstance = CManagerCtrl::GetInstance();
//    if (NULL != pInstance)
//    {
//        if(pInstance->UpdateVisionModel(iSceneID, index, iType) >= 0)
//        {
//            bRet = true;
//        }
//    }
//    return bRet;
//}

/**********************************************************************
����      : FindVisionModel
����      : ����ģ��
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool FindVisionModel(int iSceneID, int index, bool bIsB)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->FindVisionModel(iSceneID, index, bIsB);
	}
	return bRet;
}
/**********************************************************************
����      : FindVisionModel
����      : ����ģ��
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool FindWay2VisionModel(int iSceneID, int WinID,int index,char* szModePath,int PyramidLevel,int MatchNum,double& Score,
	int ProcIndex,bool IsBulid,double& row,double& col,double& phi)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->FindWay2VisionModel(iSceneID,WinID, index, szModePath, PyramidLevel, MatchNum, Score,ProcIndex,IsBulid, row, col, phi);
	}
	return bRet;
}
EXPORT_C DLL_EXPORT bool FindModelRealTime(int iSceneID, int WinID, int index,char* szModePath,int PyramidLevel,int MatchNum, double& Score,
	int ProcIndex,bool IsBulid,double& row,double& col,double& phi)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->FindModelRealTime(iSceneID,WinID, index, szModePath, PyramidLevel, MatchNum, Score,ProcIndex,IsBulid, row, col, phi);
	}
	return bRet;

}

void MoveAndRotInCamForOriMatch(int iSceneID,double PixSize,bool AxeMoveRotDirect,double& MotorRelX,double& MotorRelY,
	int LastSceneId,int NextSceneId,double SceneRotPhi,double PicCentX,double PicCentY,double* pCamParam ,int ParamNum,double* pCamPose,double CalThick,double RotCenX,
	double RotCenY,double MotorRotPhi,double CalRotPhi,int CamIndex,int AxeIndex,int ToolIndex)
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		pInstance->MoveAndRotInCamForOriMatch(iSceneID,PixSize,AxeMoveRotDirect,MotorRelX, MotorRelY,LastSceneId,
			NextSceneId,SceneRotPhi,PicCentX,PicCentY,RotCenX, RotCenY, MotorRotPhi, CalRotPhi,
			pCamParam,ParamNum,pCamPose,CalThick, CamIndex, AxeIndex, ToolIndex);
	}

}

bool AffinePointByCamInMecRot(int CamIndex,int AxisIndex,int ToolIndex,double& PointX,double& PointY)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->AffinePointByCamInMecRot(CamIndex, AxisIndex,ToolIndex, PointX, PointY);
	}
	return bRet;


}

void AffToRobotWorld(int SceneId,int CamIndex,int AxisIndex,int ToolIndex,double PixSize,double ModRotPhi,
	double& ToolWorldX,double& ToolWorldY,bool WithCamParam,double CalThickness,double* pCamParam,int ParamNum,double* CamPos)
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		pInstance->AffToRobotWorld(SceneId,CamIndex, AxisIndex, ToolIndex, PixSize, ModRotPhi,
			ToolWorldX,ToolWorldY,WithCamParam,CalThickness,pCamParam,ParamNum,CamPos);
	}
}


void AffinePointToWorld(int CamIndex,int AxisIndex,int ToolIndex,double PixSize,double OriCamX,double OriCamY,
	double OriCamPhi,double CurCamX,double CurCamY,double CurCamPhi,double& MoveX,double& MoveY,int ResType,
	bool MoveRotDirect,double CurMatchPhi,double AxeRotX,double AxeRotY)
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		pInstance->AffinePointToWorld(CamIndex,AxisIndex,ToolIndex,PixSize,OriCamX, OriCamY,OriCamPhi,CurCamX, 
			CurCamY, CurCamPhi,MoveX,MoveY, ResType,MoveRotDirect, CurMatchPhi, AxeRotX, AxeRotY);
	}
}




/**********************************************************************
����      : VisionDebugThreshold
����      : ������ֵ�ӿ�
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
index�� ���ݸ�index��ȡ����
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool VisionDebugThreshold(int iWinID, int iSceneID, int index)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->VisionDebugThreshold(iWinID, iSceneID,index);
	}
	return bRet;
}

/**********************************************************************
����      : VisionFindLine
����      : ���߽ӿ�
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
index�� ���ݸ�index��ȡ��Բ����
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool VisionFindLine(int iSceneID, int index, bool bIsFollow)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->VisionFindLine(iSceneID,index, bIsFollow);
	}
	return bRet;
}

/**********************************************************************
����      : VisionFindCircle
����      : ��Բ�ӿ�
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
index�� ���ݸ�index��ȡ��Բ����
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool VisionFindCircle(int iSceneID, int index, bool bIsFollow)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->VisionFindCircle(iSceneID,index, bIsFollow);
	}
	return bRet;
}

bool  VisionMeasureCircle(long WinId, int iSceneID, int Imageindex,int MatchIndex, double& Row,double& Col,double& Rii)
{
	bool bRet = false;
	long HalconWinID;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		FindHalconWinID( WinId,  HalconWinID);
		bRet = pInstance->VisionMeasureCircle(HalconWinID,iSceneID,  Imageindex, MatchIndex, Row, Col, Rii);
	}
	return bRet;
}



/**********************************************************************
����      : CtrlComm
����      : ��or�رչ�Դ
�������  : pCommName ��������
bOpen     �򿪲�����رղ���
iType     ��Դ����
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CtrlComm(LightInfo *pInfo, bool bOpen, LightTypeInfo *pTypeInfo)
{
	//LightInfo info;
	//info.PortName = pCommName;
	//info.LightType = (LIGHTCTL_TYPE)iType;
	if (NULL == pInfo)
	{
		return false;
	}
	if (bOpen)
	{
		if(LIGHTCTL_SUCCESS != iLightInstance(pInfo, 1, pTypeInfo))
		{
			return false;
		}
	}
	else
	{
		if(LIGHTCTL_SUCCESS != deInitLightCtrl(pInfo->PortName))
		{
			return false;
		}
	}
	return true;
}
void RestoreImagePart(int WinID, int iScene, int width, int height, int index)
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		pInstance->RestoreImagePart(WinID, iScene, width, height, index);
	}
}

void UpdateImagePart(int WinID, int iScene, int iType, double dXPer, double dYPer)
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		pInstance->UpdateImagePart(WinID, iScene, iType, dXPer, dYPer);
	}
}

/**********************************************************************
����      : VisionProcess
����      : ������
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool VisionProcess(int iSceneID, int index,CHECK_RESULT &result)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;

	memset(&result,0,sizeof(CHECK_RESULT));
	result.bIsDisp = true;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->VisionGetResult(iSceneID,index,result);
	}
	return bRet;
}

/**********************************************************************
����      : VisionProcess
����      : ������
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool VisionCalibProcess(CALIB_DATA *pData)
{
	bool bRet = true;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->VisionCalibProcess(pData);
	}
	return bRet;
}
/**********************************************************************
����      : OpenCheckStateThread
����      : �򿪼��ϵͳ״̬�߳�
�������  :   

�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool OpenCheckStateThread()
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->OpenCheckStateThread();
	}
	return bRet;
}



int ResetFinishedSendMsg(int iErrorCode)
{
	return ResetFinished(iErrorCode);
}


int JudgeLightConnectStatus(string comName)
{
	return JudgeLightConnect(comName);
}
//��ȡ��Ӧ���Ͳ�����pkg->pBody�е�λ��
//input ���ͣ� ������ pkg, 
//�����ҵ�λ��
int IFindParamData(SCENE_PKG *pkg, int iType, int index)
{
	int iRet = 0;
	iRet = CManagerCtrl::FindParamData(pkg,iType, index);
	return iRet;
}
int FindSpecTypePos(const VIRSION_DATA_PKG *pkg, unsigned int *pType, int iType)
{
	int iRet = 0;
	iRet = CManagerCtrl::FindSpecTypePos(pkg,pType, iType);
	return iRet;
}

int InsertParamItem(int SceneId, VIRSION_DATA_PKG *pkg, unsigned int *pType, int iSize, int insertPos)
{
	int iRet = 0;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		iRet = pInstance->InsertParamItem(SceneId, pkg, pType, iSize, insertPos);
	}
	return iRet;
}

int UpdateParamItem( int SceneId, const VIRSION_DATA_PKG *pkg, unsigned int *pType, INTERFACE_SET_DATA *dataTmp, int iSize, int insertPos, bool insert /*= false*/ )
{
	int iRet = 0;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		iRet = pInstance->UpdateParamItem(SceneId, pkg, pType, dataTmp, iSize, insertPos, insert);
	}
	return iRet;
}
int SwapParamItem(int SceneId, int iPosF, int iPosN)
{
	int iRet = 0;
	return iRet;
}
int DeleteParamItem(int SceneId, int insertPos)
{
	int iRet = 0;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		iRet = pInstance->DeleteParamItem(SceneId, insertPos);
	}
	return iRet;
}

int OpratorLight(string comName, int index, int value, int type)
{
	int iRet = 0;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		switch(type)
		{
		case TYPE_OPEN:
			{
				iRet = openLight(comName, index);
			}
			break;
		case TYPE_CLOSE:
			{
				iRet = closeLight(comName, index);
			}
			break;
		case TYPE_SET_VAULE:
			{
				iRet = setLightValue(comName, index, value);
			}
			break;
		default:
			iRet = -1;
			break;
		}
	}
	return iRet;
}


void  SetWebProctocol(const PROCTOCOLDATA& ProctoData)
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		pInstance->SetProctocolData(ProctoData);
	}
	SetCommuniWebProctocol(ProctoData);
}

void  GetWebProctocol(PROCTOCOLDATA& ProctoData)
{
	GetCommuniWebProctocol(ProctoData);
}


void DrawRect(int SenceId)
{
	bool bRet = false;
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		//bRet = pInstance->m_pVisionInstance->DrawRegionRect(SenceId);
	}

}

////��Ʒ���趨���������ڻ���Ʒʱһ���л�
void SetProductMap(void* pMapData)
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		((SYSTEM_PARAM*)(pInstance->GetSysParamPoint()))->ProductMap = *(LPPMAPDATA)pMapData;
		pInstance->SetProductMap(pMapData);

	}


}
void GetProductMap(void* pMapData)
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		*(LPPMAPDATA)pMapData = ((SYSTEM_PARAM*)(pInstance->GetSysParamPoint()))->ProductMap;
		//pInstance->GetProductMap(pMapData); //��Ӧ��ʵʱ�ش������ļ����ȡ
	}


}


void SetSysCurProductName(string ProductName)
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		pInstance->SetCurProductName(ProductName);
	}

}

void SaveSysRotAxleAdjust(int AxisIndex, int ToolIndex,double RotXAd,double RotYAd)
{

	ConfigSaveSysRotAxleAdjust(AxisIndex,ToolIndex,RotXAd,RotYAd);
}

void ReadSysRotAxleAdjust(int AxisIndex, int ToolIndex,double& RotXAd,double& RotYAd)
{
	ConfigReadSysRotAxleAdjust(AxisIndex,ToolIndex,RotXAd,RotYAd);
}


bool HalconProcedureLocate(long iWinID,int iSceneID,int Imageindex,string ProcedurePath,double (*inParam)[HalProcInParamNum],
	double& PosX,double& PosY,double& PosPhi)
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		return  pInstance->HalconProcedureLocate(iWinID,iSceneID,Imageindex,ProcedurePath,inParam,PosX,PosY,PosPhi);
	}
	return false;

}
bool HalconProcedureProcess(long iWinID,int iSceneID,int Imageindex,string ProcedurePath,vector<double> &inparam,vector<double> &outparam,int display,int &error,string &hmessage)
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		return  pInstance->HalconProcedureProcess(iWinID,iSceneID,Imageindex,ProcedurePath,inparam,outparam,display,error,hmessage);
	}
	return false;
}

bool IImageEnhancement(long iWinID,int iSceneID,int Imageindex,double& multinum,double& addnum,int display,int &error,string &hmessage)
{

	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		return  pInstance->ImageEnhancement(iWinID,iSceneID,Imageindex,multinum,addnum,display,error,hmessage);
	}
	return false;
}


CHECK_RESULT* GetSceneProcResult(string &scneName)
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		return  pInstance->GetSceneProcResult(scneName);
	}
	return false; 
}
CHECK_RESULT* GetSceneProcResultById(int  iSceneID)
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		return  pInstance->GetSceneProcResultById(iSceneID);
	}
	return false; 
}

int SetSceneProcResult(string &scneName,CHECK_RESULT& res)
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		return  pInstance->SetSceneProcResult(scneName,res);
	}
	return false; 
}
int  ClearProcResultByID(int iSceneID)
{
	CManagerCtrl *pInstance = NULL;
	pInstance = CManagerCtrl::GetInstance();
	if (NULL != pInstance)
	{
		return  pInstance->ClearProcResultByID(iSceneID);
	}
	return false; 
}




