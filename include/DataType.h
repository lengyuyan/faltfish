#ifndef _DATA_DEF_H_
#define _DATA_DEF_H_

#include <vector>
#include <hash_map>
#include <map>
#include <string>
using namespace std;
#ifndef SCENE_SIZE
#define SCENE_SIZE 64
#endif
#ifndef MODEL_PARAM_SIZE
#define MODEL_PARAM_SIZE 12
#endif
#ifndef BUF_SIZE
#define BUF_SIZE 256
#endif

#ifndef TOOL_NUMBER
#define  TOOL_NUMBER 6  // product number
#endif

#ifndef AXIS_NUMBER
#define  AXIS_NUMBER 8  //  
#endif

#ifndef CAMCAL_NUMBER //����궨������
#define  CAMCAL_NUMBER 12
#endif

#ifndef CAMERA_NUMBER
#define  CAMERA_NUMBER 8
#endif

#ifndef RESULT_NUMBER
#define  RESULT_NUMBER 100
#endif

#define LOGIN_TIME_OUT   5 * 60 * 1000   //5�����޲����Զ��˳���½

#ifndef CONFIG_VALUE_LENGTH
#define CONFIG_VALUE_LENGTH 64
#endif
#ifndef CONFIG_GENERAL_LENGTH
#define CONFIG_GENERAL_LENGTH 64
#endif

//����һ���л���Ʒ��ӳ�����ݡ�
#define LPPMAPDATA hash_map<string,map<int,int>>*
#define PMAPDATA hash_map<string,map<int,int>>
#define SCENEMAP map<int,int>

#define  HalProcInParamNum 12   //halcon���������Ŀ
#define  MAXPICNUMEVESCENE 8	//ÿ���������������Ĳɼ���ص���Ŀ

enum DATA_TYPE
{
    VISION_INT_TYPE = 0,    //int ���� 
    VISION_DOUBLE_TYPE,     //double ����
};

struct ST_RECTANGLE_DATA
{
	double col;          //halcon����col
	double row;          //halcon����row
	double length1;
	double length2;
	double angle;
    ST_RECTANGLE_DATA ()
    {
        col = 1294.0;
        row = 970.0;
        angle = 0.0;
        length1 = 200.0;
        length2 = 160.0;
        //angle = 0.0;
    }
    ST_RECTANGLE_DATA operator=(ST_RECTANGLE_DATA so)
    {
        col = so.col;
        row = so.row;
        length1 = so.length1;
        length2 = so.length2;
        angle = so.angle;
        return *this;
    }
};

struct ST_CIRCLE_DATA
{
	double col;     //halcon����col
	double row;     //halcon����row
	double Radius;
	double width;
    double angleB;  //Բ��ʱ��
    double angleE;
    ST_CIRCLE_DATA()
    {
        col = 1294.0;
        row = 970.0;
        Radius = 200.0;
        width = 15.0;
        angleB = 0;
        angleE = 360;
    }
    ST_CIRCLE_DATA operator=(ST_CIRCLE_DATA so)
    {
        col = so.col;
        row = so.row;
        Radius = so.Radius;
        width = so.width;
        angleB = so.angleB;
        angleE = so.angleE;
        return *this;
    }
};

struct ST_COMB_DATA
{
	vector<int> m_ShapIdList;
	vector<int> m_CombTypeList;

};

struct ST_FIND_LINE
{
    //���߸���Ȥ������Rectangle
    ST_RECTANGLE_DATA strect;
    double smooth;    //ƽ����
    int    threshold;  //���߱�Ե����
    int    ROIWidth;  //��һ��������
    int    iPosNum;     //���������
    int    iFindType;   //�ҵ㷽���� 0Ϊall, 1Ϊ�ҵ�һ���㣬 2Ϊ�����һ����
    int    iTransition;  //0Ϊall, 1Ϊpositive,2Ϊnegative,һ������ֻ�õ�positive��negative
    int    iAlgorithm;  //���ֱ��ʱ��ѡȡ���㷨(0Ϊregression��1Ϊhuber��2Ϊtukey��3Ϊgauss��4Ϊdrop��)
    int    iDirection;  //���߷���,0Ϊ����1Ϊ��
	ST_FIND_LINE()
    {
		smooth = 3.0;
		threshold = 30;
		ROIWidth = 5;
		iPosNum = 30;
		iFindType = 1;
		iTransition = 1;
		iAlgorithm = 2;
		iDirection = 0;
	}
};
struct ST_FIND_CIRCLE
{ 
    //��Բ����Ȥ������Բ��
    ST_CIRCLE_DATA stcircle;
    double smooth;    //ƽ����
    int    threshold;  //��Բ��Ե����
    int    ROIWidth;  //��һ��������
    int    iPosNum;     //���������
    int    iTransition;  //0Ϊall, 1Ϊpositive,2Ϊnegative,һ������ֻ�õ�positive��negative
    int    iFindType;   //�ҵ㷽���� 0Ϊall, 1Ϊ�ҵ�һ���㣬 2Ϊ�����һ����
    int    iAlgorithm;  //���Բʱ��ѡȡ���㷨(algebraic, ahuber, atukey, geometric, geohuber, geotukey )
    int    iDirection;  //��Բ����,0Ϊ����1Ϊ��
	ST_FIND_CIRCLE()
	{
		smooth = 3.0;
		threshold = 30;
		ROIWidth = 5;
		iPosNum = 30;
		iFindType = 1;
		iTransition = 1;
		iAlgorithm = 0;
		iDirection = 0;
	}
};

struct ST_MODEL_INFO
{
    ST_RECTANGLE_DATA modelRectangle; //ģ������
    //�鵽��ģ���λ��
    double row;
    double col;
    double angle;      //�鵽��ģ��ĽǶ�
    int modelID;           //ģ��ID
    double modelScore;           //ģ��ƥ�������0~0.1��
    double findNum;
    double fNumLevels;
    double cNumLevels;
    double contrast;
    double angleStart;
    double angleExtent;
    int modelType;
    ST_MODEL_INFO ()
    {
        modelType = 0;
        modelID = -1;
        modelScore = 0.8;
        findNum = 1;
        fNumLevels = 0;
        cNumLevels = -1;
        contrast = 0;
        angleStart = -0.39;
        angleExtent = 0.79;
        row = 0.0;
        col = 0.0;
        angle = 0.0;
    }
};

struct ST_IMAGE_PRO_INFO
{
    int iCamareNo;       //������
    int iLightNo;         //��Դ���
    int modelCount;         //ģ�����, < 0 ���ʾ����Ҫ����ģ��
    int iBlob;           //�Ƿ�����ֵ�ָ�, >0Ϊ��ֵ����, < 0 Ϊ������ֵ����
    int shutter;         //����ʱ��
    ST_RECTANGLE_DATA roiRectangle;  //����Ȥ������Rectangle
    int iLineCount;          //��ǰͼ�������߸���
    int iCircleCount;          //��ǰͼ������Բ����
};
struct SCENE_CONFIG
{
    bool   bIsDisplay;                      //�Ƿ���ʾ������Ϣ
    char   szSceneName[SCENE_SIZE];         //������
	unsigned int sceneID;             //����ID
	unsigned long widID;               //UI����ID
    unsigned int stationID;            //���ڹ���ʱ�жϰ��ĸ�����
    unsigned int iCountImage;                  //ץ������ͼ����
    unsigned int bSave; //��ӵ�ϵͳ�������ý���                     //�Ƿ��������ת��     
    vector< ST_IMAGE_PRO_INFO > vecImageProInfo;   //��ץͼ�����й�
    vector< ST_FIND_LINE >vecLineParam;                  //��Ҫ���߲�������
    vector< ST_FIND_CIRCLE >vecCircleParam;                 //��Ҫ��Բ��������
    vector< ST_MODEL_INFO> vecModelInfo;                    //ģ�����
    int threshold[SCENE_SIZE];        //Blob�õ���ֵ�б�

    SCENE_CONFIG operator=(SCENE_CONFIG so)
    {
        bIsDisplay = so.bIsDisplay;
        bSave = so.bSave;
        strncpy_s(szSceneName, so.szSceneName, SCENE_SIZE - 1);
	    sceneID = so.sceneID;
	    widID = so.widID;               //UI����ID
        stationID = so.stationID;
        iCountImage = so.iCountImage;                  //ץ������ͼ����
        vecImageProInfo = so.vecImageProInfo;   //��ץͼ�����й�
        vecLineParam = so.vecLineParam;                  //��Ҫ���߲�������
        vecCircleParam = so.vecCircleParam;                 //��Ҫ��Բ��������
        vecModelInfo = so.vecModelInfo;
        memcpy(threshold, so.threshold, SCENE_SIZE * sizeof(int));
        return *this;
    }
    SCENE_CONFIG()
    {
        bIsDisplay = false;
        bSave = false;
        sceneID = 0;
        widID = 0;
        stationID = 0;
        iCountImage = 0;
    }
};
struct CHECK_RESULT
{
    int err;						//������
    int index;						//������
    int iCount;						//��Ч�������
    char szValue[RESULT_NUMBER][BUF_SIZE/2];			//��ŵĽ��ֵ�����32��ֵ
    double time;					//���ù��̼���ʱ��
	double caturetime;				//�ɼ�ʱ��
    bool  bIsDisp;					//�����ж��Ƿ���ʾ����	
	int SeneTpye;					//�������ͣ��궨���Ӿ������,10Ϊ�궨 11Ϊ����
	int resultType;                 //��λ�������,0��ʶ�������꣬1��ʶ�������
	int testCount;                  //���������Դ���
	int CameraIndex[CAMERA_NUMBER]; //�������
	string rawimg;					//ԭʼͼƬ
	string outimg;					//���ͼƬ
	bool isGeneralProcess;			//ͨ�ô������ͽ��ʱ����ֹ����һ�����
    CHECK_RESULT()
    {	
		isGeneralProcess = 0;
		resultType = 0;				//Ĭ�Ͼ�������
		testCount = 0;
		SeneTpye =-1;
        err   = 0;
        index = 0;
        iCount = 0;
        memset(szValue, 0, sizeof(char) * 32 * 128);
        time = 0.0;
		caturetime =0.0;
        bIsDisp = true;
		rawimg = "";
		outimg = "";
	    memset(CameraIndex,-1,sizeof(CameraIndex));
	    
    }
	void operator=(CHECK_RESULT so)
	{
		isGeneralProcess = so.isGeneralProcess;
		err = so.err;
		resultType = so.resultType;				
		testCount = so.testCount;
		SeneTpye = so.SeneTpye;
		index = so.index;
		iCount = so.iCount;
		time = so.time;
		caturetime =caturetime;
		bIsDisp = so.bIsDisp;
		rawimg = so.rawimg;
		outimg = so.outimg;
		for (int i = 0;i<CAMERA_NUMBER;i++)
		{
			CameraIndex[i] = so.CameraIndex[i];
		}
		for (int i= 0;i<RESULT_NUMBER;i++)
		{
			strcpy_s(szValue[i],so.szValue[i]);
		}
	}
};


struct ST_CIRCLE
{
	double y;  //y.x�൱��halcon�е�row��col
	double x;
	double radius;
	ST_CIRCLE()
	{
		x = 0.0;
		y = 0.0;
		radius = 0.0;
	}
};

struct ST_LINE
{
	double y_begin;  //y.x�൱��halcon�е�row��col
	double x_begin;
	double y_end;
	double x_end;
	double phi;
	ST_LINE()
	{
		y_begin = 0.0;
		x_begin = 0.0;
		y_end = 0.0;
		x_end = 0.0;
	}
};


//����ͨѶЭ�������
typedef struct IpProctocolSetData
{
	char CalRevHead[64];
	char WorkRevHead[64];
	char CalSendHead[64];
	char WorkSendHead[64];
	char IPDivid[64];
	char IPEnd[64];

	bool EnableData[6];

	void MakeEndable()
	{
		EnableData[0] = strlen(CalRevHead)>0 ? true:false;
		EnableData[1] = strlen(WorkRevHead)>0 ? true:false;
		EnableData[2] = strlen(CalSendHead)>0 ? true:false;
		EnableData[3] = strlen(WorkSendHead)>0 ? true:false;
		EnableData[4] = strlen(IPDivid)>0 ? true:false;
		EnableData[5] = strlen(IPEnd)>0 ? true:false;
	}

	IpProctocolSetData()
	{
		memset(CalRevHead,0,sizeof(CalRevHead));
		memset(WorkRevHead,0,sizeof(WorkRevHead));
		memset(CalSendHead,0,sizeof(CalSendHead));
		memset(WorkSendHead,0,sizeof(WorkSendHead));
		memset(IPDivid,0,sizeof(IPDivid));
		memset(IPEnd,0,sizeof(IPEnd));
		memset(EnableData,0,sizeof(EnableData));


	}
	void operator=(IpProctocolSetData so)
	{
		memcpy(CalRevHead,so.CalRevHead,sizeof(CalRevHead));
		memcpy(WorkRevHead,so.WorkRevHead,sizeof(WorkRevHead));
		memcpy(CalSendHead,so.CalSendHead,sizeof(CalSendHead));
		memcpy(WorkSendHead,so.WorkSendHead,sizeof(WorkSendHead));
		memcpy(IPDivid,so.IPDivid,sizeof(IPDivid));
		memcpy(IPEnd,so.IPEnd,sizeof(IPEnd));
		memcpy(EnableData,so.EnableData,sizeof(EnableData));
		MakeEndable();
	}

}PROCTOCOLDATA,*LPPROCTOCOLDATA;


typedef struct CamCalData
{
	int PexSize;
	int RotX;


}CAMCALDATA,*LPCAMCALDATA;


typedef struct _SOCKET_PARAM
{
    //server
    int          nettype;
    int          SeverSocketType;
    int          protocol;
    string       strServerIPAdd;
    string       strClientIPAdd;
    unsigned int ServerPort;
    unsigned int ClientPort;

	//ͨ�������Զ���Э��
	string  CalRevHead;
	string	CalSendHead;
	string	WorkRevHead;
	string	WorkSendHead;
	string	DividLabel;
	string	EndLabel;


    _SOCKET_PARAM()
    {
        strServerIPAdd = "";
        strClientIPAdd = "";

		CalRevHead="";
		CalSendHead="";
		WorkRevHead="";
		WorkSendHead="";
		DividLabel="";
		EndLabel="";

        ServerPort = 0;
        ClientPort = 0;
        nettype = 0;
        SeverSocketType = 0;
        protocol = 0;
    }
	

    _SOCKET_PARAM &operator=(_SOCKET_PARAM copy)
    {
        strServerIPAdd = copy.strServerIPAdd;
        strClientIPAdd = copy.strClientIPAdd;

		CalRevHead=copy.CalRevHead;
		CalSendHead=copy.CalSendHead;
		WorkRevHead=copy.WorkRevHead;
		WorkSendHead=copy.WorkSendHead;
		DividLabel=copy.DividLabel;
		EndLabel=copy.EndLabel;

        ServerPort = copy.ServerPort;
        ClientPort = copy.ClientPort;
        nettype = copy.nettype;
        SeverSocketType = copy.SeverSocketType;
        protocol = copy.protocol;

        return *this;
    }

	void GetProctocolData(PROCTOCOLDATA& ProctoData)
	{
		//char szText[64]={0};
		sprintf_s(ProctoData.CalRevHead,"%s",CalRevHead.c_str());
		sprintf_s(ProctoData.WorkRevHead,"%s",WorkRevHead.c_str());
		sprintf_s(ProctoData.CalSendHead,"%s",CalSendHead.c_str());
		sprintf_s(ProctoData.WorkSendHead,"%s",WorkSendHead.c_str());
		sprintf_s(ProctoData.IPDivid,"%s",DividLabel.c_str());
		sprintf_s(ProctoData.IPEnd,"%s",EndLabel.c_str());
	}

}SOCKET_PARAM;




typedef struct _FLAT_LIGHT_INFO
{
    int iType;            //��Դ���ͣ���CST
    int iCount;           //��Դͨ������
    int baudrate;
    int parity;
    int bytesize;
    int stopbits;
    string strComm;       //��������
}FLAT_LIGHT_INFO;

struct AF_POINT    //�����
{
	double x;
	double y;
    double angle;
	double dis_pix; //ÿ�����ص������������gpz
};



struct CALIB_DATA
{
    bool bIsEnd;     //�Ƿ������ǰ�궨������
    bool bIsInteractive;     //�Ƿ���Ҫ����
    int  iSceneID;   //����ID
    int  index;      //������
    int  iCount; 
    char chSN[64];     //SN��Ϣ
    double xCalib[16];   //x����
    double yCalib[16];   //y����
	double phiCalib[16]; //��ת����
    CALIB_DATA()
    {
        bIsEnd = false;
        bIsInteractive = false;
        iSceneID = 0;
        index = 0;
        iCount = 1;
        memset(chSN, 0, 64);
        memset(xCalib, 0, 16 * sizeof(double));
        memset(yCalib, 0, 16 * sizeof(double));
		memset(phiCalib, 0, 16 * sizeof(double));
    }
};

struct ST_ROI_INFO //roi��Ϣ
{
	int iType;                         //���ָ�ROIΪԲ��ֱ�ߣ� 0 Ϊֱ�ߣ� 1ΪԲ,2Ϊ����...
	                                   // ���·��������͵������б䶯��Ҫ�𲽹��ɵ��·�������
	ST_RECTANGLE_DATA strect;
	ST_CIRCLE_DATA stcircle; 
	ST_COMB_DATA stCombine;

};


enum CAMARA_ERRORCODE
{
	CAMERA_SUCCESS = 0,
	CAMERA_OPEN_FAIL = 1,//�����ʧ��
	CAMERA_NOFIND_CAMERA = 2,//�Ҳ���ָ�����ֵ����
	IMAGE_NOINSTANCE = 3, //iIamgeCtrl û��ʵ�л�
	DLL_NOEXIST = 4, //DLL������
	FIND_NOCAM = 5, //�������ʧ��
};

enum CAMARA_ADN_CONNECT_TYPE
{
	CAMARA_TYPE_AVT_1394 = 0,//avt 1394
	CAMARA_TYPE_BASLER_GIGE = 1,

};

enum CAMARA_PARA_ITEM
{
	IMAGEFORMAT = 0,
	PHYSPEED = 1,
	PACKETSIZE = 2,
};

enum LIGHTCTL_TYPE
{
	LIGHTCTL_TYPE_NDPS = 0,//ndps
	LIGHTCTL_TYPE_OPT = 1,//opt
	LIGHTCTL_TYPE_OPT_PV = 2,//opt PV

};

enum LIGHTCTL_ERRORCODE
{
	LIGHTCTL_SUCCESS = 0,
	LIGHTCTL_NOINSTANCE = 1,//��Դ�ӿ�ûʵ����
	LIGHTCTL_COMOPEN_ERROR = 2,//com �ڴ�ʧ��
	LIGHTCTL_NOFINDTHE_COM = 3,//û�ҵ�ָ����com��
	LIGHTCTL_SEND_FAIL = 4,//�����ʧ��
};

typedef struct _lighttype
{
    string TypeName;	//������
	DWORD BaudRate; //�����ʣ�ָ��ͨ���豸�Ĵ������ʡ������Ա������ʵ�ʲ�����ֵ��������ĳ���ֵ֮һ��
    DWORD fParity; // ָ����żУ��ʹ�ܡ����˳�ԱΪ1��������żУ���� 
    BYTE ByteSize; // ͨ���ֽ�λ����4��8
    BYTE Parity; //ָ����żУ�鷽�����˳�Ա����������ֵ��EVENPARITY(2) żУ��     NOPARITY(0) ��У�� MARKPARITY(3) ���У��   ODDPARITY(1) ��У��
    BYTE StopBits; //ָ��ֹͣλ��λ�����˳�Ա����������ֵ��ONESTOPBIT 1λֹͣλ   TWOSTOPBITS 2λֹͣλ	ONE5STOPBITS   1.5λֹͣλ
    DWORD ReadIntervalTimeout; //���ַ�֮��������ʱ
    DWORD ReadTotalTimeoutMultiplier; //��ȡÿ�ַ���ĳ�ʱ
    DWORD ReadTotalTimeoutConstant; //һ�ζ�ȡ�������ݵĹ̶���ʱ
    DWORD WriteTotalTimeoutMultiplier; //д��ÿ�ַ���ĳ�ʱ
    DWORD WriteTotalTimeoutConstant; //һ��д�봮�����ݵĹ̶���ʱ

	_lighttype()
	{
    	TypeName = "NDPS";
    	BaudRate =19200;
    	fParity = 0;
    	Parity = 0;
    	ByteSize = 8;
    	StopBits = 0;
    	ReadIntervalTimeout = MAXDWORD;
    	ReadTotalTimeoutMultiplier = 0;
    	ReadTotalTimeoutConstant = 0;
    	WriteTotalTimeoutMultiplier = 10;
    	WriteTotalTimeoutConstant = 1000;
	}
    
    _lighttype operator=(_lighttype &obj)
    {
    	TypeName = obj.TypeName;
       	BaudRate =obj.BaudRate;
    	fParity = obj.fParity;
       	Parity = obj.Parity;
       	ByteSize = obj.ByteSize;
       	StopBits = obj.StopBits;
       	ReadIntervalTimeout = obj.ReadIntervalTimeout;
       	ReadTotalTimeoutMultiplier = obj.ReadTotalTimeoutMultiplier;
       	ReadTotalTimeoutConstant = obj.ReadTotalTimeoutConstant;
       	WriteTotalTimeoutMultiplier = obj.WriteTotalTimeoutMultiplier;
       	WriteTotalTimeoutConstant = obj.WriteTotalTimeoutConstant;
        return *this;
    }
}LightTypeInfo;

typedef struct lightinfo
{	
	LIGHTCTL_TYPE strLightType;//��Դ����
    int    Channel;     //ͨ������
	string PortName;	//������
    LightTypeInfo strTypeInfo;  //������Ϣ
    int   DefaultValue;   //ȱʡ����
	lightinfo()
	{
		strLightType = LIGHTCTL_TYPE_NDPS;
		PortName = "COM1";
        Channel = 4;
        DefaultValue = 200;
	}
    lightinfo operator=(lightinfo &so)
    {
        strLightType = so.strLightType;
        PortName = so.PortName;
        Channel = so.Channel;
        DefaultValue = so.DefaultValue;
        return *this;
    }

} LightInfo;
          
typedef struct _SYSTEM_PARAM
{     
    int  iAutoOL;         //0ΪԶ��IO���ƹ�Դ��1Ϊ��ǰ���򴮿ڿ���IO
    bool bLandType;        //��½
    int  iIsInstall;
    int  iWindowNum;       //������ʾUI������С�ڵ�����Ч��������

	int iCameraDllNum;     //Ҫ���ص����dll����gpz

    int  iSceneNum;        //��Ч��������
    int    iLightNum;         //��Դ����, ���֧��16����Դ
    int    iLightTypeNum;         //��Դ�������, ���ͬʱ֧��4�ֹ�Դ
    int    iLightChaNum;         //��Դͨ������
    int    iCameraNum;        //�������
	int    iAxisNum;           //ת��ʵ�ʸ���
	int    iToolNum;         // �о߸���
    //char   *pImageBuf[64];       //�����ͼ�洢Bufferÿ�����һ��buffer
    char   ImageSavePath[256];        //ͼƬ����·��
    LightInfo strLightInfo[16];       //��Դ��Ϣ
    LightTypeInfo strLightType[4];        //��Դ����
    string pstrCamareGUID[CAMERA_NUMBER];   //���GUID�б�
    string pstrSceneName[SCENE_SIZE];    //��Ч���������б�
	string pstrCameraDll[CAMERA_NUMBER];    //Ҫ���ص����dll���� gpz
	void* MainImageWndVec;  //��ͼ�񴰿�����
	enum FixType
    {
	    FIXED = 0,
	    UNFIXED = 1,
	    
    };

	//����궨
	int CalPointNum[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER];  ////����궨���ʵ������
	double CalMotorX[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER][CAMCAL_NUMBER];    //����궨ʱ��X����ƶ�������
	double CalMotorY[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER][CAMCAL_NUMBER];    //����궨ʱ��Y����ƶ�������
	double CalMotorPhi[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER][CAMCAL_NUMBER];    //����궨ʱ����ת�Ƕ����ת��������


	double CameraPixSize[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER];  //������ش�С
	bool   CalWithCamParam[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER]; //�ŵ�궨ʹ���������true�������ش�Сfalse��
	double CameraCalMachineX[CAMERA_NUMBER][16];// ����궨�Ļ�е����
	double CameraCalMachineY[CAMERA_NUMBER][16];//
	double CameraCalPixCol[CAMERA_NUMBER][16] ;  // ����궨ʱ���������ꡣ
	double CameraCalPixRow[CAMERA_NUMBER][16] ;
	double CameraCalPixPhi[CAMERA_NUMBER][16];
	//ת���ڲ�Ʒ���оߣ��е�λ��
	//double RotX[CAMERA_NUMBER][Tool_Num]; //������ÿ��������Ա궨���моߵ�ת�ᡣ
	//double RotY[CAMERA_NUMBER][Tool_Num]; // 
	double RotX[AXIS_NUMBER][TOOL_NUMBER]; //������ÿ������Զ�Ӧ���моߡ�
	double RotY[AXIS_NUMBER][TOOL_NUMBER]; 
	double RotX_Adjust[AXIS_NUMBER][TOOL_NUMBER]; //��RotXֵ������
	double RotY_Adjust[AXIS_NUMBER][TOOL_NUMBER]; 

	int CamFixedType[CAMERA_NUMBER]; //����İ�װ��ʽ
	double  ToolCalOriX[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER];
	double  ToolCalOriY[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER];
	double  ToolCalOriPhi[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER];//Angle
	double  CamCalOriX[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER];
	double  CamCalOriY[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER];
	double  CamCalOriPhi[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER]; //Angle

	int PosMoveType[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER]; //�˶����ƶ�λ�Ĵ�������
	bool AxeRotDirect[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER];  //������ͬΪfalse���෴Ϊtrue��//��������̶������ƶ���һ�㷽�����෴�ģ�
	                                   //Ŀǰ��û��������һ�µ���������ǵ��̶����������ͬһ��ʱ���е�ì�ܣ����������������������һ����
	                                   //Ŀǰ�����ڶ�λУ��ʱ�ж�У���Ƕȵ�������
	double RotpointToCamX[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER];
	double RotpointToCamY[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER];

	//���ϵ�ڻ�еϵ�е�(����������)��ת����,��һ��ֵΪ����6,����6����Ϊ����
	double CamRotMetix[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER][7]; 
	//���ϵ�ڻ�еϵ�еģ����������ģ�����
	double CamInMecRotMetix[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER][7];
	//ͼ��ϵ�ڻ�еϵ�е�(����������)����
	double ImageRelative[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER][7];
	//ͼ��ϵ�ڻ�еϵ�еģ����������ģ�����
	double ImageAbsolute[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER][7];

	//����ڲ�
	double CamParam[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER][12];
	//������
	double CamPose[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER][7];
	//�궨����
	double CalThickNess[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER];

	double PicCenterPointX[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER];//ͼƬ���ĵ�
	double PicCenterPointY[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER];

	bool GetDivCamParam( double(*pCamParam),int CamIndex,int AxeIndex,int ToolIndex,int ArrLen )
	{
		if (8!=ArrLen||CamIndex<0||CamIndex>CAMERA_NUMBER||AxeIndex<0||AxeIndex>AXIS_NUMBER||ToolIndex<0||ToolIndex>TOOL_NUMBER)
		{
		    return false;
		}

	    for (int i = 0;i<8;i++)
	    {
	        pCamParam[i]= CamParam[CamIndex][AxeIndex][ToolIndex][i];
	    }
    
		return true;
	}

	bool GetPolCamParam( double(*pCamParam),int CamIndex,int AxeIndex,int ToolIndex,int ArrLen )
	{
		if (12!=ArrLen||CamIndex<0||CamIndex>CAMERA_NUMBER||AxeIndex<0||AxeIndex>AXIS_NUMBER||ToolIndex<0||ToolIndex>TOOL_NUMBER)
		{
		    return false;
		}

	    for (int i = 0;i<12;i++)
	    {
	        pCamParam[i]= CamParam[CamIndex][AxeIndex][ToolIndex][i];
	    }
    
		return true;
	}
	//Socket����
    SOCKET_PARAM stSocketParam;
	//ȷ���Ƿ��ǲ���
	bool  IsSimulate;
	//�����Ʒӳ�������
	PMAPDATA ProductMap;  //�û���Ų�Ʒ����ӳ�����ݡ�
	string CurProductName; //�л���Ʒʱ���ڴ�Ų�Ʒ���ơ�
	string ProductById[20];
    _SYSTEM_PARAM()
    {
        iAutoOL = 0;
        bLandType = true;
        iIsInstall = 0;
        iWindowNum = 1;     
        iSceneNum = 1;      
        iLightNum = 1;    
        iLightTypeNum = 1;
        iLightChaNum = 4; 
        iCameraNum = CAMERA_NUMBER;  
		iToolNum  = TOOL_NUMBER;
		iAxisNum = 0;
		IsSimulate = false;
		memset(CameraPixSize,0,sizeof(CameraPixSize));
		memset(CalWithCamParam,0,sizeof(CalWithCamParam));
		memset(RotX,0,sizeof(RotX));
		memset(RotY,0,sizeof(RotY));
		memset(RotX_Adjust,0,sizeof(RotX_Adjust));
		memset(RotY_Adjust,0,sizeof(RotY_Adjust));
		memset(CameraCalPixPhi,0,sizeof(CameraCalPixPhi));
		memset(CalPointNum,0,sizeof(CalPointNum));
		memset(CalMotorX,0,sizeof(CalMotorX));
		memset(CalMotorY,0,sizeof(CalMotorY));
		memset(CalMotorPhi,0,sizeof(CalMotorPhi));

		memset(CamFixedType,0,sizeof(CamFixedType));
		memset(ToolCalOriX,0,sizeof(ToolCalOriX));
		memset( ToolCalOriY,0,sizeof( ToolCalOriY));//
		memset( ToolCalOriPhi,0,sizeof( ToolCalOriPhi));//ToolCalOriPhi
		memset(CamCalOriX,0,sizeof( CamCalOriX ));
		memset(CamCalOriY,0,sizeof( CamCalOriY));
		memset(CamCalOriPhi,0,sizeof( CamCalOriPhi)); 
		memset(PosMoveType,0,sizeof( PosMoveType)); //PosMoveType
		memset(AxeRotDirect,0,sizeof( AxeRotDirect)); //AxeRotDirect
		memset(RotpointToCamX,0,sizeof( RotpointToCamX)); 
        memset(RotpointToCamY,0,sizeof( RotpointToCamY)); 
		//CamRotMetix
		memset(CamRotMetix,0,sizeof(CamRotMetix));
		memset(CamInMecRotMetix,0,sizeof(CamInMecRotMetix));
		memset(ImageRelative,0,sizeof(ImageRelative));
		memset(ImageAbsolute,0,sizeof(ImageAbsolute));
		//ImageRelative,ImageAbsolute;
		memset(CamParam,0,sizeof(CamParam));
		memset(CamPose,0,sizeof(CamPose));
		memset(CalThickNess,0,sizeof(CalThickNess));
	    memset(ProductById,0,sizeof(ProductById));
	    CurProductName.empty();

		memset(PicCenterPointX,0,sizeof( PicCenterPointX ));
		memset(PicCenterPointY,0,sizeof( PicCenterPointY ));
		
    }
    void SystemParamCopy(_SYSTEM_PARAM *so)
    {
        if (NULL == so)
        {
            return;
        }
		MainImageWndVec = so->MainImageWndVec;
        iAutoOL = so->iAutoOL;  
        bLandType = so->bLandType;
        iIsInstall = so->iIsInstall;
        iWindowNum = so->iWindowNum;
        if (0 < so->iSceneNum && so->iSceneNum <= 64)
        {
            iSceneNum = so->iSceneNum;
        }
        else
        {
            iSceneNum = 1;
        }

        if (0 < so->iLightNum && so->iLightNum <= 16)
        {
            iLightNum = so->iLightNum;
        }
        else
        {
            iLightNum = 1;
        }
        iLightNum = so->iLightNum;
        if (0 < so->iLightTypeNum && so->iLightTypeNum <= 4)
        {
            iLightTypeNum = so->iLightTypeNum;
        }
        else
        {
            iLightTypeNum = 1;
        }
        iLightChaNum = so->iLightChaNum;  
        if (0 < so->iCameraNum && so->iCameraNum <= 64)
        {
            iCameraNum = so->iCameraNum;
        }
        else
        {
            //iCameraNum = 1;
        }

        int i = 0;
        for (int i = 0; i < iLightNum; i++)
        {
            strLightInfo[i] = so->strLightInfo[i];
        }
		if (0 < so->iCameraNum && so->iCameraNum <= 64)//gpz
		{
			iCameraDllNum = so->iCameraDllNum;
		}
		else
		{
			iCameraDllNum = 1;
		}
		for (i = 0; i < iCameraDllNum; i++)
		{
			pstrCameraDll[i] = so->pstrCameraDll[i];
		}
        for (i = 0; i < iSceneNum; i++)
        {
            pstrSceneName[i] = so->pstrSceneName[i];
        }
        for (i = 0; i < iCameraNum; i++)
        {
            pstrCamareGUID[i] = so->pstrCamareGUID[i];
        }

		iAxisNum = so->iAxisNum;
		IsSimulate = so->IsSimulate;
		//����궨����
		memcpy(CameraPixSize,so->CameraPixSize,sizeof(CameraPixSize));
		memcpy(CalWithCamParam,so->CalWithCamParam,sizeof(CalWithCamParam));

		memcpy(CameraCalMachineX,so->CameraCalMachineX,sizeof(CameraCalMachineX));
		memcpy(CameraCalMachineY,so->CameraCalMachineY,sizeof(CameraCalMachineY));
		memcpy(CameraCalPixCol,so->CameraCalPixCol,sizeof(CameraCalPixCol));
		memcpy(CameraCalPixRow,so->CameraCalPixRow,sizeof(CameraCalPixRow));
		memcpy(CameraCalPixPhi,so->CameraCalPixPhi,sizeof(CameraCalPixPhi));

		memcpy(CalPointNum,so->CalPointNum,sizeof(CalPointNum));
		memcpy(CalMotorX,so->CalMotorX,sizeof(CalMotorX));
		memcpy(CalMotorY,so->CalMotorY,sizeof(CalMotorY));
		memcpy(CalMotorPhi,so->CalMotorPhi,sizeof(CalMotorPhi));
		memcpy(CamFixedType,so->CamFixedType,sizeof(CamFixedType));
		memcpy(PicCenterPointX,so->PicCenterPointX,sizeof(PicCenterPointX));
		memcpy(PicCenterPointY,so->PicCenterPointY,sizeof(PicCenterPointY));
		memcpy(ToolCalOriX,so->ToolCalOriX,sizeof(ToolCalOriX));
		memcpy(ToolCalOriY,so->ToolCalOriY,sizeof(ToolCalOriY));
		memcpy(ToolCalOriPhi,so->ToolCalOriPhi,sizeof(ToolCalOriPhi));
		//ToolCalOriPhi
		memcpy(CamCalOriX,so->CamCalOriX,sizeof(CamCalOriX));
		memcpy(CamCalOriY,so->CamCalOriY,sizeof(CamCalOriY));
		memcpy(CamCalOriPhi,so->CamCalOriPhi,sizeof(CamCalOriPhi));
		memcpy(PosMoveType,so->PosMoveType,sizeof(PosMoveType));//PosMoveType
		memcpy(AxeRotDirect,so->AxeRotDirect,sizeof(AxeRotDirect));
		memcpy(RotpointToCamX,so->RotpointToCamX,sizeof(RotpointToCamX));
		memcpy(RotpointToCamY,so->RotpointToCamY,sizeof(RotpointToCamY));
		memcpy(CamParam,so->CamParam,sizeof(CamParam));
		memcpy(CamPose,so->CamPose,sizeof(CamPose));
		memcpy(CalThickNess,so->CalThickNess,sizeof(CalThickNess));

		//CamRotMetix
		memcpy(CamRotMetix,so->CamRotMetix,sizeof(CamRotMetix));
		memcpy(CamInMecRotMetix,so->CamInMecRotMetix,sizeof(CamInMecRotMetix));
		memcpy(ImageRelative,so->ImageRelative,sizeof(ImageRelative));
		memcpy(ImageAbsolute,so->ImageAbsolute,sizeof(ImageAbsolute));
		
		memcpy(RotX,so->RotX,sizeof(RotX));
		memcpy(RotY,so->RotY,sizeof(RotY));

		memcpy(RotX_Adjust,so->RotX_Adjust,sizeof(RotX_Adjust));
		memcpy(RotY_Adjust,so->RotY_Adjust,sizeof(RotY_Adjust));

		memcpy(ProductById,so->ProductById,sizeof(ProductById));
		ProductMap = so->ProductMap;
		CurProductName =  so->CurProductName;

		iToolNum = so->iToolNum;
        for (i = 0; i < iLightTypeNum; i++)
        {
            strLightType[i] = so->strLightType[i];
        }
        stSocketParam = so->stSocketParam;
		memcpy(ImageSavePath,so->ImageSavePath,sizeof(ImageSavePath));
        //strcpy_s(ImageSavePath, 256, so->ImageSavePath);
    }
}SYSTEM_PARAM;

enum EN_TRANSITION_TYPE
{
    TRANSITION_TYPE_ALL = 0,
    TRANSITION_TYPE_POSITIVE,
    TRANSITION_TYPE_NEGATIVE,
};
enum EN_FIND_TYPE
{
    FINDTYPE_ALL = 0,
    FINDTYPE_FIRST,
    FINDTYPE_LAST,
};
enum EN_ALGORITHM_TYPE
{
    ALGORITHM_TYPE_REGRESSION = 0,
    ALGORITHM_TYPE_HUBER,
    ALGORITHM_TYPE_TUKEY,
    ALGORITHM_TYPE_GAUSS,
    ALGORITHM_TYPE_DROP,
};
enum EN_ALGORITHM_TYPE_CIRCLE
{
	ALGORITHM_TYPE_ALGEBRAIC = 0,
	ALGORITHM_TYPE_AHUBER,
	ALGORITHM_TYPE_ATUKEY,
	ALGORITHM_TYPE_GEOMETRIC,
	ALGORITHM_TYPE_GEOHUBER,
	ALGORITHM_TYPE_GEOTUKEY,
};
enum EN_DIRECTION_TYPE
{
    DIRECTION_TYPE_POSITIVE = 0,
    DIRECTION_TYPE_NEGATIVE,
};

typedef union _VIRSION_DATA_PKG_
{
    double dValue; // ָ��union��С��VC������
    int    iValue;

   _VIRSION_DATA_PKG_()
   {
     dValue = 0.0;
     iValue = 0;
   }

}VIRSION_DATA_PKG;

typedef struct _SCENE_DEVICE_INFO
{
    unsigned int CameraIndex;      //�������
    unsigned int LightIndex;       //��Դͨ������
    unsigned int CameraShutter;    //�������
    unsigned int CameraGain;    //�������

    _SCENE_DEVICE_INFO()
    {
        CameraIndex = 0;
        LightIndex = 0;
        CameraShutter = 20;
        CameraGain = 8;
    }
}SCENE_DEVICE_INFO;

typedef struct _INTERFACE_SET_DATA
{
	int nIndex;			 //���пؼ�����
	int CurSceneID;		 //����ID
	int iT;				 //�б��� ��� * 100
	string BtnType;      //��ť����
	string BtnName;		 //��ť����
	int ItemType;		 //Item����
	int ItemNumb;		 //Item���
	double InitValue;    //��ʼֵ
	double ChangeRate;   //�仯��
	int OptionNumb;		 //ѡ�������
	int height;		 //�ؼ���
	char OptionName[8][16];	//ѡ������

	_INTERFACE_SET_DATA()
	{
		CurSceneID = 0;
		nIndex = 0;
		iT = 0;
		BtnType = "";
		BtnName = "";
		ItemType = 0;
		ItemNumb = 0;
		InitValue = 0.0;
		ChangeRate = 0.0;
		OptionNumb = 0;
		height = 0;
		memset(OptionName, 0, 8 * 16);
    }
    void clear()
    {
        CurSceneID = 0;
        nIndex = 0;
        iT = 0;
        BtnType = "";
        BtnName = "";
        ItemType = 0;
        ItemNumb = 0;
        InitValue = 0.0;
        ChangeRate = 0.0;
        OptionNumb = 0;
        height = 0;
        memset(OptionName, 0, 8 * 16);
    }

	_INTERFACE_SET_DATA operator=(_INTERFACE_SET_DATA so)
	{
		CurSceneID = so.CurSceneID;
		nIndex = so.nIndex;
		iT = so.iT;
		BtnType = so.BtnType;
		BtnName = so.BtnName;
		ItemType = so.ItemType;
		ItemNumb = so.ItemNumb;
		InitValue = so.InitValue;
		ChangeRate = so.ChangeRate;
		OptionNumb = so.OptionNumb;
		height = so.height;
		memcpy(OptionName, so.OptionName, 8 * 16 );
		return *this;
	}
}INTERFACE_SET_DATA;

//��������С������ɣ� С����ǰ��������Ϊ ���ͼ���С֮�����С������
struct SCENE_PKG
{
    bool   bSave; //��ӵ�ϵͳ�������ý���                     //�Ƿ��������ת��  
    bool   bIsSimulate;     //�Ƿ�ʹ��ͼƬģ��
    char   szSceneName[SCENE_SIZE];         //������
    int  display;                       //�Ƿ���ʾ������Ϣ
    unsigned int  pkgBodySize;          //����������ܴ�С
    unsigned int  pkgCount;             //������
    unsigned int  imageCount;           //����ͼ�����
    unsigned int  ModelCount;           //����ͼ�����
    unsigned int  sceneID;             //����ID
    unsigned long halconID;               //UI����ID
    unsigned int  stationID;            //���ڹ���ʱ�жϰ��ĸ�����
	int ProcProcess;    //��������ķ�ʽ��0ΪC++��װ�Ĺ̶���ʽ��1Ϊ��halcon�������õ�ʵʱ��ʽ������������ù̶��ķ�ʽ�����Ƕ��������
    void *pReserve;                      //Ԥ��
    vector<SCENE_DEVICE_INFO>  vecInfo;          //������Ϣ�� ��ȡͼ��ʱ����豸������ ����Ҫ����������̴���
    vector<unsigned int *>     vecDataType;      //����, ����ͼ�����е���������  0Ϊint�� 1Ϊdouble�� ...
    vector<VIRSION_DATA_PKG *> vecBody;          //����, ���봦������е���������
                                      //������ ÿ��С�����һ��ֵΪ ���� ������ * 10000 + �㷨���� * 100 + ������
                                      //�ڶ���ֵΪ��С���е����ݸ���
	vector<INTERFACE_SET_DATA *> InterfaceSetData;
	unsigned int InterfaceDataNumb;		//�������ݸ���
	int MotorToolIndex;   //�ó�����Ӧ�ļоߣ���е�֣�����Ҫ�����ڱ궨����
	int CalPointNum;      //�ó�����ʹ�õı궨�ĵ������������������ת�Ƕȱ궨
	int CalPointIndex;    //�����Ѿ��궨�ĵ���������������е�궨������.
	int SceneType;   //ҵ������
	int AxisIndex;
	double defcamwidth;//�����������Ĭ�ϴ�С
	double defcamheight;
	int testcount;  //�������Դ���

    SCENE_PKG()
    {
        display = false;
        bIsSimulate = false;
        bSave = false;
        memset(szSceneName, 0, SCENE_SIZE);
        pkgBodySize = 0;
        pkgCount = 1;
        imageCount = 1;
        ModelCount = 0;
        sceneID = 0;
		ProcProcess = 0;
        halconID = 0;
        stationID = 0;
        //pReserve = NULL;
		InterfaceDataNumb = 0;
        vecInfo.reserve(1);
        vecDataType.reserve(0);
        vecBody.reserve(0);
		MotorToolIndex = 0;
		CalPointNum = 6;
	    CalPointIndex = 0;
		SceneType = -1;
		defcamwidth = 2552;
		defcamheight =1944;
		AxisIndex = -1;
		testcount = 0;

    }

    SCENE_PKG operator=(SCENE_PKG so)
    {
		ProcProcess = so.ProcProcess;
        display = so.display;
        bSave = so.bSave;
        bIsSimulate = so.bIsSimulate;
        memcpy(szSceneName, so.szSceneName, SCENE_SIZE);
        pkgBodySize = so.pkgBodySize;
        pkgCount = so.pkgCount;
        imageCount = so.imageCount;
        ModelCount = so.ModelCount;
        sceneID = so.sceneID;
        halconID = so.halconID;
        stationID = so.stationID;
       // pReserve = so.pReserve;
        vecInfo = so.vecInfo;
        vecDataType = so.vecDataType;
        vecBody = so.vecBody;
		InterfaceSetData = so.InterfaceSetData;
		InterfaceDataNumb = so.InterfaceDataNumb;
		MotorToolIndex =  so.MotorToolIndex;
		CalPointNum = so.CalPointNum;
		SceneType= so.SceneType;
		defcamwidth = so.defcamwidth;
		defcamheight =so.defcamheight;
		AxisIndex = so.AxisIndex;
		testcount = so.testcount;
        return *this;
    }
    SCENE_PKG *SceneCopy(SCENE_PKG *so)
    {
		ProcProcess = so->ProcProcess;
        display = so->display;
        bSave = so->bSave;
        bIsSimulate = so->bIsSimulate;
        memcpy(szSceneName, so->szSceneName, SCENE_SIZE);
        pkgBodySize = so->pkgBodySize;
        pkgCount = so->pkgCount;
        imageCount = so->imageCount;
        ModelCount = so->ModelCount;
        sceneID = so->sceneID;
        halconID = so->halconID;
        stationID = so->stationID;
        vecInfo = so->vecInfo;
        vecDataType = so->vecDataType;
        vecBody = so->vecBody;
        InterfaceSetData = so->InterfaceSetData;
        InterfaceDataNumb = so->InterfaceDataNumb;
		MotorToolIndex =  so->MotorToolIndex;
		CalPointNum = so->CalPointNum;
		SceneType= so->SceneType;
		defcamwidth = so->defcamwidth;
		defcamheight =so->defcamheight;
		AxisIndex = so->AxisIndex;
		testcount = so->testcount;
        return this;
    }
};

#define VIRSION_DATA_PKG_HEADER_SIZE      2    //С������ͷ��С



typedef enum _PROCESS_TYPE
{

	MAIN_PRO = 0,          //��������

	MODEL_PRO,              //ģ��(����)ƥ��
	THRE_PRO,              //��ֵ
	LINE_PRO,               //��������
	CIRCLE_PRO,            //��Բ����
	MORE_PRO,				//����ѡ��

	//�¼ӹ��� 
	IMAGE_READ,             //ͼ���ȡ
	IMAGE_ACQISITION,      //ͼ��ɼ� =7
	IMAGE_PREPROCESS,      //ͼ��Ԥ���� gray*a+b
	CONTOUR_POSITION,       //������λ
	COMBSHAPE_PRO,           //�����������
	LOGIC_CTR,             //�߼�����

	POSITION_EXCUTE,     // ��λ���� 
	HALPROC_POSITIONG,    //HalProc��λ
	HALPROC_PROCESS,    //HalProcͨ�ô���




}PROCESS_TYPE;


//������Ƶ�ͼ������
enum ShapeType
{
	HoNullShapeType = 0,//������
	HoLineType = 1,//������
	HoRecTangleType = 2,//����
	HoCircleType = 3,//Բ
	HoComposeType = 4//�������

};


//�������������������
enum RoiCombType
{
	NullCombine =0,
	RoiUnionType = 1,
	RoiSubType = 2,
	RoiInsectionType = 3,
};



typedef enum _ITEM_TYPE
{
    NORMARL_TYPE = 0,
    ID_TYPE,
    ROI_TYPE,
    THRE_TYPE,
    SPEC_TYPE,
}ITEM_TYPE;

typedef enum _ITEM_VALUE
{
    INT_TYPE = 0,
    DOUBLE_TYPE,
}ITEM_VALUE_TYPE;

typedef enum _MODEL_TYPE
{
    NORMAL_MODEL = 0,
    NCC_MODEL,
    MANUAL_MODEL,   //�ֻ�ģ������
};


//  
//extern  CHECK_RESULT g_SceneProcResult;





#endif 
