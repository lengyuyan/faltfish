/*********************************************************************
    *�ļ�˵��: socket ��װ��ӿ��ļ�,���ж���ӿڶ������ﶨ��
    *����: 
    *����: 2013/11/25
    *�޸���־:  2013/11/25 CW0793 ����
                
***********************************************************************/
#pragma once
#ifndef DLL_INCLUDE_SOCKET_SKT_H
#define DLL_INCLUDE_SOCKET_SKT_H
#define SKT_VERSION "skt_1.1.1.20170122_beta"

#define WINAPI __stdcall
#define EXPORT_C extern "C"

enum EMSocketStatus
{
    SOCKET_DISCONNECTED = 0,    //δ����
    SOCKET_CONNECTED,           //������
};

//SKTģʽ
enum EMSKT_MODEL
{
    EMSKT_RECONN_CHKIP = 0x01,      //������֤IP ������server
    EMSKT_RECONN_CHKPORT = 0x02,    //������֤�˿� ������server
    EMSKT_RECONN_NOCLOSE = 0x04,    //����ʱ���ر�ԭ�е�socket
    EMSKT_NO_RECONN = 0x08,         //����Ϊ���Զ���������
    EMSKT_NO_AUTOCONN = 0x10,       //����Ϊ���Զ�����
};

/****************************************************
    *Function: ��ʼ��socket
    *Intput: NULL
    *Output: NULL
    *Return: �ɹ�����0��ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI skt_init();

/****************************************************
    *Function: ����ʼ��socket
    *Intput: NULL
    *Output: NULL
    *Return: NULL
******************************************************/
EXPORT_C void WINAPI skt_uninit();

/****************************************************
    *Function: ����һ��tcp �ͻ���,�Զ�����
               �ͷŶ���ʱ,�Զ��ر�socket.
    *Intput: server Ҫ���ӵķ����ip
             sport  Ҫ���ӵķ���˶˿�
             local  Ҫ�󶨵ı���ip,����󶨴�NULL����
             lport  Ҫ�󶨵ı��ض˿�,��ʹ�ô�0
    *Output: NULL
    *Return: ʧ�ܷ���INVALID_SKT, �ɹ�����skt���
    *PS�� �����ɹ������skt_status���Բ�ѯ�Ƿ�����ok
******************************************************/
EXPORT_C int WINAPI create_c(const char* server, unsigned short sport, const char* local = 0, unsigned short lport = 0);

/****************************************************
    *Function: ����һ��tcp �����,���Զ��ȴ�ָ���ͻ�������
               �ͷŶ���ʱ,���Զ��ر�socket.
    *Intput: server Ҫ�󶨵ķ����ip,���Դ�NULL
             sport  Ҫ�󶨵ķ���˶˿�
             local  ָ���ͻ���ip,��ָ����NULL���ɣ������IP
             lport  ָ���ͻ���port,��ָ����NULL���ɣ������port
    *Output: NULL
    *Return: ʧ�ܷ���INVALID_SKT, �ɹ�����skt���
    *PS�� �����ɹ������skt_status���Բ�ѯ�Ƿ�����ok
******************************************************/
EXPORT_C int WINAPI create_s(const char* server, unsigned short sport, const char* local = 0, unsigned short lport = 0);

/****************************************************
    *Function: �ر�һ��skt���,�����ڿͻ���/�����
    *Intput: skt skt���
    *Output: NULL
    *Return: �ɹ�����0,ʧ�ܷ��ش������
******************************************************/
EXPORT_C int WINAPI skt_close(int skt);

/****************************************************
    *Function: ��������һ��skt
    *Intput: skt skt���
    *Output: NULL
    *Return: �ɹ�����0,ʧ�ܷ��ش������
******************************************************/
EXPORT_C int WINAPI skt_conn(int skt);

/****************************************************
    *Function: ����һ��skt�������ӣ���ر�ԭ����
    *Intput: skt  skt���
    *Output: NULL
    *Return: �ɹ�����0,ʧ�ܷ��ش������
******************************************************/
EXPORT_C int WINAPI skt_reconn(int skt);

/****************************************************
    *Function: ����һ��skt���ģʽ������ο� EM_SKT_TYPE
    *Intput: skt    skt���
             nCode  sktģʽ EM_SKT_TYPE
    *Output: NULL
    *Return: �ɹ�����0,ʧ�ܷ��ش������
******************************************************/
EXPORT_C int WINAPI skt_set_model(int skt, int nCode);

/****************************************************
    *Function: ��ȡһ��skt�����ǰ����ģʽ EM_SKT_TYPE
    *Intput: skt    skt���
    *Output: NULL
    *Return: �ɹ����ص�ǰģʽ,ʧ�ܷ���0
******************************************************/
EXPORT_C int WINAPI skt_get_model(int skt);

/****************************************************
    *Function: ������Ϣ����,�����ڿͻ���/�����.
    *Intput: skt skt���
             pData Ҫ���͵�����
             len  Ҫ���͵����ݳ���
    *Output: NULL
    *Return: �ɹ����ط��ͳɹ������ݳ���,ʧ�ܷ��ش������ ���᷵��0
******************************************************/
EXPORT_C int WINAPI skt_send(int skt, const char* pData, int len);

/****************************************************
    *Function: ������Ϣ����,�����ڿͻ���/�����.
    *Intput: skt skt���
             pData �������ݻ�����
             len  �ܽ��յ�������ݳ���
             timeout ��ʱ,Ĭ�����޵ȴ�,ֱ������Ϣ
                        �������ӱ��ر�
    *Output: pData �������ݻ�����
    *Return: �ɹ����ؽ��յ������ݳ���,ʧ�ܷ��ش������,���᷵��0
    ps:����ERR_TIMEOUT��ʾû����Ϣ�ɶ�,������ʱ����
******************************************************/
EXPORT_C int WINAPI skt_recv(int skt, char* pData, int len, int timeout = 0xFFFFFFFF);

/****************************************************
    *Function: ��ѯ��ǰskt״̬,�����ڿͻ���/�����.
    *Intput: skt skt���
    *Output: NULL
    *Return: �ο�SocketNetStatus
******************************************************/
EXPORT_C int WINAPI skt_status(int skt);

/****************************************************
    *Function: ��ȡһ��skt�����Ӧ�ĵײ�socket���
    *Intput: skt skt���
    *Output: NULL
    *Return: ʧ�ܷ���INVALID_SKT, �ɹ�����soket���
    *PS: �˾������ֱ��ʹ��sysapi�ӿڵ���
******************************************************/
EXPORT_C int WINAPI get_socket(int skt);

/****************************************************
    *Function: ��ȡһ��skt��Ӧ�ĶԶ�ip/port
    *Intput: skt skt���
    *Output: lpRemote ip��ַָ�� ���Դ�NULL
             pusport  �˿�ָ�� ���Դ�NULL
    *Return: �ɹ�����0,ʧ�ܷ��ش������
******************************************************/
EXPORT_C int WINAPI get_addr(int skt, char* lpRemote = 0, unsigned short* pusport = 0);

/****************************************************
    *Function: ����skt��Ӧ����Ϣ������.
    *Intput: skt skt���
    *Output: NULL
    *Return: �ɹ�����0,ʧ�ܷ��ش������
******************************************************/
EXPORT_C int WINAPI clear_msg(int skt);

#endif  //��ֹ�ظ�����