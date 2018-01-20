#pragma once
#include "DataType.h"
#include <map>
using namespace std;

//����ֱ�ӵ���CVisionServer��,������CVisionProcess��,��������Ӧ�÷�װһ��c�ӿڵ���ȥ
class CVisionProcess;
class _declspec(dllexport) CVisionServer
{
public:
    CVisionServer(void);
    ~CVisionServer(void);

public:
    static CVisionProcess *GetInstance(int iSceneID);					//��ȡ��Ӧ�����ŵĴ������
    static void ReleaseInstance();										//�ͷ�map�е����д������
    virtual bool InitVision(void);										//�����궨����ͱ궨���ݻ������ļ���

    /**********************************************************************
    ����      : OpenWindow
    ����      : �󶨴���
    �������  : winID ��Ҫ��ͼ���ID
                x,y,width,hight Ҫ��halcon���ڵ���ʼλ�ã���͸�
    �������  : halconWinID halcon����ID
    ����ֵ    : true�ɹ� falseʧ��
    ��ע      : 
    **********************************************************************/
    virtual bool OpenWindow(long winID, int x, int y, int width, int hight, long &halconWinID);

	/**********************************************************************
    ����      : setpart
    ����      : ����halcon������mfc���ڵ���ʾ��Χ
    �������  : winID ����ID
               topx,topy,�õ�������������Ͻǵ����꣬lowx��lowy�����½�����
    �������  : 
    ����ֵ    : true�ɹ� falseʧ��
    ��ע      : 
    **********************************************************************/
    virtual bool setpart(long winID,int topx, int topy,int lowx,int lowy);

	/**********************************************************************
    ����      : HalconProcedureLocate
    ����      : halcon��λ����
    �������  : winID ����ID��iSceneID ������,halcon�����ַ,���������ַ
    �������  : ����ֵ,x,y,th
    ����ֵ    : true�ɹ� falseʧ��
    ��ע      : 
    **********************************************************************/
	virtual bool HalconProcedureLocate(long HwinID,int iSceneID, int Imageindex,string ProcedurePath,
		double (*inParam)[HalProcInParamNum], double& PosX,double& PosY,double& PosPhi);

	virtual bool HalconProcedureProcess(long HwinID,int iSceneID,int Imageindex,string ProcedurePath,vector<double> &inparam,vector<double> &outparam,int display,int &error,string &hmessage);

	virtual bool ImageEnhancement(long iWinID,int iSceneID,int Imageindex,double& multinum,double& addnum,int display,int &error,string &hmessage);
	/**********************************************************************
    ����      : getpart
    ����      : ��ȡmfc������halcon���ڵ���ʾ��Χ
    �������  : winID ����ID				
    �������  : topx,topy,�õ�������������Ͻǵ����꣬lowx��lowy�����½�����
    ����ֵ    : true�ɹ� falseʧ��
    ��ע      : 
    **********************************************************************/
    virtual bool getpart(long winID,int &topx, int &topy,int &lowx,int &lowy);

	/**********************************************************************
    ����      : getimagesize
    ����      : ��ȡͼ���С
    �������  : iSceneID ������
    �������  : 
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
    virtual bool getimagesize(int sceneID,int &width, int &height);


    /**********************************************************************
    ����      : OpenWindow
    ����      : �رմ���
    �������  : halconWinID halcon����ID
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
    virtual bool CloseHalconWindow(long halconWinID);

	//��������߿���ȡָ��ͼ���ڴ��ָ��
	virtual char * GetImageBuf(int iSceneID, int index, int iWidth, int Height);
    /**********************************************************************
    ����      : ReadImage
    ����      : �󶨴���
    �������  : pFileName ͼƬ·��
                sceneID   ����ID
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
    virtual bool ReadImage(const char *pFileName, int sceneID, int index = 0, bool bIsNew = false);

    /**********************************************************************
    ����      : DispImage
    ����      : �󶨴���
    �������  : iWinIndex ��ʾ��Ŀ�괰��
                sceneID   ����ID
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
    virtual bool DispImage(int iWinIndex, int sceneID, int index = 0);

    /**********************************************************************
    ����      : DispObj
    ����      : �󶨴���
    �������  : iWinIndex ��ʾ��Ŀ�괰��
                sceneID   ����ID
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
    virtual bool DispObj(int iWinIndex, int sceneID, bool bIsShowTmp = false);

	bool ShowWindString(long winID,char* StrContent,int topRow = -1,int topCol = -1);

	bool SetDispColorHeight(long winID,char* strColor,int FontHeight=0);
     
    /**********************************************************************
    ����      : VisionProcess
    ����      : ��ʼ������
    �������  : N/A
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
    virtual bool VisionProcess(void);

  /**********************************************************************
    ����      : CalCamMetrix
    ����      : ��������ŵ�궨���ͬ��ת�����󣬼�����ڣ�����룩��е����ϵ�е�λ��
    �������  : N/A
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
	virtual void CalCamMetrix(int CamerIndex,int DataCount,double MotorX[],double MotorY[],double CamRow[],double CamCol[],
		double PixSize,double(*pCamRot),double (*pMecAndCam),double(*pImageRela),double (*pImageAbs),double (*pCamParam),int ParamNum,double (*pCamPose),
		int ToolIndex=0,int AxeIndex=0,double CalFirstX=0.0,double CalFirstY=0.0,double CalFirstAngle=0.0,double ToolFirstX=0.0,
		double ToolFirstY=0.0,double CamRotAngle=0.0,double RotToCamX=0.0,double RotToCamY=0.0,double CalThickNess=0.0);


	virtual void AffineCoorPoint(double CoordX,double CoordY,double CoordAng,double& PointX,double& PointY);

	/**********************************************************************
    ����      : Imagebuffer
    ����      : ͼ�񻺴���
    �������  : cameraindex ���������
				sceneID   ����ID
				pbuffer ָ�򻺴�ָ��
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
    virtual  bool Imagebuffer(int cameraindex, int sceneID, long *pbuffer);
	/**********************************************************************
    ����      : drawRoi
    ����      : ��ROI
    �������  : halconID ���������
				roi   ���봫��roi����
				bflag Ĭ��Ϊ�����Σ�����ΪԲ��
				bcolor Ĭ��roi����֮�����ɫΪ��ɫ��false���Զ�����ɫ
    �������  :outroi ,���roi����
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
    virtual  bool  drawRoi(int sceneID, long  halconID , const  ST_ROI_INFO  &roi, ST_ROI_INFO &outroi ,bool bflag = true,bool bcolor = true);


    /**********************************************************************
    ����      : OndrawRoi
    ����      : ��ROI
    �������  : halconID ���������
				roi   ���봫��roi����
				bflag Ĭ��Ϊ�����Σ�����ΪԲ��
				bcolor Ĭ��roi����֮�����ɫΪ��ɫ��false���Զ�����ɫ
    �������  :outroi ,���roi����
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
    virtual  bool  OndrawRoi(int sceneID, long  halconID , const  ST_ROI_INFO  &roi, RoiCombType CombType ,PROCESS_TYPE bflag ,vector<int>* pRoiIDList,bool bcolor = true);


	void GenShape(int SceneID,const ST_ROI_INFO& roi,ShapeType shapeflag,int ShapeId,int Width,int Height);

	virtual int GetCurShapeId(int sceneID);

		/**********************************************************************
    ����      : 
    ����      : ��ȡ����ͼԪ�Ĳ���
    �������  : halconID ��ʾ����ID
    �������  :
    ����ֵ    : ��
    ��ע      : 
    **********************************************************************/
	 virtual  bool  GetRoiParam(int sceneID,int ShapeId,ST_ROI_INFO* pRoi);

	/**********************************************************************
    ����      : OnShowAllRoi
    ����      : ��ʾ����ROI
    �������  : halconID ��ʾ����ID
    �������  :
    ����ֵ    : ��
    ��ע      : 
    **********************************************************************/
	virtual  void OnShowAllRoi(int sceneID,long  halconID,const vector<int>* const pRoiDdList);

	/**********************************************************************
    ����      : DispRoi
    ����      : ��ROI
    �������  : halconID ���������
				roi   ���봫��roi����
				bflag Ĭ��Ϊ�����Σ�����ΪԲ��
				bcolor Ĭ��roi����֮�����ɫΪ��ɫ��false���Զ�����ɫ
    �������  :outroi ,���roi����
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
    virtual  bool  dispRoi(int sceneID, long  halconID , ST_ROI_INFO  roi,bool bflag = true,bool bcolor = true);

	/**********************************************************************
    ����      : saveWinImage
    ����      : ���洰��ͼ��
    �������  : winID ��ǰ����ID
				path ����·��
                SNstr  SN��Ϣ
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
	virtual bool saveWinImage(const long winID,char *path,char *outpath,string SNstr);

	/**********************************************************************
    ����      : getWinImage
    ����      : ���洰��ͼ��
    �������  : winID ��ǰ����ID
			    pBuffer ͼ�񻺴�
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
	virtual void* getWinImage(const long winID,double &width,double &heigh);

	/**********************************************************************
    ����      : saveImage
    ����      : ͼ�񻺴���
    �������  : sceneID   ����ID
				index ͼƬ����
				path ͼ�񱣴�·��	
                SNstr  SN��Ϣ
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
    virtual bool  saveImage(int  sceneID,int index,char *path,string SNstr);

	/**********************************************************************
    ����      : saveImagePath
    ����      : ������·��
    �������  : sceneID   ����ID
				index ͼƬ����
				path ͼ�񱣴�·��	
                SNstr  SN��Ϣ
				type ʲôͼƬ·�����������ԭʼͼƬ·����Ĭ�Ͻ��
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
	virtual bool saveImagePath(int  sceneID,int index,char *path,string SNstr,unsigned int type = 0);

	/**********************************************************************
    ����      : getImagePath
    ����      : ������·��
    �������  : sceneID   ����ID
				index ͼƬ����	
                SNstr  SN��Ϣ
				type ʲôͼƬ·�����������ԭʼͼƬ·����Ĭ�Ͻ��
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
	virtual char* getImagePath(int  sceneID,int index,unsigned int type = 0);
	/**********************************************************************
    ����      : buildModel
    ����      : ����ģ��
    �������  :  winID ����ID   modelID  ģ��ID
				modelroi   ����ģ��ĸ���Ȥ��
				path  ģ�屣��·��
    �������  : N/A
    ����ֵ    :ģ��ID
    ��ע      : 
    **********************************************************************/
    virtual  int  buildModel(int iWinID,int sceneID,int index, SCENE_PKG *pkg,char *sceneName,char *path);


	virtual  bool  buildModel(int iWinID,int iSceneID,int RoiID,char* modePath, int Nevels,double AngleStart,double AngleExtent,
                     int Contrast,int index = 0);
    
	//virtual  bool  MatchModel(double& row,double& col,double& phi);

	/**********************************************************************
    ����      : clearModel
    ����      : ���ģ��
    �������  : sceneID  ����ID   
                iModelID  ģ��ID
                iType     ģ������
    �������  : N/A
    ����ֵ    : ���
    ��ע      : 
    **********************************************************************/
    int clearModel(int sceneID, int iModelID, int iType = 0);
	/**********************************************************************
    ����      : readModel
    ����      : ��ȡģ��
    �������  : path ģ��·��
				sceneID   ����ID
                index    ģ������
                iType    ģ������
    �������  : N/A
    ����ֵ    : ģ��ID
    ��ע      : 
    **********************************************************************/
    virtual  int  readModel(int sceneID,char *sceneName,char *path, int index = 0, int iType = 0);

	/**********************************************************************
    ����      : findModel
    ����      : ����ģ��
    �������  : sceneID   ����ID
                param ģ�����
				data  ģ�嶨λ���
                Score ģ�����
    �������  : N/A
    ����ֵ    : ģ��ID
    ��ע      : 
    **********************************************************************/
    virtual  int  findModel(const int sceneID,int index, SCENE_PKG *pkg,  ST_RECTANGLE_DATA &data, unsigned int bDisplay = 1);


	/**********************************************************************
    ����      : ����ģ��
    �������  : sceneID   ����ID
               	
    �������  : N/A
    ����ֵ    : ģ��ID
    ��ע      : 
    **********************************************************************/
	 virtual  bool FindWay2VisionModel(const int iSceneID,int WinID,int index,char* szModePath,int PyramidLevel,int MatchNum, double& Score,
		                      int ProcIndex,bool IsBulid,double& row,double& col,double& phi);

	 virtual  bool FindModelRealTime(const int iSceneID,int WinID,int index,char* szModePath,int PyramidLevel,int MatchNum, double& Score,
		 int ProcIndex,bool IsBulid,double& row,double& col,double& phi);


	 void MoveAndRotInCamForOriMatch(int iSceneID,double PixSize,bool AxeMoveRotDirect,double& MotorRelX,double& MotorRelY,
		          int LastSceneId,int NextSceneId,double SceneRotPhi,double PicCentX,double PicCentY,double RotCenX,double RotCenY,double MotorRotPhi,
					double CalRotPhi,double (*pCamParam),int ParamNum,double (*pCamPose),double CalThickness,
					 int CamIndex,int AxeIndex,int ToolIndex);
	 	
	 bool AffinePointByCamInMecRot(int CamIndex,int AxisIndex,int ToolIndex,double& PointX,double& PointY);

	 void AffToRobotWorld(int SceneId,int CamIndex,int AxisIndex,int ToolIndex,double PixSize,double ModRotPhi,
		         double& ToolWorldX,double& ToolWorldY,bool WithCamParam,double CalThickness,double* pCamParam,int ParamNum,double* CamPos);

	 void AffinePointToWorld(int CamIndex,int AxisIndex,int ToolIndex,double PixSize,double OriCamX,double OriCamY,
		 double OriCamPhi,double CurCamX,double CurCamY,double CurCamPhi,double& PointX,double& PointY,int ResType,
		 bool MoveRotDirect,double CurMatchPhi,double AxeRotX,double AxeRotY);

	 void SetCamInMecRot(int CamIndex,int AxisIndex,int ToolIndex,double(*pMecAndCam),int MetixCount);
	 void SetCamRot(int CamIndex,int AxisIndex,int ToolIndex,double(*pCamRot),int MetixCount);
	  void SetImageMetrix(int CamIndex,int AxisIndex,int ToolIndex,double(*pImageRela),double(*pImageAbs),int MetixCount);

	/**********************************************************************
    ����      : addAffineData
    ����      : ��������
    �������  : sceneID   ����ID
                aoint ��boint ���������������ͬƽ���ϵĵ�����
				index ������
				CameraID ���ID
    �������  : N/A
    ����ֵ    : ����ID
    ��ע      : 
    **********************************************************************/
	virtual bool addAffineData(const int sceneID,SCENE_PKG *sceneName, AF_POINT apoint, AF_POINT bpoint, int index, bool bIsSave = false);

	/**********************************************************************
    ����      : affine_apply
    ����      : ����Ӧ��
    �������  : path  ����ģ��·��
				point   ���з���ǰ�ĵ�
    �������  : N/A
    ����ֵ    : ���з����ĵ�
    ��ע      : 
    **********************************************************************/
   virtual  int affine_apply(int sceneID, int iCameraID, char *sceneName, AF_POINT &apoint, AF_POINT &stOutPoint);

	/**********************************************************************
    ����      : clearWindow
    ����      : �������
    �������  : winID ����ID			
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
    virtual  bool  clearWindow(const  long  winID );
    
	/**********************************************************************
    ����      : clearWindow
    ����      : ������ֵ�ָ����
    �������  : winID ����ID	
                iMinGray �����ֵ
                iMaxGray �յ���ֵ
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
    bool  DebugThreshold(long  halconID, int sceneID, int iMinGray, int iMaxGray);

	/**********************************************************************
    ����      : findline
    ����      : hdvp�������
    �������  : sceneID ����ID	
				param ���߲���
				result �����
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
	virtual bool  findline(int sceneID,SCENE_PKG *param, int index, ST_LINE &result, bool bFollow = false);

	/**********************************************************************
    ����      : findcircle
    ����      : hdvp�������
    �������  : sceneID ����ID	
				param  ��Բ����
				result �����
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
	virtual bool  findcircle(int sceneID,SCENE_PKG *param, int index, ST_CIRCLE &result, bool bFollow = false);

	virtual bool MeasureCircle(long HwinID,int iSceneID, int Imageindex,int MatchIndex, double& Row,double& Col,double& Rii);



	/**********************************************************************
    ����      : process
    ����      : hdvp�������
    �������  : sceneID ����ID	
				param ��UI��ȡ�����ò���
				result �����
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
	virtual bool  process(int sceneID,SCENE_PKG *param, CHECK_RESULT &result);

    //��ȡ��Ӧ���Ͳ�����pkg->pBody�е�λ��
    //input ���ͣ� ������ pkg, 
    //�����ҵ�λ��
    virtual int FindParamData(SCENE_PKG *pkg, int iType, int index, int iGroup = -1);
    virtual int FindSpecTypePos(const VIRSION_DATA_PKG *pkg, unsigned int *pType, int iType);

	//����Roi

	//virtual void DrawRegionRect(int SenceId);


private:
    static map<int, CVisionProcess *>m_vecVisionInstance;
};

