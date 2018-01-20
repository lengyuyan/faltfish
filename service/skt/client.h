/*********************************************************************
    *�ļ�˵��: �ͻ�����ͷ�ļ���1.�Զ�����,2 �Զ�����
    *����: 
    *����: 2013/11/25
    *�޸���־:  2013/11/25 CW0793 ����
                
***********************************************************************/
#pragma once
#ifndef INCLUDE_SOCKET_CLIENT_H
#define INCLUDE_SOCKET_CLIENT_H
#include "ISocket.h"

class CClient: public ISocket
{
public:
    CClient();
    virtual ~CClient();

    int Init(const char* lpServer, unsigned short sport, const char* local, unsigned short lport);

    int WaitConn();

protected:

    int CreateClientSocket();   //����socket

private:

    char            m_szLocal[MAX_IP_LEN];
    unsigned short  m_lPort;
    int             m_sktTmp;
};

#endif  //��ֹ�ظ�����