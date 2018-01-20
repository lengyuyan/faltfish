// UsbCamera.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;

#ifdef _DEBUG
#pragma comment(lib, "opencv_core2410d.lib") 
#pragma comment(lib, "opencv_highgui2410d.lib")
#pragma comment(lib, "opencv_imgproc2410d.lib") //ʹ����cvtColor
#else
#pragma comment(lib, "opencv_core2410.lib") 
#pragma comment(lib, "opencv_highgui2410.lib") 
#pragma comment(lib, "opencv_imgproc2410.lib") 
#endif

#define  CAMFULLNAME_LENTH_MAX  512
#define  ICAMERAEXPORT 1
#include "ICamera.h"


class  UsbCamera : public ICamera
{
public:
	UsbCamera(void);
	UsbCamera(char*camerName);
	~UsbCamera(void);

	void (*m_fcallback)(void* context); // �ص���������

	//��ʼ�����
	bool InitCam(char* CamerName);
	bool UnInitCam();

	int GetImageWidth();				// ��ȡͼƬ���
	int GetImageHeight();				// ��ȡͼƬ�߶�

	void SetWindowID();
	// ���ûص�����, Index-����ţ�pWnd-Ŀ�괰��ָ��
	void SetCallBack(void(*Callback)(void* context), void* pWnd);	 
	void SetBuffer(BYTE* pBuffer);		// ����ͼ�񻺳���

	bool SingleSnap(bool bDisplay = true);	// ��֡�ɼ� // Ҷ

	bool StartSnap();	// �����ɼ�
	bool StopSnap();	// ֹͣ�ɼ�	

	bool SetTriggerMode(bool Mode);		//���ô���ģʽ	
	bool SetExposureTime(int iValue);	//�����ع�ʱ��	
	bool SetGain(int iValue);			//��������

	int GetExposureTime();		//��ȡ�ع�ʱ��
	int GetGain();				//��ȡ����\

	int GetParameInfo(ICAMERA_PARAMETER speciPara,unsigned long &info);///AVT�еĲ���
	int SetParameInfo(ICAMERA_PARAMETER speciPara,int val);
	virtual char*  GetFullName();
	 static int GetGuidList(list<string> &guidList);         //��ȡGUID�б�;

public:
	//CUSBCamera m_Camera;    /////DLL�����
	cv::VideoCapture m_Camera;
	BYTE* m_pImageBuffer;	
	UINT32 m_iImageWidth;
	UINT32 m_iImageHeight;

	void* m_pParent;
	int m_iCamIdx;
	char m_CamerName[CAMFULLNAME_LENTH_MAX];
	bool m_bBusy;		// �����ɼ�

private:	
	//	USBNODEINFO m_NodeInfo[3];   /////DLL�����
	UINT32 m_iResult;	
	UINT32 m_iNodeCount;
	static bool m_b_USBIintFlag;  //ģ���ʼ����־:����ͬ�����͵����������ص�ģ���Ѿ�����
	bool       singleFlag;       //����ʵ����ʼ����ʶ

	HANDLE m_hThread;		// �ɼ��߳�
};

#if !defined(_WIN64)
// This pragma is required only for 32-bit builds. In a 64-bit environment,
// C functions are not decorated.
//#pragma comment(linker, "/export:CreateCamera=__imp__CreateCamera@0")
#endif  // _WIN64

//������ֲ���ת��
int CameraParaToUsbPara(int param)
{   //����-1��ʾ���������֧�����ֲ�������
	switch(param)
	{
	case ICAMERA_IMAGEFORMAT:                          // Compact image format
	case ICAMERA_ENUMIMAGEFORMAT:                         // Enumeration (Reset,Get)
			param = -1;                    
			break;
	case ICAMERA_BRIGHTNESS:                             // Set image brightness
		param = CV_CAP_PROP_BRIGHTNESS;
		break;

	case ICAMERA_AUTOEXPOSURE:                            // Set auto exposure
		param = CV_CAP_PROP_EXPOSURE;
		break;
	case ICAMERA_SHARPNESS:                                // Set image sharpness
		param =-1;
		break;
	case ICAMERA_WHITEBALCB:                              // Blue
		param =-1;
		break;
	case ICAMERA_WHITEBALCR:                              // Red
		break;
	case ICAMERA_HUE:                                     // Set image hue
		param = CV_CAP_PROP_HUE;
		break;
	case ICAMERA_SATURATION:                             // Set color saturation
		param = CV_CAP_PROP_SATURATION;
		break;
	case ICAMERA_GAMMA:                                   // Set gamma
		param =-1;
		break;
	case ICAMERA_SHUTTER:                                    // Shutter time
			param =-1;
			break;
	case ICAMERA_GAIN:                                       // Gain
			param = CV_CAP_PROP_GAIN;
			break;
		case ICAMERA_IRIS:                                       // Iris
			param =-1;
			break;
		case ICAMERA_FOCUS:                                      // Focus
			param =-1;
			break;
		case ICAMERA_TEMPERATURE:                                // Color temperature
			param =-1;
			break;
		case ICAMERA_TRIGGER:                                   // Trigger
			param =-1;
			break;
		case ICAMERA_TRIGGERDLY:                                 // Delay of trigger
			param =-1;
			break;
		case ICAMERA_WHITESHD:                                   // Whiteshade
			param =-1;
			break;
		case ICAMERA_FRAMERATE:                                // Frame rate
			param = CV_CAP_PROP_FPS;
			break;
		case ICAMERA_ZOOM:                                      // Zoom
			param =-1;
			break;
		case ICAMERA_PAN:                                       // Pan
			param =-1;
			break;
		case ICAMERA_TILT:                                       // Tilt
			param =-1;
			break;
		case ICAMERA_OPTICALFILTER:                             // Filter
			param =-1;
			break;
		case ICAMERA_CAPTURESIZE:                                // Size of capture
			param =-1;
			break;
		case ICAMERA_CAPTUREQUALITY:                            // Quality
			param =-1;
			break;
		case ICAMERA_PHYSPEED:                                 // Set speed for asy/iso
			param = CV_CAP_PROP_ISO_SPEED;
			break;
		case ICAMERA_XSIZE:                                     // Image XSize
			param = CV_CAP_PROP_FRAME_WIDTH;
			break;
		case ICAMERA_YSIZE:                                     // Image YSize
			param =CV_CAP_PROP_FRAME_HEIGHT;
			break;
		case ICAMERA_XPOSITION:                                  // Image x position
			param =-1;
			break;
		case ICAMERA_YPOSITION:                                 // Image y position
			param =-1;
			break;
		case ICAMERA_PACKETSIZE:                                 // Packet size
			param =-1;
			break;
		case ICAMERA_DMAMODE:                                  // DMA mode (continuous or limp)
			param =-1;
			break;
		case ICAMERA_BURSTCOUNT:                                // Number of images to produce
			param =-1;
			break;
		case ICAMERA_FRAMEBUFFERCOUNT:                          // Number of frame buffers
			param =-1;
			break;
		case ICAMERA_USEIRMFORBW:                               // Allocate bandwidth or not (IsoRscMgr)
			param =-1;
			break;
		case ICAMERA_ADJUSTPARAMETERS:                           // Adjust parameters or fail
			param =-1;
			break;
		case ICAMERA_STARTIMMEDIATELY:                          // Start bursting immediately
			param =-1;
			break;
		case ICAMERA_FRAMEMEMORYSIZE:                            // Read only: Frame buffer size
			param =-1;
			break;
		case ICAMERA_COLORFORMAT:                                // Read only: Colorformat
			param =-1;
			break;
		case ICAMERA_IRMFREEBW:                                  // Read only: Free iso bytes for 400MBit
			param =-1;
			break;
		case ICAMERA_DO_FASTTRIGGER:                             // Fast trigger (no ACK)
			param =-1;
			break;
		case ICAMERA_DO_BUSTRIGGER:                              // Broadcast trigger
			param =-1;
			break;
		case ICAMERA_RESIZE:                                     // Start/Stop resizing
			param =-1;
			break;
		case ICAMERA_USEIRMFORCHN:                               // Get channel over isochronous resource manager
			param =-1;
			break;
		case ICAMERA_CAMACCEPTDELAY:                             // Delay after writing values
			param =-1;
			break;
		case ICAMERA_ISOCHANNEL:                                 // Iso channel
			param =-1;
			break;
		case ICAMERA_CYCLETIME:                                  // Read cycle time
			param =-1;
			break;
		case ICAMERA_DORESET:                                    // Reset Camera
			param =-1;
			break;
		case ICAMERA_DMAFLAGS:                                   // Flags for ISO DMA
			param =-1;
			break;
		case ICAMERA_R0C:                                      // Ring 0 call gate
			param =-1;
			break;
		case ICAMERA_BUSADDRESS:                                 // Exact bus address
			param =-1;
			break;
		case ICAMERA_CMDTIMEOUT:                                 // Global bus command timeout
			param =-1;
			break;
		case ICAMERA_CARD:                                      // Card number of this Camera (set before connect)
			param =-1;
			break;
		case ICAMERA_LICENSEINFO:                                // Query license information
			param =-1;
			break;
		case ICAMERA_PACKETCOUNT:                               // Read only: Packet count
			param =-1;
			break;
		case ICAMERA_DO_MULTIBUSTRIGGER:                        // Do trigger on several busses
			param =-1;
			break;
		case ICAMERA_CARDRESET:                                  // Do reset on card (for hard errors)
			param =-1;
			break;
		default :
			param = -1;
			break;
	}
	return param;
}

//�½����ʵ��
EXTERN_C  ICAMERASPEC  /*ICAMERASPEC88*/ CameraHANDLE WINAPI CreateCamera(char*camerName)
{
	return new UsbCamera(camerName);
}
//ɾ�����ʵ��
EXTERN_C  ICAMERASPEC  void WINAPI ReleaseCamera(CameraHANDLE handle)
{

	UsbCamera * pCamer = (UsbCamera*)handle;
	if (pCamer)
	{
		delete pCamer;
		pCamer = NULL;
	}
}

EXTERN_C  ICAMERASPEC  int WINAPI getList(list<string> &guidList,string &type)
{
	return UsbCamera::GetGuidList(guidList);
}

////UsbCameraʵ��
bool UsbCamera::m_b_USBIintFlag = false;
// �����ɼ��̺߳���
 DWORD WINAPI ContinousGrab(LPVOID pParam)
{ 

	Mat   Frame;
	UINT32    error;

	UsbCamera *pOwner = (UsbCamera *)pParam;
	pOwner->m_bBusy = TRUE;

	if (!pOwner->m_Camera.isOpened()) //�ȼ���ǲ��ǿ���
	{
		pOwner->m_Camera.open(pOwner->m_iCamIdx);      //�����
		if (!pOwner->m_Camera.isOpened())
		{
			return false;
		}

	}


	while(pOwner->m_bBusy) 
	{	
		try
		{

			Mat Frame,FrameGray;
			error = pOwner->m_Camera.read(Frame);
			if (error != 0)//��ȡ�ɹ�
			{

				cvtColor( Frame, FrameGray, CV_RGB2GRAY );
				IplImage pImg= IplImage(FrameGray); ///תΪͼ������

				if (pImg.imageData)
				{
					memcpy(pOwner->m_pImageBuffer, pImg.imageData, pOwner->m_iImageWidth * pOwner->m_iImageHeight);
					if (pOwner->m_fcallback != NULL)
					{
						pOwner->m_fcallback(pOwner->m_pParent);
					}
				}


			}
			else 
			{

				Sleep(10);

			}
		}
		catch (...)
		{
			pOwner->m_Camera.release();
		}
	} 

	pOwner->m_Camera.release();

	return 0;
}

// ���졢��������
UsbCamera::UsbCamera(void)
{
	singleFlag = false;
	m_iResult = 0;
	m_iNodeCount = 0;
	m_bBusy = false;
	m_iCamIdx = -1;
	m_pImageBuffer = NULL;
	m_fcallback = NULL;
	m_pParent = NULL;
}
UsbCamera::UsbCamera(char*camerName)
{
	singleFlag = false;
	m_iResult = 0;
	m_iNodeCount = 0;
	m_bBusy = false;
	m_iCamIdx = -1;
	m_pImageBuffer = NULL;
	m_fcallback = NULL;
	m_pParent = NULL;
	memset(m_CamerName,0,CAMFULLNAME_LENTH_MAX);
	strcpy(m_CamerName,camerName);
}


UsbCamera::~UsbCamera(void)
{

	if (m_Camera.isOpened())
	{
		m_Camera.release();
	}
	if (m_pImageBuffer != NULL)
	{
		//delete m_pImageBuffer;
		m_pImageBuffer = NULL;
	}


}

// ��ʼ�����,����������ʵ���Բ�Ҫ��ʼ������������������������ò��������Ӳ��Ǵ�
bool UsbCamera::InitCam(char* CamerName)
{

	if (true == singleFlag)
	{
		return true;
	}
	else
	{
		singleFlag = true;
	}
	unsigned int Index = atoi(m_CamerName);
	m_Camera.open(Index);
	if (!m_Camera.isOpened())
	{

		return false;
		
		
	}
	m_iCamIdx = Index; //��������

	///����ѹ����ʽ,����û�����壬ֻ��ռλ
	
	double ParmInfo = 0.0;
	/*ParmInfo = m_Camera.get(CV_CAP_PROP_FORMAT);//��ʽ
	m_Camera.set(CV_CAP_PROP_FORMAT,ParmInfo);
	ParmInfo = m_Camera.get(CV_CAP_PROP_FPS);///֡��
	m_Camera.set(CV_CAP_PROP_FPS,ParmInfo);
	*/
	ParmInfo = m_Camera.get(CV_CAP_PROP_FRAME_WIDTH);
	m_iImageWidth =(unsigned long) ParmInfo;

	ParmInfo = m_Camera.get(CV_CAP_PROP_FRAME_HEIGHT);
	m_iImageHeight = (unsigned long)ParmInfo;
	/*
	ParmInfo = m_Camera.get(CV_CAP_PROP_EXPOSURE);
	m_Camera.set(CV_CAP_PROP_EXPOSURE,ParmInfo);//�ع�ʱ��
	*/
	//m_Camera.release();
	return true;


}

bool UsbCamera::UnInitCam()
{
	m_Camera.release();//�ر����
	singleFlag = false;
	return true;
}

int UsbCamera::GetImageWidth()
{
	return m_iImageWidth;
}

int UsbCamera::GetImageHeight()
{
	return m_iImageHeight;
}

void UsbCamera::SetCallBack(void(*Callback)(void* context), void* pWnd)
{
	m_fcallback = Callback;
	m_pParent = pWnd;
}

void UsbCamera::SetBuffer(BYTE* pBuffer)
{
	m_pImageBuffer = pBuffer; ///ͼ�񻺴���
}

// ��֡�ɼ�
bool UsbCamera::SingleSnap(bool bDisplay)
{
	if (m_bBusy)
	{
		m_bBusy = false;
		Sleep(100);
	}
	bool bRet = false;

 
	if (m_iCamIdx >= 0)
	{
    //		LOG_INFO("StartDevice_s"); //�����Ȳ���ӡ��־��������Ϊusb���һֱ�ǿ���״̬���������������һ�����ӽӴ����״̬
	//	m_iResult = m_Camera.OpenCapture();
	//	m_iResult = m_Camera.StartDevice();
		
		//LOG_INFO("StartDevice_e");
		if (!m_Camera.isOpened())
		{
			m_Camera.open(m_iCamIdx);      //�����
			if (!m_Camera.isOpened())
			{
				return false;
			}

		}
		Mat Frame,FrameGray;
		m_iResult = m_Camera.read(Frame);
		if (m_iResult != 0)//��ȡ�ɹ�
		{

			cvtColor( Frame, FrameGray, CV_RGB2GRAY );
			IplImage pImg= IplImage(FrameGray); ///תΪͼ������
		
			if (pImg.imageData)
			{
				memcpy(m_pImageBuffer, pImg.imageData, m_iImageWidth * m_iImageHeight);
				bRet = true;
			}
		}
		else
		{
		}
	    m_Camera.release();
		return bRet;
	}
	else
	{
		return false;
	}
}

// �����ɼ�
bool UsbCamera::StartSnap()
{
	if(m_bBusy)
	{
		return false;
	}

	// �����ɼ��߳�
	m_hThread = CreateThread(NULL, 0,ContinousGrab, this, 0, NULL);	
	if (FAILED(m_hThread))
	{
		return false;
	}
	return true;
}

// ֹͣ�����ɼ�
bool UsbCamera::StopSnap()
{
	m_bBusy = false;
	Sleep(200);
	return true;
}

// ���ô���ģʽ
bool UsbCamera::SetTriggerMode(bool Mode) ///��֧�ֳ���ģʽ����
{	
	UINT32 nOn;	// 0 = ext. trigger off, 1 = ext. trigger on
	if (Mode){
		nOn = 1;
	}
	else{
		nOn=0;
	}

	UINT32 nPolarity = 0;	// 0 = low active input, 1 = high active input
	UINT32 nMode	 = 0;	// 0 = edge mode, 1 = level mode, 15 = bulk mode
	//UINT32 TriggerValue = MAKETRIGGER(nOn, nPolarity, 1, nMode, 0);
	//m_Camera.SetParameter(USB_TRIGGER, TriggerValue);

	return true;
}

// �����ع�ʱ��
bool UsbCamera::SetExposureTime(int iValue)
{
	m_Camera.set(CV_CAP_PROP_EXPOSURE, iValue);
	return true;
}

// ��������
bool UsbCamera::SetGain(int iValue)
{
	m_Camera.set(CV_CAP_PROP_GAIN, iValue);
	return true;
}

// ��ȡ��ǰ�ع�ʱ��
int UsbCamera::GetExposureTime()
{
	UINT32 iValue = 0;
	iValue = m_Camera.get(CV_CAP_PROP_EXPOSURE);
	return iValue;
}

// ��ȡ��ǰ����
int UsbCamera::GetGain(void)
{
	UINT32 iValue = 0;
	iValue = m_Camera.get(CV_CAP_PROP_GAIN);
	return iValue;
}

//��ȡGUID�б�;������û�������������0����ʾ�ɹ�
int UsbCamera::GetGuidList(list<string> &guidList) 
{
	int result = 0;
	UINT32 nodeCount = 0;
	list<string> guidl;
	int i =0;

	if (!m_b_USBIintFlag)
	{
		m_b_USBIintFlag = true;
	}	
	
	cv::VideoCapture temp_camera;
	int maxTested = 10;                   //�������10������ͷ
	for (i = 0; i < maxTested; i++)
	{
		cv::VideoCapture temp_camera(i);
		bool res = (!temp_camera.isOpened());
		temp_camera.release();
		if (res)
		{
			guidList = guidl;	
			return 0;
		}
		char buf[20] = {};
		_itoa_s(i,buf,10);
		guidl.push_back(buf);
	}
	guidList = guidl;	
	 
	return i;
}

////���ò���ö�����ͱ������ض����ӳ�䣬��Ҫ�ı��Ͷ�Ϊ1���ض������Ϊ2��Ҫӳ��
int UsbCamera::GetParameInfo(ICAMERA_PARAMETER speciPara,unsigned long &info) 
{
	int usbspeciPara = CameraParaToUsbPara(speciPara);
	int result = 0;
	result = (int )m_Camera.get(usbspeciPara);
	return result;
}
 int UsbCamera::SetParameInfo(ICAMERA_PARAMETER speciPara,int val)
{
	int usbspeciPara = CameraParaToUsbPara(speciPara);
	int result = 0;
	result = m_Camera.set(usbspeciPara,(double)val);
	return result;
}
 char*  UsbCamera::GetFullName()
 {
	 return m_CamerName;
 }





