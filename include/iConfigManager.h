#ifndef _I_CONFIG_MANAGER_H_
#define _I_CONFIG_MANAGER_H_

#include "DataType.h"

#ifndef EXPORT_C
#define EXPORT_C extern "C"
#endif
#ifndef DLL_EXPORT
#define DLL_EXPORT _declspec(dllexport)
#endif



/**********************************************************************
����      : InitConfig
����      : ��ʼ������
�������  : N/A
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool InitConfig(void *pData);

/**********************************************************************
����      : ReleaseConfigInstance
����      : ȥ��ʼ������
�������  : N/A
�������  : N/A
����ֵ    : N/A
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT void ReleaseConfigInstance();

/**********************************************************************
����      : SaveSysConfig
����      : ����ϵͳ����
�������  : N/A
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool SaveSysConfig(void *pData);


/**********************************************************************
����      : SaveSysCamCalibrate
����      : ��������ı궨����(��λ�궨��)
�������  : CamerIndex  �������
            DataCount   �궨���ݵ�����
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool SaveSysCamCalibrate(int CamerIndex,int ToolIndex,int DataCount,double MotorX[],double MotorY[],double PixRow[],double PixCol[]);

EXPORT_C DLL_EXPORT bool SaveMecAndCamRot(int CamIndex,int AxeIndex,int ToolIndex,double (*pCamRot),double (*pMecAndCam),
	                       double (*pImageRela),double (*pImagAbs),int DataNum);

EXPORT_C DLL_EXPORT bool SavePicCenter(int CamIndex,int AxeIndex,int ToolIndex,double X,double Y);

EXPORT_C DLL_EXPORT bool ReadMecAndCamRot(int CamIndex,int AxeIndex,int ToolIndex,double (*pMecAndCam),int& DataNum);

EXPORT_C DLL_EXPORT bool SaveSysCamPixSize(int CamIndex,int AxeIndex,  int ToolIndex,double PixSize);

EXPORT_C DLL_EXPORT bool SaveSysRotAxleToCam(int CamIndex,int AxisIndex, int ToolIndex,double RotX,double RotY);
EXPORT_C DLL_EXPORT bool SaveSysRotAxleToTool(int AxisIndex, int ToolIndex,double RotX,double RotY);

EXPORT_C DLL_EXPORT void ConfigSaveSysRotAxleAdjust(int AxisIndex, int ToolIndex,double RotXAd,double RotYAd);
EXPORT_C DLL_EXPORT void ConfigReadSysRotAxleAdjust(int AxisIndex, int ToolIndex,double& RotXAd,double& RotYAd);

EXPORT_C DLL_EXPORT void SaveProctocolData(const PROCTOCOLDATA& ProctoData);

EXPORT_C DLL_EXPORT void ConfigSaveProductMap(void* pMapData);

EXPORT_C DLL_EXPORT void ConfigReadProductMap(void* pMapData);

EXPORT_C DLL_EXPORT void ConfigSaveDefaultProduct(string productName);

EXPORT_C DLL_EXPORT void ConfigReadDefaultProduct(string& productName);


//EXPORT_C DLL_EXPORT void ConfigSaveProductId(int ProductId,string ProductName);

//EXPORT_C DLL_EXPORT void ConfigReadProductId(void* lpProductMap);


/**********************************************************************
����      : GetSceneConfig
����      : ��ʼ������
�������  : N/A
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool GetSceneConfig(const char * pConfigPath, void *pData);


EXPORT_C DLL_EXPORT char* ConfigGetsysPath();

EXPORT_C DLL_EXPORT char* ConfigGetSceneName();

EXPORT_C DLL_EXPORT char* ConfigGetScenePath(const char* SceneFloderName);

/**********************************************************************
����      : GetSceneProcProcess
����      : �������ļ��л�ȡ������������
�������  : N/A
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool GetSceneProcProcess(const char * pConfigPath, int& ProcProcess);

/**********************************************************************
����      : SetScenePara
����      : ���³���
�������  : N/A
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool SetSceneConfig(const char * pConfigPath, void *pData);





#endif 
