#ifndef _VISION_PROCESS_H_
#define _VISION_PROCESS_H_
//#include "stdafx.h"
#include "DataType.h"
#include "HDevEngineCpp.h"
using namespace HDevEngineCpp;
#include <vector>
using namespace std;
#include "HalconCpp.h"

//using namespace Halcon;
using namespace HalconCpp;

#define CALIB_DATA_MAX_SIZE  10
#define CAMERA_NUM 8
#define MAX_TOOL_NUMBER 20  //�豸�Ļ�е������

class Shape
{
public:

	HObject m_ShapeObj;
	ShapeType m_ShapeType;
	int m_ShapeId;

	Shape(void)
	{
		m_ShapeObj.Clear();
		m_ShapeType = HoNullShapeType;
		m_ShapeId = -1;
	}
	Shape(int shapeId)
	{
		m_ShapeObj.Clear();
		m_ShapeType = HoNullShapeType;
		m_ShapeId =shapeId;
	}



	~Shape(void)
	{

	}
	void operator=(const Shape& sourse)
	{
		m_ShapeObj = sourse.m_ShapeObj;
		m_ShapeType = sourse.m_ShapeType;
		//m_ShapeId = sourse.m_ShapeId;		
	}
};

class ComposeShape:public Shape
{
public:
	vector<int> m_VecComponentId;
	vector<int> m_VecCombType;
	ComposeShape()
	{
		m_ShapeType = HoComposeType;
	}
	ComposeShape(int shapeId):Shape(shapeId)
	{
		m_ShapeType = HoComposeType;
	}


	void operator=(const ComposeShape& sourse)
	{
		m_VecComponentId = sourse.m_VecComponentId ;
	}
};

class LineShape:public Shape
{
	//HoLineType
public:
	LineShape(void)
	{
		m_ShapeType = HoLineType ;
	}

	LineShape(int shapeId):Shape(shapeId)
	{
		m_ShapeType = HoRecTangleType ;
	}
	HTuple BeginRow;
	HTuple BeginCol;
	HTuple EndRow;
	HTuple EndCol;

};

class ShapeRect:public Shape
{
public:
	ShapeRect(void)
	{
		m_ShapeType = HoRecTangleType ;
	}

	ShapeRect(int shapeId):Shape(shapeId)
	{
		m_ShapeType = HoRecTangleType ;
	}
	HTuple CenRow;
	HTuple CenCol;
	HTuple pHi;
	HTuple Lenth1;
	HTuple Lenth2;
	void operator=(const ShapeRect& sourse)
	{
		CenRow = sourse.CenRow;
		CenCol = sourse.CenCol;
		pHi = sourse.pHi;
		Lenth1 = sourse.Lenth1;
		Lenth2 = sourse.Lenth2;
		m_ShapeObj =  sourse.m_ShapeObj;

	}

};

class ShapeCircle:public Shape
{
public:
	ShapeCircle(void)
	{
		m_ShapeType = HoCircleType ;
	}
	ShapeCircle(int shapeId):Shape(shapeId)
	{
		m_ShapeType = HoCircleType ;
	}
	HTuple CenRow;
	HTuple CenCol;
	HTuple CirRii;

};



class CVisionProcess
{
public:
    CVisionProcess();
    ~CVisionProcess(void);

	
public:
	//��������߿���ȡָ��ͼ���ڴ��ָ��
	char * GetImageBuf(int index, int iWidth , int Height);
    bool ReadImage(const char *pName, int index = 0, bool bNewPic = false);
    bool DispImage(int winID, int index = 0);
    bool DispObj(long winID, bool bIsShowTmp);
	static bool ShowWindString(long winID,HTuple StrContent,int topRow = -1,int topCol = -1);
	static bool SetDispColorHeight(long winID,HTuple strColor,int FontHeight=0);
	bool SaveImage(int index,char *path,string SNstr);
	bool saveImagePath(int index,char *path,string SNstr,unsigned int type = 0);//����ͼƬ·��,type=0��ʾ���ͼƬgpz 2016-8-16
	char* getImagePath(int index,unsigned int type = 0);//�õ�����ͼƬ·��,type=0��ʾ���ͼƬgpz 2016-8-16
	static bool SaveWinImage(long winID,char *path,char *outpath,string SNstr);
	static void* GetWinImage(long winID,double &width,double &heigh);
    bool addAffineData(const char *sceneName, AF_POINT apoint, AF_POINT bpoint, int index,int CameraID = 0, bool bSave = false);
	bool creat_affine(const char *sceneName, AF_POINT *aoint, AF_POINT *bpoint,int CameraID = 0);
    int  affine_apply(int iCameraID,char *sceneName, AF_POINT &apoint, AF_POINT &stOutPoint);
    int  readAffineData(int CameraID, char *sceneName);
    HTuple  GetAffineData();
    int  SetAffineData(HTuple HomMat2D);
    int  SetAngleOffset(double dAngle);

	//int buildModel(int iWinID,ST_RECTANGLE_DATA &modelroi,char *sceneName,char *path);
    int buildModel(int iWinID,VIRSION_DATA_PKG *pData, int iLength, unsigned int *pType,char *sceneName,char *path);
	bool buildModel(int iWinID,int RoiID,char* modePath, int Nevels,double AngleStart,double AngleExtent,
		int Contrast,int index = 0);
	bool  MatchModel(int winId,int index,char* szModePath,int PyramidLevel,int MatchNum,double& Score,
		                    int ProcIndex,bool IsBulid,double& row,double& col,double& phi); //ʹ��double������׼���������ݱ궨��ƥ��
	
	bool  FindModelRealTime(int winId,int index,char* szModePath,int PyramidLevel,int MatchNum,double& Score,
		int ProcIndex,bool IsBulid,double& row,double& col,double& phi); //ʹ��double������׼���������ݱ궨��ƥ��


	//ƥ�����ԭλ�˱Ƚϣ��������Ҫ�����Ļ�е����ϵ�µ���ֵ
	void  MoveAndRotInCamForOriMatch(double PixSize,bool AxeMoveRotDirect,double& MotorRelX,double& MotorRelY,
		  int LastSceneId,int CurSceneId,int NextSceneId,double SceneRotPhi,double PicCentX,double PicCentY,double RotCenX,double RotCenY,double MotorRotPhi,double CalRotPhi,
		   double (*pCamParam),int ParamNum,double (*pCamPose),double CalThickness,
		    int CamIndex,int AxeIndex,int ToolIndex);

	static bool DobubleArrayEqueal(double * A,int arraynum);//����һ���Ƚ����麯��,Ӧ�÷��ڹ�����,��������������
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
    int readModel(char *sceneName,char *path, int index = 0, int iType = 0);
    int clearModel(int iModelID, int iType);
    int findModel(const char *pSceneName, VIRSION_DATA_PKG *pData, int iLength, unsigned int *pType, ST_RECTANGLE_DATA &point,unsigned int bDisplay = 1);
	bool  process(int sceneID,SCENE_PKG *param, CHECK_RESULT &result);
	bool OndrawRoi(long  halconID , const  ST_ROI_INFO  &roi, RoiCombType CombType ,PROCESS_TYPE bflag ,vector<int>* pRoiIDList , bool bcolor );
	//int  GetLastShapeId();
	void GenShape(const ST_ROI_INFO& roi,ShapeType shapeflag,int ShapeId,int Width,int Height);
	int  GetCurShapeId(void);
	bool  GetRoiParam(int ShapeId,ST_ROI_INFO* pRoi);
	void OnShowAllRoi(long  halconID,const vector<int>* const pRoiIdList); 
    bool  drawRoi(long  halconID , const  ST_ROI_INFO  &roi, ST_ROI_INFO &outroi ,bool bflag , bool bcolor );
	bool  dispRoi(long  halconID , ST_ROI_INFO  roi,bool bflag = true,bool bcolor = true);
	//�¶���
	void ShowShap(const Shape& shapObj,long halconID);


	bool Imagebuffer(int cameraindex, int sceneID, long *pbuffer);
	bool getimagesize(int &width, int &height);
	bool getimagesize(int ImageIndex,int &width, int &height);
	static bool setpart(long winID,int topx, int topy,int lowx,int lowy);
    static bool getpart(long winID,int &topx, int &topy,int &lowx,int &lowy);

	static bool openWindow(long winID, int x, int y, int width, int hight, long &halconWinID);//gpz ��VisionServer�е�halcon��صķŵ�VisionProcess��
	static bool closeWindow(long halconWinID);//gpz ��VisionServer�е�halcon��صķŵ�VisionProcess��

	void gen_arrow_contour_xld (HObject *Arrow, HTuple Row1, HTuple Column1, HTuple Row2, HTuple Column2, HTuple HeadLength, HTuple HeadWidth);
    //������ֵ�ָ����
    bool  DebugThreshold(long  halconID, int iMinGray, int iMaxGray);
	bool  findline(char *pSceneName, VIRSION_DATA_PKG *pData, int iLength, unsigned int *pType, ST_LINE &result, unsigned int IsDisplay = 0);
	bool  findcircle(char *pSceneName, VIRSION_DATA_PKG *pData, int iLength, unsigned int *pType, ST_CIRCLE &result, unsigned int IsDisplay = 0);
	bool  MeasureCircle(long iHalconWind,int ImageIndex,int MatchIndex,double& Row,double& Col,double& Rii); 

	bool HalconProcedureLocate(long iWinID,int Imageindex,string ProcedurePath,double (*inParam)[HalProcInParamNum], 
		double& PosX,double& PosY,double& PosPhi);

	bool HalconProcedureProcess(long iWinID,int Imageindex,string ProcedurePath,vector<double> &inparam,vector<double> &outparam,int display,int &error,string &hmessage);

	bool ImageEnhancement(long iWinID,int Imageindex,double& multinum,double& addnum,int display,int &error,string &hmessage);
    //��ȡ��Ӧ���Ͳ�����pkg->pBody�е�λ��
    //input ���ͣ� ������ pkg, 
    //�����ҵ�λ��
    static int FindParamData(SCENE_PKG *pkg, int iType, int index, int iGroup = -1);
    static int FindSpecTypePos(const VIRSION_DATA_PKG *pkg, unsigned int *pType, int iType);

	//���������궨�ľ���
	static void CalCamMetrix(int CamerIndex,int DataCount,double MotorX[],double MotorY[],double CamRow[],double CamCol[],
		double PixSize,double(*pCamRot),double (*pMecAndCam),double(*pImageRela),double (*pImageAbs),
		double (*pCamParam),int ParamNum,double (*pCamPose),
		int ToolIndex=0,int AxeIndex =0,double CalFirstX =0.0,
		double CalFirstY=0.0,double CalFirstAngle=0.0,double ToolFirstX =0.0,double ToolFirstY = 0.0,
		 double CamRotAngle = 0.0,double RotToCamX = 0.0,double RotToCamY =0.0,double CalThickNess=0.0);


	static void AffineCoorPoint(double CoordX,double CoordY,double CoordAng,double& PointX,double& PointY);

	void AffToRobotWorld(int CamIndex,int AxisIndex,int ToolIndex,double PixSize,double ModRotPhi,
		       double& ToolWorldX,double& ToolWorldY,bool WithCamParam,double CalThickness,double* pCamParam,int ParamNum,double* CamPos);

    bool ProcProcess(SCENE_PKG *param, HTuple &OutputValue, HTuple &ErrorCode, double &dOutTime);
    bool ToolProcProcess(const char *pSceneName, HDevProcedure &hdvpProc,int index, HTuple InputValue,
        char *szProcName, HTuple &OutputValue, HTuple &ErrorCode, unsigned int bDisplay = 0,bool bIsUserObj = true);
	

	
	//��ȡmessage�������Ϣ
	void get_mesage_information (HTuple Message, HTuple *String,HTuple *Color);

	HObject m_pImage[MAXPICNUMEVESCENE];//....

public:
    vector<Shape*> m_ShapeList;

private:
	HObject m_hImage;
	bool    m_bIsInit[8];//ͼ������Ƿ��ʼ��

    HObject m_vecObj;       //���������ʾ�Ľ������
    HTuple  m_vecMsg;       //

	HTuple m_vecSize;

	HTuple width;
	HTuple height;

	HTuple  m_modelid;
    int     m_modelFindType;
    int     m_modelBuildType;
    int     m_modelReadType;
	char *buffer;
	//vector<Shape*> m_DrawList;
	
	//char m_testBuffer[9000000];  //��ʱ���壬����ɾ����

	char resultImgPath[_MAX_PATH];// ����ͼƬ����󱣴�·�� gpz 2016-8-16
	int m_index;

	int m_FirstCamIndex;  //��������һ�����������
    //������̶���
    HDevProcedure m_hdvProc;
    HDevProcedure m_hdvMainProc;
    HDevProcedure m_hdvFModelProc;
    HDevProcedure m_hdvFNccModelProc;
    HDevProcedure m_hdvRModelProc;
    HDevProcedure m_hdvRNccModelProc;
    HDevProcedure m_hdvBModelProc;
    HDevProcedure m_hdvBNccModelProc;
    HDevProcedure m_hdvBManualModelProc;
    HDevProcedure m_hdvAffineProc;
    HDevProcedure m_hdvCAffineProc;
    HDevProcedure m_hdvCircleProc;
    HDevProcedure m_hdvLineProc;
    HDevEngine    m_HEngine;
    HTuple        m_HomMat2D;    //������
    HTuple        m_AngleOffset;    //�Ƕ�ƫ��
    HTuple        m_Direction;    //����
	HTuple        m_Dispix;    //ÿ�����ص�������� gpz 2016-8-18

    AF_POINT      m_RealArray[CALIB_DATA_MAX_SIZE];
    AF_POINT      m_PixelArray[CALIB_DATA_MAX_SIZE];

    //�˶�����ϵ������
    HTuple        m_MoveX;   //����֮ǰ����ƣ�����ֻ�ܱ���һ������ı궨���ݣ��������������ı궨����ͳһ����ϵͳ�����С�
    HTuple        m_MoveY;
    HTuple        m_PixelRow;
    HTuple        m_PixelCol;
    int           m_iValidSize;   //������������Ч����

public:
    //����궨��ת�����󣬱궨�����������ϵ�ͻ�е����ϵ��������е����ϵ���Ĺ�ϵ
	//��ÿ����е�ֶ�Ӧ�ý���һ����е����ϵ�����о�����ϵ����Ʋ�Ʒ����ϵ����ԭ��������ԭ��ʱ�Ļ�е�ֹ����㣬�����Ǹ���ά����
	static HTuple  m_CamerasHomMat2D[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER]; //����������
	static HTuple  m_CamRotToMotorHomMat2D[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER];//����������
	static HTuple  m_ImageToMotorRelative[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER]; //�����������
	static HTuple  m_ImageToMotorAbsolute[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER]; //�����������

	//���峡��������ʱ�����λ��λ��֮��λ�ñ任�Ĺ�ϵ
	static HTuple m_ScenePosPass[30][30]; //��ʱ����30�����������������ӣ� �������̫�࣬��Ҫ������ơ�
	                                      //��һ����ʾǰһ�������ţ��ڶ�����ʾ��ǰ����ĳ����š�
	 

private:
    bool          m_bIsAppend;
    /*static*/ bool          m_bIsPass;
    static CRITICAL_SECTION *m_pSection;
    double        m_MotorAngle;   //��������ϵ����������ϵ�н�

	int m_ShapeIdCount;
	HTuple m_HvMatchModID;

	//������ҵԭʼƥ��ֵ
	HTuple m_OrgionMatchRow;
	HTuple m_OrgionMatchCol;
	HTuple m_OrgionMatchPhi;
	//������ҵ��ǰƥ��ֵ
	HTuple m_CurMatchRow;
	HTuple m_CurMatchCol;
	HTuple m_CurMatchPhi;
	int m_CurMatchTypeProcIndex;
	//������ҵ��ƥ��任�б�,��ʾ�µ�ƥ��λ�������ԭʼģ��λ�õı仯����,�ó�����ƥ�����Ŷ�Ӧ�������±ꡣ
	vector<HTuple> m_MatchHomm2DList; //��λΪ����	
	 //vector<HTuple> m_RealMatchHomm2DList;//��λΪʵ�ʻ�е��λ,   ��ʱûʹ��


public:
	
	

};

#endif