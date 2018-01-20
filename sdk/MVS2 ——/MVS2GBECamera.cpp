// MVS2.cpp 




#include "stdafx.h"
#include <string>
using namespace std;

#define MVS2GBECAMERA_EXPORTS 1

#include "MVS2GBECamera.h"
//���������ṩ��ͷ�ļ�



//��̬��Ա��ʼ��

bool MVS2GBECamera::m_isInitFlag = false;

MV_CC_DEVICE_INFO_LIST MVS2GBECamera::EnumDeviceList = {0};//����б�

MVS2GBECamera::MVS2GBECamera()
{
	m_isInitFlag=false;

	m_iscontinueGrab=false;
	m_isstopGrab=true;
	m_CamIndex=0;
	memset(m_CamerName,0,CAMFULLNAME_LENTH_MAX);
	m_handle=nullptr;
	m_pBuffer=nullptr;
	pFrameInfo=nullptr;
	p_Xmldata=nullptr;
}

MVS2GBECamera::MVS2GBECamera(char*camerName)
{
	strcpy(m_CamerName,camerName);
	m_isInitFlag=false;

	m_iscontinueGrab=false;
	m_isstopGrab=true;
	m_CamIndex=0;
	m_handle=nullptr;
	m_pBuffer=nullptr;
	pFrameInfo=nullptr;
	p_Xmldata=nullptr;
}

MVS2GBECamera::~MVS2GBECamera()
{
	//delete m_pBuffer;
	//delete pFrameInfo;
	//delete p_Xmldata;
}

 bool MVS2GBECamera::InitCam(unsigned long Index) //��ʼ�����
{
	//��ȡ����͹��ػ�����Э�� GigE USB IEEE1394 Camera Link
	unsigned int support_protocol = MV_CC_EnumerateTls();
	support_protocol=support_protocol&MV_GIGE_DEVICE;
	//�ж��Ƿ�֧��GigEЭ��
	if( MV_GIGE_DEVICE != support_protocol )
	{
		return false;
	}
	//ö�ٳ�֧�ָ�Э��������豸������ŵ��豸��Ϣ�б���
	if( MV_CC_EnumDevices(MV_GIGE_DEVICE,&EnumDeviceList) )
	{
		return false;
	}
	m_CamIndex=Index;
	//�ж�����ֵ�Ƿ��� ��ǰ���������豸��Χ��
	if( EnumDeviceList.nDeviceNum >= Index && Index <= MV_MAX_DEVICE_NUM )
	{
		//�����豸ʵ�����
		if( MV_CC_CreateHandle(&m_handle,EnumDeviceList.pDeviceInfo[Index]) )
			return false;
	}
	else
	{
		return false;
	}
	//�����豸
	if( MV_CC_OpenDevice(m_handle,MV_ACCESS_Control,0) )
		return false;
	else
		return true;
	//��ȡ�豸��Ϣ
	MV_CC_GetDeviceInfo(m_handle,&m_DeviceInfo);
}

bool MVS2GBECamera::UnInitCam() 
{
	//�ر��豸���ͷ���Դ
	MV_CC_CloseDevice(m_handle);
	//���پ�����ͷ���Դ
	MV_CC_DestroyHandle(m_handle);
	return true;
}

int MVS2GBECamera::GetImageWidth()				// ��ȡͼƬ���
{
	if( !MV_CC_GetWidth(m_handle,&ImageWidth))
		return ImageWidth.nCurValue;
	else
		return -1;
}
	

int MVS2GBECamera::GetImageHeight() 			// ��ȡͼƬ�߶�
{
	if(!MV_CC_GetHeight(m_handle,&ImageHeight))
		return ImageHeight.nCurValue;
	else
		return -1;
}

//virtual void SetWindowID() = 0;
// ���ûص�����, Index-����ţ�pWnd-Ŀ�괰��ָ��
void MVS2GBECamera::SetCallBack(void(*Callback)(void* context), void* pWnd) 	 
{
	m_fcallback=Callback;
	m_pGrabParent=pWnd;
}

void MVS2GBECamera::SetBuffer(BYTE* pBuffer_) 		// ����ͼ�񻺳���
{
	m_pBuffer=pBuffer_;
}

DWORD WINAPI MVS2GBECamera::ThreadGrading(LPVOID lpParamt)                  //�ɼ�ͼ����߳�
{
	MVS2GBECamera* temp = (MVS2GBECamera*)lpParamt; 
	while(1)
	{
		if( temp->m_iscontinueGrab)
		{
			WaitForSingleObject(temp->m_hGrabEvent,INFINITE);
		}
		if (temp->m_isstopGrab)
		{
			temp->m_isstopGrab = FALSE;   // ���˴���ֵ�������ա�
			break;
		}
		ResetEvent(temp->m_hGrabEvent );
		MVCC_INTVALUE stIntvalue = {0};
		MV_CC_GetIntValue(temp->m_handle, "PayloadSize", &stIntvalue);
		int nBufSize = stIntvalue.nCurValue + 2048; //һ֡���ݴ�С+Ԥ���ֽ�(����SDK�ڲ�����)

		MV_CC_StartGrabbing(temp->m_handle);
		MV_CC_GetOneFrameTimeout(temp->m_handle,temp->m_pBuffer,nBufSize,temp->pFrameInfo,1000);
		//temp->m_fcallback(context);
	}
	CloseHandle(temp->m_hGrabEvent);
	temp->m_hGrabEvent=NULL;
	return 0;
}

bool MVS2GBECamera::SingleSnap(bool bDisplay ) 	// ��֡�ɼ� // ʹ�õ���ͬ���ɼ�����ʹ�ûص�����
{
	//��ǰ��������ʵ�����
	//�����豸
	if( MV_CC_OpenDevice(m_handle,MV_ACCESS_Control,0) )
		return false;
	//���òɼ�ģʽΪ��֡
	m_GrabStrategy=MV_ACQ_MODE_SINGLE;
	if( MV_CC_SetAcquisitionMode(m_handle,m_GrabStrategy) )
		return false;
	else
		return true;

	MVCC_INTVALUE stIntvalue = {0};
	MV_CC_GetIntValue(m_handle, "PayloadSize", &stIntvalue);
	int nBufSize = stIntvalue.nCurValue + 2048; //һ֡���ݴ�С+Ԥ���ֽ�(����SDK�ڲ�����)
	MV_CC_StartGrabbing(m_handle);
	MV_CC_GetOneFrameTimeout(m_handle,m_pBuffer,nBufSize,pFrameInfo,1000);
}

bool MVS2GBECamera::StartSnap() 	// �����ɼ�
{
	//��ǰ��������ʵ�����
	//�����豸
	if( MV_CC_OpenDevice(m_handle,MV_ACCESS_Control,0) )
		return false;
	//���òɼ�ģʽΪ����
	m_GrabStrategy= MV_ACQ_MODE_CONTINUOUS;
	if( MV_CC_SetAcquisitionMode(m_handle,m_GrabStrategy) )
		return false;
	else
		return true;
	m_hGrabEvent = CreateEventA(NULL,true,false,NULL);
	CreateThread(NULL,0,ThreadGrading,this,0,NULL);
}

bool MVS2GBECamera::StopSnap() 	// ֹͣ�ɼ�	
{
	if( MV_CC_StopGrabbing(m_handle) )
		return false;
	else
		return true;
}

bool MVS2GBECamera::SetTriggerMode(bool Mode)		//���ô���ģʽ	
{
	if(!MV_CC_SetTriggerMode(m_handle,Mode))
		return true;
	else
		return false;
}

bool MVS2GBECamera::SetExposureTime(int iValue) 	//�����ع�ʱ��	
{
	CameraExposureTime.Mode.nCurValue=MV_EXPOSURE_AUTO_MODE_OFF;
	if(!MV_CC_SetExposureAutoMode(m_handle,CameraExposureTime.Mode.nCurValue) )
	{	
		if(!MV_CC_SetExposureTime(m_handle,iValue))
			return true;
		else
			return false;
	}
	else
		return false;
}

bool MVS2GBECamera::SetGain(int iValue) 	//��������
{
	CameraGain.Mode.nCurValue=MV_GAIN_MODE_OFF;
	//����ģʽ
	if( !MV_CC_SetGainMode(m_handle,CameraGain.Mode.nCurValue))
	{
		//��������
		if(!MV_CC_SetGain(m_handle,iValue))
			return true;
		else
			return false;
	}
	else
		return false;
}

int MVS2GBECamera::GetExposureTime() 		//��ȡ�ع�ʱ��
{
	if(!MV_CC_GetExposureTime(m_handle,&(CameraExposureTime.Time) )  )
		return CameraExposureTime.Time.fCurValue;
	else
		return -1;

}

int MVS2GBECamera::GetGain() 			//��ȡ����
{
	if(!MV_CC_GetGain(m_handle,&(CameraGain.Gain)))
		return CameraGain.Gain.fCurValue;
	else
		return -1;
}

char* MVS2GBECamera::GetFullName()
{

	return m_CamerName;
}

int MVS2GBECamera::GetParameInfo(ICAMERA_PARAMETER speciPara,unsigned long &info)///AVT�еĲ���
{
	return -1;
}

int MVS2GBECamera::SetParameInfo(ICAMERA_PARAMETER speciPara,int val)
{
	return -1;
}

int MVS2GBECamera::GetGuidList(list<string> &guidList,string &type)          //��ȡGUID�б�
{
	int nRet = MV_OK;
	void* handle = NULL;
	char CamGuid[256] = {0};
	int i = 0;
	string cuuid;
	int camerNum = EnumDeviceList.nDeviceNum;
	memset(&EnumDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
	// 1. ö���豸
	nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &EnumDeviceList);
	if (MV_OK != nRet)
	{
		printf("MV_CC_EnumDevices fail! nRet [%x]\n", nRet);
		return -1;
	}
	camerNum = EnumDeviceList.nDeviceNum;	
	for (i = 0;i<camerNum;i++)
	{
		MV_CC_DEVICE_INFO* pstMVDevInfo= EnumDeviceList.pDeviceInfo[i];
		if (NULL == pstMVDevInfo)
		{
			printf("%s\n" , "The Pointer of pstMVDevInfoList is NULL!");
			return i;
		}
		if (pstMVDevInfo->nTLayerType == MV_GIGE_DEVICE)
		{
			cuuid = (char*)pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName;
			guidList.push_back(cuuid);
			printf("%s %x\n" , "nCurrentIp:" , pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp);                   //��ǰIP
			printf("%s %s\n\n" , "chUserDefinedName:" , pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName);     //�û�������
		
		}
		else if (pstMVDevInfo->nTLayerType == MV_USB_DEVICE)
		{
			cuuid = (char*)pstMVDevInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName;
			guidList.push_back(cuuid);
			printf("UserDefinedName:%s\n\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName);
		}
		else
		{
			printf("Not support.\n");
		}
	}

	return i;
}

EXTERN_C CameraHANDLE WINAPI CreateCamera(char*camerName)
{
	MVS2GBECamera* example= new MVS2GBECamera(camerName);
	return example;
}

EXTERN_C void WINAPI ReleaseCamera(CameraHANDLE handle)
{
	handle->UnInitCam();
}

EXTERN_C int WINAPI getList(list<string> &guidList,string &type)
{

	return  MVS2GBECamera::GetGuidList(guidList,type);
}
