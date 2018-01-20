#pragma once
#include <string>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
using namespace std;

typedef struct serialPortInfo
{	
		string PortName;	//������
		DWORD BaudRate; //�����ʣ�ָ��ͨ���豸�Ĵ������ʡ������Ա������ʵ�ʲ�����ֵ��������ĳ���ֵ֮һ��
		DWORD fParity; // ָ����żУ��ʹ�ܡ����˳�ԱΪ1��������żУ���� 
		BYTE ByteSize; // ͨ���ֽ�λ����4��8
		BYTE Parity; //ָ����żУ�鷽�����˳�Ա����������ֵ��EVENPARITY(2) żУ��     NOPARITY(0) ��У�� MARKPARITY(3) ���У��   ODDPARITY(1) ��У��
		BYTE StopBits; //ָ��ֹͣλ��λ�����˳�Ա����������ֵ��ONESTOPBIT 1λֹͣλ   TWOSTOPBITS 2λֹͣλ	ONE5STOPBITS   1.5λֹͣλ
		DWORD ReadIntervalTimeout; //���ַ�֮��������ʱ
		DWORD ReadTotalTimeoutMultiplier; //��ȡÿ�ַ���ĳ�ʱ
		DWORD ReadTotalTimeoutConstant; //һ�ζ�ȡ�������ݵĹ̶���ʱ
		DWORD WriteTotalTimeoutMultiplier; //д��ÿ�ַ���ĳ�ʱ
		DWORD WriteTotalTimeoutConstant; //һ��д�봮�����ݵĹ̶���ʱ
		serialPortInfo()
		{
			PortName = "COM1";
			BaudRate = 9600;
			fParity = 0;
			ByteSize = 8;
			Parity = 0;
			StopBits = 0;
			ReadIntervalTimeout = MAXDWORD;
			ReadTotalTimeoutMultiplier = 0;
			ReadTotalTimeoutConstant = 0;
			WriteTotalTimeoutMultiplier = 10;
			WriteTotalTimeoutConstant = 1000;

		}
} SerialPortInfo;

enum SERIALPORT_ERRORCODE
{
	SECCESS = 0,
	ERROR_CLOSECOM = 1,//���û򴴽�ʧ�ܣ����ڹر�
	GETPARA_FAIL = 2, //��ȡ���ڲ���ʧ��
	COM_NO_OPEN = 3, //����û�д�
	WRITEDATA_FIAL = 4,//д����ʧ��
	READDATA_FAIL = 5,//������ʧ��
	PARA_ERROR = 6, //��������
};

class SerialPort
{
public:
	SerialPort(void);
	~SerialPort(void);

	//���ڳ�ʼ��
	int Init(SerialPortInfo para);
	//�򿪴���
	int Open();
	//�رմ���
	int Close();
	//��ȡ����
	int ReadData(void *pBuffer);
	//��������
	int WriteData(void *pBuffer,int len = 0);
	//�����Ƿ��
	int IsOpen();
	//��ȡ���ڲ���
	int GetSerialInfo(SerialPortInfo &para);
	//���ô��ڲ���
	int SetSerialInfo(SerialPortInfo para);
private:
	HANDLE m_comer;//���ھ��
	DCB m_dcb;   //���ڲ����ṹ
	COMMTIMEOUTS m_timeout; //��ʱ�ṹ

	SerialPortInfo *m_SerailPortInfo;
	bool m_bIsOpen;
};

