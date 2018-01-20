#pragma once
#ifndef DLL_INCLUDE_SYSAPI_SERIAL_H
#define DLL_INCLUDE_SYSAPI_SERIAL_H

#define  MAX_PORT_NAME_LEN 256
#define  INVALID_COM_HANDLE  -1
#define  MAX_DATA_SIZE_B 4096

struct ComParam;
struct _OVERLAPPED;
class CSerial
{//������
public:
    CSerial(void);
    ~CSerial(void);

    //�򿪴��ڣ��ɹ�����0�� ʧ�ܷ��ش�����
    int open(const ComParam* pComData);

    void close();

    int read(char* pBuffer, int nSize, int timeout, int bLock = true);  //��ȡ
    int write(const char* pBuffer, int nWrite, int bLock = true);       //д��
    int wRead(char* lpBuff, int nWrite, int nSize, int timeout);        //д�벢��ȡ,������ִ��

private:
    
    Handle  m_hFile;            //com���
    Handle  m_hOperationEvt;    //������
    _OVERLAPPED*    m_pOL;      //�첽
    int     m_bOverlapped;      //�Ƿ��첽
};
#endif // __CNTLCOM_H__