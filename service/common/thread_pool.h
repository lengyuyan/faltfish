#pragma once
#include <vector>
#include "sysapi.h"

using std::vector;

//�߳���ؽṹ��
struct ST_THREAD_FUN
{
    PROCEX  fun;        //ִ�к���
    void*   param;      //ִ�в���
    Handle  thread;     //�߳̾��
    Handle  startEvt;   //����ִ��
    Handle  hIdleEvt;   //�������
    void*   self;       //������ָ��
};

#define POOL_INSTANCE CThreadPool::GetInstance()

class CThreadPool
{
public:
    CThreadPool(void);
    virtual ~CThreadPool(void);

    static CThreadPool* GetInstance();
    static void ReleaseInstance();

    int Run(PROCEX fun, void* param);       //�߳�ִ��

protected:
    
    void RunAsNewThread(PROCEX fun, void* param);   //�������߳�ִ��
    void DelThreadData(ST_THREAD_FUN* pThread);     //ɾ��һ���߳�

    int Process(ST_THREAD_FUN* pData);  //ִ�к���
    static int WINAPI ThreadFun(void* param);

private:
    static CThreadPool*     m_sInstance;

    bool                    m_bRun;             //�Ƿ�����
    bool                    m_bAllExit;         //�Ƿ������߳��˳�
    int                     m_iNoUsePool;       //�Ƿ�ʹ���̳߳أ�1��ʹ�� 0ʹ��

    vector<ST_THREAD_FUN*>  m_vecThread;        //�߳����ݽṹ����
    Handle                  m_hSection;         //�ٽ���
};

