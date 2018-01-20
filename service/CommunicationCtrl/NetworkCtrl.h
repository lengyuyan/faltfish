#pragma once
#include "iCommunication.h"
#include "sysapi.h"
#include <map>
#include <string>

#include "vision_api.h"
using namespace std;

struct IMG_BUF
{
	int sence;
	int size;
	void* buf;
	void *network;
	IMG_BUF()
	{
		sence = -1;
		size = 0;
		buf = NULL;
		network =NULL;
	}

	IMG_BUF &operator=( IMG_BUF copy)
	{
		sence = copy.sence;
		size = copy.size;
		if (NULL != buf)
		{
			delete buf;
			buf = NULL;					
		}
		buf = new char[size];	
		memcpy(buf,copy.buf,size);
		network = copy.network;
		return *this;
	}
};
class SocketClientLogic;
class CNetworkCtrl
{
public:
    CNetworkCtrl(void);
    ~CNetworkCtrl(void);

public:
    static CNetworkCtrl *GetInstance(const char* pIp = NULL);
    static void ReleaseInstance();
    static void ReleaseInstance(const char* pIp);


    /**********************************************************************
    ����      : InitConnect
    ����      : ��ʼ������
    �������  : pCount ��ʼ��������
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : N/A
    **********************************************************************/
    int InitConnect(void *pCount);
    /**********************************************************************
    ����      : UnInitConnect
    ����      : ȥ��ʼ������
    �������  : pCount ��ʼ��������
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : N/A
    **********************************************************************/
    int UnInitConnect(void *pCount);
        
    static DWORD WINAPI ThreadClient(void *pParam);
	static DWORD WINAPI ThreadSendImg(void *pParam) ;
    /**********************************************************************
    ����      : RegesterCallBack
    ����      : ע�ᴥ���ӿ�
    �������  : �ص�����
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : N/A
    **********************************************************************/
    bool RegesterCallBack(pFunCallback pfun);

    /**********************************************************************
    ����      : RegesterCloseSys
    ����      : ֪ͨ���̹ر�
    �������  : �ص�����
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : N/A
    **********************************************************************/
    bool RegesterCloseSys(pFunCloseSys pfun);


    /**********************************************************************
    ����      : 
    ����      : ͼ��ɼ����
    �������  : iSceneID ����ID
                iErrorCode ������
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : N/A
    **********************************************************************/
    int GrabFinished(int iSceneID, CHECK_RESULT *pResult,int iErrorCode);

	/**********************************************************************
    ����      : getImagePathFinished
    ����      : ��ȡͼ��·�����
    �������  : iSceneID ����ID
	            path   ���ͼƬ·��
                iErrorCode ������
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : N/A
    **********************************************************************/
	int getImagePathFinished(int iSceneID, char* path,int iErrorCode = 0);
    /**********************************************************************
    ����      : ProcessFinished
    ����      : �������
    �������  : pResult ������
                iErrorCode ������
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
    int ProcessFinished(int iSceneID, CHECK_RESULT *pResult, int iErrorCode = 0);

	int SendData2Board(int iSceneID, CHECK_RESULT *pResult, int iErrorCode = 0);
  /**********************************************************************
    ����      : SendImage
    ����      : ����ͼ��
    �������  : pResult ������
                iErrorCode ������
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
    **********************************************************************/
	int SendImage(int iSceneID, void* pBuffer,int size,int w,int h, int iErrorCode);
    /**********************************************************************
    ����      : OpenLight
    ����      : ��Դ����
    �������  : index ��Դ����
                type  ��������, 1Ϊ���� 0Ϊ��
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : ��Դͨ��IO�ڲ����� IO�ڶ�ռ
    **********************************************************************/
    bool OpenLight(int index, int type);


	/**********************************************************************
    ����      : ResetFinished
    ����      : ����ϵͳ������Ϣ
    �������  :  iErrorCode ������
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : N/A
    **********************************************************************/
    int ResetFinished(int iErrorCode = 0);

    /**********************************************************************
    ����      : PantCheckSend
    ����      : ������ⷢ�����ݺ���
    �������  :  
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : N/A
    **********************************************************************/
    int PantCheckSend();

	void SetCommuniWebProctocol(const PROCTOCOLDATA& ProctoData);
	void GetCommuniWebProctocol( PROCTOCOLDATA& ProctoData);


	/**********************************************************************
    ����      : SetCalibRevIPHead
    ����      : ���ý��յ� �궨 IPͨ�� ͷ��־
    �������  :  
    �������  : N/A
    ����ֵ    : 
    ��ע      : N/A
    **********************************************************************/
	void SetCalibRevIPHead(const char* szRvHead);

    /**********************************************************************
    ����      : SetWorkRevIPHead
    ����      : ���ý��յ� ��ҵ IPͨ�� ͷ��־
    �������  :  
    �������  : N/A
    ����ֵ    : 
    ��ע      : N/A
    **********************************************************************/
	void SetWorkRevIPHead(const char* szRvHead);

	/**********************************************************************
    ����      : SetCalibSendIPHead
    ����      : ���÷��͵� �궨 IPͨ�� ͷ��־
    �������  :  
    �������  : N/A
    ����ֵ    : 
    ��ע      : N/A
    **********************************************************************/
	void SetCalibSendIPHead(const char* szSendHead);

    /**********************************************************************
    ����      : SetWorkSendIPHead
    ����      : ���÷��͵� ��ҵ IPͨ�� ͷ��־
    �������  :  
    �������  : N/A
    ����ֵ    : 
    ��ע      : N/A
    **********************************************************************/
    void SetWorkSendIPHead(const char* szSendHead);


	 /**********************************************************************
    ����      : SetIPDividLable
    ����      : ���� IPͨ�� ��β��־
    �������  :  
    �������  : N/A
    ����ֵ    : 
    ��ע      : N/A
    **********************************************************************/
    void SetIPDividLable(const char* szIpDivid);

    /**********************************************************************
    ����      : SetWorkSendIPHead
    ����      : ���� IPͨ�� ��β��־
    �������  :  
    �������  : N/A
    ����ֵ    : 
    ��ע      : N/A
    **********************************************************************/
    void SetIPEndLable(const char* szIpEnd);




private:
    int RecvQueue();

    bool AssembleProtocol(int index, int iType, int iPercent, char *pData, unsigned int datalength);
    int ResolveProtocol();

private:
    static CNetworkCtrl *m_pNetworkInstance;
    SocketClientLogic *m_pSocketInstance;
    pFunCallback      m_pTriggerFun;       //��¼֪ͨ������ָ��
    pFunCloseSys      m_pRemoteSysCommand;       //��¼֪ͨϵͳ��������ָ��

    bool              m_bIsRunning;
    HANDLE            m_SocketHandle;

    char              m_szRecvBuf[1024];
    map<int, int>     m_vecOperateResult;
	IMG_BUF			  m_imgb;
    HANDLE  m_Section;


	// ͨ��Э�鶨�� 
	char  m_CalibRevIPHead[64];
	char  m_WorkRevIPHead[64];
	char  m_CalibSendIPHead[64];
	char  m_WorkSendIPHead[64];
	char  m_IPDivid[32];
	char  m_IPEnd[64];
	int  m_SceneIdTemp;
	int m_serverstatus;
	
};

