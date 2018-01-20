
#pragma once
#ifndef DLL_INCLUDE_COMM_MSG
#define DLL_INCLUDE_COMM_MSG
#include "sysapi.h"
#include <hash_map>
#include <list>
#include <string>
#include "common.h"

using std::list;
using std::hash_map;
using std::string;

#define MSG_INSTANCE CMsg::GetInstance()

struct TReCallFun
{//�ص�����
    RegistCallBackProc callback;
    void* lparam;
};

struct TMsg
{//��Ϣ��
    unsigned int wparam;
	void* lparam;				//����ͨ�õ�void��Ϣ,ͨ�����ݵ���Ϣ��С�������ڴ�
	unsigned int size;			//��Ϣ��С
};

struct TMsgInfo
{//��Ϣ����
    list<TMsg*> listMsg;
    Handle      msgEvt;
};

struct TPostData
{//post��Ϣ��
    TReCallFun pFun;
    TMsg msg;
};

#define MAX_STR_CNT 256

class CMsg
{
public:
    CMsg(void);
    virtual ~CMsg(void);

    static CMsg* GetInstance();
    static void ReleaseInstance();

    int RegisterNotify(const char* name, RegistCallBackProc fun, void* lparam);
	int UnRegisterNotify(const char* name); //gpz


    //int RecvMsgStr(char* lpstr, int timeout, const char* name);//�Ȳ�ʵ��
 


    int SendMsg(unsigned int wparam, void* lparam, unsigned int lsize, const char* name);
    int PostMsg(unsigned int wparam,  void* lparam, unsigned int lsize, const char* name);
    
	int RegistMessage(CommMessageShow fun, void* param);
	int RegistIAStudioMessage(CommMessageShow fun, void* param);
    int ShowMessage(MessageType type, const char *msg);

	int RegistDevice(PushDeviceInfoCallback callback, void* param);
	int PushDeviceInfo(char *type, char *context);
protected:

    TReCallFun* FindRecall(const char* name);
    TMsgInfo*   GetMsgInfo(const char* name);

    void PushMsg(const char* name, TMsg* pMsg);

    void ReleaseReCall();
    void ReleaseMsg();

    static int WINAPI MsgPostProc(void* lparam); //post��Ϣ

private:
    static CMsg*            m_sInstance;

    bool                    m_bRun;             //post�߳��Ƿ�������

    hash_map<string, TReCallFun*>   m_hashInfo; //���ж���
    hash_map<string, TMsgInfo*>     m_hashMsg;  //������Ϣ

    Handle                  m_hQuitSign;        //post�߳��˳��ź�
    Handle                  m_hSection;         //�ٽ������

    CommMessageShow         m_msgFun;           //������Ϣ ֻ����ע��һ��
    void*                   m_msgParam;

	CommMessageShow         m_msgISFun;         //������Ϣ IAStudioע�ᣬ  ��m_msgFun��ͬһʱ��ֻ�ܵ���һ���� IAStudio����
	void*                   m_msgISParam;

	PushDeviceInfoCallback  m_pushCallback;     //�пػ򿴰���� ע��ص�
	void*                   m_pushParam;
};

#endif  //��ֹ�ظ�����