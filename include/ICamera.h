// Icamera.cpp : ���� DLL Ӧ�ó���ĵ���������
//
#ifndef ICamera_H
#define ICamera_H

/* Compile the library and export our functions */
#ifdef ICAMERAEXPORT
#define ICAMERASPEC  __declspec(dllexport) //__stdcall extern "C"
#else
#define ICAMERASPEC  __declspec(dllimport) //__stdcall extern "C"
#endif


#include <list>
using namespace std;

// Parameters
enum ICAMERA_PARAMETER
{
	ICAMERA_IMAGEFORMAT=0,                            // Compact image format
	ICAMERA_ENUMIMAGEFORMAT,                          // Enumeration (Reset,Get)
	ICAMERA_BRIGHTNESS,                               // Set image brightness
	ICAMERA_AUTOEXPOSURE,                             // Set auto exposure
	ICAMERA_SHARPNESS,                                // Set image sharpness
	ICAMERA_WHITEBALCB,                               // Blue
	ICAMERA_WHITEBALCR,                               // Red
	ICAMERA_HUE,                                      // Set image hue
	ICAMERA_SATURATION,                               // Set color saturation
	ICAMERA_GAMMA,                                    // Set gamma
	ICAMERA_SHUTTER,                                  // Shutter time
	ICAMERA_GAIN,                                     // Gain
	ICAMERA_IRIS,                                     // Iris
	ICAMERA_FOCUS,                                    // Focus
	ICAMERA_TEMPERATURE,                              // Color temperature
	ICAMERA_TRIGGER,                                  // Trigger
	ICAMERA_TRIGGERDLY,                               // Delay of trigger
	ICAMERA_WHITESHD,                                 // Whiteshade
	ICAMERA_FRAMERATE,                                // Frame rate
	ICAMERA_ZOOM,                                     // Zoom
	ICAMERA_PAN,                                      // Pan
	ICAMERA_TILT,                                     // Tilt
	ICAMERA_OPTICALFILTER,                            // Filter
	ICAMERA_CAPTURESIZE,                              // Size of capture
	ICAMERA_CAPTUREQUALITY,                           // Quality
	ICAMERA_PHYSPEED,                                 // Set speed for asy/iso
	ICAMERA_XSIZE,                                    // Image XSize
	ICAMERA_YSIZE,                                    // Image YSize
	ICAMERA_XPOSITION,                                // Image x position
	ICAMERA_YPOSITION,                                // Image y position
	ICAMERA_PACKETSIZE,                               // Packet size
	ICAMERA_DMAMODE,                                  // DMA mode (continuous or limp)
	ICAMERA_BURSTCOUNT,                               // Number of images to produce
	ICAMERA_FRAMEBUFFERCOUNT,                         // Number of frame buffers
	ICAMERA_USEIRMFORBW,                              // Allocate bandwidth or not (IsoRscMgr)
	ICAMERA_ADJUSTPARAMETERS,                         // Adjust parameters or fail
	ICAMERA_STARTIMMEDIATELY,                         // Start bursting immediately
	ICAMERA_FRAMEMEMORYSIZE,                          // Read only: Frame buffer size
	ICAMERA_COLORFORMAT,                              // Read only: Colorformat
	ICAMERA_IRMFREEBW,                                // Read only: Free iso bytes for 400MBit
	ICAMERA_DO_FASTTRIGGER,                           // Fast trigger (no ACK)
	ICAMERA_DO_BUSTRIGGER,                            // Broadcast trigger
	ICAMERA_RESIZE,                                   // Start/Stop resizing
	ICAMERA_USEIRMFORCHN,                             // Get channel over isochronous resource manager
	ICAMERA_CAMACCEPTDELAY,                           // Delay after writing values
	ICAMERA_ISOCHANNEL,                               // Iso channel
	ICAMERA_CYCLETIME,                                // Read cycle time
	ICAMERA_DORESET,                                  // Reset Camera
	ICAMERA_DMAFLAGS,                                 // Flags for ISO DMA
	ICAMERA_R0C,                                      // Ring 0 call gate
	ICAMERA_BUSADDRESS,                               // Exact bus address
	ICAMERA_CMDTIMEOUT,                               // Global bus command timeout
	ICAMERA_CARD,                                     // Card number of this Camera (set before connect)
	ICAMERA_LICENSEINFO,                              // Query license information
	ICAMERA_PACKETCOUNT,                              // Read only: Packet count
	ICAMERA_DO_MULTIBUSTRIGGER,                       // Do trigger on several busses
	ICAMERA_CARDRESET,                                // Do reset on card (for hard errors)

	ICAMERA_LAST
};

///����com like��������ӿ�
#ifdef __cplusplus
struct ICamera
{
public:
	//ICamera(void);
	//virtual  ~ICamera(void);

	void (*m_fcallback)(void* context); // �ص���������

	//��ʼ�����
	virtual bool InitCam(char* CamerName) = 0;
	virtual bool UnInitCam() = 0;

	virtual int GetImageWidth()= 0;				// ��ȡͼƬ���
	virtual int GetImageHeight() = 0;				// ��ȡͼƬ�߶�

	//virtual void SetWindowID() = 0;
	// ���ûص�����, Index-����ţ�pWnd-Ŀ�괰��ָ��
	virtual void SetCallBack(void(*Callback)(void* context), void* pWnd) = 0;	 
	virtual void SetBuffer(BYTE* pBuffer) = 0;		// ����ͼ�񻺳���

	virtual bool SingleSnap(bool bDisplay = true) = 0;	// ��֡�ɼ� // Ҷ

	virtual bool StartSnap() = 0;	// �����ɼ�
	virtual bool StopSnap() = 0;	// ֹͣ�ɼ�	

	virtual bool SetTriggerMode(bool Mode) = 0;		//���ô���ģʽ	
	virtual bool SetExposureTime(int iValue) = 0;	//�����ع�ʱ��	
	virtual bool SetGain(int iValue) = 0;			//��������

	virtual int GetExposureTime() = 0;		//��ȡ�ع�ʱ��
	virtual int GetGain() = 0;				//��ȡ����
	virtual char* GetFullName() = 0;

 	virtual int GetParameInfo(ICAMERA_PARAMETER speciPara,unsigned long &info) = 0;///AVT�еĲ���
	virtual int SetParameInfo(ICAMERA_PARAMETER speciPara,int val) = 0;

	static int GetGuidList(list<string> &guidList,string &type);         //��ȡGUID�б�;

private:
	BYTE* m_pImageBuffer;	
	UINT32 m_iImageWidth;
	UINT32 m_iImageHeight;

	void* m_pParent;
	int m_iCamIdx;
	bool m_bBusy;		// �����ɼ�

private:	
	UINT32 m_iResult;	
	UINT32 m_iNodeCount;
	static bool m_b_ICAMERAIintFlag;   //��ʼ����־

	HANDLE m_hThread;		// �ɼ��߳�
};
typedef ICamera* CameraHANDLE;
#else 
typedef struct tagCameraHANDLE {} * CameraHANDLE;///��ʱ��ʵ��
#endif

//
#ifdef __cplusplus
#   define EXTERN_C     extern "C"
#else
#   define EXTERN_C
#endif // __cplusplus


// Factory function that creates instances of the Xyz object.
EXTERN_C ICAMERASPEC CameraHANDLE WINAPI CreateCamera(char*camerName);

EXTERN_C   ICAMERASPEC void WINAPI ReleaseCamera(CameraHANDLE handle);

EXTERN_C  ICAMERASPEC int WINAPI getList(list<string> &guidList,string &type);     


#endif
