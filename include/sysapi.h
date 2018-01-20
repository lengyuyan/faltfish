#pragma once
#ifndef DLL_INCLUDE_COMMON_SYSAPI
#define DLL_INCLUDE_COMMON_SYSAPI

#define SYSAPI_VERSION "sysapi_1.1.1.20170122_beta"

#include "GlobalDefine.h" //ȫ�ֺ궨��



/*-------�궨�壬����ʹ���ĸ�ƽ̨*/
#define __WIN32
//#define __MAC
//#define __LINUX
/*--------�궨�����------------*/

#ifdef __WIN32
#include <direct.h>
#include <iomanip>
#include  <io.h>
#include <stdarg.h>
#else
#include <dir.h>
#endif

//#ifdef _DEBUG
//#define TRACEF printf
//#else
#define TRACEF
//#endif

#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1

/*-----------------------------socket����--------------------------------*/

#ifndef WINAPI
#define WINAPI __stdcall
#endif



typedef unsigned short  ushort;
typedef unsigned int    uint;
typedef unsigned char   uchar;
typedef const char*     lcpstr;

#define UINTMAX     ((uint)~((uint)0))
#define INTMAX      ((int)(UINTMAX >> 1))
#define INTMIN      ((int)~INTMAX)

#define MAX_NAME_LEN        32
#define MAX_IP_LEN          16
#define MAX_BUF_LEN         256
#define MAX_TIMEOUT         0xFFFFFFFF              //���޵ȴ�
#define ANY_IP              "0.0.0.0"               //ipͨ��

#define NULL                0
#define INVALID_SKT         (int)~0                 //��Чsocket
#define INVALID_HANDLE      (Handle)-1              //��Ч���
#define INVALID_HANDLE_INT  -1                      //��Ч���

#define EXPORT_C extern "C"

struct ST_SOCKADDR
{
    char    ip[MAX_IP_LEN];
    uint    port;
    int     s;
};

#define AF_INET         2               /* internetwork: UDP, TCP, etc. */

/*
 * Types
 */
#define SOCK_STREAM     1               /* stream socket */
#define SOCK_DGRAM      2               /* datagram socket */
#define SOCK_RAW        3               /* raw-protocol interface */
#define SOCK_RDM        4               /* reliably-delivered message */
#define SOCK_SEQPACKET  5               /* sequenced packet stream */


/*
 * Protocols
 */
//������ƽ̨��һ��,����Ҫ��ȡ��,�����communicationCtrl�е�ws2def.h�ظ�������
//#define IPPROTO_IP              0               /* dummy for IP */
//#define IPPROTO_ICMP           1               /* control message protocol */
//#define IPPROTO_IGMP            2               /* group management protocol */
//#define IPPROTO_GGP            3               /* gateway^2 (deprecated) */
//#define IPPROTO_TCP             6               /* tcp */
//#define IPPROTO_PUP             12              /* pup */
//#define IPPROTO_UDP             17              /* user datagram protocol */
//#define IPPROTO_IDP            22              /* xns idp */
//#define IPPROTO_ND              77              /* UNOFFICIAL net disk proto */
//#define IPPROTO_RAW2             255             /* raw IP packet */


/****************************************************
    *Function: ��ʼ������Э��ջ
    *Intput: ��
    *Output: ��
    *Return: �ɹ�����0��ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI sys_InitNet();

/****************************************************
    *Function: �ͷ�����Э��ջ
    *Intput: ��
    *Output: ��
    *Return: ��
******************************************************/
EXPORT_C void WINAPI sys_UninitNet();

/****************************************************
    *Function: ���һ���ͻ������������Ƿ�ok
    *Intput: lpClient �ͻ���IP��ַ
    *Output: ��
    *Return: �ɹ�����0��ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI sys_ChkNet(const char* lpClient);

/****************************************************
    *Function: ����һ��socket
    *Intput: af     AF_INET ֧��TCP UDP��ͨ��Э��
             type   һ��ѡ�� SOCK_STREAM ��Э��
             protocol  TCPΪ6 UDPΪ17������ƽ̨���ܻ᲻ͬ
    *Output: ��
    *Return: �ɹ�����socket�����ʧ�ܷ��� INVALID_SKT
    ����get_last_err��ȡ�������
******************************************************/
EXPORT_C int WINAPI sys_Socket(int af, int type, int protocol);

/****************************************************
    *Function: �ر�һ��socket
    *Intput: s socket���
    *Output: ��
    *Return: ��
******************************************************/
EXPORT_C void WINAPI sys_CloseSocket(int s);

/****************************************************
    *Function: ��һ��socket��ĳ���˿�
    *Intput: s socket���
             lpIP ip��ַ
             uPort �˿�
    *Output: ��
    *Return: �ɹ�����0��ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI sys_Bind(int s, const char* lpIP, ushort uPort);

/****************************************************
    *Function: ����һ�������socket
    *Intput: s socket���
             maxCnt ���ȴ���������,ͬʱ����ʱֻ֧��
             ����������ӣ��������Ϊ3�����4���ͻ���
             ͬʱ���ӣ�����һ��ʧ��,������һ������ok��
             �������ܳɹ�
    *Output: ��
    *Return: �ɹ�����0��ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI sys_Listen(int s, int maxCnt);

/****************************************************
    *Function: ����һ���ͻ������ӣ�ֻ֧�ַ����socket
    *Intput: s     socket���
             timeout ��ʱ���ã�Ĭ����Զ�ȴ�
    *Output: pclient ���ܵĿͻ�����Ϣ ����ͨѶ�õ�socket
    *Return: �ɹ�����0��ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI sys_Accept(int s, ST_SOCKADDR* pclient, int timeout = MAX_TIMEOUT);

/****************************************************
    *Function: ʹ��socket���ӷ����
    *Intput: s     socket���
             lpServer  �����IP
             uPort  ����˶˿�
    *Output: ��
    *Return: �ɹ�����0��ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI sys_Connect(int s, const char* lpServer, ushort uPort);

/****************************************************
    *Function: ͬ������һ��socket��Ϣ, ��select�жϣ��ٽ��գ���������
    *Intput: s     socket���
             len   ���ջ�������С �ж�����ʱ len = 0
    *Output: ��
    *Return: �ɹ����ؽ��յ����ַ������ȣ�ʧ�ܷ��ش�����
    *PS: ���� ERR_NO_CONNECT ˵��socket�Ѿ��Ͽ�����
******************************************************/
EXPORT_C int WINAPI sys_Recv(int s, char* buf, int len);

/****************************************************
    *Function: �ж�һ��socket�Ƿ�����Ϣ�ɽ���
    *Intput: s     socket�������
             count socket�����С
             len   ���ջ�������С �ж�����ʱ len = 0
             timeout  ��ʱ���ã�Ĭ����Զ�ȴ�
    *Output: ��
    *Return: �ɹ���������Ϣ������s������ʧ�ܷ��ش���
******************************************************/
EXPORT_C int WINAPI sys_Select(int* s, int count, int timeout = MAX_TIMEOUT);

/****************************************************
    *Function: ����һ��socket��Ϣ
    *Intput: s     socket���
             buf   Ҫ���͵���Ϣ����
             len   Ҫ���͵����ݴ�С
    *Output: ��
    *Return: �ɹ����ط������ݳ��ȣ�ʧ�ܷ��ش�����
    *PS: ���� ERR_NO_CONNECT ˵��socket�Ѿ��Ͽ�����
******************************************************/
EXPORT_C int WINAPI sys_Send(int s, const char* buf, int len);

/****************************************************
    *Function: ��ȡһ��socket�������Ϣ(ip,port)
    *Intput: s     socket���
    *Output: paddr ��ȡ����Ϣ�ṹ�建����
    *Return: �ɹ�����0��ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI sys_GetSockName(int s, ST_SOCKADDR* paddr);

/****************************************************
    *Function: ��ȡ��������IP��ַ�� �������� ����ips���� ��0�ָ�
    *Intput: ��
    *Output: ips ip��ַ�ϼ�����0�ָ� ˫0��β
    *Return: �ɹ����ػ�ȡ����ip������ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI sys_local_ips(char* ips);
/*---------------------------socket�������------------------------------*/


typedef struct SYS_TIME //ʱ��ṹ��
{
    ushort wYear;
    ushort wMonth;
    ushort wDayOfWeek;
    ushort wDay;
    ushort wHour;
    ushort wMinute;
    ushort wSecond;
    ushort wMilliseconds;
} *LPSYS_TIME;

typedef struct SYS_FILETIME {
    uint dwLowDateTime;
    uint dwHighDateTime;
} *LPSYS_FILETIME;

typedef struct SYS_FIND_DATAA
{
    uint dwFileAttributes;
    SYS_FILETIME ftCreationTime;
    SYS_FILETIME ftLastAccessTime;
    SYS_FILETIME ftLastWriteTime;
    uint nFileSizeHigh;
    uint nFileSizeLow;
    uint dwReserved0;
    uint dwReserved1;
    char   cFileName[MAX_BUF_LEN];
    char   cAlternateFileName[14];
#ifdef __MAC
    uint dwFileType;
    uint dwCreatorType;
    uint wFinderFlags;
#endif
} *LPSYS_FIND_DATAA;

//�����ļ��޸Ĳ���
enum EM_FILE_NOTIFY_OPTION
{
    SYS_FILE_NOTIFY_CHANGE_NAME         =   0x001,  //�ļ���
    SYS_FILE_NOTIFY_CHANGE_DIR_NAME     =   0x002,  //·��
    SYS_FILE_NOTIFY_CHANGE_ATTRIBUTES   =   0x004,  //����
    SYS_FILE_NOTIFY_CHANGE_SIZE         =   0x008,  //��С
    SYS_FILE_NOTIFY_CHANGE_LAST_WRITE   =   0x010,  //����޸�ʱ��
    SYS_FILE_NOTIFY_CHANGE_LAST_ACCESS  =   0x020,  //����ȡ���޸�
    SYS_FILE_NOTIFY_CHANGE_CREATION     =   0x040,  //����ʱ��
    SYS_FILE_NOTIFY_CHANGE_SECURITY     =   0x100,  //����Ȩ��
};

typedef void* Handle;
typedef int (WINAPI *PROCEX)(void* param);
typedef int (WINAPI *FUN)();

/****************************************************
    *Function: ����һ����ͨ�߳�
    *Intput: lpStartAddress �߳�ִ�к�����ַ
             lpParameter    ִ�к������Ӳ���
    *Output: ��
    *Return: �����߳̾����ʧ�ܷ��� INVALID_HANDLE
******************************************************/
EXPORT_C Handle WINAPI sys_CreateThread(PROCEX lpStartAddress, void* lpParameter);

/****************************************************
    *Function: ����һ���߳�
    *Intput: hThread �߳̾��
    *Output: ��
    *Return: ���ش������ EM_ERR_CODE
******************************************************/
EXPORT_C int WINAPI sys_SuspendThread(Handle hThread);

/****************************************************
    *Function: �ָ�һ���̣߳�����ʱ����ʹ��
    *Intput: hThread �߳̾��
    *Output: ��
    *Return: ���ش������ EM_ERR_CODE
******************************************************/
EXPORT_C int WINAPI sys_ResumeThread(Handle hThread);

/****************************************************
    *Function: ǿ����ֹһ���߳�
    *Intput: hThread �߳̾��
    *Output: ��
    *Return: ���ش������ EM_ERR_CODE
******************************************************/
EXPORT_C int WINAPI sys_TerminateThread(Handle hThread);

/****************************************************
    *Function: ����һ���¼�
    *Intput: bManualReset �Ƿ�Ϊ�ֶ������źţ��Զ�ģʽ����ȴ����ź��Զ���Ϊ���ź�
             bInitialState ��ʼ�ź�
             lpName �¼����ƣ�����ΪNULL������̵���ʱ��Ӧ������Ϊ�����¼�
    *Output: ��
    *Return: �����¼������ʧ�ܷ��� INVALID_HANDLE
******************************************************/
EXPORT_C Handle WINAPI sys_CreateEvent(int bManualReset, int bInitialState, const char* lpName);

/****************************************************
    *Function: �ȴ�һ���¼����ź�
    *Intput: hHandle �¼����
             dwMilliseconds �ȴ�ʱ�䣬��λms TIME_MAX Ϊ���޵ȴ�
             lpName �¼����ƣ�����ΪNULL������̵���ʱ��Ӧ������Ϊ�����¼�
    *Output: ��
    *Return: ���ش������ EM_ERR_CODE
******************************************************/
EXPORT_C int WINAPI sys_WaitForSingleObject(Handle hHandle, uint iMilliseconds = MAX_TIMEOUT);

/****************************************************
    *Function: �ȴ�����¼����ź�, ���ź�ʱ��������С
    ����ֵ��event�������صĿ��Լ�������sys_WaitForMultipleObjects
    �ȴ���ֱ��û���ź�Ϊֹ
    *Intput: nCount �¼������С
             lpHandle �¼������׵�ַ
             bWaitAll �Ƿ�ȴ������¼����ź� 0 ��ʾ����
             dwMilliseconds �ȴ�ʱ�䣬��λms TIME_MAX Ϊ���޵ȴ�
    *Output: ��
    *Return: �ɹ��������������źŵ���С����ֵ��ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI sys_WaitForMultipleObjects(uint nCount, const Handle* lpHandle, int bWaitAll, uint dwMilliseconds = MAX_TIMEOUT);

/****************************************************
    *Function: ����һ���¼�Ϊ���ź�
    *Intput: hEvent �¼����
    *Output: ��
    *Return: ���ش������ EM_ERR_CODE
******************************************************/
EXPORT_C int WINAPI sys_SetEvent(Handle hEvent);

/****************************************************
    *Function: ����һ���¼�Ϊ���ź�
    *Intput: hEvent �¼����
    *Output: ��
    *Return: ���ش������ EM_ERR_CODE
******************************************************/
EXPORT_C int WINAPI sys_ResetEvent(Handle hEvent);

/****************************************************
    *Function: ����һ���¼�Ϊ���ź�
    *Intput: hEvent �¼����
    *Output: ��
    *Return: ��
******************************************************/
EXPORT_C void WINAPI sys_CloseHandle(Handle hObject);

/****************************************************
    *Function: ����һ�������������ڽ��̼��
    *Intput: bInitialOwner �Ƿ��ʶ�Լ�Ϊ������ 0Ϊ��
             lpName  ����������
    *Output: ��
    *Return: ���ػ������¼����
******************************************************/
EXPORT_C Handle WINAPI sys_CreateMutex(int bInitialOwner, const char* lpName);

/****************************************************
    *Function: ��һ�������������ڽ��̼��
    *Intput: bInitialOwner �Ƿ��ʶ�Լ�Ϊ������ 0Ϊ��
             lpName  ����������
    *Output: ��
    *Return: ���ػ������¼����
******************************************************/
EXPORT_C Handle WINAPI sys_OpenMutex(int bInitialOwner, const char* lpName);

/****************************************************
    *Function: �ͷ�һ��������
    *Intput: bInitialOwner �Ƿ��ʶ�Լ�Ϊ������ 0Ϊ��
             lpName  ����������
    *Output: ��
    *Return: ���ش������ EM_ERR_CODE
******************************************************/
EXPORT_C int WINAPI sys_ReleaseMutex(Handle hMutex);

/****************************************************
    *Function: ��ʼ��һ���ٽ��������߳�ͬ����
    *Intput: ��
    *Output: ��
    *Return: ����һ�����
******************************************************/
EXPORT_C Handle WINAPI sys_InitializeCriticalSection();

/****************************************************
    *Function: ɾ���ٽ������
    *Intput: hSection ���������
    *Output: ��
    *Return: ���ش������ EM_ERR_CODE
******************************************************/
EXPORT_C void WINAPI sys_DeleteCriticalSection(Handle hSection);

/****************************************************
    *Function: �����ٽ���
    *Intput: hSection ���������
    *Output: ��
    *Return: ���ش������ EM_ERR_CODE
******************************************************/
EXPORT_C void WINAPI sys_EnterCriticalSection(Handle hSection);

/****************************************************
    *Function: �뿪�ٽ���
    *Intput: hSection ���������
    *Output: ��
    *Return: ���ش������ EM_ERR_CODE
******************************************************/
EXPORT_C void WINAPI sys_LeaveCriticalSection(Handle hSection);

/****************************************************
    *Function: ��̬����һ��dll
    *Intput: lpLibFileName ģ��������.dll
    *Output: ��
    *Return: ����һ��ģ����, ʧ�ܷ��� INVALID_HANDLE
******************************************************/
EXPORT_C Handle WINAPI sys_LoadLibrary(const char* lpLibFileName);

/****************************************************
    *Function: �ͷ�һ��ģ��
    *Intput: hLibModule ģ����
    *Output: ��
    *Return: ���ش������ EM_ERR_CODE
******************************************************/
EXPORT_C int WINAPI sys_FreeLibrary(Handle hLibModule);

/****************************************************
    *Function: ��ȡһ��ģ���еĺ����ӿڵ�ַ
    *Intput: hModule     ģ����
             lpProcName  ������
    *Output: ��
    *Return: �ɹ����غ�����ַ��ʧ�ܷ��ؿ�
******************************************************/
EXPORT_C FUN WINAPI sys_GetProcAddress(Handle hModule, const char* lpProcName);

/****************************************************
    *Function: ʹ��ǰ�߳�����һ��ʱ��
    *Intput: dwMilliseconds ����ʱ�� ��λms
    *Output: ��
    *Return: ��
******************************************************/
EXPORT_C void WINAPI sys_Sleep(uint dwMilliseconds);

/****************************************************
    *Function: ��ȡ����ʱ��
    *Intput: lpSysTime   ʱ��ṹ��
    *Output: ��
    *Return: ��
******************************************************/
EXPORT_C void WINAPI sys_GetLocalTime(SYS_TIME* lpSysTime);

//---�ļ�����---
//�����ļ����ʷ�ʽ
enum SYS_FILE_ACESS
{
    FGENERIC_READ    = 0x01,   //��ȡ ���� SYS_GENERIC_WRITE �����
    FGENERIC_WRITE   = 0x02,   //д�� ���� SYS_GENERIC_READ  �����
};

//�ļ��򿪷�ʽ
enum SYS_FILE_OPEN
{
    SYS_CREATE_NEW          =   1,  //����һ���µ��ļ�����������ʧ��
    SYS_CREATE_ALWAYS       =   2,  //����һ���µ��ļ����������򸲸�
    SYS_OPEN_EXISTING       =   3,  //��һ���Ѿ����ڵ��ļ����������ڣ���ʧ��
    SYS_OPEN_ALWAYS         =   4,  //��һ���Ѿ����ڵ��ļ����������ڣ��򴴽�
    SYS_TRUNCATE_EXISTING   =   5,  //��һ���Ѿ����ڵ��ļ����ÿգ��������ڣ���ʧ��
};

//�ļ�����
enum SYS_FILE_ATTRIBUTE
{
    FATTRIBUTE_NORMAL = 0x01,       //��ͨ�ļ�
    FATTRIBUTE_COMPRESSED = 0x02,   //ѹ���ļ�
    FATTRIBUTE_HIDDEN = 0x04,       //�����ļ���Ŀ¼
    FATTRIBUTE_READONLY = 0x08,     //ֻ���ļ���Ŀ¼
    FATTRIBUTE_DIRECTORY = 0x10,    //Ŀ¼���ļ���
};

/****************************************************
    *Function: ��ȡ��ǰexe/dll���ھ���·��
    *Intput:  NULL
    dwAccess  lpPath ·��������
    *Output: ��
    *Return: �ɹ�����0��ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI sys_GetCurrentPath(char* lpPath);

/****************************************************
    *Function: ��ȡ��ǰexe/dll���ھ���·��
    *Intput:  NULL
    dwAccess  lpPath ·��������
    *Output: ��
    *Return: �ɹ�����0��ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI get_module_path(char* lpPath);

/****************************************************
    *Function: ����һ���ļ��������ڴ��ڣ����ļ�����
    *Intput: lpFileName         �ļ���
             dwAccess    ���ʷ�ʽ �ο� SYS_FILE_ACESS
             dwCreationOption  �򿪷�ʽ���ο� SYS_FILE_OPEN
    *Output: ��
    *Return: ��
******************************************************/
EXPORT_C Handle WINAPI sys_CreateFile(const char* lpFileName, uint dwAccess, uint dwCreationOption);

/****************************************************
    *Function: ��һ���Ѿ��򿪵��ļ�д����
    *Intput: hFile         �ļ����
             lpBuffer      ��д������
             nBufLen       ��д�����ݴ�С
    *Output: NULL
    *Return: �ɹ�����д��ĳ��ȣ�ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI sys_WriteFile(Handle hFile, const void* lpBuffer, uint nBufLen);

/****************************************************
    *Function: ��һ���Ѿ��򿪵��ļ�������
    *Intput: hFile          �ļ����
             lpBuffer       ���ݻ�����
             nBufSize       ��������С
    *Output: NULL
    *Return: �ɹ����ض�ȡ�ĳ��ȣ�ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI sys_ReadFile(Handle hFile, void* lpBuffer, uint nBufSize);

/****************************************************
    *Function: ɾ��һ���ļ�
    *Intput: lpFileName �ļ���ϸ·��
    *Output: NULL
    *Return: �ɹ�����0��ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI sys_DeleteFile(const char* lpFileName);

/****************************************************
    *Function: ��ȡһ���ļ�������
    *Intput: lpFileName �ļ���ϸ·��
    *Output: NULL
    *Return: �ɹ�����һ����������ֵ,ʧ�ܷ��ش�����
    *PS ����ֵ������ο� SYS_FILE_ATTRIBUTE ������һ��
    �����ĺ�ֵ
******************************************************/
EXPORT_C int WINAPI sys_GetFileAttribute(const char* lpFileName);

/****************************************************
    *Function: ���ҵ�ǰĿ¼�µ�һ���ļ�
    *Intput: lpFileName �������ļ�Ҳ������Ŀ¼,Ҳ������ͨ�������*.ini
    *Output: pFindData  ���ز��ҵ����ļ��ṹ��
    *Return: �ɹ�����һ���ļ������ʧ�ܷ���INVALID_HANDLE
    *PS: ʹ����þ������� sys_FindClose �رվ��
******************************************************/
EXPORT_C Handle WINAPI sys_FindFirstFile(const char* lpFileName, LPSYS_FIND_DATAA pFindData);

/****************************************************
    *Function: ���ҵ�ǰĿ¼��һ���ļ�
    *Intput: hFindFile �ļ������ͨ�� sys_FindFirstFile ��������
    *Output: pFindData  ���ز��ҵ����ļ��ṹ��
    *Return: �ɹ�����TRUE(1) ʧ�ܷ���FALSE(0)
******************************************************/
EXPORT_C int WINAPI sys_FindNextFile(Handle hFindFile, LPSYS_FIND_DATAA pFindData);

/****************************************************
    *Function: �ر��ļ����Ҿ��
    *Intput: hFindFile  ͨ��sys_FindFirstFile ���ص���Ч���
    *Output: NULL
    *Return: �ɹ�����TRUE(1) ʧ�ܷ���FALSE(0)
******************************************************/
EXPORT_C int WINAPI sys_FindClose(Handle hFindFile);

/****************************************************
    *Function: ����һ�� ���Ŀ¼�������ļ��仯 ���
    *Intput: lpPathName �ļ���Ŀ¼
             bWatchSubtree  �Ƿ��������Ŀ¼ TRUE(1)-����  FALSE(0)-������
             dwNotifyOption ���Ӹ��Ӳ��� �ο� EM_FILE_NOTIFY_OPTION
    *Output: NULL
    *Return: �ɹ�����һ���ļ���ؾ����ʧ�ܷ���INVALID_HANDLE
    *PS: ���صľ��������sys_WaitForSingleObject/sys_WaitForMultipleObjects�ȴ��䴥��
******************************************************/
EXPORT_C Handle WINAPI sys_CreateFileChangeNotify(const char* lpPathName, int bWatchSubtree, uint dwNotifyOption);

/****************************************************
    *Function: �ر� �ļ���� ���
    *Intput: hChangeHandle  ��ؾ����ͨ������sys_CreateFileChangeNotification ���
    *Output: NULL
    *Return: �ɹ�����TRUE(1) ʧ�ܷ���FALSE(0)
******************************************************/
EXPORT_C int WINAPI sys_CloseFileChangeNotify(Handle hChangeHandle);

enum EMParity
{//������
    EMP_NO = 0,     //��У��λ
    EMP_ODD,        //��У��
    EMP_EVEN,       //żУ��
    EMP_MARK,       //mark
    EMP_SPACE,      //space
};

enum EMByteSize
{//����λ
    EMD_BITS5 = 5,  //5λ
    EMD_BITS6,      //6λ
    EMD_BITS7,      //7λ
    EMD_BITS8,      //8λ
};

enum EMStopBits
{//ֹͣλ
    EMS_ONE = 0,    //1λ
    EMS_ONE5,       //1.5λ
    EMS_TWO,        //2λ
};

struct ComParam
{//���ڲ���
    uint        comPort;    //����index 1��2��3...
    uint        baudRate;   //������600��1200��...9600...115200
    EMParity    parity;     //У��λ
    EMByteSize  btSize;     //����λ
    EMStopBits  stopBit;    //ֹͣλ
    uint        readSize;   //��������
    uint        writeSize;  //д������
    uint        bOverlapped;//�Ƿ��첽 1 �� 0����
};

/****************************************************
    *Function: ��һ������
    *Intput: pComData ���ڲ���
    *Output: ��
    *Return: �ɹ����ش��ھ����ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI sys_OpenCom(const ComParam* pComData);

/****************************************************
    *Function: �ر�һ������
    *Intput: hCom      ���ھ��
    *Output: ��
    *Return: �ɹ�����0, ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI sys_CloseCom(int hCom);

/****************************************************
    *Function: ��ȡ��������
    *Intput: hCom      ���ھ��
             pBuffer   ��������
             nBufSize  ��������С
             timeout  ��ʱ
    *Output: NULL
    *Return: �ɹ����ض�ȡ�ֽ�����ʧ�ܷ��ش������
******************************************************/
EXPORT_C int WINAPI sys_ReadCom(int hCom, char* pBuffer, int nBufSize, int timeout = 2000);

/****************************************************
    *Function: ������д������
    *Intput: hCom       ���ھ��
             pBuffer    ��д������
             nSize      ���ݴ�С
    *Output: NULL
    *Return: �ɹ�����д���ֽ�����ʧ�ܷ��ش������
******************************************************/
EXPORT_C int WINAPI sys_WriteCom(int hCom, const char* pBuffer, int nBufLen);

/****************************************************
    *Function: ������д�����ݣ�����ȡ����ֵ
    *Intput: hCom   ���
             lpBuff Ҫ�������ݻ�����
             nWrite Ҫд������ݴ�С
             nSize  ��������С
             timeout  ��ʱ
    *Output: NULL
    *Return: �ɹ����ض�ȡ�ֽ�����ʧ�ܷ��ش������
******************************************************/
EXPORT_C int WINAPI sys_wReadCom(int hCom, char* pBuffer, int nBufLen, int nBufSize, int timeout = 2000);

//----------------------------------ϵͳapi�������----------------------------------//

/****************************************************
    *Function: ��һ��utf8�����ַ���ת��Ϊgbk����
    *Intput: utf8 ��Ҫת����utf8�ַ���
    *Output: gbk ת�����gbk�ַ���
    datalen gbk�ַ�������
    *Return: NULL
******************************************************/
EXPORT_C void WINAPI utf8_to_gbk(const char* utf8, char* gbk, int datalen);

/****************************************************
    *Function: ��һ��gbk�����ַ���ת��Ϊutf8����
    *Intput: gbk ��Ҫת����gbk�ַ���
    *Output: utf8 ת�����utf8�ַ���
    datalen utf8�ַ�������
    *Return: NULL
******************************************************/
EXPORT_C void WINAPI gbk_to_utf8(const char* gbk, char* utf8, int datalen);

/****************************************************
	*Function: ��һ��gbk�����ַ���ת��Ϊascii����
	*Intput: gbk ��Ҫת����gbk�ַ���
	*Output: ascii ת�����ascii�ַ���
	datalen ascii �ַ�������
	*Return: NULL
******************************************************/
EXPORT_C void WINAPI gbk_to_ascii(const wchar_t* gbk, char* ascii, int datalen);

/****************************************************
	*Function: ��һ��ascii�����ַ���ת��Ϊgbk����
	*Intput: ascii ��Ҫת����ascii�ַ���
	*Output: gbk ת�����gbk�ַ���
	datalen gbk�ַ�������
	*Return: NULL
******************************************************/
EXPORT_C void WINAPI ascii_to_gbk(const char* ascii, wchar_t* gbk, int datalen);

/****************************************************
    *Function: ����ĳusb�豸�Ƿ����
    *Intput: keywork ���ҹؼ���(Ӳ��ID)
    *Output: NULL
    *Return: �ɹ�����1 ʧ�ܷ���0
******************************************************/
EXPORT_C int WINAPI search_usb_dev(const char* keywork);

/****************************************************
    *Function: ����һ���ַ�����������
    *Intput: pbuff �ַ���
    *Output: NULL
    *Return: �ɹ�����0 ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI copy_to_board(const char* pbuff);


/****************************************************
    *Function: ӳ���ַ�ռ�,ssk�õ�
    *Intput: lpBaseAddress
    *Output: NULL
    *Return: �ɹ�����0 ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI sys_UnmapViewOfFile(const void* lpBaseAddress);

/****************************************************
    *Function: ��ӳ��,ssk�õ�
    *Intput: lpBaseAddress
    *Output: NULL
    *Return: �ɹ�����0 ʧ�ܷ��ش�����
******************************************************/
HANDLE WINAPI sys_OpenFileMapping(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCSTR lpName);
/****************************************************
    *Function: �����ļ�ӳ��,ssk�õ�
    *Intput: lpBaseAddress
    *Output: NULL
    *Return: �ɹ�����0 ʧ�ܷ��ش�����
******************************************************/
HANDLE WINAPI sys_CreateFileMapping(HANDLE hFile, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCSTR lpName);
/****************************************************
    *Function: ӳ���ļ���ͼ,ssk�õ�
    *Intput: lpBaseAddress
    *Output: NULL
    *Return: �ɹ�����0 ʧ�ܷ��ش�����
******************************************************/
void* WINAPI sys_MapViewOfFile(HANDLE hFileMappingObject, DWORD dwDesiredAccess);


/****************************************************
    *Function: ����,debugʱ�������ͨѶ���� log�õ�
    *Intput: lpOutputString
    *Output: NULL
    *Return: �ɹ�����0 ʧ�ܷ��ش�����
******************************************************/
void WINAPI sys_OutputDebugString(LPCSTR lpOutputString);

#endif
