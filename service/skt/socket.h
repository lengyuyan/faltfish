/*********************************************************************
    *�ļ�˵��: 
    *����: 
    *����: 2013/11/25
    *�޸���־:  2013/11/25 CW0793 ����
                2016/03/16 EW-0551 �޸�
                1. ��vector����������hash�б���߲�ѯЧ��
                2. ����sktΪ�Զ����ɵı�ʶID��֮ǰ�Ƿ���ָ���ַ
                
***********************************************************************/
#pragma once
#ifndef _INCLUDE_SOCKET_H
#define _INCLUDE_SOCKET_H
#include "sysapi.h"
#include <vector>
#include <hash_map>

using std::vector;
using stdext::hash_map;

#define SKT_INSTANCE CSSocket::GetInstance()

class ISocket;
class CListen;
class CSSocket
{
public:
    CSSocket(void);
    virtual ~CSSocket(void);

    int CreateC(const char* server, unsigned short sport, const char* local = 0, unsigned short lport = 0); //����һ��client

    int CreateS(const char* server, unsigned short sport, const char* local, unsigned short lport);    //����һ��server

    int SetModel(int skt, int nCode);       //����ģʽ
    int GetModel(int skt);                  //��ȡģʽ

    int Close(int skt); //�ر�һ��skt,�ر��������ʱ,������������ͻ��˶���ر�

    int Connect(int skt);   //�ֶ�����

    int Reconn(int skt);

    int Send(int skt, const char* pData, int len);

    int Recv(int skt, char* pData, int len, int timeout = -1);

    int GetStatus(int skt);

    int GetSocket(int skt);

    int GetAddr(int skt, char* lpRemote, unsigned short* pusport);   //��ȡԶ��IP���˿�

    int ClearMsg(int skt);   //������Ϣ������

    static CSSocket* GetInstance();
    static void ReleaseInstance();

protected:

    ISocket* FindSKT(int skt);      //ͨ��һ��skt�������skt�ṹ��

    int GetAutoSktID() { return m_InitSktID++; }   //�Զ�����һ��ID

private:
    static CSSocket*        m_Instance;

    int                     m_InitSktID;        //��ʼ״̬ʱ��ID
    Handle                  m_hSection;         //�ٽ���

    vector<CListen*>        m_vecListen;        //���м�����
    hash_map<int, ISocket*> m_hash_skt;         //����skt��Ա
};

#endif  //��ֹ�ظ�����