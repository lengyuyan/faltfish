/*********************************************************************
    *�ļ�˵��: �������
    *����: 
    *����: 2013/11/25
    *�޸���־:  2013/11/25 CW0793 ����
                
***********************************************************************/
#pragma once
#ifndef INCLUDE_SOCKET_SERVER_H
#define INCLUDE_SOCKET_SERVER_H
#include "ISocket.h"

class CServer: public ISocket
{
public:
    CServer();

    int Init(CListen* pListen, const char* local, unsigned short lport);

    int WaitConn();

private:

    char            m_szClient[MAX_IP_LEN]; //ip
    unsigned short  m_ucPort;
};


#endif  //��ֹ�ظ�����