/*********************************************************************
    *�ļ�˵��: common��ͷ�ļ������е���������������
    *����: luojing
    *����: 2013/10/20
    *�޸���־:  2013/10/22 CW0793 ����
                
***********************************************************************/
#ifndef DLL_INCLUDE_COMMON
#define DLL_INCLUDE_COMMON
#include "GlobalDefine.h"

#pragma once


#ifndef RETURN_NULLPTR  //�˴��жϿ�ֵ��0ֵ�ú꣬�����пɿ���ʹ�ã��򻯴���
#define RETURN_NULLPTR(p,v) if (NULL == p) \
{	\
    return v; \
} 
#define RETURN_NULL(p) if (NULL == p) \
{	\
    return; \
} 
#endif

////��ӡ����
//#define LEVEL_CRIT  4
//#define LEVEL_ERR   3
//#define LEVEL_WARN  2
//#define LEVEL_INFO  1
//
////�˴������ӡlog�ӿ�
//#define LOG_INFO(msg, ...) trace(LEVEL_INFO, __FUNCTION__, __LINE__, msg, __VA_ARGS__)
//#define LOG_WARN(msg, ...) trace(LEVEL_WARN,__FUNCTION__, __LINE__, msg, __VA_ARGS__)
//#define LOG_ERR(msg, ...)  trace(LEVEL_ERR,__FUNCTION__, __LINE__, msg, __VA_ARGS__)
//#define LOG_CRIT(msg, ...) trace(LEVEL_CRIT,__FUNCTION__, __LINE__, msg, __VA_ARGS__)

enum EMStrType
{//�ַ�������
	EMSTR_STRING = 0,   //�ַ���
	EMSTR_INT,          //����
	EMSTR_DOUBLE,       //������
};

enum EM_LOG_LEVEL
{
	LEVEL_CRIT = -3,
	LEVEL_ERR,
	LEVEL_WARN,
	LEVEL_INFO1,
	LEVEL_INFO2,
	LEVEL_INFO3,
};

//�˴������ӡlog�ӿ�
#define LOG_INFO(msg, ...) trace(LEVEL_INFO1, __FILE__, __LINE__, __FUNCTION__, msg, __VA_ARGS__)
#define LOG_INF1(msg, ...) trace(LEVEL_INFO1, __FILE__, __LINE__, __FUNCTION__, msg, __VA_ARGS__)
#define LOG_INF2(msg, ...) trace(LEVEL_INFO2, __FILE__, __LINE__, __FUNCTION__, msg, __VA_ARGS__)
#define LOG_INF3(msg, ...) trace(LEVEL_INFO3, __FILE__, __LINE__, __FUNCTION__, msg, __VA_ARGS__)
#define LOG_WARN(msg, ...) trace(LEVEL_WARN, __FILE__, __LINE__, __FUNCTION__, msg, __VA_ARGS__)
#define LOG_ERR(msg, ...)  trace(LEVEL_ERR, __FILE__, __LINE__, __FUNCTION__, msg, __VA_ARGS__)
#define LOG_CRIT(msg, ...) trace(LEVEL_CRIT, __FILE__, __LINE__, __FUNCTION__, msg, __VA_ARGS__)


//�̺߳������ͣ�������ʹ���̳߳أ����ʹ���̳߳غ�������
typedef int (WINAPI *THREAD_PROC)(void* param);




//���¶��ǹ��ڷ�����Ϣ������
#define DLL_NAME_LEN 31 //ģ������󳤶�

enum    {DES_ENCRYPT,DES_DECRYPT};

//��Ϣʱ������
enum SYSTEM_MSG_TYPE
{
    EVENT_USUALLY = 0,  //ͨ����Ϣ
    EVENT_SYS_CLOSE,    //�����˳��¼�
    EVENT_OCTOPUS_OP,   //������֮��ĶԻ���Ϣ ר��
    EVENT_LOGIN_MSG,   //��½��Ϣ
    EVENT_FLATFISH_RUN,   //������֮��ĶԻ���Ϣ ר��


    EVENT_FLATFISH_BUF,      //������֮��ĶԻ���Ϣ, �ַ�����Ϣ
    EVENT_FLATFISH_STRUCT,   //������֮��ĶԻ���Ϣ, �ṹ����Ϣ
};
//ui������Ϣ
enum MessageType
{//��Ϣ����
	MT_NORMAL = 0,  //������ʾ��Ϣ
	MT_SELECT,      //ѡ����Ϣ -- ��/��
	MT_RETRY,       //������Ϣ -- ���� / ���� / ȡ��
	MT_WARN,        //������Ϣ
	MT_CRIT,        //�������󣬳������ر�
	MT_RESTART,     //Ҫ��ϵͳ����������Ϣ
};
enum MessageResult
{//������Ϣ���ؽ��
	MR_OK = 0,  //ok
	MR_CANCEL,  //ȡ��
	MR_ABORT,   //��ֹ
	MR_RETRY,   //����
	MR_IGNORE,  //����
	MR_YES,     //ѡ����
	MR_NO,      //ѡ���
	MR_ABNORMAL,  //�쳣
};

typedef MessageResult (WINAPI *CommMessageShow)(const char*, MessageType, void*);
typedef int (WINAPI *PushDeviceInfoCallback)(char *, char *, void*);//����Ҫ

//�ص�����ԭ�ͣ�ע����Ϣ�ӿ�ʱ����ԭ����������
//typedef int (WINAPI *RegistCallBackProc)(unsigned int, void *, SYSTEM_MSG_TYPE msgType);

//�ص�����ԭ�ͣ�ע����Ϣ�ӿ�ʱ����ԭ����������
//typedef int (WINAPI *RegistCallBackProc)(unsigned int, unsigned int, void* lparam);

//�ص�����ԭ�ͣ�ע����Ϣ�ӿ�ʱ����ԭ����������:�Ľ��ĵط�:����Ϊ��Ϣ���ͣ���Ϣ����(����Ϊ��������),����������(c++һ��Ϊthis,c�������Ϊ����һ�����Ӳ���ȫ�ֵĻ��߾�̬��)
//��4��Ϊ��������
typedef int (WINAPI *RegistCallBackProc)(unsigned int, void *lparam, void* otherparam);

//�������
enum EM_ERR_COMMON
{
    ERR_TOOLARGE = -7,  //����̫��
    ERR_TIMEOUT2,        //�ȴ���ʱ
    ERR_NOMSGLIST,      //û�п��õ���Ϣ������
    ERR_NOCONNECT,      //�ͻ���δ����
    ERR_NOREGIST,       //�Է�δע�������Ϣ�ӿ�
    ERR_NOINIT,         //δ��ʼ��
    ERR_PARAM,          //��������
    COMMON_NORMAL,      //����
};

enum USER_DEFINE_MESSAGE
{
    RESET_SYS = 0,
    CLOSE_SYS,

	CALIB_ROTTOOL = 5, //�궨��ת��е��
    CALIB_CAMERA = 10,  //�궨���
    WORKING_CAMERA,
    RECV_REMOTE_MSG,        //�յ�Զ����Ϣ
    RECV_UI_LOGIN_MSG,        //�յ�Զ����Ϣ
    MSG_LOGIN_SYSTEM,
    MSG_SCENE_BUSY,          //��ǰ�������ڹ�����
	MSG_SEND_IMAGE,			//����ͼƬ
	MSG_SEND_IMAGE_PATH,			//����ͼƬ·��

    MSG_SUCCESSFUL               = 20,      //����������ʾ
    MSG_CAMERA_INIT_FAILED,                //�����ʼ��ʧ��
    MSG_LIGHT_INIT_FAILED,                 //��Դ��ʼ��ʧ��
    MSG_HALCON_ERROR,                      //halcon�����쳣
    MSG_IMAGEDEAL_INIT_FAILED,             //��ʼ��ͼ����ʧ��
    MSG_CONNECT_FAILED,              //��ʼ��ͨ������ʧ��
    MSG_CONNECT_SUCCESS,             //ͨ�����ӳɹ�
    MSG_INITCONFIG_INIT_FAILED,           //��ʼ������ʧ��
    MSG_DISP_WORKING_INFO,
    MSG_CONNECT_CAMERA_FAILED,
    MSG_CONNECT_CAMERA_SUCCESS,
	MSG_DISP_CLIB_INFO,            //

    MSG_DISP_IMAGE               = 100,
    MSG_DISP_IMAGE_DUMP,

    MSG_PROCESS_ERROR            = 200,       

    MSG_CLOSE_SYSTEM             = 1000,
    MSG_RESET_SYSTEM,

    MSG_GRABIMAGE_INDEX_ERROR     = 1100,
    MSG_GRABIMAGE_ERROR,
    MSG_GRABIMAGE_LIGHTON_ERROR,
    MSG_GRABIMAGE_LIGHTOFF_ERROR,
    MSG_GRABIMAGE_LIGHTON_SUCCESS,
    MSG_GRABIMAGE_LIGHTOFF_SUCCESS,

    MSG_RESET_PARAMETER           =2000,

    MSG_LANDTYPE_INFORMATION      = 2200,
  

    MSG_GET_MEMORY_INFO      = 3000,


	MSG_COM_COMMUNITE = 4000,

	MSG_DIS_MAINWND_NAME = 5000,

    
};
enum COMMAND_TYPE
{
};

//�˴�������Ϣ���ƣ���ģ���ģ�����ƣ�
//Ҳ�����Լ�ʹ���Լ������ƣ�ֻҪ��ͨ�ŷ�Э�̺ü���
#define MODULE_SOCKETSERVER     "socketserver"  //socket serverģ��
#define MODULE_ROBOT            "robot"         //robot
#define MODULE_OPS              "ops"           //������
#define MODULE_MOTOR            "motor"         //���˶�
#define MODULE_UI               "ui"            //ui���� 
#define MODULE_OP               "op"            //ҵ����ģ��
#define MODULE_CONFIG           "config"        //����ģ��

#define MODULE_SCENE_MAIN_UI     "scene_main_ui"    //����������
#define MODULE_SCENE_DEBUG_UI     "scene_debug_ui"   //��������UI
#define MODULE_SCENE_MAIN_SDK     "scene_main_sdk"   //��������UI

#define MODULE_ALL              "all"           //����ģ��


#define MODULE_SYSTEMCONFIG_UI   "systemconfig"  //�����������ý���

/****************************************************
    *Function: ��ʼ��common��Դ
    *Intput: NULL
    *Output: NULL
    *Return: �ɹ�����true ��ʧ�ܷ���false
******************************************************/
EXPORT_C bool WINAPI init_common();

/****************************************************
    *Function: ����common��Դ
    *Intput: NULL
    *Output: NULL
    *Return: ��
******************************************************/
EXPORT_C void WINAPI uninit_common();

//һЩͨ�ú�����������������������global.cpp


/****************************************************
    *Function: �ж�һ���ļ��Ƿ����
    *Intput: filepath ��Ҫ�����ļ�ȫ·��
    *Output: NULL
    *Return: ���ڷ���true �������ڷ���false
******************************************************/
EXPORT_C bool WINAPI is_file_exist(LPCSTR filepath);

/****************************************************
    *Function: �ж�һ���ļ��Ƿ�ֻ��
    *Intput: filepath ��Ҫ�����ļ�ȫ·��
    *Output: NULL
    *Return: ֻ������true �����򷵻�false
******************************************************/
EXPORT_C bool WINAPI is_file_read(LPCSTR filepath);

/****************************************************
    *Function: ��ȡϵͳʱ��
    *Intput: ��
    *Output: NULL
    *Return: ���ص�ǰʱ���ʽ�ַ��������ص��ַ���ָ��
        ��ȫ��ָ�룬���ԣ�������ȡ���Σ����һ�λ�ȡ��
        ʱ��Ҳ���ɵڶ��ε�ʱ�䣬���������Լ�����
******************************************************/
EXPORT_C const char* WINAPI get_time_str();



EXPORT_C void WINAPI create_image_file();

/****************************************************
*Function: ע����Ϣ���պ���,��ע���޷����յ���Ϣ
*Intput: dllName ����ģ������,��㶨��,ֻҪ����Ϣ��֪��
        fun ������Ϣ������ַ,�����߱��밴RegistCallBackProc
            ԭ�Ͷ���һ����������������Ϣ
*Output: ��
*Return: ע��ɹ�����true�����򷵻�false
******************************************************/
//EXPORT_C bool WINAPI register_notify(LPCSTR dllName, RegistCallBackProc fun);

/****************************************************
*Function: ��ע����Ϣ���պ���,��ע���Է�������Ϣ��ʧ��
*Intput: dllName ����ģ������,�����ע��ʱ��������һ��
*Output: ��
*Return: ע��ɹ�����true�����򷵻�false
******************************************************/
//EXPORT_C void WINAPI unregister_notify(LPCSTR dllName);

/****************************************************
*Function: ������Ϣ�������Է�����ע����ܷ��ͳɹ�
*Intput: wparam ��Ϣ
         lparam ��Ϣ���Ӳ���
         dllName ��Ϣ���շ�ģ�����֣�ע���˲��� Ĭ�Ϲ㲥
         msgType ��Ϣ���� Ĭ��Ϊͨ����Ϣ
*Output: ��
*Return: ���ͳɹ�����0�����򷵻ش�����
******************************************************/
//EXPORT_C int WINAPI msg_send(unsigned int wparam, void *lparam,
//        LPCSTR dllName = 0, SYSTEM_MSG_TYPE msgType = EVENT_USUALLY);

//EXPORT_C int WINAPI msg_send(unsigned int wparam, unsigned int lparam, const char* name);
/****************************************************
    *Function: ��ʼ��post��Ϣ���ƣ��������uninit_msg
                ������Դ���ر��߳�
    *Intput: NULL
    *Output: NULL
    *Return: �ɹ�����true ��ʧ�ܷ���false
******************************************************/
//EXPORT_C bool WINAPI init_msg();

/****************************************************
    *Function: �ر�post�̣߳�������Դ
    *Intput: NULL
    *Output: NULL
    *Return: ��
******************************************************/
//EXPORT_C void WINAPI uninit_msg();

/****************************************************
*Function: ������Ϣ������ֻ���͸�common����common�����͸��Է�
*Intput: wparam ��Ϣ
        lparam ��Ϣ���Ӳ���(ָ������)
        lsize  ָ���ڴ��С��λ�� �ֽ�, ������ַ���������������ĳ���
        dllName ��Ϣ���շ�ģ�����֣�ע���˲��� Ĭ�Ϲ㲥
        msgType ��Ϣ���� Ĭ��Ϊͨ����Ϣ
*Output: ��
*Return: ��������0���������󷵻ش�����
******************************************************/
//EXPORT_C int WINAPI msg_post(unsigned int wparam,void *lparam, unsigned int lsize,
//    LPCSTR dllName = 0, SYSTEM_MSG_TYPE msgType = EVENT_USUALLY);

EXPORT_C int WINAPI msg_post(unsigned int wparam, void* lparam, unsigned int lsize,const char* name);

/****************************************************
*Function: ��ӡlog����,��̬����ʱ����ֱ���ú����
*Intput: LogLevel��ӡ���𣬷�Ϊinfo warn error crit
        msgFormat �ַ�����ʽ����printf�÷���ͬ
        ����ɼ�N�����������֧��1024���ֽ�
*Output: ��
*Return: ��
******************************************************/
EXPORT_C void WINAPI trace(int LogLevel, const char* file, int line, const char* fun, const char *pszFormat, ...);

typedef void (WINAPI *RECALL_LOG)(LPCSTR, LPVOID);

//type  0000 ÿλ��ʾһ��, 1Ϊ�� 0Ϊ��
//     crit/error/warn/info
//��ӡlogʱ,�ж���û��ע����Ľӿ�,�����,����type����
//����type=0b1111,��������־ȫ��������
bool WINAPI regist_recall_status(int type, RECALL_LOG rcLog, LPVOID lparam);




//--------------------------------�˴�������̼��ͨ��--------------------------------------//
typedef int (WINAPI *RegistRecvProc)(char*, UINT);
#define INVALID_SSK     ((DWORD)-1)      //��Ч���

typedef HANDLE SSK_ST;  //ssk���
/****************************************************
*Function: ��ʼ��sskģ��,���Բ����ã�����sskʱ��
            �Զ�����
*Intput: ��
*Output: ��
*Return: �ɹ�����true, ʧ�ܷ���false
******************************************************/
EXPORT_C bool WINAPI init_ssk();

/****************************************************
*Function: ����sskģ����Դ��������ʹ��ssk���񣬱����
*Intput: ��
*Output: ��
*Return: ��
******************************************************/
EXPORT_C void WINAPI uninit_ssk();

/****************************************************
*Function: ����һ��ssk����ˣ�ʹ��ssk���̼�ͨ�ű���
        �ȵ��ô˽ӿڴ������������ϲ���ͨ��
*Intput: sskName ��Ҫ������ssk��������connectʱ�õ�
*Output: ��
*Return: �ɹ�����ssk���, ʧ�ܷ���INVALID_SSK
******************************************************/
EXPORT_C DWORD WINAPI ssk_server(LPCSTR sskName);

/****************************************************
*Function: ����һ��ssk����ͨ�����������ӣ�������
        �����ɹ�����������
*Intput: sskName ��Ҫ���ӵ�ssk������
*Output: ��
*Return: �ɹ�����ssk���, ʧ�ܷ���INVALID_SSK
******************************************************/
EXPORT_C DWORD WINAPI ssk_conn(LPCSTR sskName);

/****************************************************
*Function: ��ѯһ��ssk��������״̬
*Intput: ssk ��Ҫ��ѯ��ssk���
*Output: ��
*Return: ���ص�ǰ״̬
        COMMON_NORMAL   ����
        ERR_PARAM       ssk��Ч
        ERR_NOINIT      δ��ʼ�� ����ssk���񡢷����
        ERR_NOCONNECT   �ͻ���δ����
******************************************************/
EXPORT_C int WINAPI ssk_status(DWORD ssk);

/****************************************************
*Function: ��ѯһ��ssk�����server����client
*Intput: ssk ssk�����conn init���᷵�صľ��
*Output: ��
*Return: ERR_PARAM       ssk��Ч
        ERR_NOINIT      δ��ʼ�� ����ssk���񡢷����
        ERR_NOCONNECT   �ͻ���δ����
        0 ��ʾ��server
        1 ��ʾ��client
******************************************************/
EXPORT_C int WINAPI ssk_is_server(DWORD ssk);

/****************************************************
*Function: �ر�һ��ssk����ʹ��ssk���̼�ͨ����Ϻ����
        ���ô˽ӿڹرշ����ͷ���Դ
*Intput: ssk ssk�����conn init���᷵�صľ��
*Output: ��
*Return: ��
******************************************************/
EXPORT_C void WINAPI ssk_close(DWORD ssk);

/****************************************************
*Function: ע��һ��sskͨ����Ϣ�ص����������յ�����
    �Լ�ssk�������Ϣʱ�����Զ�֪ͨע�᷽
*Intput: ssk ssk�����conn init���᷵�صľ��
         proc �ص�����ָ�룬���밴�ո�ʽд
*Output: ��
*Return: �ɹ�����true, ʧ�ܷ���false
******************************************************/
EXPORT_C bool WINAPI regist_ssk_recv(DWORD ssk, RegistRecvProc proc);

/****************************************************
*Function: ��ssk������Ϣ
*Intput: ssk ssk�����conn init���᷵�صľ��
         pdata ���ݴ�д��Ļ�����
         len ��������С
         timeout �ȴ�ʱ�䵥λms INFINITE ���޵ȴ�
*Output: ��
*Return: �ɹ����ؽ��յ������ݳ���, ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI ssk_recv(DWORD ssk, char* pdata, int len, DWORD timeout = INFINITE);

/****************************************************
*Function: ��ssk������Ϣ
*Intput: ssk ssk�����conn init���᷵�صľ��
        pdata ���ݴ�д��Ļ�����
        len ��������С
*Output: ��
*Return: �ɹ�����0, ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI ssk_send(DWORD ssk, char* pdata, int len);

/****************************************************
*Function: ���ssk������Ϣ����
*Intput: ssk ssk�����conn init���᷵�صľ��
*Output: ��
*Return: �ɹ�����0, ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI ssk_clear_msg(DWORD ssk);

EXPORT_C bool WINAPI Execution_3DES(char *Out, char *In, long datalen,const char *Key, int keylen, bool Type);

/****************************************************
    *Function: ����һ���ļ��У������ڣ��򲻴���
    *Intput: path �ļ���ȫ·��
    *Output: NULL
    *Return: �ɹ�����0, ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI create_path(const char* path);


/****************************************************
    *Function: ���һ��IP��ַ�Ƿ�Ϸ�
    *Intput: lpIP ip��ַ�ַ���
    *Output: NULL
    *Return: ip��ַ�Ϸ�����1 ���Ϸ�����0
******************************************************/
EXPORT_C int WINAPI chk_ipaddress(const char* lpIP);

/****************************************************
    *Function: ʹ��comm�̳߳�ִ��һ������,��ʹ���̳߳�
    comm����ʱ�����Զ��ر������߳�,�������Ҫ�̰߳�ȫ�˳�
    �������comm����ǰ��֤�߳��˳�
    *Intput: fun ������ַ
             lparam ������ָ��
    *Output: NULL
    *Return: �ɹ�����0��ʧ�ܷ��ش�����
    *PS: �̳߳�ֻ���������ظ�Ч�ʸߣ�ʱ��̵Ĺ���
    ���ܵȴ������ܹرգ�������
******************************************************/
EXPORT_C int WINAPI pool_thread(THREAD_PROC fun, void* lparam);


/****************************************************
    *Function: ����Ŀ���ַ�����ֱ�����������ҵ�ָ���ַ�
    *Intput: src Ŀ���ַ���
             dest�������ַ���
             ch  ָ���ַ�
    *Output: NULL
    *Return: ���ؽ�����ָ���ַ(src+strlen(dest))��ָ��0������
******************************************************/
EXPORT_C char* WINAPI comm_strcpy(char* src, const char* dest, char ch = 0);

/****************************************************
    *Function: �ж�һ���ַ����Ƿ�Ϊ����
    *Intput: pstr ������ַ���
    *Output: NULL
    *Return: double����2 ��������1, ��������0 �ο�EMStrType
******************************************************/
EXPORT_C int WINAPI get_str_type(const char* pstr);
/****************************************************
    *Function: ɾ��һ���ַ���ǰ����ͷ�������ַ�(���ַ��Ƚ�)
    *Intput:    pbuff   �������ַ���
                key     �����ַ�����
    *Output: NULL
    *Return: NULL
******************************************************/
EXPORT_C void WINAPI trim_char(char* pbuff, const char* key = " ");

/****************************************************
    *Function: ��ȡ���һ�����õĴ������
    *Intput: NULL
    *Output: NULL
    *Return: ����������Ĵ������
******************************************************/
EXPORT_C int WINAPI get_last_err();


//������Ϣ
//��Ϣ��ӿ�
EXPORT_C int WINAPI register_notify(const char* name, RegistCallBackProc fun, void* lparam);
EXPORT_C int WINAPI unregister_notify(const char* name, RegistCallBackProc fun);

//EXPORT_C int WINAPI msg_recv_str(char* lpstr, int timeout, const char* name);

EXPORT_C int WINAPI msg_send(unsigned int wparam,  void* lparam, unsigned int lsize, const char* name);
EXPORT_C int WINAPI msg_post(unsigned int wparam, void* lparam, unsigned int lsize,const char* name);


#endif  //��ֹ�ظ�����ͷ�ļ�