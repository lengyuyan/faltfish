// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� BASLERGIGECAMERA_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// BASLERGIGECAMERA_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
// 



#ifdef BASLERGIGECAMERA_EXPORTS
#define BASLERGIGECAMERA_API __declspec(dllexport)
#else
#define BASLERGIGECAMERA_API __declspec(dllimport)
#endif

#pragma once
#include "stdafx.h"

#define  ICAMERAEXPORT 1
#include "ICamera.h"

#include <pylon/PylonIncludes.h>
#ifdef PYLON_WIN_BUILD
#include <pylon/PylonGUI.h>
#endif

#include <pylon/gige/BaslerGigEInstantCamera.h>
typedef Pylon::CBaslerGigEInstantCamera Camera_t;
using namespace Basler_GigECameraParams;


#define  CAMFULLNAME_LENTH_MAX  512
#define  FILENAME_LENTH_MAX  512

using namespace Pylon;




#ifdef BASLERGIGECAMERA_EXPORTS
#define BASLERGIGECAMERA_API __declspec(dllexport)
#else
#define BASLERGIGECAMERA_API __declspec(dllimport)
#endif

// �����Ǵ� BaslerGigeCamera.dll ������
class BASLERGIGECAMERA_API CBaslerGigeCamera:public ICamera {
public:
	CBaslerGigeCamera(void);
	CBaslerGigeCamera(char*camerName);
	// TODO: �ڴ�������ķ�����
	~CBaslerGigeCamera();

public:
	
#define MSG_DISP_IMAGE  1124    

	enum SynMode
	{
		NoMode = 0 ,		
		SynchroGrab =1 ,
		AsynchroGrab = 2

	};
public:  //������

	virtual bool InitCam(char* CamerName) ;//��ʼ�����
	virtual bool UnInitCam() ;

	virtual int GetImageWidth();				// ��ȡͼƬ���
	virtual int GetImageHeight() ;				// ��ȡͼƬ�߶�

	//virtual void SetWindowID() = 0;
	// ���ûص�����, Index-����ţ�pWnd-Ŀ�괰��ָ��
	virtual void SetCallBack(void(*Callback)(void* context), void* pWnd) ;	 
	virtual void SetBuffer(BYTE* pBuffer) ;		// ����ͼ�񻺳���

	virtual bool SingleSnap(bool bDisplay = true) ;	// ��֡�ɼ� // ʹ�õ���ͬ���ɼ�����ʹ�ûص�����

	virtual bool StartSnap() ;	// �����ɼ�
	virtual bool StopSnap() ;	// ֹͣ�ɼ�	

	virtual bool SetTriggerMode(bool Mode);		//���ô���ģʽ	
	virtual bool SetExposureTime(int iValue) ;	//�����ع�ʱ��	
	virtual bool SetGain(int iValue) ;			//��������

	virtual int GetExposureTime() ;		//��ȡ�ع�ʱ��
	virtual int GetGain() ;				//��ȡ����
	virtual char* GetFullName();

	virtual int GetParameInfo(ICAMERA_PARAMETER speciPara,unsigned long &info);///AVT�еĲ���
	virtual int SetParameInfo(ICAMERA_PARAMETER speciPara,int val);





public:
	//��ʼ����������
	static void IniTrlFactory();

	//�������ͬ���첽ģʽ
	void SetSynchroMode(SynMode mode);

	//�����ҵ���������������õ���Ӧ������û���������
	char* GetDeviceUserName(int FindCamIndex);	

	//����������������õ���Ӧ�������������
	char* GetDeviceTypeStr(int FindCamIndex);
	//��������������õ�������������к�
	char* GetDeviceSerialNo(int FindCamIndex);
	//��������������õ���������Ѻ�����
	char* GetDeviceFriendlyName(int FindCamIndex);
	//�����ҵ����������������ȡ���ȫ��
	//�˺��������Ի�ȡ���������
	static char* GetCamFullName(int& FindCamIndex);

	//��ȡ�������
	static int GetCamCount();

	//�������ȫ���ƣ��õ���Ӧ�������
	int  GetDeviceIndexByFullName(char* DeviceFullName);

	//����������������õ���Ӧ���������������Ϣ
	CDeviceInfo GetDeviceType(int FindCamIndex);

	//����������������趨�û���������

	//����������������������������
	BOOL OpenModeCam(int CamIndex,SynMode Mode,EGrabStrategy strategy);
	//�ر����
	BOOL CloseBaslerCam();


	//��ȡ�����ǰ�ع�ʱ��
	double GetBaslerExposureTime();
	//��ȡ����ع�ʱ��
	double GetMaxExposureTime();
	//��ȡ��С�ع�ʱ��
	double GetMinExposureTime();
	//��������ع�ʱ��,��λ΢��
	BOOL SetBaslerExposureTime(UINT MilSeconds);

	//��ȡ�����ǰ����
	double GetCamGain();
	//��ȡ����������
	double GetMaxGain();
	//��ȡ�����С����
	double GetMinGain();
	//�����������
	BOOL SetCamGain(double GainValue);

	//��ȡͼ���趨�ĵ�ǰ���
	int64_t GetWidth();
	//�趨ͼ����
	BOOL SetWidth(int64_t width);
	//��ȡͼ���趨�����
	int64_t GetMaxWidth();
	//��ȡͼ���趨��С���
	int64_t GetMinWidth();

	//��ȡͼ���趨�ĵ�ǰ�߶�
	int64_t GetHeight();

	//�趨ͼ����
	BOOL SetHeight(int64_t width);
	//��ȡͼ���趨���߶�
	int64_t GetMaxHeight();
	//��ȡͼ���趨��С�߶�
	int64_t GetMinHeight();

	//��ȡͼ��Xƫ��
	int64_t GetOffsetX();
	//��ȡX���ƫ��
	int64_t GetMaxOffsetX();
	//��ȡX��Сƫ��
	int64_t GetMinOffsetX();
	//�趨ͼ��Xƫ��
	BOOL SetOffsetX(int64_t offsetX);

	//��ȡͼ��Yƫ��
	int64_t GetOffsetY();
	//��ȡY���ƫ��
	int64_t GetMaxOffsetY();
	//��ȡY��Сƫ��
	int64_t GetMinOffsetY();
	//�趨ͼ��Yƫ��
	BOOL SetOffsetY(int64_t offsetY);

	//��ȡ�������������
	int64_t GetCamParamNum();

	// ��ȡ�����ӳ� ****��Ч����***
	double GetTriggerDelay();
	//�趨�����ӳ�  ****��Ч����***
	BOOL SetTriggerDelay(double DelayValue);

	//���������ǰ�Ĳ������ļ�
	BOOL SaveParamToFile(char* FileFullName);
	//���ļ��ж�ȡ���������
	BOOL LoadParamFromFile(char*  FileFullName);


	//�ж�����Ƿ��
	BOOL IsCamOpen();

	//�趨ͼ��ץȡ����
	void SetGrabStrategy(EGrabStrategy strategy); //GrabStrategy_OneByOne   c_countOfImagesToGrab   GrabStrategy_LatestImageOnly
	// ��ȡץȡ����
	EGrabStrategy GetGrabStrategy();
	
	//ͬ����ʽ׼��ץȡ
	BOOL BeginGrab(); 
	//����ͬ����ʽץȡ����ͼ��
	BOOL BaslerCamGrab();

	//׼���ûص��ķ�ʽ׼��ץȡ�����������ص�������Ҫʵ�֣���һ���ص������ݸ�ʽת����BYTE*������SetBuffer�Ĳ�����
	//typedef void (*pCallbackGenBuffer)(BYTE* pBffer, CGrabResultPtr m_ptrGrabResult);  //ת��ͼ������m_ptrGrabResult����ΪBYTE*,Ϊ������
	                                                    //�̳е�void SetBuffer(BYTE* pBuffer)�麯����
	typedef void (*pCallbackFunc)(void* pCamera);  //���������ָ�봫�룬��ʵ�ֺ���ʱ���õ���Ա����ͼ������ָ�룬������ͼ�����m_pBufferָ��Ķ���


	BOOL BeginAsynchGrab();//(void* pGrabParent, pCallbackFunc );

	//���ûص��ķ�ʽץȡͼ��
	BOOL AsynchGrab();


	//�����ص���ʽ��ץȡ�̣߳�
	void StopAsynchGrab();

	//static int GetMaxCamNum(void);
	static int GetUnOpenIndex(void);

	//����������������������������
	BOOL CreateOpenBaslerCam(int  CamIndex,EGrabStrategy strategy = GrabStrategy_OneByOne);

	//����������������������������
	BOOL OpenBaslerCam(int  CamIndex,EGrabStrategy strategy = GrabStrategy_OneByOne);
	//�ر����
protected:
	//�жϸ����������Ƿ�Ϸ�
	BOOL IsAvailabCamIndex(int CamIndex);

	

private:
    static void GetDeviceInfoList(DeviceInfoList_t& DeviceList);
	//GetDeviceInfoList();


public:  //�ɼ�ͼ���Ĵ���
	//��ȡ�ɼ���ͼ����
	long GetImageAcqWidth();
	//��ȡ�ɼ���ͼ��߶�
	long GetImageAcqHeight();

public:
	static BOOL  IsIniTrlFactory;//�Ƿ��ʼ����
	static int  CameraNum;//�Ƿ��ʼ����
	static DeviceInfoList_t EnumDeviceList; //������Ϣ������ÿ�ε������л�EnumerateDevices�Ľ���ǲ�һ����,
	BOOL  IsOpenCam;  // ����Ƿ��
	BOOL IsReadyGrab; // ׼��ץͼ
	BOOL m_IsGrab;   //ͼ���Ƿ�ɼ��ɹ�
	BOOL m_StopGrab; // ȷ���Ƿ�ֹͣ�ɼ�

	EGrabStrategy m_GrabStrategy; //ͼ��ɼ����� GrabStrategy_OneByOne   c_countOfImagesToGrab   GrabStrategy_LatestImageOnly
	BOOL  m_IsContinuGrab;   // �Ƿ������ɼ��������ɼ�ʱ���ܿ��ƣ�һֱ�ɼ����������GrabStrategy_OneByOne��
	
	
	//�ɼ�����ͼ�������ָ�룬��������ͼ��;
	CGrabResultPtr m_ptrGrabResult;
	BYTE* m_pBuffer;

	int  m_CamIndex;  //��ǰ�򿪵�������
	char m_CamerName[CAMFULLNAME_LENTH_MAX];
    

	CInstantCamera camera;  //  CInstantCamera  //Camera_t
	BOOL IsBeginGrab;
	IPylonDevice* m_pDevice;
	
	SynMode m_SynchroMode;  //ͼ��ɼ���ͬ���첽ģʽ

	static HANDLE Basler_Mutex;
	
	//static int OpendIndex;


	static bool m_CamOpenedMap[64];


public:
	HANDLE     m_hGrabThreadRun;	//
	//pCallbackGenBuffer m_pGenBuffer;
	pCallbackFunc m_pCallBack;
	void* m_pGrabParent;



	static DWORD WINAPI BalerCamGetDevice(void* pThis);

	//
	static DWORD WINAPI GrabThread(void* pThis);







};

//Basler�����ʹ��ǰ��Ҫ���������ʹ�ÿ�ʱ������á�
void BaslercamStartUp(void); 
//Basler�����ж����Ҫ���������
void BaslercamRelease(void); 


extern BASLERGIGECAMERA_API int nBaslerGigeCamera;

BASLERGIGECAMERA_API int fnBaslerGigeCamera(void);

