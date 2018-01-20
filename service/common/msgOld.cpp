/*********************************************************************
    *�ļ�˵��: �ṩͬ����ģ��֮���ͨѶ����
    *����: luojing
    *����: 2013/10/20
    *�޸���־:  2013/10/26 CW0793 ����
                
***********************************************************************/
#include <vector>
#include <list>
#include "common.h"
#include "sysapi.h"

using std::list;
using std::vector;

//�ص������ṹ�壬����Ϣ�ã���ģ�����ע���˲��ܽ�����Ϣ
struct ST_CALLBACK
{
    char dll[DLL_NAME_LEN+1];
    RegistCallBackProc callback;
    ST_CALLBACK()
    {
        memset(dll, 0, DLL_NAME_LEN+1);
        callback = NULL;
    }
};

//��Ϣ�ṹ�壬���ڽ�����Ϣ���У�post��Ϣʱ�õ�
struct ST_MSG
{
    char dll[DLL_NAME_LEN+1];
    unsigned int wparam;
    void *       lparam;  //
    unsigned int isize;   //lparam�ֽڴ�С
    SYSTEM_MSG_TYPE type;
    ST_MSG()
    {
        memset(dll, 0, DLL_NAME_LEN+1);
        wparam = 0;
        lparam = NULL;
        isize = sizeof(unsigned int);
        type = EVENT_USUALLY;
    }
};

vector<ST_CALLBACK*> g_callBack;    //�ص������ϼ�

BOOL g_ismsgInit = FALSE;  //�Ƿ������У�post��Ϣ��������ʱ������
list<ST_MSG*> g_msgList; //����ȫ����Ϣ����
HANDLE g_hThreadPost = INVALID_HANDLEVALUE;   //post��Ϣ�߳̾��
//CRITICAL_SECTION g_cs;
HANDLE g_cs;

void ClearMsgList()
{
    ST_MSG* pst_msg = NULL;

    sys_EnterCriticalSection(g_cs);
    while(g_msgList.size() > 0)
    {
        pst_msg = g_msgList.front();
        g_msgList.pop_front();
        if (NULL != pst_msg)
        {
            if (NULL != pst_msg->lparam)
            {
                delete pst_msg->lparam;
                pst_msg->lparam = NULL;
            }
            delete pst_msg;
            pst_msg = NULL;
        }
    }
    sys_LeaveCriticalSection(g_cs);
}

int WINAPI MsgPostProc(void* lpThreadParameter)
{
    vector<ST_CALLBACK*>::iterator it;
    ST_MSG* pst_msg = NULL;

    while(g_ismsgInit)
    {
        if (0 == g_msgList.size())
        {
            sys_Sleep(100);
            continue;
        }
        sys_EnterCriticalSection(g_cs);
        pst_msg = g_msgList.front();
        g_msgList.pop_front();  //ֻ����һ�Σ�����Ϣ����
        sys_LeaveCriticalSection(g_cs);

        for(it = g_callBack.begin(); it!= g_callBack.end(); it++)
        {
            if (0 == strcmp(pst_msg->dll, (*it)->dll) ||
                0 == strcmp(pst_msg->dll, MODULE_ALL))
            {
                ((*it)->callback)(pst_msg->wparam, pst_msg->lparam, pst_msg->type);    //������Ϣ
            }
        }

        if (NULL != pst_msg)
        {
            if (NULL != pst_msg->lparam)
            {
                delete pst_msg->lparam;
                pst_msg->lparam = NULL;
            }
            delete pst_msg;
            pst_msg = NULL;
        }
		
    }
    return 0;
}

//��ʼ��post��Ϣ����
EXPORT_C bool WINAPI init_msg()
{
    if (g_ismsgInit)
    {
        return true;
    }
    g_cs =  sys_InitializeCriticalSection();
    g_ismsgInit = TRUE;
    g_hThreadPost = sys_CreateThread(MsgPostProc, NULL);
    return true;
}

EXPORT_C void WINAPI uninit_msg()
{
    if (g_ismsgInit)
    {
        g_ismsgInit = FALSE;
        sys_WaitForSingleObject(g_hThreadPost, INFINITE);   //�ȴ��߳��˳�
        sys_CloseHandle(g_hThreadPost);
        ClearMsgList();
    }
    for(vector<ST_CALLBACK*>::iterator it = g_callBack.begin(); it != g_callBack.end(); it++)
    {
        LOG_INFO("callback proc address.0x%x\n", (*it)->callback);
        delete *it;
    }

	sys_DeleteCriticalSection(g_cs);
}

//ע����Ϣ���պ���
EXPORT_C bool WINAPI register_notify(LPCSTR dllName, RegistCallBackProc fun)
{
    vector<ST_CALLBACK*>::iterator it;
    ST_CALLBACK* pst_callback = NULL;

    RETURN_NULLPTR(dllName, false);
    RETURN_NULLPTR(fun, false);
    RETURN_NULLPTR(strlen(dllName), false);

    for(it = g_callBack.begin(); it!= g_callBack.end(); it++)
    {
        if (0 == strcmp(dllName, (*it)->dll))
        {//���������ظ�ע����
            return false;
        }
    }
    pst_callback = new ST_CALLBACK;
    strcpy_s(pst_callback->dll, DLL_NAME_LEN, dllName);
    pst_callback->callback = fun;
    g_callBack.push_back(pst_callback);
    return true;
}

EXPORT_C void WINAPI unregister_notify(LPCSTR dllName)
{
    vector<ST_CALLBACK*>::iterator it;
    ST_CALLBACK* pst_callback = NULL;

    RETURN_NULL(dllName);
    RETURN_NULL(strlen(dllName));

    for(it = g_callBack.begin(); it!= g_callBack.end(); it++)
    {
        if (0 == strcmp(dllName, (*it)->dll))
        {
            pst_callback = *it;
            g_callBack.erase(it);
            delete pst_callback;
            pst_callback = NULL;
            break;
        }
    }
}

EXPORT_C int WINAPI msg_send(unsigned int wparam, void *lparam,
    LPCSTR dllName, SYSTEM_MSG_TYPE msgType)
{
    vector<ST_CALLBACK*>::iterator it;
    bool isAll = false;

    if (!dllName)
    {
        isAll = true;
    }
    else if (0 == strcmp(dllName, MODULE_ALL))
    {
        isAll = true;
    }

    for(it = g_callBack.begin(); it!= g_callBack.end(); it++)
    {
        if (isAll)
        {//�գ���ȫ������
            if ((*it)->callback)
            {
                ((*it)->callback)(wparam, lparam, msgType);
            }
            continue;
        }
        if (0 == strcmp(dllName, (*it)->dll))
        {
            if ((*it)->callback)
            {
                return ((*it)->callback)(wparam, lparam, msgType);
            }
        }
    }

    return isAll ? COMMON_NORMAL : ERR_NOREGIST;
}

EXPORT_C int WINAPI msg_post(unsigned int wparam,void *lparam, unsigned int lsize,
    LPCSTR dllName, SYSTEM_MSG_TYPE msgType)
{
    vector<ST_CALLBACK*>::iterator it;

    if (!g_ismsgInit)
    {
        init_msg();
    }

    ST_MSG* pst_msg = new ST_MSG;
    if (dllName)
    {
        strcpy_s(pst_msg->dll, DLL_NAME_LEN, dllName);
    }
    else
    {
        strcpy_s(pst_msg->dll, DLL_NAME_LEN, MODULE_ALL);
    }
    
    pst_msg->wparam = wparam;
    if (NULL != lparam)
    {
        pst_msg->lparam = new char[lsize];
        memcpy(pst_msg->lparam,lparam, lsize);
    }
    else
    {
        pst_msg->lparam = NULL;
    }
    pst_msg->type = msgType;

    sys_EnterCriticalSection(g_cs);
    g_msgList.push_back(pst_msg);
    sys_LeaveCriticalSection(g_cs);
    return COMMON_NORMAL;
}