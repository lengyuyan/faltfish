/*********************************************************************
    *�ļ�˵��: �ṩ���̼�ͨ�Ź���,����һ�����̵���create��������ˣ�
            ����һ�����̵���conn���ӷ���ˣ��ɹ��������ģ��socket
            ���෢����Ϣ������send_text������Ϣ recv_fexe������Ϣ
            ps:һ�������ֻ�ܺ�һ���ͻ���ͨ��
    *����: luojing
    *����: 2013/10/20
    *�޸���־:  2013/10/23 CW0793 ����
                
***********************************************************************/

#include <string>
#include "GlobalDefine.h"
#include "sysapi.h"
#include "common.h"

#define SSK_FILE_EVENT  "OCTOPUS_COMMON_SSK_EVENT-CW1024"   //�����ڴ���ʻ�����
#define MAPPING_NAME    "OCTOPUS_COMMON_SSK_MAPPING-CW1024" //�����ڴ湲�� ����
#define MAPPING_EVENT   "OCTOPUS_COMMON_SSK_MAPPING-CW1023" //�����ڴ� ������
#define MAPPING_FINISH  "OCTOPUS_COMMON_SSK_FINISH-CW1024"  //�����ڴ� ������� ������
#define PAGE_READWRITE 0x04
#define FILE_MAP_READ  0x0004
#define FILE_MAP_WRITE 0x0002

#define MAX_SEND_SIZE   4096    //���η�����󳤶� 4kb
#define MAX_BUFF_SIZE   128     //��Ϣ����������
#define MAX_MSG_SIZE    1014    //��Ϣ�峤��
#define SSK_ARRAY_LEN   32      //ssk����������
#define SSK_NAME_LEN    30      //ssk���Ƴ���
#define SERVER 0x0100
#define CLIENT 0x1000
//#define SSK_CLOSE_MSG   1       //�Է��Ѿ��ر�������
//#define SSK_CONN_MSG    2       //�ͻ���������������
#define BUFF_VALID      0       //δ��
#define BUFF_INVALID    1       //����

struct ST_SSK          //�ɲ����������
{
    short sc;    //�ͻ��˻��Ƿ���ˣ� SERVER  CLIENT   ���ܻ�˳��
    short index; //ͨѶ�ϼ��������е�����
    ST_SSK(DWORD copy = INVALID_SSK)
    {
        index = copy>>16 & 0xFFFF;
        sc = copy & 0xFFFF;
    }

    friend bool operator==(DWORD dw, ST_SSK& ssk)
    {
        return (ssk.index == (dw>>16 & 0xFFFF) && ssk.sc==(dw&0xFFFF));
    }
    ST_SSK& operator=(DWORD copy)
    {
        index = copy>>16 & 0xFFFF;
        sc = copy & 0xFFFF;
        return *this;
    }
};

struct ST_EXE_MSG //��Ϣ�ṹ��
{
    ST_EXE_MSG* next;               //��һ������ָ�� ����list
    int         size;               //���ݴ�С
    char        msg[MAX_MSG_SIZE];  //����,���Ǵ洢����
    bool        isNext;             //�Ƿ����и���Ϣ������Ϣ�Ƚϴ��������и����
    char        reserved;           //��ʾ�û������Ƿ�ʹ��
};

ST_EXE_MSG* msg_malloc();           //��Ϣ���������� �����ڴ�
void msg_free(ST_EXE_MSG* pst_msg); //��Ϣ���������� �ͷ��ڴ�

struct MSG_LIST //��Ϣ����
{
    ST_EXE_MSG*     top;    //ͷ���ָ��
    ST_EXE_MSG*     end;    //β�ڵ�ָ��
    unsigned int    cnt;    //��ǰ�ж��ٸ���Ա

    unsigned int size(){return cnt;}
    ST_EXE_MSG* front(){return top;}
    void pop_front()
    {
        ST_EXE_MSG* ptmp = top;
        top = top->next;
        cnt--;
        if (0 == cnt)
        {
            end = NULL;
        }
    }
    void push_back(ST_EXE_MSG* pmsg)
    {
        if (!pmsg)
        {
            return;
        }

        pmsg->next = NULL;
        if (0 == cnt)
        {
            top = pmsg;
        }
        else
        {
            end->next = pmsg;
        }
        cnt++;
        end = pmsg;
    }
};

struct ST_EXE_SOCKET //ssk �洢���ṹ��
{
    char            name[SSK_NAME_LEN]; //��������,��������Ҳ��,���31λ
    bool            connected;          //�Ƿ������ӿͻ���
    char            reserved;           //��ʾ�û������Ƿ�ʹ��
    MSG_LIST        msgList[2];         //����˺Ϳͻ�����Ϣ����
    ST_EXE_SOCKET*  next;               //��һ������ָ�룬����node

    bool IsValid(){return 0 == *name ? false : true;}       //�жϸ�ssk server�Ƿ���Ч
    bool IsConn() {return connected;}   //�жϸ�ssk�Ƿ��Ѿ��пͻ�����

    void ClearMsg(unsigned int sc)
    {
        ST_EXE_MSG* pst_msg = NULL;
        int index = (SERVER == sc) ? 0 : 1;
        while(msgList[index].size() > 0)
        {
            pst_msg = msgList[index].front();
            msgList[index].pop_front();
            msg_free(pst_msg);
        }
    }
    void ClearSSK(unsigned int sc)
    {
        connected = false;
        ClearMsg(sc);

        if (SERVER == sc)    //����server��
        {
            ClearSSK(CLIENT);   //ͬʱ�رտͻ���
            memset(name, 0, sizeof(name));
        }
    }
};

struct SSK_NODE //ssk����
{
    ST_EXE_SOCKET* top;
    ST_EXE_SOCKET* end;
    unsigned int cnt;

    SSK_NODE(int cnt)
    {
        top = NULL;
        end = NULL;
        cnt = 1;
    }
    unsigned int size(){ return cnt;}
    bool push(ST_EXE_SOCKET* pnode)
    {
        if (NULL == pnode)
        {
            return false;
        }
        pnode->next = NULL;
        if (NULL == top)
        {
            top = pnode;
        }
        else
        {
            end->next = pnode;
        }
        cnt++;
        end = pnode;
        return true;
    }
    bool popup(ST_EXE_SOCKET* pnode)
    {
        ST_EXE_SOCKET* ptmp;
        ST_EXE_SOCKET* prev;

        if (NULL == pnode || NULL == top)
        {
            return false;
        }

        if (pnode == top)   //ͷ�ڵ�
        {
            if (top == end) //ֻ��һ���ڵ�
            {
                end = NULL;
            }
            top = top->next;
            cnt--;
            return true;
        }

        ptmp = top->next;
        prev = top->next;
        while(NULL != ptmp)
        {
            if (pnode == ptmp)  //�ҵ�Ŀ��
            {
                if (end == ptmp)    //β�ڵ�
                {
                    end = prev;
                }
                prev->next = ptmp->next;
                cnt--;
                return true;
            }
            prev = ptmp;
            ptmp = ptmp->next;
        }
        return false;
    }
    
    bool ChkName(LPCSTR lpName) //���һ���������Ƿ����
    {
        ST_EXE_SOCKET* ptmp = top;
        if (NULL == lpName)
        {
            return false;
        }
        for(int i=0; i<(int)cnt; i++)
        {
            if (0 == strcmp(ptmp->name, lpName))
            {//������ͬ
                return false;
            }
            ptmp = ptmp->next;
        }
        return true;
    }
};

#pragma data_seg("SharedData")
//���¶��������Ϊ�������ݣ������е���dll�Ľ��̿��Թ���,�������ʼ��
ST_EXE_SOCKET   g_sockArray[SSK_ARRAY_LEN] = {0};   //��Ե�ͨѶ�ϼ������֧��SSK_ARRAY_LEN��
ST_EXE_MSG      g_msgArray[MAX_BUFF_SIZE] = {0};    //��Ϣ������, ���֧��MAX_BUFF_SIZE����Ϣ
BOOL            g_isMainExist = TRUE;               //�������Ƿ���ڣ�
#pragma data_seg()
#pragma comment(linker, "/SECTION:SharedData,RWS")

//�Լ����̲����õ�ȫ�ֱ���
ST_EXE_SOCKET*   g_psockArray = NULL;    //��Ե�ͨѶ�ϼ����ڴ�ӳ���ļ��еĻ���
ST_EXE_MSG*      g_pmsgArray = NULL;     //��Ϣ������, 
SSK_NODE*        g_psskNode = NULL;      //ssk����װ��������Чssk

HANDLE g_hEvent = INVALID_HANDLEVALUE;     //д��Ϣ�����ٽ���
HANDLE g_hMapFile = INVALID_HANDLEVALUE;   //�ڴ�ӳ���ļ����
HANDLE g_hMainThread = INVALID_HANDLEVALUE;//д�ڴ�ӳ���ļ��߳̾��
HANDLE g_hMapEvent = INVALID_HANDLEVALUE;  //д�ڴ�ӳ���ļ��¼�
HANDLE g_hMapFinish = INVALID_HANDLEVALUE; //д�ڴ�ӳ���ļ������¼�
BOOL   g_isskInit = FALSE;                  //�Ƿ��Ѿ���ʼ��
BOOL   g_isMain = FALSE;                    //�Ƿ�Ϊ�����̣�

//�����ٽ���
void EnterCSection()
{
    if (INVALID_HANDLEVALUE == g_hEvent)
    {
        g_hEvent = sys_CreateEvent(FALSE, TRUE, SSK_FILE_EVENT);    //���У����ȡ���е�
    }
    sys_WaitForSingleObject(g_hEvent,INFINITE);
}

//�뿪�ٽ���
void LeaveCSection()
{
    if (INVALID_HANDLEVALUE == g_hEvent)
    {
        g_hEvent = sys_CreateEvent(FALSE, TRUE, SSK_FILE_EVENT);
    }
    sys_SetEvent(g_hEvent);
}

//���һ��ssk����Ƿ���Ч
int chk_ssk(ST_SSK* pssk)
{
    RETURN_NULLPTR(pssk, FALSE);
    if (INVALID_SSK == *pssk)
    {
        return ERR_PARAM;
    }

    if (pssk->index < 0 || pssk->index >= SSK_ARRAY_LEN ||
        (SERVER != pssk->sc && CLIENT != pssk->sc))
    {
        LOG_ERR("the ssk is invalid. sc=%d,index=%d", pssk->sc, pssk->index);
        return ERR_PARAM;
    }

    if (!g_sockArray[pssk->index].IsValid())
    {
        return ERR_NOINIT;
    }

    return COMMON_NORMAL;
}

//���������� �����ڴ�
ST_EXE_MSG* msg_malloc()
{
    ST_EXE_MSG* pst_msg = NULL;
    while(g_isskInit)
    {
        for(unsigned int i= 0; i<MAX_BUFF_SIZE; i++)
        {
            if (BUFF_VALID == g_msgArray[i].reserved) //δʹ��
            {
                g_msgArray[i].reserved = BUFF_INVALID; //��ʾ��ʹ��
                pst_msg = g_msgArray + i;
                return pst_msg;
            }
        }
        sys_Sleep(1);
        LOG_WARN("msg space all used.");
    }
    return NULL;    //������������
}

//���������� �ͷ��ڴ�
void msg_free(ST_EXE_MSG* pst_msg)
{
    if (NULL == pst_msg)
    {
        return;
    }
    __int64 i = pst_msg - g_msgArray;
    if (i >= MAX_BUFF_SIZE)
    {
        return;
    }
    memset(pst_msg, 0, sizeof(ST_EXE_MSG));
}

ST_EXE_SOCKET* ssk_malloc() //ssk����������
{
    ST_EXE_SOCKET* pst_ssk = NULL;
    for(unsigned int i= 0; i<SSK_ARRAY_LEN; i++)
    {
        if (BUFF_VALID == g_sockArray[i].reserved) //δʹ��
        {
            pst_ssk = g_sockArray + i;
            pst_ssk->reserved = BUFF_INVALID;   //��ʾ��ʹ��
            return pst_ssk;
        }
    }
    return NULL;    //������������
}

void ssk_free(ST_EXE_SOCKET* pst_ssk) //ssk������ �ͷ��ڴ�
{
    if (NULL == pst_ssk)
    {
        return;
    }

    __int64 i = pst_ssk - g_sockArray;
    if (i >= SSK_ARRAY_LEN)
    {
        return;
    }
    memset(g_sockArray + i, 0, sizeof(ST_EXE_SOCKET));
}

//����������Ƿ��˳�
//���������д��̣߳�����ѹ����ڴ�����ݿ����ڴ�ӳ���ļ�
int WINAPI ThreadChkExe(void* lpThreadParameter) 
{
    while(TRUE)
    {//ռ��������
        if (!g_isskInit)
        {
            return 0;
        }

        if (g_isMainExist)
        {//�����̴��ڣ��ǾͲ��øɻ�
            sys_Sleep(500);
            continue;
        }
        g_isMainExist = TRUE;
        g_isMain = TRUE;    //���Լ���Ϊ������
        break;
    }
    LOG_INFO("this is main tanger.");
    while(TRUE)
    {//�����̸���д�ڴ�ӳ���ļ�
        sys_WaitForSingleObject(g_hMapEvent, INFINITE);
        if (!g_isskInit)    //�˳��źţ�
        {
            break;
        }
        EnterCSection(); //������������д����
        memcpy(g_psockArray, g_sockArray, sizeof(g_sockArray));
        memcpy(g_pmsgArray, g_msgArray, sizeof(g_msgArray));
        LeaveCSection(); //�˴����ͷ��źţ��ȴ��½��̿�����ɺ����ͷ�?
        sys_SetEvent(g_hMapFinish);
    }
    return 0;
}

void UpdateSSKArray()
{
    __int64 index = 0;
    ST_EXE_SOCKET* ptmp = g_psskNode->top;
    ST_EXE_MSG* pmsg = NULL;
    sys_WaitForSingleObject(g_hMapFinish, INFINITE);
    EnterCSection(); //�˴�Ӧ�ñ�֤���źţ��������̶�Ӧ���޷�д��
    while(ptmp)
    {
        index = ptmp - g_sockArray;       //�����ַƫ��
        memcpy(ptmp, g_psockArray + index, sizeof(ST_EXE_SOCKET));  //����һ��ssk��Ч������
        for(int i= 0; i< 2; i++)
        {
            pmsg = ptmp->msgList[i].top;
            while(pmsg)
            {
                index = pmsg - g_msgArray;       //����msg��ַƫ��
                memcpy(pmsg, g_pmsgArray + index, sizeof(ST_EXE_MSG));  //����һ��msg��Ч������
                pmsg = pmsg->next;
            }
        }
        ptmp = ptmp->next;
    }
    LeaveCSection();    //������ϣ��ͷ�д���¼�
}

EXPORT_C bool WINAPI init_ssk()
{
    if (g_isskInit)
    {
        return true;
    }

    UINT sskMemory = sizeof(ST_EXE_SOCKET) * SSK_ARRAY_LEN;
    UINT msgMemory = sizeof(ST_EXE_MSG) * MAX_BUFF_SIZE;
    UINT nodeMemory = sizeof(SSK_NODE);
    char* pmalloc = NULL;

    g_isskInit = TRUE;
    g_hMapEvent = sys_CreateEvent(FALSE, FALSE, MAPPING_EVENT);
    g_hMapFinish = sys_CreateEvent(FALSE, FALSE, MAPPING_FINISH);
    g_hMainThread = sys_CreateThread(ThreadChkExe, NULL);

    g_hMapFile = sys_OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, MAPPING_NAME);
    if (NULL == g_hMapFile)
    {
        LOG_WARN("sys_OpenFileMapping fail, create the mapping File.");
        g_hMapFile = sys_CreateFileMapping(INVALID_HANDLEVALUE, PAGE_READWRITE, 
            0, nodeMemory+msgMemory+sskMemory, MAPPING_NAME);
        g_isMain = TRUE;        //��һ�����̵���ssk Ĭ����Ϊ������
        g_isMainExist = FALSE;  //��ʾ�����̲����ڣ��õ�ǰ�߳̿�����ռ������λ��
    }

    if (NULL == g_hMapFile)  //�ٴ�ʧ�ܣ���ķ���������
    {
        LOG_WARN("sys_CreateFileMapping Create File error.");
        uninit_ssk();
        return false;
    }

    pmalloc = (char*)sys_MapViewOfFile(g_hMapFile, FILE_MAP_READ | FILE_MAP_WRITE);

    g_psskNode = (SSK_NODE*)pmalloc;
    g_psockArray = (ST_EXE_SOCKET*)(pmalloc + nodeMemory);
    g_pmsgArray = (ST_EXE_MSG*)(pmalloc + nodeMemory + sskMemory);

    if (g_isMain)
    {//�մ����ļ�����ʼ����ֵ
        memset(pmalloc, 0, nodeMemory+msgMemory+sskMemory);
    }
    else
    {//�����̴��ڣ���ӳ���ļ����¹����ڴ�
        sys_SetEvent(g_hMapEvent);  //�����¼��������̿�ʼ�ӻ��������µ��ڴ�ӳ���ļ�
        UpdateSSKArray();           //���µ���������
    }

    return true;
}

EXPORT_C void WINAPI uninit_ssk()
{
    if (INVALID_HANDLEVALUE != g_hEvent)
    {
        sys_CloseHandle(g_hEvent);
    }

    if (!g_isskInit)
    {
        return;
    }

    void* pmalloc = (void*)g_psskNode;
    if (pmalloc)
    {
        sys_UnmapViewOfFile(pmalloc);
    }

    if (NULL != g_hMapFile)
    {
        sys_CloseHandle(g_hMapFile);
    }

    g_isskInit = FALSE;
    if (TRUE == g_isMain)       //�������˳�
    {
        sys_SetEvent(g_hMapEvent);
        EnterCSection();
        g_isMainExist = FALSE;  //�ñ�����ȥ
        LeaveCSection();
    }
    
    sys_WaitForSingleObject(g_hMainThread, INFINITE);
    sys_CloseHandle(g_hMapEvent);
    sys_CloseHandle(g_hMapFinish);
    sys_CloseHandle(g_hMainThread);
}

EXPORT_C DWORD WINAPI ssk_server(LPCSTR sskName)
{
    ST_SSK ssk;
    DWORD ret = 0;

    RETURN_NULLPTR(sskName, INVALID_SSK);

    if (!g_isskInit)
    {
        init_ssk();
    }

    if (false == g_psskNode->ChkName(sskName))
    {//����ssk����������������Ƿ��ظ�
        LOG_ERR("init ssk error . the ssk name already exists.");
        return INVALID_SSK;
    }

    EnterCSection();
    ST_EXE_SOCKET* pssk = ssk_malloc();
    if (NULL == pssk)
    {
        LeaveCSection();
        LOG_ERR("ssk_malloc return NULL.");
        return INVALID_SSK;
    }
    ssk.index = (int)(pssk - g_sockArray);
    ssk.sc = SERVER;
    g_psskNode->push(pssk);
    strcpy_s(pssk->name, SSK_NAME_LEN, sskName);
    memcpy(&ret, &ssk, sizeof(DWORD));
    LeaveCSection();
    return ret;
}

EXPORT_C DWORD WINAPI ssk_conn(LPCSTR sskName)
{
    ST_SSK st_ssk;
    DWORD ssk = 0;
    //char ch = SSK_CONN_MSG;    //��������������ʾ�������ˡ�

    RETURN_NULLPTR(sskName, INVALID_SSK);

    if (!g_isskInit)
    {
        init_ssk();
    }

    ST_EXE_SOCKET* ptmp = g_psskNode->top;
    for(int i=0; i<(int)g_psskNode->cnt; i++)
    {
        if (0 == strcmp(ptmp->name, sskName))
        {//������ͬ
            if (true == ptmp->connected)
            {
                return INVALID_SSK;
            }

            EnterCSection();
            ptmp->connected = true;    //��ʾ��������
            LeaveCSection();

            st_ssk.index =(int)(ptmp - g_sockArray);
            st_ssk.sc = CLIENT;
            memcpy(&ssk, &st_ssk, sizeof(DWORD));
            return ssk;
        }
        ptmp = ptmp->next;
    }

    return INVALID_SSK;
}

EXPORT_C int WINAPI ssk_status(DWORD ssk)
{
    ST_SSK st_ssk = ssk;
    //char ch = SSK_CLOSE_MSG;    //����ֹͣ������
    RETURN_NULLPTR(g_isskInit, ERR_NOINIT);

    int ret = chk_ssk(&st_ssk);
    if (COMMON_NORMAL != ret)
    {
        return ret;
    }

    if (!g_sockArray[st_ssk.index].IsConn())
    {
        return ERR_NOCONNECT;
    }

    return COMMON_NORMAL;
}

EXPORT_C int WINAPI ssk_is_server(DWORD ssk)
{
    ST_SSK st_ssk = ssk;
    int ret = chk_ssk(&st_ssk);
    if (COMMON_NORMAL != ret)
    {
        return ret;
    }

    return (SERVER == st_ssk.sc) ? 0 : 1;
}

EXPORT_C void WINAPI ssk_close(DWORD ssk)
{
    ST_SSK st_ssk = ssk;
    //char ch = SSK_CLOSE_MSG;    //����ֹͣ������
    if (!g_isskInit)
    {
        return;
    }

    int ret = chk_ssk(&st_ssk);
    if (COMMON_NORMAL != ret)
    {
        return;
    }

    //ssk_send(ssk, &ch, sizeof(ch));  //����������,1��ʾ�ѹر�
    EnterCSection();
    ST_EXE_SOCKET* pssk = &g_sockArray[st_ssk.index];
    pssk->ClearSSK(st_ssk.sc);
    if (SERVER == st_ssk.sc)
    {
        g_psskNode->popup(pssk);
        ssk_free(pssk);
    }
    LeaveCSection();
}

EXPORT_C bool WINAPI regist_ssk_recv(DWORD ssk, RegistRecvProc proc)
{
    RETURN_NULLPTR(g_isskInit, false);

    return false;
}

#define TIME_BASE 1     //����� ��λ ms
EXPORT_C int WINAPI ssk_recv(DWORD ssk, LPSTR pdata, int len, DWORD timeout)
{
    ST_SSK st_ssk = ssk;
    ST_EXE_MSG* pst_msg = NULL;
    int datalen = 0;
    int nLen = 0;
    int ret = COMMON_NORMAL;
    bool isNext = true;
    LPSTR ptmp = pdata;
    int tout = (int)timeout;

    RETURN_NULLPTR(g_isskInit, ERR_NOINIT);

    int index = (SERVER == st_ssk.sc) ? 0 : 1;
    while(tout >= 0 || INFINITE == timeout)
    {
        ret = ssk_status(ssk);
        if (COMMON_NORMAL != ret)
        {
            return ret;
        }

        if (0 == g_sockArray[st_ssk.index].msgList[index].size())   //�ж���û����Ϣ
        {
            sys_Sleep(TIME_BASE);
            tout -= TIME_BASE;
            continue;
        }
        tout = 0;
        break;
    }

    if (0 != tout)
    {
        return ERR_TIMEOUT2;
    }

    while(isNext)
    {//��ֹ�ж�����ϢҪ�ϲ������Ը���ѭ��
        if (0 == g_sockArray[st_ssk.index].msgList[index].size())   //�ж���û����Ϣ
        {
            break;
        }
        EnterCSection();
        pst_msg = g_sockArray[st_ssk.index].msgList[index].front(); //ȡ����һ����Ϣ
        g_sockArray[st_ssk.index].msgList[index].pop_front();
        datalen = Min(len, pst_msg->size);
        if (datalen > 0 && COMMON_NORMAL == ret)    //���Կ�������
        {//����
            memcpy(ptmp, pst_msg->msg, datalen);
            ptmp += datalen;
            nLen += datalen;
            len -= datalen;
        }
        if (datalen <= 0 && COMMON_NORMAL == ret)   //�޴洢�ռ�
        {//С��0 ��˵��Ӧ�ò���Ļ�������С��������������Ҫ��������Ϣ����
            LOG_ERR("recv data size is so small.datalen=%d, but input len=%d", pst_msg->size, len);
            ret = ERR_TOOLARGE;
        }
        
        isNext = pst_msg->isNext;
        msg_free(pst_msg);
        LeaveCSection();
    }
    
    return nLen;
}

EXPORT_C int WINAPI ssk_send(DWORD ssk, LPSTR pdata, int len)
{
    ST_SSK st_ssk = ssk;
    int datalen = 0;
    int sendlen = 0;

    RETURN_NULLPTR(g_isskInit, ERR_NOINIT);

    int ret = ssk_status(ssk);
    if (COMMON_NORMAL != ret)
    {
        return ret;
    }

    if (len > MAX_SEND_SIZE)
    {
        return ERR_TOOLARGE;
    }

    //����ȡ��,��Ϊ��ȡ�����Լ���sc��ʾ������Ϣʱ������д�뵽�Է�����Ϣ����
    int index = (CLIENT == st_ssk.sc) ?  0: 1;
    ST_EXE_MSG* pst_msg = NULL;
    while(TRUE)
    {
        pst_msg = msg_malloc(); //malloc�ڲ����ܼ��������������Ŷ
        if (NULL == pst_msg)
        { //�ж��Ƿ����뵽�ڴ�
            LOG_ERR("msg_malloc return NULL.");
            return sendlen;
        }
        datalen = Min(len, MAX_MSG_SIZE);
        EnterCSection();
        memcpy(pst_msg->msg, pdata+sendlen, datalen);
        sendlen += datalen;
        pst_msg->size = datalen;
        g_sockArray[st_ssk.index].msgList[index].push_back(pst_msg);   //����Ϣѹ����Ϣ���к���
        len -= MAX_MSG_SIZE;
        if (len <= 0)
        {
            LeaveCSection();
            break;
        }
        pst_msg->isNext = true; //����û�˳�������Ϊ�Ǵ��ļ�����ʾ���滹��
        LeaveCSection();
    }

    return sendlen;
}

EXPORT_C int WINAPI ssk_clear_msg(DWORD ssk)
{
    ST_SSK st_ssk = ssk;

    RETURN_NULLPTR(g_isskInit, ERR_NOINIT);

    int ret = ssk_status(ssk);
    if (COMMON_NORMAL != ret)
    {
        return ret;
    }
    EnterCSection();
    g_sockArray[st_ssk.index].ClearMsg(st_ssk.sc);
    LeaveCSection();
    return COMMON_NORMAL;
}