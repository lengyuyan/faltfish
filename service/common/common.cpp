/*********************************************************************
    *�ļ�˵��: ���ļ���Ҫ��װ��ģ���ͨ�Žӿڣ�common��Դ����ӿ�
    *����: luojing
    *����: 2013/10/20
    *�޸���־:  2013/10/22 CW0793 ����
                
***********************************************************************/
#include "GlobalDefine.h"
#include "GolobalError.h"
#include "common.h"
#include "log.h"
#include "thread_pool.h"
#include "msg.h"

typedef struct HINSTANCE__
{
	int unused;
} *HINSTANCE;
#ifndef DLL_PROCESS_ATTACH
#define DLL_PROCESS_ATTACH   1
#endif

#ifndef DLL_THREAD_ATTACH
#define DLL_THREAD_ATTACH    2
#endif

#ifndef DLL_THREAD_DETACH
#define DLL_THREAD_DETACH    3
#endif

#ifndef DLL_PROCESS_DETACH
#define DLL_PROCESS_DETACH   0
#endif


extern BOOL g_ismsgInit;    //msg�Ƿ��ʼ��
extern BOOL g_isskInit;     //ssk�Ƿ��ʼ��

bool g_bInitComm = 0;
EXPORT_C int APIENTRY
    DllMain(HINSTANCE hInstance, unsigned long dwReason, void* lpReserved)
{
    // ���ʹ�� lpReserved���뽫���Ƴ�
    //UNREFERENCED_PARAMETER(lpReserved);

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        // ��չ DLL һ���Գ�ʼ��
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        //TRACE0("OpPlugin.DLL ������ֹ!\n");
        //printf("detach..");
        // �ڵ�����������֮ǰ��ֹ�ÿ�

    }
    return 1;
}

EXPORT_C bool WINAPI init_common()
{
    //�˴�����ʼ���κι���
    //����Ҫ��ģ�����г�ʼ����msg ssk log�ڵ�����غ���ʱ���Զ���ʼ��
	if (false != g_bInitComm)
	{
		return false;
	}
	g_bInitComm = true;

	MSG_INSTANCE;

	init_log();

    return true;
}

EXPORT_C void WINAPI uninit_common()
{
	if (false == g_bInitComm)
	{
		return ;
	}
	 g_bInitComm = false;

    if (g_ismsgInit)
    {//�ж�msg�Ƿ��������Դ
      //  uninit_msg();
    }
	 MSG_INSTANCE->ReleaseInstance();    //�ͷ���Ϣ��

    if (g_isskInit)
    {//�ж�ssk�Ƿ��Ѿ�������Դ
        uninit_ssk();
    }
	uinit_log();
}
EXPORT_C int WINAPI pool_thread(THREAD_PROC lpStartAddress, void* lpParameter)
{
	RETURN_CHK(g_bInitComm, ERR_NOINIT_COMM);
	return POOL_INSTANCE->Run(lpStartAddress, lpParameter);
}
//����Ϣ���Ƽӽ���



EXPORT_C int WINAPI register_notify(const char* name, RegistCallBackProc fun, void* lparam)
{//ע����Ϣ���պ���
	RETURN_CHK(g_bInitComm, ERR_NOINIT_COMM);
	return MSG_INSTANCE->RegisterNotify(name, fun, lparam);
}
//��Ϣ��ӿ�
EXPORT_C int WINAPI unregister_notify(const char* name, RegistCallBackProc fun)
{//ע����Ϣ���պ���
	RETURN_CHK(g_bInitComm, ERR_NOINIT_COMM);
	return MSG_INSTANCE->UnRegisterNotify(name);
}


//EXPORT_C int WINAPI msg_recv_str(char* lpstr, int timeout, const char* name)
//{
//	RETURN_CHK(g_bInitComm, ERR_NOINIT_COMM);
//	return MSG_INSTANCE->RecvMsgStr(lpstr, timeout, name);
//}

EXPORT_C int WINAPI msg_send(unsigned int wparam,  void* lparam, unsigned int lsize, const char* name)
{
	RETURN_CHK(g_bInitComm, ERR_NOINIT_COMM);
	return MSG_INSTANCE->SendMsg(wparam, lparam, lsize,name);
}

EXPORT_C int WINAPI msg_post(unsigned int wparam,  void* lparam, unsigned int lsize, const char* name)
{
	RETURN_CHK(g_bInitComm, ERR_NOINIT_COMM);
	return MSG_INSTANCE->PostMsg(wparam, lparam, lsize, name);
}