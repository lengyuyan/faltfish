

// FlatFishManager.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "ManagerCtrl.h"
#include "VisionServer.h"
#include "iCommunication.h"
#include "iConfigManager.h"
#include "iLightCtrl.h"
#include "iImageCtrl.h"
#include <process.h>
#include  <io.h>
#include <assert.h>
#include "FuncDefine.h"
#include "sysapi.h"
#include "common.h"

#define OPEN_OP           1;   //��
#define CLOSE_OP          0;   //�ر�

unsigned long         m_CurrentError;      //��¼ϵͳ��ʼ�����쳣

#define					HEART_BEAT_TIME		2000
#define					MAN_OK				0
#define					MAN_ERROR			-1

CHECK_RESULT  SceneProcResult;

vector<SCENE_PKG *> CManagerCtrl::m_vecSceneParam;

class ManageDataInvalib
{
public:
	ManageDataInvalib (bool *bIsValib)
	{
		if (NULL != bIsValib)
		{
			*bIsValib = true;
		}
		m_bIsValib = bIsValib;
	}

	~ManageDataInvalib ()
	{
		if (NULL != m_bIsValib)
		{
			*m_bIsValib = false;
		}
	}
private:
	bool *m_bIsValib;
};

class LockStock 
{
public:
	LockStock (CRITICAL_SECTION *LockSection)
	{
		if (NULL != LockSection)
		{
			EnterCriticalSection(LockSection);
		}
		m_LockSection = LockSection;
	}

	~LockStock ()
	{
		if (NULL != m_LockSection)
		{
			LeaveCriticalSection(m_LockSection);          
		}
	}
private:
	CRITICAL_SECTION *m_LockSection;
};

CManagerCtrl *CManagerCtrl::m_pInstance = NULL;
CRITICAL_SECTION *CManagerCtrl::m_pSection = NULL;


CManagerCtrl::CManagerCtrl(void)
{
	m_pVisionInstance = NULL;
	m_pVisionInstance = new CVisionServer;
	m_CalibWindID = -1;
	memset(m_chSN,0,64);
	memset(m_SceneWidth, 0, SCENE_SIZE);
	memset(m_SceneHeight, 0, SCENE_SIZE);
	memset(m_bIsWorking,0, SCENE_SIZE);
	m_CurrentError = INIT_NULL;
	m_iExitThread = true;
	InitReadConfig();
	register_notify(MODULE_SCENE_MAIN_SDK, RemoteManualMsg,this);
	m_RecvEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_WorkingStatus = 0;
	if (NULL == m_pSection)
	{
		m_pSection = new CRITICAL_SECTION;
		InitializeCriticalSection(m_pSection);
	}

	//��ʼ����ų��������map
	m_mapSceneResultSection = sys_InitializeCriticalSection();
	ClearSceneProcResultMap();
	//����ϵͳ���������ɽ��map
	InitSceneProcResultMap();


}

CManagerCtrl::~CManagerCtrl(void)
{
	WORKING_THREAD_DATA *pInfo = NULL;
	map<int, WORKING_THREAD_DATA *>::iterator itTh = m_ThSceneData.begin();
	while (itTh != m_ThSceneData.end())
	{
		pInfo = itTh->second;
		if (NULL != pInfo)
		{
			SetEvent(pInfo->thExit);
			delete pInfo;
			pInfo = NULL;
			//��������������йر�
		}
		m_pInstance->m_ThSceneData.erase(itTh);
		itTh = m_ThSceneData.begin();
	}

	CVisionServer::ReleaseInstance();
	if (NULL != m_pVisionInstance)
	{
		delete m_pVisionInstance;
		m_pVisionInstance = NULL;
	}
	//unregister_notify(MODULE_SCENE_MAIN_SDK);
	vector<SCENE_PKG *>::iterator it;
	SCENE_PKG *pConfig = NULL;
	for (it = m_vecSceneParam.begin(); it != m_vecSceneParam.end(); it++)
	{
		pConfig = (*it);
		//delete 
		if (!pConfig->vecBody.empty())
		{
			vector<VIRSION_DATA_PKG *>::iterator it;
			it = pConfig->vecBody.begin();
			for (; it != pConfig->vecBody.end(); )
			{
				delete *it;
				pConfig->vecBody.erase(it);
				it = pConfig->vecBody.begin();
			}
		}
		if (!pConfig->vecDataType.empty())
		{
			vector<unsigned int *>::iterator it;
			for (it = pConfig->vecDataType.begin(); it != pConfig->vecDataType.end();)
			{
				delete *it;
				pConfig->vecDataType.erase(it);
				it = pConfig->vecDataType.begin();
			}
		}
		if (!pConfig->InterfaceSetData.empty())
		{
			vector<INTERFACE_SET_DATA *>::iterator it;
			for (it = pConfig->InterfaceSetData.begin(); it != pConfig->InterfaceSetData.end(); )
			{
				delete *it;
				pConfig->InterfaceSetData.erase(it);
				it = pConfig->InterfaceSetData.begin();
			}
		}
		delete pConfig;
		pConfig = NULL;
	}
	//�ͷŽ��map��
	ClearSceneProcResultMap();
	sys_DeleteCriticalSection(m_mapSceneResultSection);


	ReleaseConnectInstance();
	m_iExitThread=false;
	//�ͷŹ�Դ
	destroyLightInstance();
	deinitImage();
	CloseHandle(m_RecvEvent);
	ReleaseConfigInstance();
	unregister_notify(MODULE_SCENE_MAIN_SDK,RemoteManualMsg);
}

CManagerCtrl *CManagerCtrl::GetInstance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new CManagerCtrl;
	}
	return m_pInstance;
}

void CManagerCtrl::ReleaseInstance()
{
	if (NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

/**********************************************************************
����      : InitReadConfig
����      : ��ʼ����ȡ�����ĵ�����
�������  : N/A
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
void  CManagerCtrl::InitReadConfig()
{
	SCENE_PKG *pSceneConfig;
	if (InitConfig(&m_stSysParam))//��ȡϵͳ����
	{
		m_vecSceneParam.clear();
		for (int i = 0; i < m_stSysParam.iSceneNum; i++)//����ϵͳ������ĳ�����������ȡ��������
		{
			pSceneConfig = new SCENE_PKG;
			pSceneConfig->sceneID = i;//����IDΪ��ϵͳ�ļ��е�����˳��
			strncpy(pSceneConfig->szSceneName, m_stSysParam.pstrSceneName[i].c_str(), SCENE_SIZE - 1);
			GetSceneConfig(m_stSysParam.pstrSceneName[i].c_str(), (void *)pSceneConfig);
			m_vecSceneParam.push_back(pSceneConfig);  
			//����������visionprocess�����������࣬��Ҫ��̬���ɣ���̬���ɵĺô��ǿ����ٵ��ڴ�,�������ĺô��ǿ����ڴ����������ģ��ʱ���Զ�ȡģ��Ȼ�󱣴�ģ��id
			//����ÿ�μ�ⶼȥ��ȡģ��

			CVisionServer::GetInstance(i);

		}
		SetCurrentStatus(INIT_CONFIG_OK);
	}
	else
	{
		msg_post(MSG_DISP_WORKING_INFO, "��ʼ������ʧ��", (unsigned int)strlen("��ʼ������ʧ��") + 1, MODULE_SCENE_MAIN_UI); 
	}
	AlterMainWndTitle(m_stSysParam.CurProductName);
}
/**********************************************************************
����      : InitManager
����      : ��ȡϵͳ�����ļ�,������������ʼ���������Դ
�������  : N/A
�������  : ��
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::InitManager()
{
	bool bRet = true;
	if (CAMERA_SUCCESS != imageInstance(m_stSysParam.iCameraDllNum,m_stSysParam.pstrCameraDll))  //����Ϊ������
	{
		//�����쳣��UI;
		msg_post(MSG_DISP_WORKING_INFO, "�����ʼ��ʧ��",(unsigned int)strlen("�����ʼ��ʧ��")+1, MODULE_SCENE_MAIN_UI); 
		for (int in = 0; in < m_stSysParam.iCameraNum; in++)
		{
			msg_post(MSG_CONNECT_CAMERA_FAILED, &in,sizeof(int), MODULE_SCENE_MAIN_UI); 
		}
		bRet = false;
	}
	else
	{
		int iResult = 0;
		int width = 0;
		int height = 0;
		for (int i = 0; i < m_stSysParam.iCameraNum; i++)
		{
			if (CAMERA_SUCCESS != openCamera(m_stSysParam.pstrCamareGUID[i]))   
			{
				msg_post(MSG_DISP_WORKING_INFO, "�����ʧ��",(unsigned int)strlen("�������ʧ��")+1, MODULE_SCENE_MAIN_UI); 
				msg_post(MSG_CONNECT_CAMERA_FAILED, &i,sizeof(int), MODULE_SCENE_MAIN_UI); 
			}
			else
			{
				msg_post(MSG_CONNECT_CAMERA_SUCCESS, &i,sizeof(int), MODULE_SCENE_MAIN_UI); 
				string camerasuccess = m_stSysParam.pstrCamareGUID[i]+ "����򿪳ɹ�";
				const char *cameraID = (camerasuccess).c_str();
				msg_post(MSG_DISP_WORKING_INFO, (void*)cameraID,(unsigned int)strlen(camerasuccess.c_str())+1, MODULE_SCENE_MAIN_UI);
			}
		}
		SetCurrentStatus(INIT_CAMERA_OK);
	}
	Sleep(200); //TBD
	//��ʼ�������е��ת����
	for (int i = 0;i<m_stSysParam.iCameraNum;i++)
	{
		for (int AxeIndex = 0;AxeIndex<m_stSysParam.iAxisNum;AxeIndex++)
		{
			for (int j = 0;j<m_stSysParam.iToolNum;j++)
			{
				m_pVisionInstance->SetCamInMecRot(i,AxeIndex,j,m_stSysParam.CamInMecRotMetix[i][AxeIndex][j],(int)m_stSysParam.CamInMecRotMetix[i][AxeIndex][j][0]);
				m_pVisionInstance->SetCamRot(i,AxeIndex,j,m_stSysParam.CamRotMetix[i][AxeIndex][j],(int)m_stSysParam.CamRotMetix[i][AxeIndex][j][0]);
				m_pVisionInstance->SetImageMetrix(i,AxeIndex,j,m_stSysParam.ImageRelative[i][AxeIndex][j],m_stSysParam.ImageAbsolute[i][AxeIndex][j],
					(int)m_stSysParam.ImageRelative[i][AxeIndex][j][0]);

			}
		}
	}
	//��ʼ����Դ
	if (LIGHTCTL_SUCCESS != iLightInstance(m_stSysParam.strLightInfo, m_stSysParam.iLightNum, m_stSysParam.strLightType,4))
	{
		//�����쳣��UI;
		msg_post(MSG_DISP_WORKING_INFO, "��Դ��ʼ��ʧ��",(unsigned int)strlen("��Դ��ʼ��ʧ��") + 1, MODULE_SCENE_MAIN_UI); 
		bRet = false;
	}
	else
	{
		//��ʼ����Դ��С
		for (int iL = 0; iL < m_stSysParam.iLightNum; iL++)
		{
			for (int iC = 0; iC < m_stSysParam.strLightInfo[iL].Channel; iC++)
			{
				setLightValue(m_stSysParam.strLightInfo[iL].PortName, iC, m_stSysParam.strLightInfo[iL].DefaultValue);
				closeLight(m_stSysParam.strLightInfo[iL].PortName, iC);
			}
		}
		SetCurrentStatus(INIT_LIGHT_OK);
	}
	//��ʼ��ͼ����
	if(m_pVisionInstance->InitVision())
	{
		SetCurrentStatus(INIT_VISION_OK);
	}
	else 
	{
		msg_post(MSG_DISP_WORKING_INFO, "��ʼ��ͼ����ʧ��",(unsigned int)strlen("��ʼ��ͼ����ʧ��") + 1, MODULE_SCENE_MAIN_UI); 
		bRet = false;
	}
	//��ʼ��ͨѶ����
	if (0 <= InitConnect((void *)&m_stSysParam.stSocketParam))
	{
		SetCurrentStatus(INIT_CONNECT_OK);
		msg_post(MSG_DISP_WORKING_INFO, "��ʼ�����ӳɹ�",(unsigned int)strlen("��ʼ�����ӳɹ�") + 1, MODULE_SCENE_MAIN_UI);  
	}
	else
	{
		//��list���������log
		msg_post(MSG_DISP_WORKING_INFO, "��ʼ������ʧ��",(unsigned int)strlen("��ʼ������ʧ��") + 1, MODULE_SCENE_MAIN_UI);  
		bRet = false;
	}
	//��socketע�ᴥ�����ص��ӿ�
	// RegesterCallBack(CManagerCtrl::TriggerVisionProcess);
	// RegesterCloseSys(CManagerCtrl::RemoteSystemMsg); //��������Զ��֪ͨ��Ϣ
	return bRet;
}

bool CManagerCtrl::UnInitManager()
{
	bool bRet = false;
	deinitImage();
	//�ͷŴ���
	for (int iL = 0; iL < m_stSysParam.iLightNum; iL++)
	{
		deInitLightCtrl(m_stSysParam.strLightInfo[iL].PortName);
	}
	UnInitConnect();
	//ReleaseInstance();///ΪɶҪע�͵���䣿
	return bRet;
}

void  CManagerCtrl::GetSceneProcess(pSceneProcess* pProcess)
{

	m_SceneProcess = pProcess;

}

/**********************************************************************
����      : TriggerVisionProcess
����      : �����Ӿ�������
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::TriggerVisionProcess(int iSceneID, int iPercent)
{
	bool bRet = false;
	//�������(�߳�ִ��);
	map<int, WORKING_THREAD_DATA*>::iterator itTh;
	WORKING_THREAD_DATA *pthInfo = NULL;
	itTh = m_ThSceneData.find(iSceneID);  
	LockStock Stock(m_pSection);
	if(itTh != m_ThSceneData.end())
	{
		pthInfo = itTh->second;
		//��ǰ�����Ĵ����߳��Ѵ���
		if (WAIT_OBJECT_0 != WaitForSingleObject(pthInfo->thRunning, 0))  //��⵱ǰ���������Ƿ������У����¼�Ϊ���ź�ʱ WAIT_OBJECT_0��
		{
			//���źţ���ʾ����û��ִ��
			ResetEvent(pthInfo->thBegin);  //�������״̬
			SetEvent(pthInfo->thBegin);    //������������
		}
		else  //���źţ���ʾ��������ִ�С�
		{
			int SceneLocal = iSceneID;
			msg_post(MSG_SCENE_BUSY, &SceneLocal, sizeof(int), MODULE_SCENE_MAIN_SDK);//֪ͨ��ǰ������æ ��
		}
	}
	else
	{
		WORKING_THREAD_DATA *pthInfo = new WORKING_THREAD_DATA;
		if (NULL != pthInfo)
		{
			pthInfo->iSceneID = iSceneID;
			pthInfo->thHandle = (HANDLE)_beginthreadex(NULL,0,(unsigned (__stdcall *)(void*))CManagerCtrl::ThreadVisionProcess,(void *)pthInfo,0,NULL);
			SetEvent(pthInfo->thBegin);
			m_ThSceneData.insert(make_pair(iSceneID, pthInfo));
		}
	}

	return bRet;
}

/**********************************************************************
����      : ThreadVisionProcess
����      : ��̬������ͼ�����߳�
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
void CManagerCtrl::ThreadVisionProcess(void *pParam)
{
	WORKING_THREAD_DATA *thInfo = (WORKING_THREAD_DATA *)pParam;
	int iSceneID = (int)thInfo->iSceneID;	
	bool bIsFinde = false;
	int  iErrorCode = 0;
	int  iTimeCircle = 0;
	int  pThInfo = (int)thInfo;
	char szErr[126] = {0};
	HANDLE hThEvnets[2] = {thInfo->thBegin, thInfo->thExit};
	DWORD  hEventID = -1;
	SCENE_PKG *pConfig = m_pInstance->GetSceneHandle(iSceneID);
	CHECK_RESULT  *g_SceneProcResult = NULL;
	string sname = pConfig->szSceneName;
	if (NULL == pConfig || 0 == strlen(pConfig->szSceneName))
	{
		return;
	}
	do 
	{
		hEventID = WaitForMultipleObjects(2, hThEvnets, false, INFINITE);
		g_SceneProcResult = m_pInstance->GetSceneProcResult(sname);		
		SetEvent(thInfo->thRunning);    //������������
		if (WAIT_OBJECT_0 == hEventID)
		{
			LARGE_INTEGER  start,end,litmp;
			QueryPerformanceFrequency(&litmp);//��ȡϵͳʱ��Ƶ��
			QueryPerformanceCounter(&start); 
			if (NULL != m_pInstance)
			{
				if (0 == m_pInstance->m_WorkingStatus)
				{		
					//memset(g_SceneProcResult->szValue[RESULT_NUMBER],0,sizeof(char)*RESULT_NUMBER*BUF_SIZE/2);
					iErrorCode = m_pInstance->SingleVisionProcess(iSceneID, *g_SceneProcResult,WORKING_CAMERA);
					thInfo = (WORKING_THREAD_DATA*)pThInfo;
					//�Ƿ���Ҫ���з���ת�� 
				}
				else
				{
					sprintf_s(szErr, "���˳���½");
					msg_post(MSG_DISP_WORKING_INFO, szErr, (unsigned int)strlen(szErr) + 1, MODULE_SCENE_MAIN_UI); 
					g_SceneProcResult->err = -2;
				}
			}  
			if (1000 == iErrorCode)//�Ӿ�����OK
			{
				int TotalIndex = g_SceneProcResult->iCount;
				//memcpy(g_SceneProcResult->szValue[0],"1",sizeof(g_SceneProcResult->szValue[0]));
				//memcpy(g_SceneProcResult->szValue[1],g_SceneProcResult->szValue[TotalIndex-3],sizeof(g_SceneProcResult->szValue[TotalIndex-3]));
				//memcpy(g_SceneProcResult->szValue[2],g_SceneProcResult->szValue[TotalIndex-2],sizeof(g_SceneProcResult->szValue[TotalIndex-2]));
				//memcpy(g_SceneProcResult->szValue[3],g_SceneProcResult->szValue[TotalIndex-1],sizeof(g_SceneProcResult->szValue[TotalIndex-1]));

			}else
			{
				//���ﲻ�������Щ����
				//memcpy(g_SceneProcResult->szValue[0],"0",sizeof(g_SceneProcResult->szValue[0]));
			}
			g_SceneProcResult->SeneTpye = WORKING_CAMERA;
			int LocalSceneID =iSceneID;

			QueryPerformanceCounter(&end) ; 
			double dTime = (double)(end.QuadPart-start.QuadPart) / (double)litmp.QuadPart;    //��ȡʱ�䵥λΪ��	
			char szBuf[125] = {0};
			g_SceneProcResult->time = dTime - g_SceneProcResult->caturetime;
			m_pInstance->ManagerProcFinish(LocalSceneID, g_SceneProcResult, iErrorCode);  //���紫�ʹ�����
			if ( 0<=iErrorCode )
			{	//֪ͨUI��ʾ���,��ʾ�ڹ���������
				msg_post(MSG_DISP_IMAGE_DUMP, (void *)&iSceneID, sizeof(int), MODULE_SCENE_MAIN_UI);  //gpz����Ҫ����ͼƬ,���﷢ͼƬҲ������ֻ�ܷ�1024���ֽ�
			}
			m_pInstance->ClearProcResultByID(iSceneID);
			sprintf_s(szBuf, "���� %d ��ʱ %0.3lfs",iSceneID, dTime);
			msg_post(MSG_DISP_WORKING_INFO, (void *)szBuf, (unsigned int)strlen(szBuf)+1, MODULE_SCENE_MAIN_UI);
			ResetEvent(thInfo->thRunning);  //�������״̬
		}
		//�˳��̣߳� ���������
		else if ((WAIT_OBJECT_0 + 1) == hEventID)
		{
			break;  //�˳��߳�
		}
		ResetEvent(thInfo->thBegin);  //�������״̬
	} while (true);
}

int CManagerCtrl::SceneGrabImage(int iSceneID,int ImageIndex,int CamIndex,int shutterTime,int Gain,int Interval,int LightIndex,int LightValue)
{
	int  iErrorCode = 0;
	char szErr[128] = {0};
	sprintf_s(szErr, "Grab start,cameraindex:%d,sceneID:%d,imageidx:%d.", CamIndex,iSceneID,ImageIndex);
	LOG_INFO(szErr);
	if (!m_pInstance->GrabWay2Sceneimage(iSceneID, ImageIndex,CamIndex,shutterTime,Gain,Interval,LightIndex,LightValue))
	{	
		iErrorCode = -1;	
		sprintf_s(szErr, "Grab fail,cameraindex:%d,sceneID:%d,imageidx:%d.", CamIndex,iSceneID,ImageIndex);
		LOG_ERR(szErr);
		msg_post(MSG_DISP_WORKING_INFO, szErr, (unsigned int)strlen(szErr) + 1, MODULE_SCENE_MAIN_UI); 
		return iErrorCode;
	}
	sprintf_s(szErr, "Grab end,cameraindex:%d,sceneID:%d,imageidx:%d.", CamIndex,iSceneID,ImageIndex);
	LOG_INFO(szErr);
	return iErrorCode;
}

/**********************************************************************
����      : SingleVisionProcess
����      : ץͼ����
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
result   ���ؽ��
type     ��������
�������  : N/A
����ֵ    : ���ؽ��
��ע      : 
�޸ļ�¼
����            �޸���            �޸�����
2016-08-16     guopengzhou        ������Ҫ�Ǳ궨�ʹ����õ��˺���������Э�鷵�ؿͻ���ָ����벻ͬ��
�����type�����֣�����ʱ�践���������ָ��
**********************************************************************/
int CManagerCtrl::SingleVisionProcess(int iSceneID, CHECK_RESULT &result,unsigned int type)
{
	int  iErrorCode = 0;
	SCENE_PKG *pscene;
	int LocalSceneId = iSceneID;
	char szErr[126] = {0};
	bool bIsWorking = m_bIsWorking[iSceneID];
	if (bIsWorking)
	{
		iErrorCode = -1;
		sprintf_s(szErr, "��ǰ����(%d)���ڴ�����", iSceneID);
		msg_post(MSG_DISP_WORKING_INFO, szErr, (unsigned int)strlen(szErr) + 1, MODULE_SCENE_MAIN_UI); 
		result.err = -2;
		return iErrorCode;
	}
	ManageDataInvalib Invalib(&m_bIsWorking[iSceneID]);
	pscene = m_pInstance->GetSceneHandle(iSceneID);
	if (!pscene)
	{
		return -1;
	}

	if ( 0 == pscene->ProcProcess)  //�·�������������
	{
		iErrorCode = 1000;
	}

	if (-1 != iErrorCode)
	{    //step4 ����ͼ������� //SYSTEMTIME st;�Ѱѹ��̼���ʱ��ӵ�����ṹ����
		sprintf_s(szErr, "T_��ǰ����(%d)Procee_start", iSceneID);
		LOG_INFO(szErr);
		if (!m_pInstance->VisionGetResult(iSceneID, 0,result))////�ڶ���������Ӧ��Ϊ��ֵ��������ܲɼ��˼�����Ƭ
		{
			LOG_ERR("ͼ����ʧ��");
			sprintf_s(szErr, "ͼ����ʧ��(%d)", iSceneID);
			msg_post(MSG_DISP_WORKING_INFO, szErr, (unsigned int)strlen(szErr) + 1, MODULE_SCENE_MAIN_UI); 
			iErrorCode = -2;
		}
		sprintf_s(szErr, "T_��ǰ����(%d)Procee_end", iSceneID);
		LOG_INFO(szErr);
	}
	return iErrorCode;
}

void CManagerCtrl::ThreadCalibProcess(void *pParam)
{
	bool bRet = true;
	CManagerCtrl *pInstance = NULL;
	pInstance = GetInstance();
	CALIB_DATA *pData = (CALIB_DATA *)pParam;
	//û��½��������б궨����
	if (NULL == pInstance || NULL == pData)
	{
		return;
	}
	bRet = pInstance->VisionCalibProcess(pParam);
	if (bRet)
	{
		msg_post(MSG_DISP_IMAGE_DUMP, (void *)&pData->iSceneID, sizeof(int), MODULE_SCENE_MAIN_UI);  
	}
}

/**********************************************************************
����      : ThreadCalibProcess
����      : �궨ͼ�����߳�
�������  : �궨����  
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::VisionCalibProcess(void *pParam)
{
	double PixSize = 0.0;
	int CameraID = -1;
	int iError = 0;
	int AixsIndex = -1;
	CALIB_DATA *pData = (CALIB_DATA *)pParam;
	char szBuf[256] = {0};
	char *pTmp = NULL;
	CManagerCtrl *pInstance = NULL;
	int index = 0;
	pInstance = GetInstance();
	//û��½��������б궨����
	if (NULL == pInstance || (m_WorkingStatus == 0 && pData->bIsInteractive))
	{
		return false;
	}

	SCENE_PKG *pConfig = NULL;
	string sname ;
	CHECK_RESULT  *g_SceneProcResult = NULL;
	//��ȡ����·��--��Ҫ�õ��ù�ϵ�Ļ��� ֻ���ǵ������
	pConfig = pInstance->GetSceneHandle(pData->iSceneID);
	pConfig->halconID = pInstance->m_CalibWindID;
	sname = pConfig->szSceneName;

	//index = pData->index;  //�궨������
	g_SceneProcResult = m_pInstance->GetSceneProcResult(sname);


	//���ñ궨������������)
	iError = pInstance->SingleVisionProcess(pData->iSceneID, *g_SceneProcResult,CALIB_CAMERA);
	index = pConfig->CalPointIndex;//�����Ѿ��궨�ĵ������,��ʼ��0,���������㣬��סҪ���㣬��ø�һ�����㰴ť
	int CamIndex = g_SceneProcResult->CameraIndex[0]; //��Ϊ�궨����ֻ��һ����������������±�Ϊ0
	AixsIndex = pConfig->AxisIndex;
	int ToolIndex = pConfig->MotorToolIndex;
	if (AixsIndex<0)
	{
		msg_post(MSG_DISP_CLIB_INFO, "����Ŵ����δ�趨", (unsigned int)strlen("����Ŵ����δ�趨") + 1, MODULE_SCENE_MAIN_UI); 
		return false;
	}
	if (CamIndex<0)
	{
		msg_post(MSG_DISP_CLIB_INFO, "�����Ŵ����δ�趨", (unsigned int)strlen("�����Ŵ����δ�趨") + 1, MODULE_SCENE_MAIN_UI); 
		return false;
	}
	if (ToolIndex<0)
	{
		msg_post(MSG_DISP_CLIB_INFO, "�о���Ŵ����δ�趨", (unsigned int)strlen("�о���Ŵ����δ�趨") + 1, MODULE_SCENE_MAIN_UI); 
		return false;
	}
	if ( m_stSysParam.CalPointNum[CamIndex][AixsIndex][ToolIndex]<3)
	{
		msg_post(MSG_DISP_CLIB_INFO, "�궨����������3����", (unsigned int)strlen("�궨����������3����") + 1, MODULE_SCENE_MAIN_UI); 
		return false;
	}
	MyFunClass::ParshRelativeToObsolute(m_stSysParam.CalMotorX[CamIndex][AixsIndex][pConfig->MotorToolIndex],
		m_stSysParam.CameraCalMachineX[CamIndex], m_stSysParam.CalPointNum[CamIndex][AixsIndex][pConfig->MotorToolIndex]);
	MyFunClass::ParshRelativeToObsolute(m_stSysParam.CalMotorY[CamIndex][AixsIndex][pConfig->MotorToolIndex], 
		m_stSysParam.CameraCalMachineY[CamIndex], m_stSysParam.CalPointNum[CamIndex][AixsIndex][pConfig->MotorToolIndex]);

	if (0 == pConfig->ProcProcess && 0 <= iError)
	{
		//1���궨 ����� ��е������������� ����ϵͳ�����У��ұ��浽ϵͳ�����ļ��� ģ��ƥ���ֵ����1,2,3
		m_stSysParam.CameraCalPixRow[CamIndex][index] = atof(g_SceneProcResult->szValue[1]);
		m_stSysParam.CameraCalPixCol[CamIndex][index]  = atof(g_SceneProcResult->szValue[2]);
		m_stSysParam.CameraCalPixPhi[CamIndex][index]  = atof(g_SceneProcResult->szValue[3]);
		double CamDisPhi=m_stSysParam.CameraCalPixPhi[CamIndex][index]-m_stSysParam.CameraCalPixPhi[CamIndex][index-1];//�ǶȲ�index=0��ʧ��
		//double DisToolPhi = m_stSysParam.CalMotorPhi[CamIndex][AixsIndex][ToolIndex][index] - m_stSysParam.CalMotorPhi[CamIndex][AixsIndex][ToolIndex][index-1] ;
		//����ԭʼ���ݵ������ļ�
		//SaveSysCamCalibrate(CamIndex,pConfig->MotorToolIndex,index+1,m_stSysParam.CameraCalMachineX[CamIndex], m_stSysParam.CameraCalMachineY[CamIndex],
		//m_stSysParam.CameraCalPixRow[CamIndex], m_stSysParam.CameraCalPixCol[CamIndex]);
		//2����� ������������3���Ϳ��Խ��м����ˣ���������ݱ������Ӿ���ľ�̬��Ա�С�
		if (index >= pConfig->CalPointNum-1)
		{
			double CamRow[16]={0.0};
			double CamCol[16]={0.0};
			double MechAndCamRot[6]={0};
			double MechAndCam[6]={0};
			double RelaImageToMotor[6] ={0};
			double AbsImageToMotor[6] ={0};
			PixSize = MyFunClass::CalPixSize(m_stSysParam.CameraCalMachineX[CamIndex],m_stSysParam.CameraCalMachineY[CamIndex],
				m_stSysParam.CameraCalPixCol[CamIndex], m_stSysParam.CameraCalPixRow[CamIndex],index+1);
			m_stSysParam.CameraPixSize[CamIndex][AixsIndex][pConfig->MotorToolIndex] = PixSize;
			//�������ش�С;
			SaveSysCamPixSize(CamIndex,AixsIndex,ToolIndex,PixSize);	
			for (int i = 0;i<=index;i++ )
			{
				CamRow[i] = m_stSysParam.CameraCalPixRow[CamIndex][i];
				CamCol[i] = m_stSysParam.CameraCalPixCol[CamIndex][i];

			}
			//��ȡ������ƶ����ǹ̶�����ȡ�����ź�����ţ���ȡ����ڵ�һ���궨�����ꣻ��ȡ�о��ڵ�һ���궨������			   
			double CalFirstAngle=0.0;			  
			switch ( m_stSysParam.CamFixedType[CamIndex])
			{
			case SYSTEM_PARAM::FIXED://�̶����Ϊ0
				{
					CalFirstAngle = 0.0;  
				}
				break;
			case SYSTEM_PARAM::UNFIXED:
				{
					CalFirstAngle = 3.1416;//�ƶ����Ϊ180
				}
				break;
			}
			double ToolFirstX = m_stSysParam.ToolCalOriX[CamIndex][AixsIndex][ToolIndex];
			double ToolFirstY = m_stSysParam.ToolCalOriY[CamIndex][AixsIndex][ToolIndex];
			double CalFirstX = m_stSysParam.CamCalOriX[CamIndex][AixsIndex][ToolIndex];
			double CalFirstY = m_stSysParam.CamCalOriY[CamIndex][AixsIndex][ToolIndex];

			double RotToCamX = m_stSysParam.RotpointToCamX[CamIndex][AixsIndex][ToolIndex];//��ת���ĵ�����ľ���
			double RotToCamY = m_stSysParam.RotpointToCamY[CamIndex][AixsIndex][ToolIndex];
			double CamRotAngle = m_stSysParam.ToolCalOriPhi[CamIndex][AixsIndex][ToolIndex]-
				m_stSysParam.CamCalOriPhi[CamIndex][AixsIndex][ToolIndex]; //�����PhiӦ����Angle
			if (CamRotAngle>-0.1||CamRotAngle<0.1)//û�õ�
			{
				CamRotAngle = 0.0;
			}
			CamRotAngle = CamRotAngle*3.1416/180;  //���ڻ����˼����������ı궨

			//���ʹ�õ�����������������ش�С�趨Ϊ1,������;
			double CalPixSize = PixSize;
			if (m_stSysParam.CalWithCamParam[CamIndex][AixsIndex][ToolIndex])
			{
				CalPixSize = 1;
			}

			//����Ϊ:�������,�궨������,��Ե�һ����ľ�������x��y,ģ����������x,y,���ش�С(�����޵�λ),��ת����,����,
			//��Ծ���,���Ծ���,����ڲ�(12),�ڲ�����,�����Σ�7),��������,������,�����һ����x,y,����̶�(0)���̶�(3.14),������һ����x,y,
			//�����һ��������ߵ�һ���ĽǶ�,��ת���ĵ�����ľ���x��y,�궨����
			m_pVisionInstance->CalCamMetrix(CamIndex,index+1,m_stSysParam.CameraCalMachineX[CamIndex], m_stSysParam.CameraCalMachineY[CamIndex],
				CamRow,CamCol,CalPixSize,MechAndCamRot,MechAndCam,RelaImageToMotor,AbsImageToMotor,
				m_stSysParam.CamParam[CamIndex][AixsIndex][ToolIndex],12,
				m_stSysParam.CamPose[CamIndex][AixsIndex][ToolIndex],ToolIndex,AixsIndex,CalFirstX,CalFirstY,CalFirstAngle,ToolFirstX,
				ToolFirstY,CamRotAngle,RotToCamX,RotToCamY,m_stSysParam.CalThickNess[CamIndex][AixsIndex][ToolIndex]);
			//int BeginIndex = result.iCount;
			//�����еϵ�����ϵ�ĽǶȱ任���󣬣�ƽ�Ƶ�ԭ���غϣ������ƶ���			   
			SaveMecAndCamRot(CamIndex,AixsIndex,ToolIndex,MechAndCamRot,MechAndCam,RelaImageToMotor,
				AbsImageToMotor,sizeof(MechAndCamRot)/sizeof(double));
			memcpy(m_stSysParam.CamInMecRotMetix[CamIndex][AixsIndex][ToolIndex],MechAndCam,sizeof(MechAndCam));
			memcpy(m_stSysParam.CamRotMetix[CamIndex][AixsIndex][ToolIndex],MechAndCamRot,sizeof(MechAndCamRot));
			memcpy(m_stSysParam.ImageRelative[CamIndex][AixsIndex][ToolIndex],RelaImageToMotor,sizeof(RelaImageToMotor));
			memcpy(m_stSysParam.ImageAbsolute[CamIndex][AixsIndex][ToolIndex],AbsImageToMotor,sizeof(AbsImageToMotor));
			//����ͼ���������ص�
			m_stSysParam.PicCenterPointX[CamIndex][AixsIndex][ToolIndex]= CamRow[0];
			m_stSysParam.PicCenterPointY[CamIndex][AixsIndex][ToolIndex]= CamCol[0];
			SavePicCenter(CamIndex,AixsIndex,ToolIndex,CamRow[0],CamCol[0]);
		}
		//���3���궨�㣬��ô���ǵ�2������㣬�˷�֧û��,��ע��
		//if ( m_stSysParam.CalPointNum[CamIndex][AixsIndex][pConfig->MotorToolIndex]-2 == index)  //pConfig->CalPointNum-1
		if(0)
		{
			// ���һ���궨�㣬����ת���ʱ���������ϵ�е�λ������; ������Σ�����Ҫ����ת����оߣ���Ʒ�����λ�ˡ�
			double CenX,CenY;
			if (CamDisPhi>0.017||CamDisPhi<-0.017)//��һ�����ǰһ�������ص�֮��ĽǶ�
			{
				CenX = m_stSysParam.CameraCalPixCol[CamIndex][index-1] * PixSize;//��Ʒ������е�λ��,Match value
				CenY = m_stSysParam.CameraCalPixRow[CamIndex][index-1] * PixSize;//
				MyFunClass::CalRotCenterTool(CenX,CenY,m_stSysParam.CameraCalPixCol[CamIndex][index]*PixSize,
					m_stSysParam.CameraCalPixRow[CamIndex][index] * PixSize,-CamDisPhi); //CamDisPhiȡ��������ͼ���еķ���������ϵ���෴��

				m_stSysParam.RotpointToCamX[CamIndex][AixsIndex][ToolIndex] = CenX;
				m_stSysParam.RotpointToCamY[CamIndex][AixsIndex][ToolIndex] = CenY;
				SaveSysRotAxleToCam(CamIndex,AixsIndex,ToolIndex,CenX,CenY);
				//����ת����оߣ���Ʒ�����λ��
				//2,��Ʒ���оߣ�������е�λ��

				double ToolX = m_stSysParam.CameraCalPixCol[CamIndex][index] * PixSize;
				double ToolY = m_stSysParam.CameraCalPixRow[CamIndex][index] * PixSize;
				double ToolPhi = -atof(g_SceneProcResult->szValue[2]);
				m_pVisionInstance->AffineCoorPoint(ToolX,ToolY,ToolPhi,CenX,CenY);

			}else
			{
				CenX = 0.0;
				CenY = 0.0;
			}
			m_stSysParam.RotX[AixsIndex][pConfig->MotorToolIndex] = CenX;
			m_stSysParam.RotY[AixsIndex][pConfig->MotorToolIndex] = CenY;
			SaveSysRotAxleToTool(AixsIndex,pConfig->MotorToolIndex,CenX,CenY);

			sprintf_s(g_SceneProcResult->szValue[4],"%d",0);//home
			sprintf_s(g_SceneProcResult->szValue[5],"%d",0); // finish

		}else if(m_stSysParam.CalPointNum[CamIndex][AixsIndex][pConfig->MotorToolIndex] == index)
		{
			sprintf_s(g_SceneProcResult->szValue[4],"%d",1);
			sprintf_s(g_SceneProcResult->szValue[5],"%d",1);
		}else
		{
			sprintf_s(g_SceneProcResult->szValue[4],"%d",0);//  home
			sprintf_s(g_SceneProcResult->szValue[5],"%d",0); // finish
		}

		if (pConfig->CalPointIndex == m_stSysParam.CalPointNum[CamIndex][AixsIndex][pConfig->MotorToolIndex] )
		{
			pConfig->CalPointIndex = 0;
		}
		pConfig->CalPointIndex++;
		//step5 ���ͼ������ź�	
		sprintf_s(g_SceneProcResult->szValue[0],"%d",1);
		sprintf_s(g_SceneProcResult->szValue[1],"%.4lf",m_stSysParam.CalMotorX[CamIndex][AixsIndex][pConfig->MotorToolIndex][index]);
		sprintf_s(g_SceneProcResult->szValue[2],"%.4lf",m_stSysParam.CalMotorY[CamIndex][AixsIndex][pConfig->MotorToolIndex][index]);
		sprintf_s(g_SceneProcResult->szValue[3],"%.4lf",m_stSysParam.CalMotorPhi[CamIndex][AixsIndex][pConfig->MotorToolIndex][index]);
	}
	else
	{
		sprintf_s(g_SceneProcResult->szValue[1],"%.4lf",0.0);
		sprintf_s(g_SceneProcResult->szValue[2],"%.4lf",0.0);
		sprintf_s(g_SceneProcResult->szValue[3],"%.4lf",0.0);
		sprintf_s(g_SceneProcResult->szValue[0],"%d",0);
		if (m_stSysParam.IsSimulate)
		{
			sprintf_s(g_SceneProcResult->szValue[0],"%d",1);		 
			if(m_stSysParam.CalPointNum[CamIndex][AixsIndex][pConfig->MotorToolIndex]-1 == index)
			{
				sprintf_s(g_SceneProcResult->szValue[4],"%d",1);
				sprintf_s(g_SceneProcResult->szValue[5],"%d",1);
			}else
			{
				sprintf_s(g_SceneProcResult->szValue[4],"%d",0);
				sprintf_s(g_SceneProcResult->szValue[5],"%d",0);
			}

		}else
		{
			index = pConfig->CalPointIndex = 0;
			sprintf_s(g_SceneProcResult->szValue[4],"%d",1);
			sprintf_s(g_SceneProcResult->szValue[5],"%d",1);
		}		
	}
	g_SceneProcResult->iCount= 6;
	g_SceneProcResult->SeneTpye = CALIB_CAMERA;
	g_SceneProcResult->iCount = 6;
	Sleep(100);
	int LocalSceneID = pData->iSceneID;
	ManagerProcFinish(LocalSceneID, g_SceneProcResult, iError);
	return g_SceneProcResult->bIsDisp;
}
/**********************************************************************
����      : OpenWindow
����      : ��halcon������UI����
�������  : iWinID ���洰��ID
x,y,width,hight ���洰��λ�ô�С
�������  : 
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::BindWindow(long iWinID, int x, int y, int width, int hight, bool bDebug)
{
	long halconID = -1;
	map<long ,long>::iterator it;; 
	it=m_mapWinID.find(iWinID);
	if(it!=m_mapWinID.end())
	{
		halconID = it->second;
		if (-1 != halconID)
		{
			m_pVisionInstance->CloseHalconWindow(halconID);
		}
	}
	m_pVisionInstance->OpenWindow(iWinID, x, y, width, hight, halconID);
	m_mapWinID.insert(make_pair(iWinID,halconID));
	if (bDebug)
	{
		m_CalibWindID = halconID;
	}
	return true;
}

bool CManagerCtrl::SetDebugWindowID(int iSceneID, long iWinID)
{
	bool bRet = false;
	SCENE_PKG *pConfig = NULL;
	map<long ,long>::iterator it;
	long halconID = -1;
	//��ȡ����·��--��Ҫ�õ��ù�ϵ�Ļ��� ֻ���ǵ������
	pConfig = GetSceneHandle(iSceneID);

	it=m_mapWinID.find(iWinID);
	//������IDת����halcon����ID
	if(it!=m_mapWinID.end())
	{
		halconID = it->second;
		if (-1 != halconID)
		{
			m_CalibWindID = halconID;
			pConfig->halconID = m_CalibWindID;
			bRet = true;
		}
	}
	return bRet;
}

/**********************************************************************
����      : SetSysParam
����      : �����Ӿ�����
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
pData    Ҫ����ֵ�Ľṹ��ָ��
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::SetSysParam(void *pData)
{
	SYSTEM_PARAM *pSys = (SYSTEM_PARAM *)pData;
	if (NULL == pSys)
	{
		return false;
	}

	if (NULL != pSys)
	{
		m_stSysParam.SystemParamCopy(pSys);
	}
	return true;
}


void CManagerCtrl::SetProctocolData(const PROCTOCOLDATA& ProctoData)
{
	m_stSysParam.stSocketParam.CalRevHead = ProctoData.CalRevHead;
	m_stSysParam.stSocketParam.WorkRevHead = ProctoData.WorkRevHead ;
	m_stSysParam.stSocketParam.CalSendHead = ProctoData.CalSendHead;
	m_stSysParam.stSocketParam.WorkSendHead = ProctoData.WorkSendHead;
	m_stSysParam.stSocketParam.DividLabel = ProctoData.IPDivid;
	m_stSysParam.stSocketParam.EndLabel = ProctoData.IPEnd;
	//���浽�����ļ�
	SaveProctocolData(ProctoData);


}

void CManagerCtrl::SetProductMap(void* pMapData)
{
	//���泡��ӳ�� 
	ConfigSaveProductMap(pMapData);


	//�����Ʒ���Ƽ���ID
	//LPPMAPDATA lpData = (LPPMAPDATA)pMapData;
	//PMAPDATA::iterator it;
	//ConfigSaveProductId();
	//for (it = lpData->begin();it!=lpData->end();it++)
	//{
	//     ConfigSaveProductId(GenProductId(it->first),it->first);
	//}



}

void CManagerCtrl::GetProductMap(void* pMapData)
{
	ConfigReadProductMap(pMapData);


}




/**********************************************************************
����      : SaveSysParam
����      : ���²������Ӿ�����
�������  : N/A 
pData    Ҫ����ֵ�Ľṹ��ָ��
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::SaveSysParam(void *pData)
{
	if (NULL != pData)
	{
		SetSysParam(pData);
	}
	SaveSysConfig(pData);
	return true;
}

/**********************************************************************
����      : GetSysParam
����      : ��ȡϵͳ����
�������  : pDataҪ��ȡ�Ľṹ��ָ��
�������  : pDataҪ��ȡ�Ľṹ��ָ��
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::GetSysParam(void *pData)
{
	SYSTEM_PARAM *pSys = (SYSTEM_PARAM *)pData;
	if (NULL != pSys)
	{
		pSys->SystemParamCopy(&m_stSysParam);
	}
	return true;
}


void* CManagerCtrl::GetSysParamPoint()
{
	return  &m_stSysParam;
}


char* CManagerCtrl::GetSysconfigPath()
{
	return ConfigGetsysPath();

}


char* CManagerCtrl::GetSceneName()
{
	return ConfigGetSceneName();
}

/**********************************************************************
����      : SetSceneParam
����      : �����Ӿ�����
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
pData    Ҫ����ֵ�Ľṹ��ָ��
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::SetSceneParam(void *pData)
{
	bool bRet = true;
	int iSceneID;
	if (NULL == pData)
	{
		return false;
	}
	SCENE_PKG *pconfig = (SCENE_PKG *)pData;
	vector<SCENE_PKG *>::iterator it;
	bool bIsFinde = false;
	iSceneID = pconfig->sceneID;

	//����б����иó�������������£�û�������
	for (it = m_vecSceneParam.begin(); it != m_vecSceneParam.end(); it++)
	{
		if (iSceneID == (*it)->sceneID)
		{
			(*it)->SceneCopy(pconfig);
			bIsFinde = true;
			break;
		}
	}
	if (!bIsFinde)
	{
		m_stSysParam.iSceneNum++;
		m_stSysParam.pstrSceneName[iSceneID] = pconfig->szSceneName;
		SCENE_PKG *pNewConfig = new SCENE_PKG;
		*pNewConfig = *pconfig;
		pNewConfig->vecBody = pconfig->vecBody;
		// pNewConfig->pReserve = pconfig->pReserve;
		pNewConfig->vecDataType = pconfig->vecDataType;
		pNewConfig->vecInfo = pconfig->vecInfo;
		m_vecSceneParam.push_back(pNewConfig);
	}
	return bRet;
}





/**********************************************************************
����      : SetSceneParam
����      : �����Ӿ�����
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
pData    Ҫ����ֵ�Ľṹ��ָ��
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::SaveSceneParam(int iSceneID, void *pData)
{
	bool bRet = false;
	SCENE_PKG *pConfig = NULL;
	if (NULL != pData)
	{
		SetSceneParam(pData);
		pConfig = (SCENE_PKG *)pData;
	}
	else
	{
		pConfig = GetSceneHandle(iSceneID);

	}
	if (NULL != pConfig)
	{
		SetSceneConfig(m_stSysParam.pstrSceneName[iSceneID].c_str(), pConfig);
		bRet = true;
	}
	return bRet;
}

/**********************************************************************
����      : GetSceneParam
����      : �����Ӿ�����
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
�������  : pData    Ҫ��ȡ�Ľṹ��ָ��
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::GetSceneParam(int iSceneID, void *pData)
{
	bool bRet = true;
	SCENE_PKG *pConfig = NULL;
	pConfig = GetSceneHandle(iSceneID);
	if (NULL == pConfig)
	{
		bRet = false;
	}
	else
	{
		if (NULL != pData)
		{
			((SCENE_PKG *)pData)->SceneCopy(pConfig);
			//(*(SCENE_PKG *)pData) = *pConfig;
		}
	}
	return bRet;
}

char* CManagerCtrl::GetSceneconfigPath(int SceneID)
{
	//char SceneFolderName[256]={0};

	for (int i = 0;i<m_vecSceneParam.size();i++)
	{
		if (SceneID == m_vecSceneParam.at(i)->sceneID)
		{
			return  ConfigGetScenePath( m_vecSceneParam.at(i)->szSceneName);

		}
	}
	return NULL;

	//sprintf(SceneFolderName,"%s",m_stSysParam.pstrSceneName[SceneIndex]);
	//return ConfigGetScenePath(SceneFolderName);
}

char* CManagerCtrl::GetScenePath(int SceneID)
{
	for (int i = 0;i<m_vecSceneParam.size();i++)
	{
		if (SceneID == m_vecSceneParam.at(i)->sceneID)
		{
			char szBuf[64] = {0};
			char szName[64] = {0};
			memset(m_ScenePath,0,sizeof(m_ScenePath));   
			get_module_path(m_ScenePath);  //ȡ��config.ini�ĵ�ַ
			strcat_s(m_ScenePath,m_vecSceneParam.at(i)->szSceneName);
			if(-1 == access(m_ScenePath, 0))//ini�ļ�������
			{
				return NULL;
			}else
			{
				return m_ScenePath;//szCurrentPath
			}		
		}
	}
	return NULL;
}




int CManagerCtrl::GetSceneNum()
{
	return  m_stSysParam.iSceneNum;

}

int CManagerCtrl::GetSceneProcType(int SceneIndex)
{
	if (m_vecSceneParam.size()<= SceneIndex)
	{
		return -1;
	}
	return  m_vecSceneParam.at(SceneIndex)->ProcProcess;

}

int	CManagerCtrl::GetSceneId(int SceneIndex)
{
	if (m_vecSceneParam.size()<= SceneIndex)
	{
		return -1;
	}
	//��ȡ������ʱ��,�ǰ������ļ����˳��������id��,����������
	return  m_vecSceneParam.at(SceneIndex)->sceneID;

}


/**********************************************************************
����      : GrabWay2Sceneimage
����      : �����·�����ȡͼƬ����
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
index ��ǰ�����еĵڼ���ͼ�񣨴��ڶ���ͼ������ܸ���index���ֲ�ͬ��Դ���ͼ������
.....��Դ���ع� �Ȳ�����

�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
void CManagerCtrl::SetCamParam(int CamIndex,int shutterTime,int Gain)
{
	string strCameraGuid;
	strCameraGuid = m_pInstance->m_stSysParam.pstrCamareGUID[CamIndex];  
	// �������shutter
	if (getExposureTime(strCameraGuid) != shutterTime)
	{	
		setExposureTime(strCameraGuid, shutterTime);
		Sleep(200);
	}
	//�����������
	if (getGain(strCameraGuid) != Gain)
	{		
		setGain(strCameraGuid, Gain);
		Sleep(200);
	}
}

bool CManagerCtrl::GrabWay2Sceneimage(int SceneID,int ImageIndex,int CamIndex,int shutterTime,int Gain,int Interval,int LightIndex,int LightValue)
{
	//ImageIndex��ʶ��ǰ�����еĵڼ���ͼ��ɼ����Ĵ�����
	bool bRet = false;
	SCENE_PKG *pscene;
	int  iErrorCode = 0;
	int  iTimeCircle = 0;
	char szBuf[126] = {0};
	char *pBuf = NULL;
	string strCameraGuid;
	int    width = 0;
	int    height = 0;
	int    iCameraIndex = 0;
	int    iLightIndex = 0;
	int    iShutter = 0;
	pscene = GetSceneHandle(SceneID);	
	strCameraGuid = m_pInstance->m_stSysParam.pstrCamareGUID[CamIndex];  
	// �������shutter
	if (getExposureTime(strCameraGuid) != shutterTime)
	{		
		setExposureTime(strCameraGuid, shutterTime);
		Sleep(Interval);//��Ƶ�����ò�����ȥҪ��ʱ��
	}
	//�����������
	if (getGain(strCameraGuid) != Gain)
	{		
		setGain(strCameraGuid, Gain);
		Sleep(Interval);
	}
	//��ȡͼ������
	if (NULL != pscene && 0 != pscene->bIsSimulate) //��Ϊģ��Ͳ�ץͼ
	{
		char szBuf[256] = {0};
		void *pBuf = NULL;
		pBuf = m_pInstance->m_pVisionInstance->GetImageBuf(SceneID, ImageIndex,(int)pscene->defcamwidth, (int)pscene->defcamheight);
		size_t  buffSize = strlen((char*)pBuf);
		//���buf
		if (pBuf == NULL)
		{
			return false;
		}
		sprintf_s(szBuf, ".\\%s\\demo%d.jpg", m_pInstance->m_stSysParam.pstrSceneName[SceneID].c_str(), CamIndex);
		if (0 == access(szBuf, 0))
		{
			m_pInstance->m_pVisionInstance->ReadImage(szBuf, SceneID, CamIndex, false);
		}
		return true;
	}

	if (CAMERA_SUCCESS == getImageSize(strCameraGuid, width, height))
	{
		void *pBuf = NULL;
		//��ȡ����ͼ�����ݵ��ڴ�
		pBuf = m_pInstance->m_pVisionInstance->GetImageBuf(SceneID, ImageIndex,width, height);		
		if (pBuf == NULL)
		{
			return false;
		}
		size_t bufferSize =  strlen((char*)pBuf);
		if (0 != getImage(strCameraGuid, pBuf))
		{
			LOG_ERR("ץͼʧ��");
			sprintf_s(szBuf, "ץͼʧ��(%d)", SceneID);
			msg_post(MSG_DISP_WORKING_INFO, szBuf,(unsigned int)strlen(szBuf)+1, MODULE_SCENE_MAIN_UI); 
		}
		else
		{
			bufferSize =  strlen((char*)pBuf);
			bRet = true;

		}
	}
	return bRet;
}


/**********************************************************************
����      : LightCtrl
����      : ���ع�Դ
�������  : index ��Դ���
iType 0Ϊ�أ� 1Ϊ��
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::LightCtrl(int index, int iType)
{
	bool bRet = false;
	int iRet = -1;
	if (0 < m_stSysParam.iAutoOL)
	{
		string strComm;
		int    iChannel = -1;
		for (int i = 0; i < m_stSysParam.iLightNum; i++)
		{
			if (index < m_stSysParam.strLightInfo[i].Channel)
			{
				iChannel = index;
				strComm = m_stSysParam.strLightInfo[i].PortName;
				break;
			}
			else
			{
				index = index - m_stSysParam.strLightInfo[i].Channel;
			}
		}
		if (-1 != iChannel)
		{
			if (GetCurrentStatus(INIT_LIGHT_OK))
			{
				if (1 == iType)
				{
					iRet = openLight(strComm,iChannel);
				}
				else
				{
					iRet = closeLight(strComm, iChannel);
				}
				if (0 == iRet)
				{
					bRet = true;
				}
				else
				{
					bRet = false;
				}
			}
		}
		else
		{
			bRet = false;
		}
	}
	else
	{
		//����ʧ���򲻷���
		if (GetCurrentStatus(INIT_CONNECT_OK))
		{
			bRet = OpenLight(index, iType);
		}
	}
	return bRet;
}

//�ɷ���
bool CManagerCtrl::InitGrabSceneimage(int iSceneID, int index)
{
	bool bRet = false;
	SCENE_PKG *pscene;
	int  iErrorCode = 0;
	int  iTimeCircle = 0;

	//step1 ���ݳ���ID�ҵ���Ӧ����
	pscene = GetSceneHandle(iSceneID);

	int iPos = m_pVisionInstance->FindParamData(pscene, MAIN_PRO, index);
	if (NULL == pscene || iPos < 0 || index >= pscene->vecInfo.size())
	{
		return false;
	}

	//step2 �ɼ�ͼ�񣬽�����֪ͨ�ɼ����
	do
	{
		char *pBuf = NULL;
		string strCameraGuid;
		int    width = 0;
		int    height = 0;
		int    iCameraIndex = pscene->vecInfo[index].CameraIndex;
		strCameraGuid = m_pInstance->m_stSysParam.pstrCamareGUID[iCameraIndex];  

		if (NULL != pscene && 0 != pscene->bIsSimulate) //��Ϊģ��Ͳ�ץͼ
		{
			char szBuf[256] = {0};
			void *pBuf = NULL;
			pBuf = m_pInstance->m_pVisionInstance->GetImageBuf(iSceneID, index,(int)pscene->defcamwidth, (int)pscene->defcamheight);
			size_t  buffSize = strlen((char*)pBuf);
			//���buf
			if (pBuf == NULL)
			{
				return false;
			}
			sprintf_s(szBuf, ".\\%s\\demo%d.jpg", m_pInstance->m_stSysParam.pstrSceneName[iSceneID].c_str(), index);
			if (0 == access(szBuf, 0))
			{
				m_pInstance->m_pVisionInstance->ReadImage(szBuf, iSceneID, index, false);
			}
			return true;
		}
		//��ȡͼ�����棬��ȡ�������
		if (CAMERA_SUCCESS == getImageSize(strCameraGuid, width, height))
		{
			void *pBuf = NULL;
			//��ȡ����ͼ�����ݵ��ڴ�
			pBuf = m_pInstance->m_pVisionInstance->GetImageBuf(iSceneID, index,width, height);
			size_t  buffSize = strlen((char*)pBuf);
			//���buf
			if (pBuf == NULL)
			{
				return false;
			}
			if(0 != getImage(strCameraGuid, pBuf))
			{
				bRet = FALSE;
			}
			buffSize = strlen((char*)pBuf);
		}
		else
		{
			bRet = FALSE;
		}

		bRet = true;
	}while(0);

	return bRet;
}



bool CManagerCtrl::InitGrabSceneimage1(int CamIndex ,int iSceneID,int ImageIndex)
{
	bool bRet = false;
	int    width = 0;
	int    height = 0;
	string strCameraGuid;
	strCameraGuid = m_pInstance->m_stSysParam.pstrCamareGUID[CamIndex];  

	SCENE_PKG *pscene = GetSceneHandle(iSceneID); //��ȡ������

	if (NULL != pscene && 0 != pscene->bIsSimulate) //��Ϊģ��Ͳ�ץͼ
	{
		int index = 0;//����ÿ����������һ��ͼƬ����Ϊdemo0.jpg
		char szBuf[256] = {0};
		void *pBuf = NULL;
		//��ȡ����ͼ�����ݵ��ڴ�
		pBuf = m_pInstance->m_pVisionInstance->GetImageBuf(iSceneID, ImageIndex,(int)pscene->defcamwidth, (int)pscene->defcamheight);
		size_t  buffSize = strlen((char*)pBuf);
		//���buf
		if (pBuf == NULL)
		{
			return false;
		}
		sprintf_s(szBuf, ".\\%s\\demo%d.jpg", m_pInstance->m_stSysParam.pstrSceneName[iSceneID].c_str(), index);
		if (0 == access(szBuf, 0))
		{
			m_pInstance->m_pVisionInstance->ReadImage(szBuf, iSceneID, index, false);
		}
		return true;
	}


	if (CAMERA_SUCCESS == getImageSize(strCameraGuid, width, height))
	{
		void *pBuf = NULL;
		//��ȡ����ͼ�����ݵ��ڴ�
		pBuf = m_pInstance->m_pVisionInstance->GetImageBuf(iSceneID, ImageIndex,width, height);
		size_t  buffSize = strlen((char*)pBuf);
		//���buf
		if (pBuf == NULL)
		{
			return false;
		}




		if(0 != getImage(strCameraGuid, pBuf))
		{
			bRet = FALSE;
		}		
		bRet = true;
	}
	else
	{
		bRet = FALSE;
	}
	return bRet;
}


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
bool CManagerCtrl::SetGain(int iSceneID, int iValue, int index)
{
	string strCameraGuid;
	SCENE_PKG *pscene;

	//step1 ���ݳ���ID�ҵ���Ӧ����
	pscene = GetSceneHandle(iSceneID);
	int iPos = m_pVisionInstance->FindParamData(pscene, MAIN_PRO, index);
	if (NULL == pscene || iPos < 0 || index >= pscene->vecInfo.size())
	{
		return false;
	}
	strCameraGuid = m_pInstance->m_stSysParam.pstrCamareGUID[pscene->vecInfo[index].CameraIndex];  
	setGain(strCameraGuid, iValue);
	return true;
}

/**********************************************************************
����      : GetGain
����      : ��ȡ�����ͼ����
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
index ��ǰ�����еĵڼ���ͼ�񣨴��ڶ���ͼ������ܸ���index���ֲ�ͬ��Դ���ͼ������
�������  : N/A
����ֵ    : ��ȡ������ֵ
��ע      : 
**********************************************************************/
int CManagerCtrl::GetGain(int iSceneID, int index)
{
	string strCameraGuid;
	SCENE_PKG *pscene;
	int   iGainValue = 0;

	//step1 ���ݳ���ID�ҵ���Ӧ����
	pscene = GetSceneHandle(iSceneID);
	int iPos = m_pVisionInstance->FindParamData(pscene, MAIN_PRO, index);
	if (NULL == pscene || iPos >= 0 || index >= pscene->vecInfo.size())
	{
		return false;
	}
	strCameraGuid = m_pInstance->m_stSysParam.pstrCamareGUID[pscene->vecInfo[index].CameraIndex];  
	GetGain(iGainValue);
	return iGainValue;
}
/**********************************************************************
����      : ReadImage
����      : ���ر���ͼƬ
�������  : iSceneID  
pFileName �ļ�����
index ��ǰ�����еĵڼ���ͼ�񣨴��ڶ���ͼ������ܸ���index���ֲ�ͬ��Դ���ͼ������
�������  : 
����ֵ    : �ɹ���ʧ�� 
��ע      : 
**********************************************************************/
bool CManagerCtrl::ReadImage(int iSceneID, const char *pFileName, int index)
{
	m_pVisionInstance->ReadImage(pFileName, iSceneID, index);
	return true;
}

bool CManagerCtrl::SaveImage(int iSceneID,  int index, string SNstr)
{
	bool bRet = false;
	char szPath[MAX_PATH] = {0};
	//·��Ϊ:    /����·��/RawImage/ʱ��(����)/������/
	SYSTEMTIME st;
	GetLocalTime(&st);
	SCENE_PKG  Getsceneparam;
	SYSTEM_PARAM   Sparam;
	GetSysParam(&Sparam);
	GetSceneParam(iSceneID, &Getsceneparam);//��ȡ�ṹ������
	strcpy_s(szPath,256, Sparam.ImageSavePath);
	if (create_path(szPath))
	{
		LOG_ERR("access %s failed when save pic",szPath);
		return false;
	}

	sprintf_s(szPath, "%s/%s", szPath,"RawImage");
	if (create_path(szPath))
	{
		LOG_ERR("access %s failed when save pic",szPath);
		return false;
	}
	sprintf_s(szPath, "%s/%04d-%02d-%02d", szPath, st.wYear, st.wMonth, st.wDay);
	if (create_path(szPath))
	{
		LOG_ERR("access %s failed when save pic",szPath);
		return false;
	}
	sprintf_s(szPath, "%s/%s", szPath, Getsceneparam.szSceneName);
	if (create_path(szPath))
	{
		LOG_ERR("access %s failed when save pic",szPath);
		return false;
	}
	SYSTEMTIME lt;
	char temPath[_MAX_PATH*2] = {0};
	GetLocalTime(&lt);
	if (SNstr.empty())
	{
		sprintf_s(temPath,"%02d_%02d_%02d_%02d.jpg",lt.wHour,lt.wMinute,lt.wSecond,lt.wMilliseconds);
		SNstr = temPath;
	}
	sprintf_s(temPath,"%s/%s",szPath,SNstr.c_str());
	CHECK_RESULT *res = GetSceneProcResultById(iSceneID);
	if (NULL == res)
	{
		LOG_ERR("access %s failed when save pic",szPath);
		return false;
	}
	res->rawimg = temPath;
	bRet = m_pVisionInstance->saveImage(iSceneID, index, szPath, SNstr);
	return bRet;
}
/**********************************************************************
����      : SaveImageDump
����      : ���洰��ͼ��
�������  : winID ��ǰ����ID
iSceneID ����ID
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
�޸ļ�¼��
����        �޸���                  �޸�����
2016-08-16  guopengzhou            visionProcessʵ������ͼƬ�������·��,�Ա�ͨѶЭ���ȡ
**********************************************************************/
bool CManagerCtrl::SaveImageDump( const long winID,int iSceneID,char* outpath )
{
	bool bRet = NULL;
	char szPath[256] = {0};
	long halconID = -1;
	map<long ,long>::iterator it;; 
	//·��Ϊ:    /����·��/ResultImage/ʱ��(����)/������/
	SCENE_PKG  *sceneparam = NULL;
	SYSTEM_PARAM   *Sparam = NULL;
	SYSTEMTIME st;
	char temPath[_MAX_PATH*2] = {0};
	GetLocalTime(&st);
	Sparam = (SYSTEM_PARAM   *)GetSysParamPoint();
	sceneparam = (SCENE_PKG  *)GetSceneParamHandle(iSceneID);
	strcpy_s(szPath, 256, Sparam->ImageSavePath);
	if (create_path(szPath))
	{
		LOG_ERR("access result pic dir%s failed when save pic",szPath);
		return false;
	}
	sprintf_s(szPath, "%s\\%s", szPath,"ResultImage");
	if (create_path(szPath))
	{
		LOG_ERR("access result pic dir%s failed when save pic",szPath);
		return false;
	}	
	sprintf_s(szPath, "%s\\%04d-%02d-%02d", szPath, st.wYear, st.wMonth, st.wDay);
	if (create_path(szPath))
	{
		LOG_ERR("access result pic dir%s failed when save pic",szPath);
		return false;
	}
	sprintf_s(szPath, "%s\\%s", szPath, sceneparam->szSceneName);
	if (create_path(szPath))
	{
		LOG_ERR("access result pic dir%s failed when save pic",szPath);
		return false;
	}

	//if (SNstr.empty())
	//{
	//	sprintf_s(temPath,"%02d_%02d_%02d_%02d.jpg",lt.wHour,lt.wMinute,lt.wSecond,lt.wMilliseconds);
	//	SNstr = temPath;
	//}
	//sprintf_s(temPath,"%s/%s",szPath,SNstr.c_str());
	//CHECK_RESULT *res = GetSceneProcResultById(iSceneID);
	//if (NULL == res)
	//{
	//	LOG_ERR("access %s failed when save pic",szPath);
	//	return false;
	//}

	it=m_mapWinID.find(winID);
	//������IDת����halcon����ID
	if(it!=m_mapWinID.end())
	{
		halconID = it->second;
		if (-1 != halconID)
		{
			bRet = m_pVisionInstance->saveWinImage(halconID, szPath,outpath,m_chSN);
			if (true == bRet)
			{
				bRet = m_pVisionInstance->saveImagePath(iSceneID, 0, outpath, m_chSN);

			}

		}
	}
	return bRet;
}

/**********************************************************************
����      : SaveImageDump
����      : ���洰��ͼ��
�������  : winID ��ǰ����ID
iSceneID  ����ID
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
void* CManagerCtrl::GetImageDump( const long winID,int iSceneID,double &width,double &heigh )
{
	bool bRet = false;
	long halconID = -1;
	map<long ,long>::iterator it;
	it=m_mapWinID.find(winID);
	char* buff = NULL;

	//������IDת����halcon����ID
	if(it!=m_mapWinID.end())
	{
		halconID = it->second;
		if (-1 != halconID)
		{
			buff = (char*)m_pVisionInstance->getWinImage(halconID,width,heigh);
		}
	}
	return buff;
}

char* CManagerCtrl::getImagePath(int iSceneID, int index,unsigned int type)
{
	return m_pVisionInstance->getImagePath(iSceneID, index, type);
}

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
bool CManagerCtrl::DispImage(long iWinID, int iSceneID, int index)
{
	bool bRet = false;
	long halconID = -1;
	map<long ,long>::iterator it;; 
	it=m_mapWinID.find(iWinID);
	//������IDת����halcon����ID
	if(it!=m_mapWinID.end())
	{
		halconID = it->second;
		if (-1 != halconID)
		{
			bRet = m_pVisionInstance->DispImage(halconID,iSceneID, index);
		}
	}
	return bRet;
}

bool  CManagerCtrl::FindHalconWinID(int iWinID, long& HalconWinID)
{

	bool bRet = false;
	long halconID = -1;
	map<long ,long>::iterator it;; 
	it=m_mapWinID.find(iWinID);
	//������IDת����halcon����ID
	if(it!=m_mapWinID.end())
	{
		halconID = it->second;
		if (-1 != halconID)
		{
			HalconWinID = halconID;
			bRet = true;
		}
	}
	return bRet;
}

bool CManagerCtrl::ShowVisionString(long winID,char* StrContent,int topRow ,int topCol)
{

	return   m_pVisionInstance->ShowWindString(winID,StrContent,topRow,topCol);

}



bool CManagerCtrl::SetVisionColorHeight(long hwinID,char* strColor,int FontHeight)
{
	return	m_pVisionInstance->SetDispColorHeight(hwinID,strColor,FontHeight);
}





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
bool CManagerCtrl::DispObj(int iWinID, int iSceneID, bool bIsShowTmp)
{
	bool bRet = false;
	long halconID = -1;
	map<long ,long>::iterator it;; 
	it=m_mapWinID.find(iWinID);
	//������IDת����halcon����ID
	if(it!=m_mapWinID.end())
	{
		halconID = it->second;
		if (-1 != halconID)
		{
			bRet = m_pVisionInstance->DispObj(halconID,iSceneID, bIsShowTmp);
		}
	}
	return bRet;
}
/**********************************************************************
����      : DispROI
����      : ��Ϊ����ROIģʽ
�������  : iWinID ����ID
iSceneID  ����ID�����ݸ�ID��ȡ����
index ��ǰ�����еĵڼ���ROI
iType, 0,Ϊ����ͼ������, 1Ϊģ������2Ϊ��ֱ�ߣ� 3Ϊ��Բ�� ����Ϊö������
�������  : 
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::DispROI(int iWinID, int iSceneID, int index, PROCESS_TYPE iType)
{
	bool bRet = false;
	ST_ROI_INFO stInRoiInfo;
	long halconID = -1;
	SCENE_PKG *pConfig;
	bool bIsFinde = false;
	map<long ,long>::iterator it;
	it=m_mapWinID.find(iWinID);
	bool bIsRect = true;
	//������IDת����halcon����ID
	if(it!=m_mapWinID.end())
	{
		halconID = it->second;
		pConfig = GetSceneHandle(iSceneID);
		if (-1 != halconID && NULL != pConfig)
		{
			int iPos = m_pVisionInstance->FindParamData(pConfig, iType, index);
			if (iPos < 0 || iPos >= pConfig->vecBody.size() || iPos >= pConfig->vecDataType.size())
			{
				return bRet;
			}
			VIRSION_DATA_PKG *pData = pConfig->vecBody[iPos];

			iPos = FindSpecTypePos(pConfig->vecBody[iPos], pConfig->vecDataType[iPos], ROI_TYPE);
			switch (iType)
			{
			case MAIN_PRO:
			case MODEL_PRO:
			case LINE_PRO:
				{
					stInRoiInfo.strect.row = pData[iPos + 0].dValue;    //ROI  row
					stInRoiInfo.strect.col = pData[iPos + 1].dValue;    //ROI  col
					stInRoiInfo.strect.angle = pData[iPos + 2].dValue;  //ROI  angle
					stInRoiInfo.strect.length1 = pData[iPos + 3].dValue;  //ROI l1
					stInRoiInfo.strect.length2 = pData[iPos + 4].dValue;  //ROI l2
					bIsFinde = true;
				}
				break;
			case CIRCLE_PRO:
				{
					stInRoiInfo.stcircle.row = pData[iPos + 0].dValue;   //ROI  row
					stInRoiInfo.stcircle.col = pData[iPos + 1].dValue;   //ROI  col
					stInRoiInfo.stcircle.Radius = pData[iPos + 2].dValue;  //ROI R 
					stInRoiInfo.stcircle.width = pData[iPos + 3].dValue;   //ROI w
					stInRoiInfo.stcircle.angleB = pData[iPos + 4].dValue;  //ROI b 
					stInRoiInfo.stcircle.angleE = pData[iPos + 5].dValue;   //ROI e
					bIsRect = false;
					bIsFinde = true;
				}
				break;
			default:
				break;
			}
			if (bIsFinde)
			{
				bRet = m_pVisionInstance->dispRoi(iSceneID,halconID , stInRoiInfo, bIsRect);
			}
			else
			{
				bRet = bIsFinde;
			}

		}
	}
	return bRet;
}
/**********************************************************************
����      : DrawROI
����      : ֱ�ӻ���ROI
�������  : iWinID ����ID
iSceneID  ����ID�����ݸ�ID��ȡ����
iType, ��ֱ�ߣ� ��Բ��
�������  : 
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::OnDrawROI(int iWinID, int iSceneID, PROCESS_TYPE iType,RoiCombType CombType,vector<int>* pRoiIdList)
{
	bool bRet = false;
	ST_ROI_INFO stInRoiInfo;
	long halconID = -1;
	SCENE_PKG *pConfig;
	map<long ,long>::iterator it;
	it=m_mapWinID.find(iWinID);	 
	int  iSceneindex = iSceneID;
	//������IDת����halcon����ID
	if(it!=m_mapWinID.end())
	{
		halconID = it->second;
		pConfig = GetSceneHandle(iSceneID);
		if (-1 != halconID && NULL != pConfig)
		{
			int ImageWidth = 0,ImageHeight = 0;
			m_pVisionInstance->getimagesize(iSceneID,ImageWidth,ImageHeight);
			switch (iType)
			{
			case LINE_PRO:
				{
					stInRoiInfo.strect.row = ImageHeight/2;
					stInRoiInfo.strect.col = ImageWidth/2;
					stInRoiInfo.strect.angle = 0.0;
					stInRoiInfo.strect.length1 = ImageWidth / 20;
					stInRoiInfo.strect.length2 = ImageHeight /20;
				}
				break;
			case CIRCLE_PRO:
				{
					stInRoiInfo.stcircle.row =  ImageHeight/2;
					stInRoiInfo.stcircle.col = ImageWidth/2;
					stInRoiInfo.stcircle.Radius = ImageHeight/20;
					stInRoiInfo.stcircle.width =stInRoiInfo.stcircle.Radius/8; 
				}
				break;
			case COMBSHAPE_PRO:
				{

				}
				break;
			default:
				break;
			}
			bRet = m_pVisionInstance->OndrawRoi(iSceneID,halconID , stInRoiInfo, CombType, iType,pRoiIdList);
		}

	}

	return TRUE;
}



void CManagerCtrl::GenShape(int SceneID,const ST_ROI_INFO& roi,ShapeType shapeflag,int ShapeId,int CamIndex)
{
	string strCameraGuid;
	int widthI,heightI;
	strCameraGuid = m_pInstance->m_stSysParam.pstrCamareGUID[CamIndex];  

	//ͨ��������ȡ����,��Ϊ��ȡ��pain�����û�г�����
	if (CAMERA_SUCCESS == getImageSize(strCameraGuid, widthI, heightI)) 
	{
		m_pVisionInstance->GenShape(SceneID,roi,shapeflag,ShapeId,widthI,heightI);

	}
}



int CManagerCtrl::GetCurShapeId( int iSceneID)
{

	return m_pVisionInstance->GetCurShapeId(iSceneID);

}

bool  CManagerCtrl::GetRoiParam(int sceneID,int ShapeId,ST_ROI_INFO* pRoi)
{

	return m_pVisionInstance->GetRoiParam(sceneID,ShapeId,pRoi);

}



void CManagerCtrl::OnShowAllRoi(int iWinID, int iSceneID,const vector<int>* const pRoiDdList)
{
	long halconID = -1;
	//SCENE_PKG *pConfig;
	map<long ,long>::iterator it;
	it=m_mapWinID.find(iWinID);
	//bool bIsRect = true;

	//int  iSceneindex = iSceneID;
	//������IDת����halcon����ID
	if(it!=m_mapWinID.end())
	{
		halconID = it->second;
		m_pVisionInstance->OnShowAllRoi(iSceneID,halconID,pRoiDdList);

	}

}


/**********************************************************************
����      : DrawROI
����      : ��Ϊ����ROIģʽ
�������  : iWinID ����ID
iSceneID  ����ID�����ݸ�ID��ȡ����
index ��ǰ�����еĵڼ���ROI
iType, 0,Ϊ����ͼ������, 1Ϊģ������2Ϊ��ֱ�ߣ� 3Ϊ��Բ�� ����Ϊö������
�������  : 
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::DrawROI(int iWinID, int iSceneID, int index, PROCESS_TYPE iType)
{
	bool bRet = false;
	ST_ROI_INFO stInRoiInfo;
	ST_ROI_INFO stOutRoiInfo;
	long halconID = -1;
	SCENE_PKG *pConfig;
	map<long ,long>::iterator it;
	it=m_mapWinID.find(iWinID);
	bool bIsRect = true;
	int  iSceneindex = iSceneID;
	//������IDת����halcon����ID
	if(it!=m_mapWinID.end())
	{
		halconID = it->second;
		pConfig = GetSceneHandle(iSceneID);
		int iPos = m_pVisionInstance->FindParamData(pConfig, iType, index);


		if (iPos >= pConfig->vecBody.size() || iPos >= pConfig->vecDataType.size())
		{

			return false;
		}
		VIRSION_DATA_PKG *pData = pConfig->vecBody[iPos];
		if (-1 != halconID && NULL != pConfig)
		{
			//�ڵ�ǰ�����������ض����Ͳ�����һ�γ��ֵ�λ��
			iPos = FindSpecTypePos(pConfig->vecBody[iPos], pConfig->vecDataType[iPos], ROI_TYPE);
			//���ҳ���ID�Ƿ���Ч
			switch (iType)
			{
				//case MAIN_PRO:
				//    {
				//        stInRoiInfo.strect = pConfig->vecImageProInfo[index].roiRectangle;
				//    }
				//    break;
			case MODEL_PRO:
			case LINE_PRO:
				{
					stInRoiInfo.strect.row = pData[iPos].dValue;
					stInRoiInfo.strect.col = pData[iPos + 1].dValue;
					stInRoiInfo.strect.angle = pData[iPos + 2].dValue;
					stInRoiInfo.strect.length1 = pData[iPos + 3].dValue;
					stInRoiInfo.strect.length2 = pData[iPos + 4].dValue;
				}
				break;
			case CIRCLE_PRO:
				{
					stInRoiInfo.stcircle.row = pData[iPos + 0].dValue;
					stInRoiInfo.stcircle.col = pData[iPos + 1].dValue;
					stInRoiInfo.stcircle.Radius = pData[iPos + 2].dValue;
					stInRoiInfo.stcircle.width = pData[iPos + 3].dValue;
					bIsRect = false;
				}
				break;
			default:
				break;
			}
			bRet = m_pVisionInstance->drawRoi(iSceneID,halconID , stInRoiInfo, stOutRoiInfo, bIsRect);

			//���Ż�
			switch (iType)
			{
				//case MAIN_PRO:
				//    {
				//        m_vecSceneParam[iSceneindex].vecImageProInfo[index].roiRectangle = stOutRoiInfo.strect;
				//    }
				//    break;
			case MODEL_PRO:
			case LINE_PRO:
				{
					pData[iPos + 0].dValue = stOutRoiInfo.strect.row;
					pData[iPos + 1].dValue = stOutRoiInfo.strect.col;
					pData[iPos + 2].dValue = stOutRoiInfo.strect.angle;
					pData[iPos + 3].dValue = stOutRoiInfo.strect.length1;
					pData[iPos + 4].dValue = stOutRoiInfo.strect.length2;
				}
				break;
			case CIRCLE_PRO:
				{
					pData[iPos + 0].dValue = stOutRoiInfo.stcircle.row;
					pData[iPos + 1].dValue = stOutRoiInfo.stcircle.col;
					pData[iPos + 2].dValue = stOutRoiInfo.stcircle.Radius;
					pData[iPos + 3].dValue = stOutRoiInfo.stcircle.width;
					bIsRect = false;
				}
				break;
			default:
				break;
			}
		}
	}
	return bRet;
}

/**********************************************************************
����      : CreateVisionModel
����      : ����ģ��
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����
index ������ͼ�����  
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::CreateVisionModel(int iWinID,int iSceneID, int index)
{
	bool bRet = false;
	SCENE_PKG *pConfig;
	char szBuf[512] = {0};
	//���ҳ���ID�Ƿ���Ч
	pConfig = GetSceneHandle(iSceneID);
	int iPos = m_pVisionInstance->FindParamData(pConfig, MODEL_PRO, index);
	if (NULL != pConfig && iPos >= 0 && iPos < pConfig->vecBody.size() && iPos < pConfig->vecDataType.size())
	{
		long halconID = -1;
		map<long ,long>::iterator it;; 
		it=m_mapWinID.find(iWinID);
		//������IDת����halcon����ID
		if(it!=m_mapWinID.end())
		{
			halconID = it->second;
		}
		int iModelID = -1;
		get_module_path((LPSTR)szBuf);
		sprintf_s(szBuf, "%s%s\\model%d", szBuf,m_stSysParam.pstrSceneName[iSceneID].c_str(), index);
		//����ͼ����ӿڴ���ROI //���޸�
		iModelID = m_pVisionInstance->buildModel(halconID,iSceneID,index,pConfig,pConfig->szSceneName,szBuf);
		if ( -1 != iModelID)
		{
			//�ڵ�ǰ�����������ض����Ͳ�����һ�γ��ֵ�λ��
			VIRSION_DATA_PKG *pData = pConfig->vecBody[iPos];
			int idPos = FindSpecTypePos(pConfig->vecBody[iPos], pConfig->vecDataType[iPos], ID_TYPE);
			if (idPos >= 0 && -1 != pData[idPos].iValue)
			{
				int iType = FindSpecTypePos(pConfig->vecBody[iPos], pConfig->vecDataType[iPos], SPEC_TYPE);
				m_pVisionInstance->clearModel(iSceneID,pData[idPos].iValue, pData[iType].iValue);
			}
			pData[idPos].iValue = iModelID;
		}
	}
	return bRet;
}

bool CManagerCtrl::CreateVisionModel(int iWinID,int iSceneID,int RoiID,char* modePath, int Nevels,double AngleStart,double AngleExtent,
	int Contrast,int index )
{
	return  m_pVisionInstance->buildModel( iWinID, iSceneID, RoiID, modePath,  Nevels, AngleStart, AngleExtent,Contrast, index);
}


/**********************************************************************
����      : UpdateVisionModel
����      : ����ģ��
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
int CManagerCtrl::UpdateVisionModel(int iSceneID, int index, int iType)
{
	int iRet = false;
	SCENE_PKG *pConfig;
	char szBuf[512] = {0};
	//���ҳ���ID�Ƿ���Ч
	pConfig = GetSceneHandle(iSceneID);
	if (NULL != pConfig)
	{
		char szBuf[256] = {0};
		GetModuleFileName(NULL, szBuf, 255);
		string strFilePath = szBuf;
		int lastSpit = (int)strFilePath.find_last_of('\\');
		strFilePath.replace(lastSpit, string::npos, "");//���صĹ��̴���·��	
		sprintf_s(szBuf, "%s\\%s\\mypic",strFilePath.c_str(),pConfig->szSceneName);
		sprintf_s(szBuf, "%s\\%s\\model%d", strFilePath.c_str(),m_stSysParam.pstrSceneName[iSceneID].c_str(), index);
		iRet = m_pVisionInstance->readModel(iSceneID,pConfig->szSceneName,szBuf, index, iType);
	}
	return iRet;
}
/**********************************************************************
����      : FindVisionModel
����      : ����ģ��
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::FindVisionModel(int iSceneID, int index, bool bIsB)
{
	bool bRet = false;
	SCENE_PKG *pConfig = NULL;
	char szBuf[512] = {0};
	//���ҳ���ID�Ƿ���Ч
	pConfig = GetSceneHandle(iSceneID);
	if (NULL != pConfig)
	{
		ST_RECTANGLE_DATA rtData;
		int Score = 0;
		//�ж�ģ��ID�Ƿ���Ч
		int iPos = FindParamData(pConfig, MODEL_PRO, index);
		if (iPos >= pConfig->vecBody.size() || iPos >= pConfig->vecDataType.size())
		{
			return false;
		}
		int iW = FindSpecTypePos(pConfig->vecBody[iPos], pConfig->vecDataType[iPos], ID_TYPE);
		int iTypeP = FindSpecTypePos(pConfig->vecBody[iPos], pConfig->vecDataType[iPos], SPEC_TYPE);
		if (0 > pConfig->vecBody[iPos][iW].iValue)
		{
			int iType = pConfig->vecBody[iPos][iTypeP].iValue;
			pConfig->vecBody[iPos][iW].iValue = UpdateVisionModel(iSceneID, index, iType);
		}
		//����ͼ����ӿڻ�ȡģ��
		m_pVisionInstance->findModel(iSceneID, index,pConfig, rtData, 0);
		if (bIsB && pConfig->vecBody[iPos][iW].iValue >= 0)
		{
			iW = FindSpecTypePos(pConfig->vecBody[iPos], pConfig->vecDataType[iPos], ROI_TYPE);
			pConfig->vecBody[iPos][iW + 5].dValue = rtData.row;
			pConfig->vecBody[iPos][iW + 6].dValue = rtData.col;
			pConfig->vecBody[iPos][iW + 7].dValue = rtData.angle;
			SetSceneParam(pConfig);
		}
	}
	return true;
}
/**********************************************************************
����      : FindWay2VisionModel
����      : ����ģ��
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::FindWay2VisionModel(int iSceneID, int WinID,int index,char* szModePath,int PyramidLevel,int MatchNum,double& Score,
	int ProcIndex,bool IsBulid,double& row,double& col,double& phi)
{

	bool bRet = false;
	SCENE_PKG *pConfig = NULL;
	char szBuf[512] = {0};
	//���ҳ���ID�Ƿ���Ч
	//pConfig = GetSceneHandle(iSceneID);
	return m_pVisionInstance->FindWay2VisionModel(iSceneID,WinID,index,szModePath,PyramidLevel,MatchNum,Score, ProcIndex,IsBulid,row,col,phi);

}
bool CManagerCtrl::FindModelRealTime(int iSceneID, int WinID,int index,char* szModePath,int PyramidLevel,int MatchNum,double& Score,
	int ProcIndex,bool IsBulid,double& row,double& col,double& phi)
{

	bool bRet = false;
	SCENE_PKG *pConfig = NULL;
	char szBuf[512] = {0};
	//���ҳ���ID�Ƿ���Ч
	//pConfig = GetSceneHandle(iSceneID);
	return m_pVisionInstance->FindModelRealTime(iSceneID,WinID,index,szModePath,PyramidLevel,MatchNum,Score, ProcIndex,IsBulid,row,col,phi);

}

void CManagerCtrl::MoveAndRotInCamForOriMatch(int iSceneID,double PixSize,bool AxeMoveRotDirect,double& MotorRelX,
	double& MotorRelY,int LastSceneId,int NextSceneId,double SceneRotPhi,double PicCentX,double PicCentY,double RotCenX,double RotCenY,double MotorRotPhi,
	double CalRotPhi,double (*pCamParam),int ParamNum,double (*pCamPose),double CalThick,int CamIndex,
	int AxeIndex,int ToolIndex)
{
	m_pVisionInstance->MoveAndRotInCamForOriMatch(iSceneID,PixSize,AxeMoveRotDirect,MotorRelX, MotorRelY,
		LastSceneId,NextSceneId,SceneRotPhi,PicCentX,PicCentY,RotCenX, RotCenY, MotorRotPhi,CalRotPhi,pCamParam,ParamNum,pCamPose,CalThick,
		CamIndex, AxeIndex, ToolIndex);
}

bool CManagerCtrl::AffinePointByCamInMecRot(int CamIndex,int AxisIndex,int ToolIndex,double& PointX,double& PointY)
{
	return m_pVisionInstance->AffinePointByCamInMecRot( CamIndex, AxisIndex,ToolIndex, PointX, PointY);

}

void CManagerCtrl::AffToRobotWorld(int SceneID,int CamIndex,int AxisIndex,int ToolIndex,double PixSize,double ModRotPhi,
	double& ToolWorldX,double& ToolWorldY,bool WithCamParam,double CalThickness,double* pCamParam,int ParamNum,double* CamPos)
{

	m_pVisionInstance->AffToRobotWorld(SceneID,CamIndex,AxisIndex,ToolIndex,PixSize,ModRotPhi,ToolWorldX,ToolWorldY,
		WithCamParam,CalThickness,pCamParam,ParamNum,CamPos);

}


void CManagerCtrl::AffinePointToWorld(int CamIndex,int AxisIndex,int ToolIndex,double PixSize,double OriCamX,double OriCamY,
	double OriCamPhi,double CurCamX,double CurCamY,double CurCamPhi,double& MoveX,double& MoveY,int ResType,
	bool MoveRotDirect,double CurMatchPhi,double AxeRotX,double AxeRotY)
{

	m_pVisionInstance->AffinePointToWorld(CamIndex,AxisIndex,ToolIndex,PixSize,OriCamX, OriCamY,
		OriCamPhi,CurCamX, CurCamY, CurCamPhi,MoveX,MoveY,ResType,MoveRotDirect, CurMatchPhi, AxeRotX, AxeRotY);
}



/**********************************************************************
����      : VisionProcess
����      : ������
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::VisionGetResult(int iSceneID, int index,CHECK_RESULT &result)
{
	bool bRet = false;
	SCENE_PKG *pConfig = NULL;
	/*CHECK_RESULT stresult;*/
	vector<SCENE_PKG>::iterator it;
	bool bIsFinde = false;
	//���ҳ���ID�Ƿ���Ч
	//int IntTest = 8;
	pConfig = GetSceneHandle(iSceneID);
	if (NULL == pConfig)
	{
		return false;
	}
	//�жϴ���ķ�ʽ


	if (0 == pConfig->ProcProcess )
	{
		if(m_pInstance->m_SceneProcess(iSceneID,result))
		{	
			return true;
		}

		return false;
	}

	////���ģ��û���������һ��, ģ���ڴ�һ��ϴ�
	int iPos = 0;
	int iTPos = 0;
	int indexM = 0;
	do 
	{
		iPos = FindParamData(pConfig, MODEL_PRO, indexM);
		if (0 > iPos ||iPos >= pConfig->vecBody.size() || iPos >= pConfig->vecDataType.size())
		{
			break;
		}
		iTPos = FindSpecTypePos(pConfig->vecBody[iPos], pConfig->vecDataType[iPos], ID_TYPE);
		if (iPos >= pConfig->vecBody.size() || iPos >= pConfig->vecDataType.size())
		{
			break;
		}
		int iID = pConfig->vecBody[iPos][iTPos].iValue;
		int iTypeP = FindSpecTypePos(pConfig->vecBody[iPos], pConfig->vecDataType[iPos], SPEC_TYPE);
		if(0 > pConfig->vecBody[iPos][iTPos].iValue)
		{
			int iType = pConfig->vecBody[iPos][iTypeP].iValue;
			pConfig->vecBody[iPos][iTPos].iValue = UpdateVisionModel(iSceneID, indexM, iType);
		}
		indexM++;
	} while (1);
	m_pVisionInstance->process(iSceneID,pConfig,result);
	if (result.err < 0)
	{
		bRet = false;
	}
	else
	{
		bRet = true;
	}
	return bRet;

}

/**********************************************************************
����      : VisionDebugThreshold
����      : ������ֵ�ӿ�
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
index�� ���ݸ�index��ȡ��Բ����
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::VisionDebugThreshold(int iWinID, int iSceneID, int index)
{
	ST_LINE stresult;
	SCENE_PKG *pConfig;
	vector<SCENE_PKG>::iterator it;
	bool bIsFinde = false;
	long halconID = -1;
	int iMin = 0;
	int iMax = 255;
	int iCount = 0;
	map<long ,long>::iterator itW;
	itW=m_mapWinID.find(iWinID);
	//������IDת����halcon����ID
	if(itW!=m_mapWinID.end())
	{
		halconID = itW->second;
	}
	int iPos = 0;
	pConfig = GetSceneHandle(iSceneID);

	//�ҵ������ֵ��С��ֵ
	iPos = m_pVisionInstance->FindParamData(pConfig, THRE_PRO, index);
	if (NULL == pConfig || iPos < 0 || iPos >= pConfig->vecBody.size() || iPos >= pConfig->vecDataType.size())
	{
		return false;
	}
	VIRSION_DATA_PKG *pData = pConfig->vecBody[iPos];
	unsigned int *pType = pConfig->vecDataType[iPos];
	if (index > 0)
	{
		iPos = FindSpecTypePos(pData, pType, THRE_TYPE);
		if (iPos >= sizeof(pData))
		{
			return false;
		}
		iMax = pData[iPos].iValue;
		iPos = m_pVisionInstance->FindParamData(pConfig, THRE_PRO, index - 1);
		if (iPos >= pConfig->vecBody.size() || iPos >= pConfig->vecDataType.size())
		{
			return false;
		}
		pData = pConfig->vecBody[iPos];
		pType = pConfig->vecDataType[iPos];
		iPos = FindSpecTypePos(pData, pType, THRE_TYPE);
		if (iPos >= sizeof(pData))
		{
			return false;
		}
		iMin = pData[iPos].iValue;
	}
	else
	{
		iPos = FindSpecTypePos(pData, pType, THRE_TYPE);
		if (iPos >= sizeof(pData))
		{
			return false;
		}
		iMin = pData[iPos]. iValue;
		iPos = m_pVisionInstance->FindParamData(pConfig, THRE_PRO, index + 1);
		if (iPos >= 0 && iPos < pConfig->vecBody.size() && iPos < pConfig->vecDataType.size())
		{
			pData = pConfig->vecBody[iPos];
			pType = pConfig->vecDataType[iPos];
			iPos = FindSpecTypePos(pData, pType, THRE_TYPE);
			iMax = pData[iPos].iValue;
		}
	}

	m_pVisionInstance->DebugThreshold(halconID,iSceneID, iMin, iMax);
	return true;
}

/**********************************************************************
����      : VisionFindLine
����      : ���߽ӿ�
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
index�� ���ݸ�index��ȡ��Բ����
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::VisionFindLine(int iSceneID, int index, bool bIsFollow)
{
	ST_LINE stresult;
	SCENE_PKG *pConfig;
	//ST_FIND_LINE data;
	pConfig = GetSceneHandle(iSceneID);
	/* int iTPos = 0;*/

	//û�ҵ���Ч�������˳�
	int iPos = m_pVisionInstance->FindParamData(pConfig, LINE_PRO, index);

	if (NULL == pConfig || iPos < 0)
	{
		return false;
	}

	m_pVisionInstance->findline(iSceneID,pConfig,index, stresult, bIsFollow);
	return true;
}
/**********************************************************************
����      : VisionFindCircle
����      : ��Բ�ӿ�
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
index�� ���ݸ�index��ȡ��Բ����
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::VisionFindCircle(int iSceneID, int index, bool bIsFollow)
{
	ST_CIRCLE stresult;
	SCENE_PKG *pConfig;
	//ST_FIND_CIRCLE data;
	pConfig = GetSceneHandle(iSceneID);
	int iPos = 0;
	//int iTPos = 0;  //�ض����ܹ�λ��
	iPos = m_pVisionInstance->FindParamData(pConfig, CIRCLE_PRO, index);

	if (NULL == pConfig || iPos < 0)
	{
		return false;
	}
	m_pVisionInstance->findcircle(iSceneID,pConfig,index,stresult, bIsFollow);
	return true;
}


bool CManagerCtrl::VisionMeasureCircle(long HwinID,int iSceneID, int Imageindex,int MatchIndex, double& Row,double& Col,double& Rii)
{
	//ST_CIRCLE stresult;
	//SCENE_PKG *pConfig;
	return m_pVisionInstance->MeasureCircle(HwinID,iSceneID,Imageindex,MatchIndex,Row, Col,Rii);

}





bool CManagerCtrl::ClearWindow(int iWindowID)
{
	bool bRet = false;
	long halconID = -1;
	map<long ,long>::iterator it;; 
	it=m_mapWinID.find(iWindowID);
	//������IDת����halcon����ID
	if(it!=m_mapWinID.end())
	{
		halconID = it->second;
		if (-1 != halconID)
		{
			bRet = m_pVisionInstance->clearWindow(halconID);
		}
	}
	return bRet;
}

SYSTEM_PARAM   CManagerCtrl::m_stSysParam;

int WINAPI CManagerCtrl::RemoteManualMsg( unsigned int wParam, void * lParam, void* otherparam)
{
	// //ֻ��ͼ����ڶ����������ǳ����ţ�����������Ϣ�ĵڶ��������ǽṹ�塣
	//1�� ��ȡ������ 
	int TelSceneNo;
	if (MSG_COM_COMMUNITE == wParam || MSG_SCENE_BUSY == wParam)
	{
		TelSceneNo = *(int*)lParam;
	}
	else if(  RECV_UI_LOGIN_MSG == wParam || MSG_LOGIN_SYSTEM== wParam)
	{
		RemoteSystemMsg(wParam, lParam);	
		return 0;
	}
	else
	{
		TelSceneNo = *(int*)lParam;//((CALIB_DATA*)lParam)->iSceneID;
	}
	//2���Գ����Ž���ӳ�� ͨѶ����ID�����ʵ�ʳ���ID: ͨѶid����ֻ��1��λ0�����ǲ�ͬ�Ĳ�Ʒ��Ӧ��id��ͬ�������ܻ���Ʒ��ȥ�������ļ�
	int MapSceneNo = m_pInstance->MapSceneIdTelToReal(TelSceneNo);
	if (MapSceneNo>=0)
	{
		TelSceneNo = MapSceneNo;
	}
	//3,���¸�ֵ������
	if (MSG_COM_COMMUNITE == wParam )
	{
		SCENE_PKG *pConfig = NULL;
		pConfig = GetSceneHandle(TelSceneNo);
		wParam = pConfig->SceneType;      //�����б�Ҫ��������?11Ϊ����,10Ϊ�궨
		CALIB_DATA CommuniteData;
		CommuniteData.iSceneID = TelSceneNo;
		lParam = &CommuniteData;
	}else
	{
		((CALIB_DATA*)lParam)->iSceneID = TelSceneNo;
	}
	RemoteSystemMsg(wParam, (void *)lParam);
	return 0;
}

void CManagerCtrl::RemoteSystemMsg(int iType, void *param)
{
	//���������߳�
	unsigned long MainWindID = 0;
	unsigned long MessageID = 0;
	CManagerCtrl *pInstance = NULL;
	pInstance = GetInstance();
	if (NULL == pInstance)
	{
		return;
	}

	switch (iType)   //iType
	{
	case WORKING_CAMERA:
		{
			memset(m_pInstance->m_chSN,0,64);
			CALIB_DATA Data;
			memcpy(&Data, (CALIB_DATA *)param, sizeof(CALIB_DATA));
			for (int i = 0; i<strlen(Data.chSN); i++)
			{
				m_pInstance->m_chSN[i] = Data.chSN[i];
			}
			m_pInstance->TriggerVisionProcess(Data.iSceneID, *(int *)param);
		}
		break;
	case RECV_UI_LOGIN_MSG:
		{
			pInstance->m_WorkingStatus = *(int *)param;
		}
		break;
	case CLOSE_SYS:
		{
			pInstance->UnInitManager();
			// msg_post(MSG_CLOSE_SYSTEM, 0, 0);  //�㲥�˳�ϵͳ��Ϣ
		}
		break;
	case RESET_SYS:
		{
			pInstance->UnInitManager();
			//            msg_post(MSG_RESET_SYSTEM, 0, 0);  //�㲥������Ϣ
		}
		break;
	case CALIB_CAMERA:
		{
			CALIB_DATA *pData = (CALIB_DATA *)param;
			memcpy(&pInstance->m_CalibData, pData, sizeof(CALIB_DATA));

			bool bRet = false;
			//�������(�߳�ִ��);
			HANDLE handle = INVALID_HANDLE_VALUE;

			handle = (HANDLE)_beginthreadex(NULL,0,(unsigned (__stdcall *)(void*))CManagerCtrl::ThreadCalibProcess,(void *)&pInstance->m_CalibData,0,NULL);
			//��������Ҫ��������� ��������ǰ�ͷ�
			if (INVALID_HANDLE_VALUE != handle)
			{
				CloseHandle(handle);
			}
		}
	case CALIB_ROTTOOL:  
		{

		}
		break;
	case RECV_REMOTE_MSG:
		{
			SetEvent(m_pInstance->m_RecvEvent);
		}
		break;
	case MSG_SCENE_BUSY:
		{
			int iSceneID = *(int *)param;
			int iErrorCode = -1;
			char szErr[126] = {0};
			CHECK_RESULT result;
			SCENE_PKG *pConfig = NULL;
			pConfig = GetSceneHandle(iSceneID);
			result.SeneTpye = pConfig->SceneType;      //�����б�Ҫ��������?11Ϊ����,10Ϊ�궨
			for (int i = 0;i<RESULT_NUMBER;i++)
			{
				memset(result.szValue[i],0,BUF_SIZE/2);
			}
			if (WORKING_CAMERA ==result.SeneTpye)
			{
				result.iCount = 4;
			}
			else if (CALIB_CAMERA ==result.SeneTpye)
			{
				result.iCount = 6;
			}
			else
			{
				result.iCount = RESULT_NUMBER;
			}
			sprintf_s(szErr, "��ǰ����(%d)���ڴ�����,�ȷ���OK", iSceneID);
			msg_post(MSG_DISP_WORKING_INFO, szErr, (unsigned int)strlen(szErr) + 1, MODULE_SCENE_MAIN_UI);          
			//result.err = -1;
			//m_pInstance->ManagerProcFinish(iSceneID, &result, iErrorCode);

			//���ڴ���ʱ���յ���ͬ�����������ʱ�ȷ�������ok
			GrabFinished(iSceneID, &result,0);

		}
		break;
	case MSG_SEND_IMAGE_PATH:
		{
			int scenceID = *(int*)param;
			int iErrorCode = -1;
			char* ImgPath = NULL;
			ImgPath = m_pInstance->getImagePath(scenceID,0,0);
			if (NULL != ImgPath)
			{
				iErrorCode = 0;
			}
			getImagePathFinished(scenceID, ImgPath,iErrorCode);
		}
		break;

	default:
		{

		}
		break;
	}
}

/**********************************************************************
����      : OpenContinousGrab
����      : ����ʵʱ��ʾ
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
֪ͨ������ʾ�������ϢID
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : ��ʵʱ�� �ýӿ�ע��ص���ʵʱ�ɼ��� �ص���֪ͨ������ʾͼ��
��ϢID   
**********************************************************************/
bool CManagerCtrl::OpenContinousGrab(int iWindowID, int iSceneID, int index, unsigned int MsgID,int CameraIndex)
{
	bool bRet = false;
	//�����߳�, ֪ͨ�����̵���ͼ����ʾ�ӿ���ʾͼ��
	//��ȡ��ǰ������Ӧ�������
	int iCameraIndex = 0;
	char *pBuf = NULL;
	int  iWidth = 0;
	int  iHeight = 0;
	vector<SCENE_PKG>::iterator it;
	SCENE_PKG *pConfig = NULL;
	char szBuf[126] = {0};
	LOG_INFO("Open Continous Grab, Scene ID is %d", iSceneID);
	pConfig = GetSceneHandle(iSceneID);
	if ( 0 == pConfig->ProcProcess)
	{
		iCameraIndex = CameraIndex;

	}	
	if ( 0 > iCameraIndex)
	{
		return false;
	}
	int iRet = getImageSize(m_stSysParam.pstrCamareGUID[iCameraIndex],iWidth,iHeight);
	if (CAMERA_SUCCESS  == iRet)
	{
		pBuf = m_pVisionInstance->GetImageBuf(iSceneID, index, iWidth, iHeight);
		if (pBuf == NULL)
		{
			return false;
		}
		startSnap(m_stSysParam.pstrCamareGUID[iCameraIndex], 
			pBuf/*ͼ���ڴ�ָ��*/, PostMsgToUIForDisp, (void *)iSceneID/*������Ϣ*/);
		bRet = true;
	}
	return bRet;
}

/**********************************************************************
����      : ColseContinousGrab
����      : �ر�ʵʱ��ʾ
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::ColseContinousGrab(int iSceneID, int index,int CameraIndex)
{
	bool bRet = false;
	//֪ͨͼ�������ʵʱ�ɼ�
	SCENE_PKG *pConfig = NULL;
	SYSTEM_PARAM param;
	int iCameraIndex = 0;
	char szBuf[126] = {0};
	pConfig = GetSceneHandle(iSceneID);

	if ( 0 == pConfig->ProcProcess)
	{
		iCameraIndex = CameraIndex;

	}else
	{
		if (  NULL != pConfig && index < pConfig->vecInfo.size())
		{

			iCameraIndex = pConfig->vecInfo[index].CameraIndex;
			//�رչ�Դ
			if(LightCtrl(pConfig->vecInfo[index].LightIndex, 0))
			{
				sprintf_s(szBuf, "�رչ�Դ�ɹ�(%d)", pConfig->vecInfo[index].LightIndex);
				msg_post(MSG_DISP_WORKING_INFO, szBuf,(unsigned int)strlen(szBuf)+1, MODULE_SCENE_MAIN_UI); 
			}
			else
			{
				sprintf_s(szBuf, "�رչ�Դʧ��(%d)", pConfig->vecInfo[index].LightIndex);
				msg_post(MSG_DISP_WORKING_INFO, szBuf,(unsigned int)strlen(szBuf)+1, MODULE_SCENE_MAIN_UI); 
			}
		}else
		{
			return false;
		}

	}

	if ( 0 > iCameraIndex)
	{
		return false;
	}

	if (CAMERA_SUCCESS == stopSnap(m_stSysParam.pstrCamareGUID[iCameraIndex]))
	{
		bRet = true;
	}

	return bRet;
}


/**********************************************************************
����      : PostMsgToUIForDisp
����      : ��ʾͼ��ӿ�
�������  : context, ץͼ�õ���ͼ�� 
�������  : N/A
����ֵ    : N/A
��ע      : 
**********************************************************************/
void CManagerCtrl::PostMsgToUIForDisp(void* context)
{
	// bool bRet = true;
	//unsigned int sceneID = (unsigned int)context;
	//����ͼ��
	//֪ͨUI��ʾͼ��, ���޸�Ϊ��UI������Ϣ֪ͨUI����
	//m_pInstance->m_pVisionInstance->DispImage(pConfig->widID, pConfig->sceneID);
	//��Ҫ�ѹ�վ��������UI�� 
	msg_post(MSG_DISP_IMAGE, context, sizeof(unsigned int), MODULE_SCENE_DEBUG_UI);
}

SCENE_PKG *CManagerCtrl::GetSceneHandle(int iSceneID)
{
	//step1 ���ݳ���ID�ҵ���Ӧ����
	bool bIsFinde = false;
	SCENE_PKG *pConfig = NULL;
	vector<SCENE_PKG *>::iterator it;
	for (it = m_pInstance->m_vecSceneParam.begin(); 
		it != m_pInstance->m_vecSceneParam.end(); it++)
	{
		if (iSceneID == (*it)->sceneID)
		{
			pConfig = (*it);
			bIsFinde = true;
			break;
		}
	}
	//û�ҵ����ʳ���
	if (!bIsFinde)
	{
		pConfig = NULL;
	}
	return pConfig;
}

/**********************************************************************
����      : GetCurrentStatus
����      : ��ȡָ�����ܳ�ʼ�����
�������  : ID,  �����ܶ�Ӧ��ʶ
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::GetCurrentStatus(unsigned long ID)
{
	bool bRet = false;
	unsigned long flag = m_CurrentError;
	flag = flag & ID;
	if (0 != flag)
	{
		bRet = true;
	}
	return bRet;
}

void CManagerCtrl::RestoreImagePart(int WinID, int iScene, int width, int height, int index)
{
	bool bRet = false;
	int widthI = 0;
	int heightI = 0;
	int widthP = 0;
	int heightP = 0;
	int x = 0;
	int y = 0;
	map<long ,long>::iterator itID;
	itID = m_mapWinID.find(WinID);
	if(itID==m_mapWinID.end())
	{
		return;
	}
	string strCameraGuid;
	strCameraGuid = m_pInstance->m_stSysParam.pstrCamareGUID[index];  

	if (CAMERA_SUCCESS != getImageSize(strCameraGuid, widthI, heightI))
	{
		SCENE_PKG *p = NULL;
		p = GetSceneHandle(iScene); //���ÿ�����//���ڲ�Ӧ�ø��������
		if (p)
		{
			widthI = (int)p->defcamwidth;
			heightI = (int)p->defcamheight;
		}

	}
	bRet = true;
	if (heightI > 0 && height > 0)
	{
		double wd = (double)width/widthI;
		double hd = (double)height/heightI;
		if (wd < hd)
		{
			widthP = widthI;
			heightP = widthI * height/width;
			x = 0;
			y = -(heightP - heightI)/2;
			heightP += y;
		}
		else
		{
			widthP = width * heightI / height;
			heightP = heightI;
			x = -(widthP - widthI)/2;
			y = 0;
			widthP += x;
		}
	}
	else
	{
		widthP = widthI;
		heightP = heightI;
	}
	if (bRet && width > 0 && height > 0)
	{
		m_pVisionInstance->setpart(itID->second, x, y, widthP, heightP);
		//m_pVisionInstance->setpart(itID->second, 0, 0, width, height);
	}
}
//ͼ��������ק
//1. ����ID, 
//2. ��ק���ͣ�a, ��;b, ��;c, ��;d, ��;e, �Ŵ�;f, ��С, g, ��ԭ��
//3. ���ǰ��λ��(���ڵ�ǰ�����ϵ�λ��)
//4. ����ֵ
void CManagerCtrl::UpdateImagePart(int WinID, int iScene, int iType, double dXPer, double dYPer)
{
	int xb = 0;
	int yb = 0;
	int xe = 0;
	int ye = 0;
	int xm = 0;
	int ym = 0;
	int width = 0;
	int height = 0;
	double dP = 1;
	map<long ,long>::iterator itID;
	itID = m_mapWinID.find(WinID);
	if(itID==m_mapWinID.end())
	{
		return;
	}
	m_pInstance->m_pVisionInstance->getpart(itID->second, xb, yb, xe, ye);
	m_pVisionInstance->getimagesize(iScene, width, height);
	double xP = (double)(xe - xb)/width;
	double yP = (double)(ye - yb)/height;
	double dPer = (xP + yP) / 2;
	switch(iType)
	{
	case MOVE_TPYE:
		{
			xm = (int)(width * dXPer);
			ym = (int)(height * dYPer);
			xb -= xm;
			yb -= ym;
			xe -= xm;
			ye -= ym;
		}
		break;
		//�Ŵ�
	case ENLARGE_TYPE:
		{
			dPer -= 0.1;
			xm = (int)(width * (dP- dPer));
			ym = (int)(height * (dP - dPer));
			xb = (int)(xm * dXPer);
			yb = (int)(ym * dYPer);
			xe = width - (int)(xm *(dP - dXPer));
			ye = height - (int)(ym *(dP - dYPer));
		}
		break;
	case SHRINK_TYPE:
		{
			dPer += 0.1;
			xm = (int)(width * (dP- dPer));
			ym = (int)(height * (dP - dPer));
			xb = (int)(xm * dXPer);
			yb = (int)(ym * dYPer);
			xe = width - (int)(xm *(dP - dXPer));
			ye = height - (int)(ym *(dP - dYPer));
		}
		break;
		//��ԭ
	case RESTORE_TYPE:
		{
			xb = 0;
			yb = 0;
			xe = width;
			ye = height;
		}
		break;
	default:
		break;
	}
	if (xb < xe && yb < ye)
	{

		m_pInstance->m_pVisionInstance->setpart(itID->second, xb, yb, xe, ye);
	}
}

/**********************************************************************
����      : SetCurrentStatus
����      : ����ָ�����ܳ�ʼ���ɹ�
�������  : ID,  �����ܶ�Ӧ��ʶ
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::SetCurrentStatus(unsigned long ID)
{
	bool bRet = true;
	m_CurrentError = m_CurrentError | ID;
	return bRet;
}
/**********************************************************************
����      : DelCurrentStatus
����      : ȡ��ָ�����ܳ�ʼ���ɹ���ʶ
�������  : ID,  �����ܶ�Ӧ��ʶ
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::DelCurrentStatus(unsigned long ID)
{
	bool bRet = true;
	unsigned long flag = ID;
	flag = ~flag;
	m_CurrentError = m_CurrentError & flag;
	return bRet;
}

/**********************************************************************
����      : AffineTransPoint
����      : ����ϵת���� ��ͼ��������ݱ궨����ת��Ϊ�궨ʱ����������ϵ��
�������  : iSceneID ����ID�� ����ID�жϵ��ýӿڼ�����  
resultIn ת��ǰ��ֵ
resultOut ת�����ֵ
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : gpz ���result���м�����Ч���ݲ���һ��
**********************************************************************/
bool CManagerCtrl::AffineTransPoint(int iSceneID, int iCameraID, const CHECK_RESULT resultIn,CHECK_RESULT &resultOut)
{
	bool bRet = true;
	SCENE_PKG *pConfig = NULL;
	AF_POINT  pointIn;
	AF_POINT  pointOut;
	char szBuf[256] = {0};
	pConfig = GetSceneHandle(iSceneID);
	if (NULL != pConfig && pConfig->bSave)
	{
		pointIn.x = atof(resultIn.szValue[0]);
		pointIn.y = atof(resultIn.szValue[1]);
		pointIn.angle = atof(resultIn.szValue[2]);
		m_pVisionInstance->affine_apply(iSceneID, iCameraID, pConfig->szSceneName, pointIn, pointOut);
		sprintf(resultOut.szValue[0], "%lf", pointOut.x);
		sprintf(resultOut.szValue[1], "%lf", pointOut.y);
		sprintf(resultOut.szValue[2], "%lf", pointOut.angle);
	}
	return bRet;
}

/**********************************************************************
����      : OpenCheckStateThread
����      : �򿪼��ϵͳ״̬�߳�
�������  :   

�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
bool CManagerCtrl::OpenCheckStateThread()
{
	HANDLE CheckThread=CreateThread(NULL,0,CheckStateThread,NULL,0,NULL);
	CloseHandle(CheckThread);
	return true;
}

DWORD WINAPI CManagerCtrl::CheckStateThread(LPVOID lpParameter)
{

	while (NULL != m_pInstance && m_pInstance->m_iExitThread)
	{
		if (WAIT_OBJECT_0 != WaitForSingleObject(m_pInstance->m_RecvEvent, HEART_BEAT_TIME))
		{

			int reval = 1;
			if (NULL != m_pInstance && m_pInstance->m_iExitThread)
			{
				reval =PantCheckSend();
			}
			if (-1==reval)
			{    
				UnInitConnect();
				if (0 <= InitConnect((void *)&m_pInstance->m_stSysParam.stSocketParam))
				{
					SetCurrentStatus(INIT_CONNECT_OK);
					msg_post(MSG_CONNECT_SUCCESS, 0, 0, MODULE_SCENE_MAIN_UI);  //����״̬��
					//��list���������log
					msg_post(MSG_DISP_WORKING_INFO, "���ӳɹ�",(unsigned int)strlen("���ӳɹ�") + 1, MODULE_SCENE_MAIN_UI); 
				}
				else
				{
					if (GetCurrentStatus(INIT_CONNECT_OK))
					{
						DelCurrentStatus(INIT_CONNECT_OK);
						msg_post(MSG_CONNECT_FAILED, 0, 0, MODULE_SCENE_MAIN_UI);  //����״̬��
					}
					//��list���������log
					msg_post(MSG_DISP_WORKING_INFO, "����ʧ��",(unsigned int)strlen("����ʧ��") + 1, MODULE_SCENE_MAIN_UI); 
				}
			}

		}
		else
		{
			ResetEvent(m_pInstance->m_RecvEvent);//�ֶ�����Ϊ
		}
	}
	return 0;
}

int CManagerCtrl::FindSpecTypePos(const VIRSION_DATA_PKG *pkg, unsigned int *pType, int iType)
{
	int iRet = -1;
	if (NULL != pkg && NULL != pType)
	{
		for (int i = 2; i < pkg[1].iValue + VIRSION_DATA_PKG_HEADER_SIZE; i++)
		{
			if (iType == pType[i] / 100 )
			{
				iRet = i;
				break;
			}
		}
	}
	return iRet;
}

int CManagerCtrl::FindParamData(SCENE_PKG *pkg, int iType, int index)
{
	int iRet = -1;
	CManagerCtrl *pInstance = GetInstance();
	if (NULL != pInstance->m_pVisionInstance)
	{
		iRet = pInstance->m_pVisionInstance->FindParamData(pkg, iType, index);
	}
	return iRet;
}

int CManagerCtrl::InsertParamItem(int SceneId, const VIRSION_DATA_PKG *pkg, unsigned int *pType, int iSize, int insertPos)
{
	int iRet = -1;
	SCENE_PKG *param = NULL;
	VIRSION_DATA_PKG *pkgTmp = NULL;
	unsigned int *pTypeN = NULL;
	int iNewSize = 0;

	do 
	{
		param = GetSceneHandle(SceneId);
		if (NULL == param || NULL == pkg)
		{
			break;
		}
		iNewSize = param->pkgBodySize + iSize;

		pkgTmp = new VIRSION_DATA_PKG[iSize];
		pTypeN = new unsigned int[iSize];
		memcpy(pkgTmp, pkg, iSize * sizeof(VIRSION_DATA_PKG));
		memcpy(pTypeN, pType, iSize * sizeof(unsigned int));

		vector<VIRSION_DATA_PKG *>::iterator itV = param->vecBody.begin() + insertPos;
		vector<unsigned int *>::iterator itI = param->vecDataType.begin() + insertPos;
		param->vecBody.insert(itV, pkgTmp);
		param->vecDataType.insert(itI, pTypeN);

		param->pkgBodySize = iNewSize;
		param->pkgCount++;
		int iT = pkg[0].iValue % 10000;
		int iAddValue = 1;
		if (MAIN_PRO == iT/100)
		{
			param->imageCount++;
			SCENE_DEVICE_INFO pInfo;
			int iG = pkg[0].iValue / 10000;
			vector<SCENE_DEVICE_INFO>::iterator itS = param->vecInfo.begin() + iG;
			param->vecInfo.insert(itS, pInfo);
			iAddValue += 10000;//������ͼ��� ����Groupͬʱ��1
		}


		//���º���Ԫ��
		int iPos = insertPos + 1;
		itV = param->vecBody.begin() + iPos;
		if (itV == param->vecBody.end())
		{
			iRet = 0;
			break;
		}
		for (;itV != param->vecBody.end(); itV++)
		{
			pkgTmp = *itV;
			if ((iT / 100) == (pkgTmp[0].iValue%10000)/100)
			{
				pkgTmp[0].iValue += iAddValue;
			}
		}
		SetSceneParam(param);
		iRet = 0;
	} while (0);

	return iRet;
}

int CManagerCtrl::UpdateParamItem( int SceneId, const VIRSION_DATA_PKG *pkg, unsigned int *pType, INTERFACE_SET_DATA *dataTmp, int iSize, int insertPos, bool insert /*= false*/ )
{
	int iRet = -1;
	SCENE_PKG *param = NULL;
	VIRSION_DATA_PKG *pkgTmp = NULL;
	unsigned int *pTypeN = NULL;
	INTERFACE_SET_DATA *pInterfaceData = NULL;
	int iNewSize = 0;

	do 
	{
		param = GetSceneHandle(SceneId);
		if (NULL == param || NULL == pkg || pkg[0].iValue != param->vecBody[insertPos][0].iValue
			|| insertPos >= param->vecBody.size() || insertPos >= param->vecDataType.size())
		{
			break;
		}
		iNewSize = param->pkgBodySize + iSize - param->vecBody[insertPos][1].iValue;
		pkgTmp = new VIRSION_DATA_PKG[iNewSize];
		pTypeN = new unsigned int[iNewSize];
		pInterfaceData = new INTERFACE_SET_DATA;
		memcpy(pkgTmp, pkg, iSize * sizeof(VIRSION_DATA_PKG));
		memcpy(pTypeN, pType, iSize * sizeof(unsigned int));
		memcpy(pInterfaceData, dataTmp, sizeof(INTERFACE_SET_DATA));
		delete param->vecBody[insertPos];
		param->vecBody[insertPos] = pkgTmp;
		delete param->vecDataType[insertPos];
		param->vecDataType[insertPos] = pTypeN;
		param->pkgBodySize = iNewSize;
		if (VIRSION_DATA_PKG_HEADER_SIZE == iSize)	//�������
		{
			param->InterfaceDataNumb = 0;
			param->InterfaceSetData.clear();
		}
		else if (insert)	//��������
		{
			param->InterfaceSetData.push_back(pInterfaceData);
			param->InterfaceDataNumb = pInterfaceData->nIndex + 1;
		}
		else if (!insert)		//��������
		{
			param->InterfaceSetData[pInterfaceData->ItemNumb] = pInterfaceData;
		}
		SetSceneParam(param);
		iRet = 0;
	} while (0);

	return iRet;
}
int CManagerCtrl::SwapParamItem(int SceneId, int iPosF, int iPosN)
{
	int iRet = -1;
	SCENE_PKG *param = NULL;
	VIRSION_DATA_PKG *pkgTmpF = NULL;
	VIRSION_DATA_PKG *pkgTmpN = NULL;
	unsigned int *pTypeF = NULL;
	unsigned int *pTypeN = NULL;
	int iNewSize = 0;

	do 
	{
		param = GetSceneHandle(SceneId);
		if (NULL == param || iPosN >= param->vecBody.size() || iPosN >= param->vecDataType.size() 
			|| iPosF >= param->vecBody.size() || iPosF >= param->vecDataType.size())
		{
			break;
		}
		pkgTmpF = param->vecBody[iPosF];
		pkgTmpN = param->vecBody[iPosN];
		param->vecBody[iPosF] = pkgTmpN;
		param->vecBody[iPosN] = pkgTmpF;

		pTypeF = param->vecDataType[iPosF];
		pTypeN = param->vecDataType[iPosN];
		param->vecDataType[iPosF] = pTypeN;
		param->vecDataType[iPosN] = pTypeF;


		//���������������ͬ�� ���µ�һ��Ԫ��
		//�����������һ����MAIN_PRO�� ���޸���һ����Group
		int iDF = pkgTmpN[0].iValue;   //�������
		int iDN = pkgTmpF[0].iValue;   //�������
		int iTF = (iDF %10000)/100;
		int iTN = (iDN %10000)/100;
		if (iTF == iTN)
		{
			pkgTmpN[0].iValue = iDN;
			pkgTmpF[0].iValue = iDF;
		}
		else if (MAIN_PRO == iTF) //�������ǰ��һ��, �򽻻���ĺ�һ���޸�group
		{
			pkgTmpF[0].iValue = pkgTmpF[0].iValue + 10000;
		}
		else if (MAIN_PRO == iTN)
		{
			pkgTmpN[0].iValue = pkgTmpN[0].iValue - 10000;
		}

		SetSceneParam(param);
		iRet = 0;
	} while (0);

	return iRet;
}
int CManagerCtrl::DeleteParamItem(int SceneId, int insertPos)
{
	int iRet = -1;
	SCENE_PKG *param = NULL;
	VIRSION_DATA_PKG *pkgTmp = NULL;
	unsigned int *pType = NULL;
	int iNewSize = 0;
	int iLastSize = 0;

	do 
	{
		param = GetSceneHandle(SceneId);
		if (NULL == param || insertPos >= param->vecBody.size() || insertPos >= param->vecDataType.size())
		{
			break;
		}

		pkgTmp = param->vecBody[insertPos];
		pType = param->vecDataType[insertPos];
		int iDelSize = pkgTmp[1].iValue + 2;
		iNewSize = param->pkgBodySize - iDelSize;
		int iT = pkgTmp[0].iValue;

		param->pkgBodySize = iNewSize;
		param->pkgCount--;
		vector<VIRSION_DATA_PKG *>::iterator itV = param->vecBody.begin() + insertPos;
		vector<unsigned int *>::iterator itU = param->vecDataType.begin() + insertPos;
		param->vecBody.erase(itV);
		param->vecDataType.erase(itU);
		delete pkgTmp;
		delete pType;

		//���ɾ������ͼ��
		int iG = iT /10000;
		int iType = (iT %10000)/100;
		int iDelValue = 1;
		if (MAIN_PRO == iType)
		{
			vector<SCENE_DEVICE_INFO>::iterator itS = param->vecInfo.begin() + iG;
			param->vecInfo.erase(itS);
			iDelValue += 10000;
			param->imageCount--;
		}

		//���º���Ԫ��
		int iPos = insertPos;
		itV = param->vecBody.begin() + insertPos;
		for (;itV != param->vecBody.end(); itV++)
		{
			pkgTmp = *itV;
			if (((iT %10000)/ 100) == (pkgTmp[0].iValue%10000)/100)
			{
				pkgTmp[0].iValue -= iDelValue;
			}
		}

		SetSceneParam(param);
		iRet = 0;
	} while (0);

	return iRet;
}


int CManagerCtrl::GenProductId(string name)
{
	int ProductId = -1;
	PMAPDATA::iterator it;
	for (int i = 0;i<20;i++)
	{
		for (it = m_stSysParam.ProductMap.begin();it!= m_stSysParam.ProductMap.end();it++)
		{
			if (m_stSysParam.ProductById[i] == it->first)
			{
				break;
			}
		}
		//���û�ҵ�
		ProductId =  i;
		//m_stSysParam.ProductById[i] = name;

		break;
	}
	return ProductId;

}

int CManagerCtrl::FindProductId(string name)
{
	int ProductId = -1;
	for (int i = 0;i<20;i++)
	{	
		if (name == m_stSysParam.ProductById[i])
		{
			ProductId =  i;
			break;
		}

	}
	return ProductId;
}


void CManagerCtrl::SetCurProductName(string name)
{
	//ϵͳ��������
	m_stSysParam.CurProductName = name;

	//�������ļ��б��浱ǰ��ƷΪĬ�ϲ�Ʒ
	ConfigSaveDefaultProduct(name);

	//�ڽ�������ʾ��ǰ��Ʒ����
	AlterMainWndTitle(name);

}

void CManagerCtrl::AlterMainWndTitle(string name)
{

	//�ڽ�������ʾ��ǰ��Ʒ����
	char szText[256]={0};
	sprintf_s(szText,"%s",name.c_str());
	msg_post(MSG_DIS_MAINWND_NAME, szText, (unsigned int)strlen(szText) + 1, MODULE_SCENE_MAIN_UI); 
}

//��ͨ�Ŵ����ĳ�����ӳ�䵽ʵ�ʳ�����,��ֻ��һ�ֲ�Ʒ��Ӧ����ӳ���
int CManagerCtrl::MapSceneIdTelToReal(int TelSceneId)
{
	//int RealSceneID;
	int MapSceneNo = -1;
	SYSTEM_PARAM* pSysParam = (SYSTEM_PARAM*)GetSysParamPoint();		      
	//2.1,��ȡ��ǰ��Ʒ�����֡�
	string curProductName = pSysParam->CurProductName;
	//2.2���ҵ���ǰ��Ʒ�ĳ�����ӳ��ֵ�����ӳ��ֵС��0��ӳ�䡣
	if (pSysParam->ProductMap.empty())//ӳ���Ϊ����ͨѶ����Ϊ������
	{	
		MapSceneNo =  TelSceneId;
	}
	PMAPDATA::iterator it;
	it = pSysParam->ProductMap.find(curProductName);
	if (it != pSysParam->ProductMap.end())
	{
		if (it->second.find(TelSceneId)!= it->second.end())
		{
			MapSceneNo =  it->second.find(TelSceneId)->second;
		}
	}
	LOG_INFO("ͨѶ������:%d,ʵ�ʳ�����Ϊ:%d",TelSceneId,MapSceneNo);
	return MapSceneNo;
}

//��ʵ�ʳ�����ӳ�䵽ͨ�ų�����
int CManagerCtrl::MapSceneIdRealToTel(int RealSceneId)
{
	int MapSceneNo = -1;
	SYSTEM_PARAM* pSysParam = (SYSTEM_PARAM*)GetSysParamPoint();		      
	//2.1,��ȡ��ǰ��Ʒ�����֡�
	string curProductName = pSysParam->CurProductName;
	//2.2���ҵ���ǰ��Ʒ�ĳ�����ӳ��ֵ�����ӳ��ֵС��0��ӳ�䡣
	PMAPDATA::iterator it;
	SCENEMAP::iterator subit;
	it = pSysParam->ProductMap.find(curProductName);
	if (pSysParam->ProductMap.empty())
	{
		return RealSceneId;
	}
	if (it != pSysParam->ProductMap.end())
	{
		for (subit = (it->second).begin();subit!=(it->second).end();subit++ )
		{
			if (RealSceneId == subit->second)
			{
				MapSceneNo = subit->first;
			}
		}

		//if (it->second.find(SceneId)!= it->second.end())
		//{
		//	MapSceneNo =  it->second.find(SceneId)->second;
		//}

	}
	return MapSceneNo;


}

int  CManagerCtrl::ManagerProcFinish(int& iSceneID, CHECK_RESULT *pResult, int iErrorCode)
{
	int TelSceneId = MapSceneIdRealToTel(iSceneID);
	if (TelSceneId<0)
	{
		TelSceneId = iSceneID;
	}
	iSceneID = TelSceneId;
	if (m_stSysParam.IsSimulate)
	{
		for (int i= 0;i<pResult->iCount;i++)
		{
			sprintf_s(pResult->szValue[i],"%0.4lf",0.0);
		}
		sprintf_s(pResult->szValue[0],"%d",1);

	}

	return  CommunicationProcessFinished(TelSceneId,pResult,iErrorCode);


}


void CManagerCtrl::ManagerGrabFinish(int& iSceneID, CHECK_RESULT *res,int ErrorCode)
{
	int TelSceneId = MapSceneIdRealToTel(iSceneID);
	if (TelSceneId<0)
	{
		TelSceneId = iSceneID;
	}
	iSceneID = TelSceneId;

	GrabFinished(TelSceneId, res,ErrorCode);

}


bool CManagerCtrl::HalconProcedureLocate(long iWinID,int iSceneID,int Imageindex,string ProcedurePath,
	double (*inParam)[HalProcInParamNum], double& PosX,double& PosY,double& PosPhi)
{
	map<long ,long>::iterator itW;
	itW=m_mapWinID.find(iWinID);
	//������IDת����halcon����ID
	long halconID = -1;

	if(itW!=m_mapWinID.end())
	{
		halconID = itW->second;
	}
	if (-1 ==halconID)
	{
		halconID = 3600;
		//return false;
	}
	return m_pVisionInstance->HalconProcedureLocate(halconID,iSceneID,Imageindex,ProcedurePath,inParam,PosX,PosY,PosPhi);
}

bool CManagerCtrl::HalconProcedureProcess(long iWinID,int iSceneID,int Imageindex,string ProcedurePath,vector<double> &inparam,vector<double> &outparam,int display,int &error,string &hmessage)
{
	map<long ,long>::iterator itW;
	itW=m_mapWinID.find(iWinID);
	//������IDת����halcon����ID
	long halconID = -1;

	if(itW!=m_mapWinID.end())
	{
		halconID = itW->second;
	}
	if (-1 ==halconID)
	{
		halconID = 3600;
		//return false;
	}
	return m_pVisionInstance->HalconProcedureProcess(halconID,iSceneID,Imageindex,ProcedurePath,inparam,outparam,display,error,hmessage);

}
bool CManagerCtrl::ImageEnhancement(long iWinID,int iSceneID,int Imageindex,double& multinum,double& addnum,int display,int &error,string &hmessage)
{
	map<long ,long>::iterator itW;
	itW=m_mapWinID.find(iWinID);
	//������IDת����halcon����ID
	long halconID = -1;

	if(itW!=m_mapWinID.end())
	{
		halconID = itW->second;
	}
	if (-1 ==halconID)
	{
		halconID = 3600;
		//return false;
	}
	return m_pVisionInstance->ImageEnhancement(halconID,iSceneID,Imageindex,multinum,addnum,display,error,hmessage);

}

void CManagerCtrl::ClearSceneProcResultMap()
{
	sys_EnterCriticalSection(m_mapSceneResultSection);
	for(auto itresult = m_mapSceneProcResult.begin(); itresult != m_mapSceneProcResult.end(); itresult++)
	{
		CHECK_RESULT *temp = itresult->second;
		if (NULL != temp)
		{		
			delete temp;
		}
	}
	m_mapSceneProcResult.clear();
	sys_LeaveCriticalSection(m_mapSceneResultSection);
}
int CManagerCtrl::InitSceneProcResultMap()
{
	SYSTEM_PARAM *sysParam = NULL;
	int sceneNum = 0;
	CHECK_RESULT *tempres = NULL;
	string strSceneName ;
	sysParam = (SYSTEM_PARAM *)GetSysParamPoint();
	if (NULL == sysParam)
	{
		return MAN_ERROR;
	}
	sceneNum = sysParam->iSceneNum;
	sys_EnterCriticalSection(m_mapSceneResultSection);
	for (int i = 0;i<sceneNum;i++)
	{
		tempres = new CHECK_RESULT();
		tempres->index = i;
		strSceneName = sysParam->pstrSceneName[i];
		m_mapSceneProcResult.insert((map<string,   struct CHECK_RESULT *>::value_type(strSceneName,tempres)));
	}
	sys_LeaveCriticalSection(m_mapSceneResultSection);
	return MAN_OK;
}

CHECK_RESULT* CManagerCtrl::GetSceneProcResult(string &scneName)
{
	CHECK_RESULT* res = NULL ;

	sys_EnterCriticalSection(m_mapSceneResultSection);
	map<string,CHECK_RESULT*>::iterator it;
	it = m_mapSceneProcResult.find(scneName);
	if (it != m_mapSceneProcResult.end())
	{
		res = it->second; //����ָ��,����ʧȥ����������
	}
	sys_LeaveCriticalSection(m_mapSceneResultSection);
	return res;
}
CHECK_RESULT* CManagerCtrl::GetSceneProcResultById(int iSceneID)
{
	CHECK_RESULT* res = NULL ;
	//GetSysParamPoint();
	sys_EnterCriticalSection(m_mapSceneResultSection);
	for (auto it = m_mapSceneProcResult.begin();it!= m_mapSceneProcResult.end();it++)
	{
		if (it->second->index == iSceneID)
		{
			res = it->second; //����ָ��,����ʧȥ����������
			break;
		}
	}
	sys_LeaveCriticalSection(m_mapSceneResultSection);
	return res;
}
int CManagerCtrl::SetSceneProcResult(string &scneName,CHECK_RESULT& res)
{
	int ret = MAN_OK;
	sys_EnterCriticalSection(m_mapSceneResultSection);
	map<string,CHECK_RESULT*>::iterator it;
	it = m_mapSceneProcResult.find(scneName);
	if (it != m_mapSceneProcResult.end())
	{
		*(it->second )= res;//ǳ�㸴��
	}
	sys_LeaveCriticalSection(m_mapSceneResultSection);
	return ret;
}
int CManagerCtrl::ClearProcResultByID(int iSceneID)
{
	int ret = MAN_OK;
	sys_EnterCriticalSection(m_mapSceneResultSection);
	map<string,CHECK_RESULT*>::iterator it;
	for (it = m_mapSceneProcResult.begin();it!= m_mapSceneProcResult.end();it++)
	{
		if (it->second->index == iSceneID)
		{
			it->second->caturetime = 0;
			it->second->err = 0;
			it->second->iCount = 0;
			it->second->time = 0;
			it->second->isGeneralProcess = 0;
			for (int i = 0;i<RESULT_NUMBER;i++)
			{
				memset(it->second->szValue[i],0,BUF_SIZE/2);
			}

		}
	}
	sys_LeaveCriticalSection(m_mapSceneResultSection);
	return ret;
}
//3982��