// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣  // iConfigManager��
#include "stdafx.h"
#include "iConfigManager.h"
#include "ConfigManager.h"

/**********************************************************************
����      : InitConfig
����      : ��ʼ������
�������  : N/A
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool InitConfig(void *pData)
{
    bool bRet = false;
    CConfigManager *pInstance = NULL;
    pInstance = CConfigManager::GetInstance();
    if (NULL != pInstance)
    {
        bRet = pInstance->InitConfig(pData);
    }
    return bRet;
}
/**********************************************************************
����      : ReleaseConfigInstance
����      : ȥ��ʼ������
�������  : N/A
�������  : N/A
����ֵ    : N/A
��ע      : 
**********************************************************************/
void ReleaseConfigInstance()
{
    CConfigManager::ReleaseInstance();
}

/**********************************************************************
����      : SaveSysConfig
����      : ����ϵͳ����
�������  : N/A
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool SaveSysConfig(void *pData)
{
    bool bRet = false;
    CConfigManager *pInstance = NULL;
    pInstance = CConfigManager::GetInstance();
    if (NULL != pInstance)
    {
        bRet = pInstance->SaveConfig(pData);
    }
    return bRet;
}



DLL_EXPORT bool SaveSysCamCalibrate(int CamerIndex,int ToolIndex,int DataCount,double MotorX[],double MotorY[],double PixRow[],double PixCol[])
{
	bool bRet = false;
	CConfigManager *pInstance = NULL;
	pInstance = CConfigManager::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->SaveCamCalibrate( CamerIndex,ToolIndex, DataCount, MotorX, MotorY, PixRow, PixCol);
	}
	return bRet;

}
EXPORT_C DLL_EXPORT bool SaveMecAndCamRot(int CamIndex,int AxeIndex,int ToolIndex,double (*pCamRot),
	double (*pMecAndCam), double (*pImageRela),double (*pImagAbs),int DataNum)
{
	bool bRet = false;
	CConfigManager *pInstance = NULL;
	pInstance = CConfigManager::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->SaveMecAndCamRot(CamIndex,AxeIndex,ToolIndex,pCamRot,pMecAndCam,
			           pImageRela,pImagAbs,DataNum);
	}
	return bRet;
}
EXPORT_C DLL_EXPORT bool SavePicCenter(int CamIndex,int AxeIndex,int ToolIndex,double X,double Y)
{
	bool bRet = false;
	CConfigManager *pInstance = NULL;
	pInstance = CConfigManager::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->SavePicCenter(CamIndex,AxeIndex,ToolIndex,X,Y);
	}
	return bRet;
}
EXPORT_C DLL_EXPORT bool ReadMecAndCamRot(int CamIndex,int AxeIndex,int ToolIndex,double (*pMecAndCam),int& DataNum)
{
	bool bRet = false;
	CConfigManager *pInstance = NULL;
	pInstance = CConfigManager::GetInstance();
	if (NULL != pInstance)
	{
		//bRet = pInstance->ReadMecAndCamRot( CamIndex,AxeIndex,ToolIndex, pMecAndCam,DataNum);
	}
	return bRet;


}


EXPORT_C DLL_EXPORT bool SaveSysCamPixSize(int CamIndex, int AxeIndex,int ToolIndex,double PixSize)
{
	bool bRet = false;
	CConfigManager *pInstance = NULL;
	pInstance = CConfigManager::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->SaveSysCamPixSize( CamIndex,AxeIndex,ToolIndex, PixSize);
	}
	return bRet;

}

EXPORT_C DLL_EXPORT bool SaveSysRotAxleToCam(int CamIndex,int AxisIndex, int ToolIndex,double RotX,double RotY)
{
	bool bRet = false;
	CConfigManager *pInstance = NULL;
	pInstance = CConfigManager::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->SaveSysRotAxleToCam(CamIndex, AxisIndex,ToolIndex, RotX,RotY);
	}
	return bRet;

}


EXPORT_C DLL_EXPORT bool SaveSysRotAxleToTool(int AxisIndex, int ToolIndex,double RotX,double RotY)
{
	bool bRet = false;
	CConfigManager *pInstance = NULL;
	pInstance = CConfigManager::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->SaveSysRotAxleToTool( AxisIndex,ToolIndex, RotX,RotY);
	}
	return bRet;


}

void ConfigSaveSysRotAxleAdjust(int AxisIndex, int ToolIndex,double RotXAd,double RotYAd)
{
	
	CConfigManager *pInstance = NULL;
	pInstance = CConfigManager::GetInstance();
	if (NULL != pInstance)
	{
		 pInstance->SaveSysRotAxleAdjust( AxisIndex,ToolIndex, RotXAd,RotYAd);
	}

}

void ConfigReadSysRotAxleAdjust(int AxisIndex, int ToolIndex,double& RotXAd,double& RotYAd)
{

	CConfigManager *pInstance = NULL;
	pInstance = CConfigManager::GetInstance();
	if (NULL != pInstance)
	{
		pInstance->ReadSysRotAxleAdjust( AxisIndex,ToolIndex, RotXAd,RotYAd);
	}

}




 void SaveProctocolData(const PROCTOCOLDATA& ProctoData)
{
	CConfigManager *pInstance = NULL;
	pInstance = CConfigManager::GetInstance();
	if (NULL != pInstance)
	{
		pInstance->SaveProctocolData(ProctoData);

	}
}

 void ConfigSaveProductMap(void* pMapData)
 {
	 CConfigManager *pInstance = NULL;
	 pInstance = CConfigManager::GetInstance();
	 if (NULL != pInstance)
	 {
		 pInstance->SaveProductMap(pMapData);

	 }

 }
 void ConfigReadProductMap(void* pMapData)
 {
	 CConfigManager *pInstance = NULL;
	 pInstance = CConfigManager::GetInstance();
	 if (NULL != pInstance)
	 {
		 pInstance->ReadProductMap(pMapData);

	 }

 }

 void ConfigSaveDefaultProduct(string productName)
 {

	 CConfigManager *pInstance = NULL;
	 pInstance = CConfigManager::GetInstance();
	 if (NULL != pInstance)
	 {
		 pInstance->SaveDefaultProduct(productName);

	 }


 }

void ConfigReadDefaultProduct(string& productName)
{
	CConfigManager *pInstance = NULL;
	pInstance = CConfigManager::GetInstance();
	if (NULL != pInstance)
	{
		pInstance->ReadDefaultProduct(productName);

	}
}



//��ʱ����
//void ConfigSaveProductId(int ProductId,string ProductName)
//{
//	CConfigManager *pInstance = NULL;
//	pInstance = CConfigManager::GetInstance();
//	if (NULL != pInstance)
//	{
//		pInstance->SaveProductId(ProductId,ProductName);
//
//	}
//
//
//}
//
////��ʱ����
//void ConfigReadProductId(void* lpProductMap)
//{
//	CConfigManager *pInstance = NULL;
//	pInstance = CConfigManager::GetInstance();
//	if (NULL != pInstance)
//	{
//		pInstance->ReadProductId(lpProductMap);
//
//	}
//
//
//}






/**********************************************************************
����      : InitConfig
����      : ��ʼ������
�������  : N/A
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool GetSceneConfig(const char * pConfigPath, void *pData)
{
    bool bRet = false;
    CConfigManager *pInstance = NULL;
    pInstance = CConfigManager::GetInstance();
    if (NULL != pInstance)
    {
        bRet = pInstance->ReadSceneConfig(pConfigPath, pData);
    }
    return bRet;
}

DLL_EXPORT char* ConfigGetsysPath()
{

	CConfigManager *pInstance = NULL;
	pInstance = CConfigManager::GetInstance();
	if (NULL != pInstance )
	{
		return pInstance->m_SysconfigPath;
	}
	return NULL;

}

DLL_EXPORT char* ConfigGetSceneName()
{
	CConfigManager *pInstance = NULL;
	pInstance = CConfigManager::GetInstance();
	if (NULL != pInstance )
	{
		return pInstance->m_SceneconfigName;
	}
	return NULL;
}


DLL_EXPORT char* ConfigGetScenePath(const char* SceneFloderName)
{

	CConfigManager *pInstance = NULL;
	pInstance = CConfigManager::GetInstance();
	if (NULL != pInstance )
	{
		return pInstance->GetSceneconfigPath(SceneFloderName);
	}
	return NULL;

}








 bool GetSceneProcProcess(const char * pConfigPath, int& ProcProcess)
{
	bool bRet = false;
	CConfigManager *pInstance = NULL;
	pInstance = CConfigManager::GetInstance();
	if (NULL != pInstance)
	{
		bRet = pInstance->ReadSceneProcProcess(pConfigPath, ProcProcess);
	}
	return bRet;

}

/**********************************************************************
����      : SetScenePara
����      : ���³���
�������  : N/A
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool SetSceneConfig(const char * pConfigPath, void *pData)
{
    bool bRet = false;
    CConfigManager *pInstance = NULL;
    pInstance = CConfigManager::GetInstance();
    if (NULL != pInstance)
    {
        bRet = pInstance->SaveSceneConfig(pConfigPath, pData);
    }
    return bRet;
}