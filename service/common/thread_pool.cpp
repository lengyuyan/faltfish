#include "sysapi.h"
#include "common.h"
#include "thread_pool.h"

#include "GolobalError.h"

CThreadPool* CThreadPool::m_sInstance = NULL;
CThreadPool::CThreadPool(void)
{
    m_bRun = true;
    m_bAllExit = true;
    m_iNoUsePool = 1;//get_cfg_int(COMM_CFG_KEY, NUSE_POOL);
    m_hSection = sys_InitializeCriticalSection();
}

CThreadPool::~CThreadPool(void)
{
    int tm = clock();
    vector<ST_THREAD_FUN*>::iterator it;

    m_bRun = false; //��ʶ�˳��ź�
    sys_EnterCriticalSection(m_hSection);
    for(it = m_vecThread.begin(); it != m_vecThread.end(); it++)
    {//�����ź�
        if (INVALID_HANDLE != (*it)->startEvt)
        {//�̳߳زŻ����¼�
            sys_SetEvent((*it)->startEvt);
        }
    }
    sys_LeaveCriticalSection(m_hSection);

    while(!m_bAllExit)
    {//�ȴ��˳�
        sys_Sleep(10);
        if (clock() - tm > 5000)
        {
            tm = clock();
            LOG_WARN("����5s ���߳�δ�˳�����skt���δ�رգ���ȷ�ϡ�");
        }
    }

    if (!m_bAllExit)
    {
        for(it = m_vecThread.begin(); it != m_vecThread.end(); it++)
        {//�����ź�
            sys_TerminateThread((*it)->thread);
            sys_CloseHandle((*it)->thread);
            if (INVALID_HANDLE != (*it)->hIdleEvt)
            {//�̳߳زŻ����¼�
                sys_CloseHandle((*it)->hIdleEvt);
            }
            if (INVALID_HANDLE != (*it)->startEvt)
            {//�̳߳زŻ����¼�
                sys_CloseHandle((*it)->startEvt);
            }
            delete *it;
        }
    }

    LOG_INF2("all thread pool exit.");
    sys_DeleteCriticalSection(m_hSection);
}

//��ȡʵ��
CThreadPool* CThreadPool::GetInstance()
{
    if(NULL == m_sInstance)
    {
        m_sInstance = new CThreadPool();
    }
    return m_sInstance;
}

//ɾ��ʵ��
void CThreadPool::ReleaseInstance()
{
    FREE_ANY(m_sInstance);
}

//�̳߳��߳�ִ�к���
int WINAPI CThreadPool::ThreadFun(void* param)
{
    int ret = 0;
    ST_THREAD_FUN* pdata = (ST_THREAD_FUN*)param;
    CThreadPool* pMain = (CThreadPool*)(pdata->self);
    ret = pMain->Process(pdata);
    return ret;
}

int CThreadPool::Run(PROCEX fun, void* param)
{
    int nCnt = 0;
    vector<Handle> vecThread;
    vector<ST_THREAD_FUN*>::iterator it;

    RETURN_CHK(fun, ERR_INPUT_PARAM);

    sys_EnterCriticalSection(m_hSection);
    nCnt = (int)m_vecThread.size();
    LOG_INF2("CThreadPool::Run(0x%x,0x%x), current total thread=%d.", fun, param, nCnt);
    if (m_iNoUsePool || 0 == nCnt)
    {//��ʹ���̳߳�
        RunAsNewThread(fun, param);
        sys_LeaveCriticalSection(m_hSection);
        return 0;
    }

    for(it = m_vecThread.begin(); it != m_vecThread.end(); it++)
    {
        vecThread.push_back((*it)->hIdleEvt);
    }
    int res = sys_WaitForMultipleObjects(nCnt, &vecThread[0], FALSE, 0);
    if (res >= 0 && res < nCnt)
    {//�ɹ����򷵻ؾ����¼�����
        m_vecThread[res]->fun = fun;
        m_vecThread[res]->param = param;
        sys_ResetEvent(m_vecThread[res]->hIdleEvt); //�����ź�
        sys_SetEvent(m_vecThread[res]->startEvt);   //�ÿ�ʼ�ź�
    }
    else if (ERR_TIMEOUT == res)
    {//δ�ҵ������߳�
        RunAsNewThread(fun, param);
    }
    sys_LeaveCriticalSection(m_hSection);
    return 0;
}

void CThreadPool::DelThreadData(ST_THREAD_FUN* pThread)
{
    int nCnt = 0;
    vector<ST_THREAD_FUN*>::iterator it;

    sys_EnterCriticalSection(m_hSection);
    for(it = m_vecThread.begin(); it != m_vecThread.end(); it++)
    {
        if ((*it) == pThread)
        {//�ҵ�
            LOG_INF2("CThreadPool::DelThreadData 0x%x,0x%x,0x%x", (*it)->thread, (*it)->startEvt, (*it));
            sys_CloseHandle((*it)->thread);
            if (INVALID_HANDLE != (*it)->startEvt)
            {
                sys_CloseHandle((*it)->startEvt);
            }
            if (INVALID_HANDLE != (*it)->hIdleEvt)
            {
                sys_CloseHandle((*it)->hIdleEvt);
            }
            delete *it;
            m_vecThread.erase(it);
            break;
        }
    }
    nCnt = (int)m_vecThread.size();
    sys_LeaveCriticalSection(m_hSection);

    if (0 == nCnt)
    {//�����߳��˳�
        m_bAllExit = true;
    }
}

//�ⲿ����
void CThreadPool::RunAsNewThread(PROCEX fun, void* param)
{
    ST_THREAD_FUN* pThread = new ST_THREAD_FUN;

    m_bAllExit = false;
    pThread->fun = fun;
    pThread->param = param;
    pThread->self = this;
    pThread->startEvt = m_iNoUsePool ? INVALID_HANDLE : sys_CreateEvent(FALSE, TRUE, NULL);
    pThread->hIdleEvt = m_iNoUsePool ? INVALID_HANDLE : sys_CreateEvent(TRUE, FALSE, NULL);
    pThread->thread = sys_CreateThread(ThreadFun, pThread);
    m_vecThread.push_back(pThread);
}

int CThreadPool::Process(ST_THREAD_FUN* pData)
{
    int res = 0;

    LOG_INF2("CThreadPool::Process..handle=0x%x", pData->thread);
    while(m_bRun)
    {//�Ƿ���������
        if (INVALID_HANDLE != pData->startEvt)
        {//�̳߳�
            res = sys_WaitForSingleObject(pData->startEvt, 60 * 1000 * 2);    //�ȴ�2����
            if (res != RETURN_OK || !m_bRun)
            {//��ʱ�����˳�����ʱ, �߳��˳�
                break;
            }
        }

        if (pData->fun)
        {
            LOG_INF2("CThreadPool::Process(0x%x,0x%x) run..", pData->fun, pData->param);
            pData->fun(pData->param); //ִ�к���
            LOG_INF2("CThreadPool::Process thread:0x%x fun:0x%x run finished.", pData->thread, pData->fun);
        }

        if (INVALID_HANDLE == pData->hIdleEvt)
        {//��ʹ���̳߳�
            break;
        }
        sys_SetEvent(pData->hIdleEvt);  //����״̬
    }

    DelThreadData(pData); //������ɾ��
    return 0;
}