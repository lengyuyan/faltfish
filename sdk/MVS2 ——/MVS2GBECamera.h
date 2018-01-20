/********************************************************
//��������GBE���
// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� MVS2GBECAMERA_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// BASLERGIGECAMERA_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
*********************************************************/
#include "stdafx.h"


#include "ICamera.h"
/*
��ֹ�ظ�����ͷ�ļ�����ظ����壬����ΰ���ֻ�ᶨ��һ��
#ifndef MVS2GBECAMERA_H
#define MVS2GBECAMERA_H
#endif
*/
#ifndef MVS2GBECAMERA_H
#define MVS2GBECAMERA_H


#ifdef  MVS2GBECAMERA_EXPORTS        
#define ICAMERASPEC __declspec(dllexport)
#else
#define ICAMERASPEC __declspec(dllimport)
#endif

#define  CAMFULLNAME_LENTH_MAX  512
#define  FILENAME_LENTH_MAX  512

#include "MvCameraControl.h"


// �����Ǵ� MVS2GBECamrea.dll ������
class ICAMERASPEC MVS2GBECamera : public ICamera 
{
//����
	public:
		typedef void (*fcallback)(void* context);
		fcallback m_fcallback;   //�ص�����
		static bool m_isInitFlag; //�Ƿ��ʼ��
		bool m_iscontinueGrab;  //�Ƿ�������ͼ
		bool m_isstopGrab;        //�Ƿ�ֹͣ��ͼ
		int  m_CamIndex;  //��ǰ�򿪵�������
		char m_CamerName[CAMFULLNAME_LENTH_MAX];
		/*
		MV_ACQ_MODE_SINGLE        ��֡ģʽ
		MV_ACQ_MODE_MUTLI         ��֡ģʽ
		MV_ACQ_MODE_CONTINUOUS �����ɼ�ģʽ
		*/
		MV_CAM_ACQUISITION_MODE m_GrabStrategy; //ͼ��ɼ�����
		
		void* m_handle;            //�豸���

		static MV_CC_DEVICE_INFO_LIST EnumDeviceList;   //�豸��Ϣ�б�

		MV_CC_DEVICE_INFO m_DeviceInfo;//�豸��Ϣ
		MVCC_INTVALUE ImageWidth;    //ͼƬ���
		MVCC_INTVALUE ImageHeight;   //ͼƬ�߶�

		typedef struct 
		{
			MVCC_ENUMVALUE Mode;    //����ģʽ
			MVCC_FLOATVALUE Gain;   //����
		}CAMERAGAIN;
		CAMERAGAIN CameraGain; 

		typedef struct 
		{
			MVCC_ENUMVALUE Mode;   //�ع�ģʽ
			MVCC_FLOATVALUE  Time; //�ع�ʱ��
		}CAMERAEXPOSURETIME;
		CAMERAEXPOSURETIME CameraExposureTime;     //����ع�ʱ��    

		MV_FRAME_OUT_INFO_EX* pFrameInfo;//ͼ�����ݵ�֡��Ϣ
		BYTE* m_pBuffer; //�ɼ�����ͼ�������ָ�룬��������ͼ��

		static HANDLE MVS2GBE_Mutex;
		HANDLE     m_hGrabEvent;	//�ɼ�ͼ���¼�
		void* m_pGrabParent;

		unsigned char    *p_Xmldata; //xml�ļ����ݻ����ַ��xml����������ĸ��ֲ���
	public:
		MVS2GBECamera();
		MVS2GBECamera(char*camerName_);
		~MVS2GBECamera();
		virtual bool InitCam(unsigned long Index) ;//��ʼ�����
		virtual bool UnInitCam() ;
		virtual int GetImageWidth();				// ��ȡͼƬ���
		virtual int GetImageHeight() ;				// ��ȡͼƬ�߶�
		//virtual void SetWindowID() = 0;
		// ���ûص�����, Index-����ţ�pWnd-Ŀ�괰��ָ��
		virtual void SetCallBack(void(*Callback)(void* context), void* pWnd) ;	 
		virtual void SetBuffer(BYTE* pBuffer_) ;		// ����ͼ�񻺳���
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
		static int GetGuidList(list<string> &guidList,string &type);         //��ȡGUID�б�
	//�Զ��庯��
	public:
		static DWORD WINAPI ThreadGrading(LPVOID lpParamt); //�ɼ�ͼ����߳�
		
};





#endif
