#ifndef _I_FLAT_FISH_MANAGER_H_
#define _I_FLAT_FISH_MANAGER_H_

#ifndef EXPORT_C
#define EXPORT_C extern "C"
#endif

#ifndef DLL_EXPORT
#define DLL_EXPORT _declspec(dllexport)
#endif



#include <list>
#include <string>
#include "DataType.h"

typedef BOOL (pSceneProcess)(int,CHECK_RESULT&);/*WINAPI*/

typedef enum _IMAGE_ZOOM_TYPE
{
    MOVE_TPYE = 0,          //�ƶ�
    ENLARGE_TYPE,              //�Ŵ�
    SHRINK_TYPE,               //��С
    RESTORE_TYPE,            //��ԭ
}IMAGE_ZOOM_TYPE;


/**********************************************************************
����      : InitManager
����      : ��ʼ������ƹ�,�����
�������  : N/A
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool /*WINAPI*/ InitManager(); //��WINAPI,��û��def�ļ���������Ƹı�


/**********************************************************************
����      : UnInitManager
����      : ȥ��ʼ��
�������  : N/A
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool UnInitManager();

/**********************************************************************
����      : GetSceneProcess
����      : ��ȡ������������ַ
�������  : pSceneProcess  ������ַ
�������  : 
����ֵ    : 
��ע      : SDK��ں���
**********************************************************************/
EXPORT_C DLL_EXPORT void GetSceneProcess(pSceneProcess* pProcess);

/**********************************************************************
����      : GetSceneNameInfo
����      : ��ȡ������Ϣ
�������  : N/A
�������  : iSceneNum ��������
            pSceneName ���������б�
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool GetSceneNameInfo(int &iSceneNum, char **pSceneName);

/**********************************************************************
����      : SetSysParam
����      : �����Ӿ�����
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
            pData    Ҫ����ֵ�Ľṹ��ָ��
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool SetSysParam(void *pData);

/**********************************************************************
����      : SaveSysParam
����      : ���²������Ӿ�����
�������  : N/A 
            pData    Ҫ����ֵ�Ľṹ��ָ��
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool SaveSysParam(void *pData = NULL);

/**********************************************************************
����      : GetSysParam
����      : ��ȡϵͳ����
�������  : pDataҪ��ȡ�Ľṹ��ָ�� 
�������  : pDataҪ��ȡ�Ľṹ��ָ��
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool GetSysParam(void *pData);

/**********************************************************************
����      : GetSysParam
����      : ��ȡϵͳ����
�������  : N/A
�������  : 
����ֵ    : Ҫ��ȡ�Ľṹ��ָ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT void* GetSysParamPoint();

/**********************************************************************
����      : SetSceneParam
����      : �����Ӿ�����
�������  : pData    Ҫ���ó����Ľṹ��ָ��
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool SetSceneParam(void *pData);

/**********************************************************************
����      : SaveSceneParam
����      : ���沢�����Ӿ�����
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
            pData    Ҫ����ֵ�Ľṹ��ָ��
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool SaveSceneParam(int iSceneID, void *pData = NULL);

/**********************************************************************
����      : GetSceneParam
����      : �����Ӿ�����
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
�������  : pData    Ҫ��ȡ�Ľṹ��ָ��
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool GetSceneParam(int iSceneID, void *pData);



EXPORT_C DLL_EXPORT void* GetSceneParamHandle(int iSceneID);


/**********************************************************************
����      : GetSceneNum
����      : ��ȡ��������
�������  : 
�������  : 
����ֵ    : �����ļ��趨�ĳ������������ܻ��пյĳ���
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT int GetSceneNum();

EXPORT_C DLL_EXPORT  char* GetSceneConfigName();

EXPORT_C DLL_EXPORT const char* GetSceneConfigPath(int SceneID);

EXPORT_C DLL_EXPORT const char* GetScenePath(int SceneID);


EXPORT_C DLL_EXPORT char* GetSysConfigPath();

EXPORT_C DLL_EXPORT int GetSceneProcType(int SceneIndex);

EXPORT_C DLL_EXPORT int GetSceneId(int SceneIndex);

/////////////////////////////////////////////////////////////////////////////////////////
//�������
// 
/**********************************************************************
����      : SceneAcqImage
����      : ��ȡͼƬ����
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
            index ��ǰ�����еĵڼ���ͼ�񣨴��ڶ���ͼ������ܸ���index���ֲ�ͬ��Դ���ͼ������
			...���ղ���

�������  : N/A
����ֵ    : 
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT int SceneGrabImage(int iSceneID,int ImageIndex,int CamIndex,int shutterTime,int Gain,int Interval,int LightIndex,int LightValue);

EXPORT_C DLL_EXPORT void SetCamParam(int CamIndex,int shutterTime,int Gain);

EXPORT_C DLL_EXPORT int SceneGrabFinished(int iSceneID, CHECK_RESULT *res,int iErrorCode);


 
/**********************************************************************
����      : InitGrabSceneimage
����      : ��ʼ����ȡͼƬ����
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
            index ��ǰ�����еĵڼ���ͼ�񣨴��ڶ���ͼ������ܸ���index���ֲ�ͬ��Դ���ͼ������
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool InitGrabSceneimage(int iSceneID, int index = 0);

/**********************************************************************
����      : InitGrabSceneimage
����      : ��ʼ����ȡͼƬ����
�������  : strCameraGuid ���Uid�� 
           
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      :  ���ܻ�δʵ��
**********************************************************************/
EXPORT_C DLL_EXPORT bool InitGrabSceneimage1(int CamIndex ,int iSceneID,int ImageIndex = 0);

/**********************************************************************
����      : SetGain
����      : ���������ͼ����
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
            iValue ���õ�ֵ
            index ��ǰ�����еĵڼ���ͼ�񣨴��ڶ���ͼ������ܸ���index���ֲ�ͬ��Դ���ͼ������
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool SetGain(int iSceneID, int iValue, int index = 0);

/**********************************************************************
����      : GetGain
����      : ��ȡ�����ͼ����
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
            index ��ǰ�����еĵڼ���ͼ�񣨴��ڶ���ͼ������ܸ���index���ֲ�ͬ��Դ���ͼ������
�������  : N/A
����ֵ    : ��ȡ������ֵ
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT int GetGain(int iSceneID, int index = 0);

//ö�����
/**********************************************************************
����      : GetCameraIDList
����      : ��ȡ���GUID�б�
�������  : guidlist�� GUID�б�
�������  : N/A
����ֵ    : ���ؽ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT int GetCameraIDList(std::list<std::string> &guidlist,int dllnum,string dlllist[]);

/**********************************************************************
����      : TriggerVisionProcess
����      : �����Ӿ�������
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool TriggerVisionProcess(int iSceneID = 0, int iPercent = 100);

/**********************************************************************
����      : BindWindow
����      : ��halcon������UI����
�������  : iWinID ���洰��ID
            x,y,width,hight ���洰��λ�ô�С
�������  : 
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool BindWindow(long iWinID, int x, int y, int width, int hight, bool bDebug = false);

/**********************************************************************
����      : SetDebugWindowID
����      : ��halcon������UI����
�������  : iWinID ���洰��ID
            x,y,width,hight ���洰��λ�ô�С
�������  : 
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool SetDebugWindowID(int iSceneID, long iWinID);
/**********************************************************************
����      : ClearWindow
����      : �����Ӧ��������Ϣ
�������  : iWinID ���洰��ID
�������  : 
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool ClearWindow(long iWinID);

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
EXPORT_C DLL_EXPORT  bool ReadImage(const char *pFileName, int iSceneID, int index = 0);

/**********************************************************************
����      : ����ָ��ͼƬ
�������  : iSceneID  ��ǰ����ID
index ��ǰ�����еĵڼ���ͼ�񣨴��ڶ���ͼ������ܸ���index���ֲ�ͬ��Դ���ͼ������
SNstr SN��Ϣ
�������  : 
����ֵ    : �ɹ���ʧ��
��ע      :  path ΪNULLʱ�� ���浽E:/
**********************************************************************/
EXPORT_C DLL_EXPORT bool SaveImage(int iSceneID, int index = 0 , string SNstr = "");

/**********************************************************************
����      : SaveImageDump
����      : ���洰��ͼ��
�������  : winID ��ǰ����ID
            iSceneID  ��ǰ����ID
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : path ΪNULLʱ�� ���浽E:/
**********************************************************************/
EXPORT_C DLL_EXPORT bool SaveImageDump(const long winID, int iSceneID,char* outpath);

/**********************************************************************
����      : SendImageDump
����      : ���ʹ���ͼ��
�������  : winID ��ǰ����ID
            iSceneID  ��ǰ����ID
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool SendImageDump(void *buff, int size,int w,int h,int iSceneID);

/**********************************************************************
����      : ��ȡ����ͼ��
�������  : winID ��ǰ����ID
            iSceneID  ��ǰ����ID
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT void* GetImageDump(int winID, int sence,double &width,double &heigh);

/**********************************************************************
����      : getImagePath
����      : ��ȡͼ��·��
�������  : iSceneID ��ǰ����ID
            index  ͼƬ����
			type  ͼƬ����
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT char* getImagePath(int iSceneID, int index,unsigned int type);

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
EXPORT_C DLL_EXPORT bool DispObj(int iWinID, int iSceneID, bool bIsShowTmp = false);

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
EXPORT_C DLL_EXPORT bool DispImage(long iWinID, int iSceneID, int index = 0);

EXPORT_C DLL_EXPORT bool FindHalconWinID(int iWinID, long& HalconWinID);

EXPORT_C DLL_EXPORT bool ShowVisionString(long hwinID,char* StrContent,int topRow = -1,int topCol = -1);
EXPORT_C DLL_EXPORT bool SetVisionColorHeight(long hwinID,char* strColor,int FontHeight=0);

/**********************************************************************
����      : DispROI
����      : ��ʾROI
�������  : iWinID  �������
index ��ǰ�����еĵڼ���ROI
iType, 0,Ϊ����ͼ������, 1Ϊģ������2Ϊ��ֱ�ߣ� 3Ϊ��Բ�� 
�������  : 
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT  bool DispROI(int iWinID, int iSceneID, int index, PROCESS_TYPE iType = MODEL_PRO);

/**********************************************************************
����      : DrawROI
����      : ��Ϊ����ROIģʽ
�������  : iWinID  �������
index ��ǰ�����еĵڼ���ROI
iType, 0,Ϊ����ͼ������, 1Ϊģ������2Ϊ��ֱ�ߣ� 3Ϊ��Բ�� 
�������  : 
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT  bool DrawROI(int iWinID, int iSceneID, int index, PROCESS_TYPE iType = MODEL_PRO);


/**********************************************************************
����      : DrawROI
����      : ֱ�ӻ���ROI
�������  : iSceneID  �������
            iType, ��ֱ�ߣ� ��Բ�� 
�������  : 
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool OnDrawROI(int iWinID, int iSceneID,PROCESS_TYPE iType,RoiCombType CombType,vector<int>* const pRoiDdList);
EXPORT_C DLL_EXPORT void  GenShape(int SceneID,const ST_ROI_INFO& roi,ShapeType shapeflag,int ShapeId,int CamIndex);

EXPORT_C DLL_EXPORT int GetCurShapeId(int iSceneID);


EXPORT_C DLL_EXPORT bool   GetRoiParam(int sceneID,int ShapeId,ST_ROI_INFO* pRoi);


EXPORT_C DLL_EXPORT void OnShowAllRoi(int iWinID, int iSceneID,const vector<int>* const pRoiDdList);

/**********************************************************************
����      : CreateVisionModel
����      : ����ģ��
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
                index ������ͼ�����
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool CreateVisionModel(int iWinID,int iSceneID, int index = 0);


EXPORT_C DLL_EXPORT bool CreateWay2VisionModel(int iWinID,int iSceneID,int RoiID,char* modePath, int Nevels,double AngleStart,double AngleExtent,
	                 int Contrast,int index = 0);

//
///**********************************************************************
//����      : UpdateVisionModel
//����      : ����ģ��
//�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
//            index ������ͼ�����
//�������  : N/A
//����ֵ    : �ɹ���ʧ��
//��ע      : 
//**********************************************************************/
//EXPORT_C DLL_EXPORT bool UpdateVisionModel(int iSceneID, int index = 0, int iType = 0);

/**********************************************************************
����      : FindVisionModel
����      : ����ģ��
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
            index ������ͼ�����
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool FindVisionModel(int iSceneID, int index = 0, bool bIsB = false);


EXPORT_C DLL_EXPORT bool FindWay2VisionModel(int iSceneID, int WinID, int index,char* szModePath,int PyramidLevel,int MatchNum, double& Score,
	            int ProcIndex,bool IsBulid,double& row,double& col,double& phi);

//����һ������ʵʱʵ�ֻ����˶�׼��ʮ��
EXPORT_C DLL_EXPORT bool FindModelRealTime(int iSceneID, int WinID, int index,char* szModePath,int PyramidLevel,int MatchNum, double& Score,
	int ProcIndex,bool IsBulid,double& row,double& col,double& phi);

EXPORT_C DLL_EXPORT void MoveAndRotInCamForOriMatch(int iSceneID,double PixSize,bool AxeMoveRotDirect,double& MotorRelX,double& MotorRelY,
	     int LastSceneId,int NextSceneId,double SceneRotPhi,double PicCentX =0.0,double PicCentY = 0.0,double* pCamParam = NULL,int ParamNum = 0,double* pCamPose = NULL,double CalThick=0.0,
		 double RotCenX= 0.0,double RotCenY = 0.0 ,double MotorRotPhi = 0.0,double CalRotPhi = 0.0,
		 int CamIndex = -1,int AxeIndex = -1,int ToolIndex = -1);

EXPORT_C DLL_EXPORT bool AffinePointByCamInMecRot(int CamIndex,int AxisIndex,int ToolIndex,double& PointX,double& PointY);

EXPORT_C DLL_EXPORT void AffinePointToWorld(int CamIndex,int AxisIndex,int ToolIndex,double PixSize,double OriCamX,double OriCamY,
	double OriCamPhi,double CurCamX,double CurCamY,double CurCamPhi,double& MoveX,double& MoveY,int ResType,
	bool MoveRotDirect,double CurMatchPhi,double AxeRotX,double AxeRotY);

EXPORT_C DLL_EXPORT void AffToRobotWorld(int SceneID,int CamIndex,int AxisIndex,int ToolIndex,double PixSize,double ModRotPhi,
	      double& ToolWorldX,double& ToolWorldY,bool WithCamParam,double CalThickness,double* pCamParam,int ParamNum,double* CamPos);


/**********************************************************************
����      : VisionProcess
����      : ������
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool VisionProcess(int iSceneID, int index,CHECK_RESULT &result);

/**********************************************************************
����      : VisionCalibProcess
����      : �궨����
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool VisionCalibProcess(CALIB_DATA *pData);
/**********************************************************************
����      : VisionDebugThreshold
����      : ������ֵ�ӿ�
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
index�� ���ݸ�index��ȡ����
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
 EXPORT_C DLL_EXPORT bool VisionDebugThreshold(int iWinID, int iSceneID, int index);

/**********************************************************************
����      : VisionFindLine
����      : ���߽ӿ�
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
            index�� ���ݸ�index��ȡ��Բ����
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool VisionFindLine(int iSceneID, int index, bool bIsFollow = false);

/**********************************************************************
����      : VisionFindCircle
����      : ��Բ�ӿ�
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
            index�� ���ݸ�index��ȡ��Բ����
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool VisionFindCircle(int iSceneID, int index, bool bIsFollow = false);


EXPORT_C DLL_EXPORT bool VisionMeasureCircle(long WinId,int iSceneID, int Imageindex,int MatchIndex, double& Row,double& Col,double& Rii);


/**********************************************************************
����      : OpenContinousGrab
����      : ����ʵʱ��ʾ
�������  : iWindowID ���ھ��
            iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
            index ץͼ���
            ֪ͨ������ʾ�������ϢID
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : ��ʵʱ�� �ýӿ�ע��ص���ʵʱ�ɼ��� �ص���֪ͨ������ʾͼ��
            ��ϢID   
**********************************************************************/
EXPORT_C DLL_EXPORT bool OpenContinousGrab(int iWindowID, int iSceneID, int index, unsigned int MsgID,int CameraIndex = -1);

/**********************************************************************
����      : ColseContinousGrab
����      : �ر�ʵʱ��ʾ
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool ColseContinousGrab(int iSceneID, int index,int CameraIndex = -1);

//comm�ڲ����ӿ�
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
EXPORT_C DLL_EXPORT bool CtrlComm(LightInfo *pInfo, bool bOpen = true, LightTypeInfo *pTypeInfo = NULL);

/**********************************************************************
����      : RestoreImagePart
����      : �ָ�ͼ����ʾ����
�������  : WinID   ����ID
            iScene  ����ID
            
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT void RestoreImagePart(int WinID, int iScene, int width, int height, int index = 0);
//ͼ��������ק
//1. ����ID, 
//2. ��ק���ͣ�a, ��;b, ��;c, ��;d, ��;e, �Ŵ�;f, ��С, g, ��ԭ��
//3. ���ǰ��λ��(���ڵ�ǰ�����ϵ�λ��)
//4. ����ֵ
EXPORT_C DLL_EXPORT void UpdateImagePart(int WinID, int iScene, int iType, double dXPer = 1.0, double dYPer = 1.0);
/**********************************************************************
����      : ResetFinished
����      : ����ϵͳ������Ϣ
�������  : iErrorCode ������
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : N/A
**********************************************************************/
EXPORT_C DLL_EXPORT int ResetFinishedSendMsg(int iErrorCode = 0);
/**********************************************************************
����      : OpenCheckStateThread
����      : �򿪼��ϵͳ״̬�߳�
�������  :   

�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT bool OpenCheckStateThread();
/**********************************************************************
����      : JudgeLightConnect
����      : �жϹ�Դ�������Ƿ����ӳɹ�
�������  : ��������
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT int JudgeLightConnectStatus(string comName);

//��ȡ��Ӧ���Ͳ�����pkg->pBody�е�λ��
//input ���ͣ� ������ pkg, 
//�����ҵ�λ��
EXPORT_C DLL_EXPORT int IFindParamData(SCENE_PKG *pkg, int iType, int index);

EXPORT_C DLL_EXPORT int FindSpecTypePos(const VIRSION_DATA_PKG *pkg, unsigned int *pType, int iType = 0);

EXPORT_C DLL_EXPORT int InsertParamItem(int SceneId, VIRSION_DATA_PKG *pkg, unsigned int *pType, int iSize, int insertPos);
EXPORT_C DLL_EXPORT int UpdateParamItem(int SceneId, const VIRSION_DATA_PKG *pkg, unsigned int *pType, INTERFACE_SET_DATA *dataTmp, int iSize, int insertPos, bool insert = false);
EXPORT_C DLL_EXPORT int SwapParamItem(int SceneId, int iPosF, int iPosN);
EXPORT_C DLL_EXPORT int DeleteParamItem(int SceneId, int insertPos);

//������Դ�� comNameΪ��������,  indexΪͨ�������� ��0��ʼ�� valueΪ���õ�ֵ, typeΪ�������ͣ��򿪡��رա�����
EXPORT_C DLL_EXPORT int OpratorLight(string comName, int index, int value, int type);

/**********************************************************************
����      : ������״
����      : ��ͼ�񴰿��ϻ���ͼ��
�������  : 
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/

//��������ͨѶ����Э��ı���
EXPORT_C DLL_EXPORT void  SetWebProctocol(const PROCTOCOLDATA& ProctoData);

EXPORT_C DLL_EXPORT void  GetWebProctocol(PROCTOCOLDATA& ProctoData);


////��Ʒ���趨���������ڻ���Ʒʱһ���л�
EXPORT_C DLL_EXPORT void SetProductMap(void* pMapData);

EXPORT_C DLL_EXPORT void GetProductMap(void* pMapData);

EXPORT_C DLL_EXPORT void SetSysCurProductName(string ProductName);

EXPORT_C DLL_EXPORT void SaveSysRotAxleAdjust(int AxisIndex, int ToolIndex,double RotXAd,double RotYAd);
EXPORT_C DLL_EXPORT void ReadSysRotAxleAdjust(int AxisIndex, int ToolIndex,double RotXAd,double RotYAd);

//halcon��λ,����ֱ�Ӷ�λ���㷨dll,���辭��manager������Ҫ������
EXPORT_C DLL_EXPORT bool HalconProcedureLocate(long iWinID,int iSceneID,int Imageindex,string ProcedurePath,
	double (*inParam)[HalProcInParamNum],double& PosX,double& PosY,double& PosPhi);

//halcon����
EXPORT_C DLL_EXPORT bool HalconProcedureProcess(long iWinID,int iSceneID,int Imageindex,string ProcedurePath,vector<double> &inparam,vector<double> &outparam,int display,int &error,string &hmessage);

//halcon��ǿ
EXPORT_C DLL_EXPORT bool IImageEnhancement(long iWinID,int iSceneID,int Imageindex,double& multinum,double& addnum,int display,int &error,string &hmessage);


EXPORT_C DLL_EXPORT CHECK_RESULT* GetSceneProcResult(string &scneName);						//��ȡָ���������ֵĽ������ָ��ʧȥ����������,���ֲ��ܷ��ؽṹ��,�����Ľ�		new
EXPORT_C DLL_EXPORT CHECK_RESULT* GetSceneProcResultById(int  iSceneID);
EXPORT_C DLL_EXPORT int SetSceneProcResult(string &scneName,CHECK_RESULT& res);				//����ָ���������ֵĽ��		new
EXPORT_C DLL_EXPORT int ClearProcResultByID(int iSceneID);									//���ָ������id��Ľ������
#endif 
