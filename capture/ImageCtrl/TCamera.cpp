// TCamara.cpp : ���� DLL Ӧ�ó���ĵ���������
#include "stdafx.h"
#include "common.h"  //����־
#include "TCamera.h"
TCamera* TCamera::m_this = NULL;

TCamera::TCamera(void)
{
	m_en_camaratype = CAMARA_TYPE_AVT_1394;
	m_iresult = CAMERA_SUCCESS;
}

TCamera* TCamera::GetInstance()
{
	if (NULL == m_this)
	{
		m_this = new TCamera;
	}
	return m_this;
}

void TCamera::ReleaseInstance()
{
    if (NULL != m_this)
    {
        delete m_this;
        m_this = NULL;
    }
}

//��ʼ�����
int TCamera::Init(int dllnum, string dlllist[])
{
	int result = 0;
	//m_en_camaratype = camaratype;
	int i = 0;
	for (i= 0;i<dllnum;i++)
	{
		list<string> guidlist;
		result = GetGUIDList(guidlist,dlllist[i]);//����ÿ��������ַ�������
		list<string>::iterator it;
		for (it = guidlist.begin();it != guidlist.end();it++)
		{
			if (NULL != it->c_str())
			{
				Camer *camer = new Camer(it->c_str(),dlllist[i]);//dll������ʱû�õ� 
				m_hm_camera.insert(pair<string,Camer*>(it->c_str(),camer));//����ֻҪ����һ��,���ǵ����Ϊ2ʱ�������⣬�����Ȼ��ǲ���һ��
				if (camer->IsBindRealCam())
				{
					m_hm_camera.insert(pair<string,Camer*>(camer->GetCamFullName().c_str(),camer));
				}
			}
		}
	}
	return 0;
}

//�ͷ����
int TCamera::Deinit()
{
    Camer *pCamera = NULL;
	for (m_it_camera = m_hm_camera.begin();m_it_camera != m_hm_camera.end();m_it_camera++)
	{
		pCamera = GetCamer(m_it_camera->first.c_str());
		if (NULL != pCamera)
		{
			delete pCamera;
			pCamera = NULL;
		}
	}		
	return m_iresult;
}

//�����
int TCamera::Open(string name)
{	
    int iResult = 0;
    Camer *pCamera = GetCamer(name);
    if (NULL != pCamera)
    {
        iResult = pCamera->open();
	}	
    else
    {
		LOG_ERR("�����(%s)ʧ�ܣ�",name.c_str());
        iResult = IMAGE_NOINSTANCE;
    }
	return iResult;
}

//��ȡ���ͼ���С
int TCamera::GetImageSize(string name, int &w, int &h)
{
    int iResult = 0;
    Camer *pCamera = GetCamer(name);
    if (NULL != pCamera)
    {
        iResult = pCamera->getImageSize(w,h);
	}
    else
    {
        iResult = IMAGE_NOINSTANCE;
    }
	return iResult;
}

//�ر����
int TCamera::Close(string name)
{
    int iResult = 0;
    Camer *pCamera = GetCamer(name);
    if (NULL != pCamera)
    {
        iResult = pCamera->close();
	}
	return iResult;
}

//��ȡ���ͼ��
int TCamera::GetImage(string name, void *pImage)
{
    int iResult = 0;
    Camer *pCamera = GetCamer(name);
    if (NULL != pCamera)
    {
        iResult = pCamera->getImage(pImage);
    }
    else
    {
        iResult = IMAGE_NOINSTANCE;
    }
	return iResult;
}

// �����������
int TCamera::SetGain(string name,unsigned int GainValue) 
{
    int iResult = 0;
    Camer *pCamera = GetCamer(name);
    if (NULL != pCamera)
    {
        iResult = pCamera->SetSetGain(GainValue);
    }
    else
    {
        iResult = IMAGE_NOINSTANCE;
    }
	return iResult;
}
// ��ȡ�������
int TCamera::GetGain(string name) 
{
    int GainValue = 0;
    Camer *pCamera = GetCamer(name);
    if (NULL != pCamera)
    {
        GainValue = pCamera->GetSetGain();
    }
    return GainValue;
}

// ��������ع�ʱ��
int TCamera::SetExposureTime(string name,unsigned int ExpTimeValue)
{
    int iResult = 0;
    Camer *pCamera = GetCamer(name);
    if (NULL != pCamera)
    {
        iResult = pCamera->SetExposureTime(ExpTimeValue);
    }
    else
    {
        iResult = IMAGE_NOINSTANCE;
    }
	return iResult;
}
// ��ȡ����ع�ʱ��
int TCamera::GetExposureTime(string name)
{
    Camer *pCamera = GetCamer(name);
    if (NULL != pCamera)
    {
        m_iresult = pCamera->GetExposureTime();
    }
    return m_iresult;
}

// �������ID��
int TCamera::GetGUIDList(list<string> &guidlist,string &type)
{
	int ret = Camer::GetGUIDList(guidlist,type);
	return ret;
}

//��ȡ�������
int TCamera::GetParameInfo(string name,CAMARA_PARA_ITEM speciPara,unsigned long &info)
{
    Camer *pCamera = GetCamer(name);
    if (NULL != pCamera)
    {
        m_iresult = pCamera->GetParameInfo((ICAMERA_PARAMETER)speciPara,info);
	}
	return m_iresult;
}

//�����������
int TCamera::SetParameInfo(string name,CAMARA_PARA_ITEM speciPara,int val)
{
    Camer *pCamera = GetCamer(name);
    if (NULL != pCamera)
    {
        m_iresult = pCamera->SetParameInfo((ICAMERA_PARAMETER)speciPara,val);
	}
	return m_iresult;
}

//��ʼʵʱȡ��
int TCamera::StartSnap(string name,void *pImage,void(*Callback)(void* context),void *pParent)
{
    Camer *pCamera = GetCamer(name);
    if (NULL != pCamera)
	{
		m_iresult = pCamera->StartSnap(pImage,Callback,pParent);
	}
	return m_iresult;
}

//ֹͣʵʱȡ��
int TCamera::StopSnap(string name)
{
	Camer *pCamera = GetCamer(name);
	if (NULL != pCamera)
	{
		m_iresult = pCamera->StopSnap();
	}
	return m_iresult;
}

//��ȡ�������
Camer *TCamera::GetCamer(string name)
{
    Camer *pCamera = NULL;
    hash_map<string,Camer*>::iterator it;
	it = m_hm_camera.find(name);

	if (m_hm_camera.end() != it)
	{
        pCamera = it->second;
	}
	return pCamera;
}
