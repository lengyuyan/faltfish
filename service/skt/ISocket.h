/*********************************************************************
    *�ļ�˵��: socket���࣬�Ӵ���̳�Server/Client��
    *����: logo
    *����: 2016/07/25
    *�޸���־:  2016/07/25 CW34945 ����
                
***********************************************************************/
#pragma once
#ifndef INCLUDE_ISOCKET_H
#define INCLUDE_ISOCKET_H
#include "sysapi.h"
#include <vector>

using std::vector;

class CListen
{//������
public:
    CListen();
    virtual ~CListen(void);

    int Init(const char* lpServer, unsigned short usPort);

    int WaitConnect(ST_SOCKADDR& addr);

    //ip�Ͷ˿��Ƿ���Լ���ͬ?
    bool IsSelf(const char* lpServer, unsigned short usPort) { return (usPort == m_usPort && 0 == strcmp(lpServer, m_szServer)) ? true : false; }

    //����ʹ�ð����ľ��
    void PushHandle(int skt);
    void PopupHandle(int skt);
    int GetHandleCnt() { return (int)m_vecskt.size(); }

protected:

    int CreateSocket();  //����socket
    int ListenSocket();  //�󶨶˿�
    int AcceptClient();  //�������� �̶��ȴ�50ms

    bool ChkAddressValid(const char* ip);   //�ж��Ƿ������Ϊ�����IP

    ST_SOCKADDR* GetClient(const char* lpClient, unsigned short uPort);        //ͨ��ip+port���ҿͻ���

private:

    bool            m_bBind;        //�Ƿ��Ѿ��󶨶˿�
    int             m_sktListen;    //socket�������
    char            m_szServer[MAX_IP_LEN]; //ip
    unsigned short  m_usPort;

    vector<int>             m_vecskt;   //ʹ�ô˷���˵�skt���
    vector<ST_SOCKADDR*>    m_vecPTP;   //������skt�ṹ��
    Handle                  m_hSection; //�ٽ���
};

//socket����
class ISocket
{
public:
    ISocket(void);
    virtual ~ISocket(void);

    void Close();    //�ر�

    CListen* GetListen() { return m_pListen;}
    int GetModel() { return m_emWorkModel;} //��ȡ����ģʽ
    int GetSocket() { return m_socket; }
    int GetStatus();   //��ȡ״̬

    int Send(const char* pData, int len);
    int Recv(char* pData, int len, int timeout);

    int ManualConn();   //�ֶ�����

    int Reconn();

    int SetModel(int nCode);    //���ù���ģʽ EM_SKT_TYPE

    void ClearMsgList();    //��ս�����Ϣ����
    void GetAddr(char* lpRemote, unsigned short& usport);

    virtual int WaitConn() = 0; //�ɹ�����socket�����ʧ�ܷ���INVALID_SKT

protected:

    int ChangeStatus(int status);   //����״̬

    int Connect();  //����

    int SetRemoteIP(const char* szIP, int usPort);

    int RecvEx(char* pData, int len, int timeout);

    void RunThreadConnect();    //�߳�����
    static int WINAPI Run(void* lparam);

    CListen*    m_pListen;  //������

private:

    bool    m_bRun;         //�߳����б�ʶ
    Handle  m_hQuitSign;    //�߳��˳��ź�

    int     m_socket;       //socket���
    char    m_szIP[MAX_IP_LEN];
    int     m_usPort;
    int     m_iStatus;      //״̬
    int     m_emWorkModel;  //����ģʽ
    Handle  m_hSection;     //�ٽ���
};

#endif  //��ֹͷ�ļ��ظ�����