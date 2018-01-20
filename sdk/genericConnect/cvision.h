#pragma once
#ifndef DLL_INCLUDE_GENERIC_VISIONPROCESS
#define DLL_INCLUDE_GENERIC_VISIONPROCESS
#include "sysapi.h"
#include <hash_map>
#include "skt.h"

using std::hash_map;

#define INVALID_VALUE -1
#define DATA_SEG_FLAG   ","            //��������
#define SPLIT_FLAG      "|"            //�ָ����
#define END_CHAR        '#'
#define REPLY_TIMEOUT   5000

struct TVisionMsg
{//��Ϣ�ṹ��
    int     ccdok;  //���ս��
    int     res;    //�Ƿ��д�����   0 ��ʶ����Ϣ ����Ϊ�������
    int     cures;  //�Ƿ����Զ�����Ϣ 0 ��ʶ����Ϣ ����Ϊ�������
    char    msg[MAX_BUF_LEN];   //������
    char    cus[MAX_BUF_LEN];   //�Զ�����Ϣ
};

struct TVisionResult;
class CVision
{
public:
	CVision(void);
	virtual ~CVision(void);

    int GetStatus() { return SOCKET_CONNECTED == skt_status(m_socket) ? EMVISION_NORMAL : EMVISION_ERROR; }

	int Init(const char* lpSrv, unsigned short sport, int bClient = 0);

	int VisionProcess(int secne); //new add 
	int VisionProcessResult(int secne, TVisionResult* pres, int timeout);//new add 
	int GetVisionImage(int secne, char* pdata);//new add 
	int GetVisionImagePath(int secne, char* path);//new add 
	int SendVisionCalibData(int secne, double x, double y);//new add 
	int RecvVisionCalibData(int secne, double x, double y);//new add 
	int ConvertCalibData(unsigned short visionID, int secne, TVisionResult* pres);//new add 

	int SendCustomMsg(const char* cmd, const char* msg);      //�����Զ�����Ϣ
	int RecvCustomMsg(const char* cmd, char* recvMsg, int timeout);   //�����Զ�����Ϣ

protected:
	// TODO: �ڴ˶����ڲ����ܽӿ�
    void ClearMsg(int secne);
    void ClearCusMsg(int secne);
    int ParseResult(char *psrc, TVisionResult* pres);

    int ReadOkMsg(int secne);
    int ReadMsg(int secne, char* recvMsg);    //��ȡ��������Ϣ������
    int ReadCusMsg(int secne, char* recvMsg);
    int RecvCCDOK(int secne, int timeout);
	int RecvResult(int secne, char* recvMsg, int timeout = 0);   //������Ϣ
    int RecvMsg();
    int SendMsg(const char* pdata);

    //�����Ѿ��յ��ĵ�����Ϣ�������Ƿ����ҵ�Ŀ����Ϣ
    bool ParseMessage();

private:

	bool    m_bInit;        //�Ƿ��ʼ��
	bool    m_bClient;      //��ʶ�Ƿ�Ϊ�ͻ���
    bool    m_bRecv;        //�Ƿ��ڽ���
	int     m_socket;       //��visionͨѶ���
	char    m_szSrv[MAX_IP_LEN];    //srver  ip
	char    m_szCli[MAX_IP_LEN];    //client ip
	unsigned short m_usPort;        //srv port
	unsigned short m_ucPort;        //client port

    Handle  m_hSection;     //�ٽ���

    char    m_szRecvMsg[MAX_BUF_LEN];   //������һ����Ϣ��ȡ��Ĳ�������Ϣ
    hash_map<int, TVisionMsg*>  m_hash_msg;  //��Ϣ����map key = scene
};

#endif