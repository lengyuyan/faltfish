#pragma once
//#pragma comment(lib, "\\FGCamer.lib")
#include "FGCamera.h"
#include <list>
using namespace std;
class AVTCam
{
public:		
	AVTCam(void);
	~AVTCam(void);

	void (*m_fcallback)(void* context); // �ص���������

	//��ʼ�����
	bool InitCam(unsigned long Index);
    bool UnInitCam();

	int GetImageWidth();				// ��ȡͼƬ���
	int GetImageHeight();				// ��ȡͼƬ�߶�
	
    void SetWindowID();
	// ���ûص�����, Index-����ţ�pWnd-Ŀ�괰��ָ��
	void SetCallBack(void(*Callback)(void* context), void* pWnd);	 
	void SetBuffer(BYTE* pBuffer);		// ����ͼ�񻺳���
	
	bool SingleSnap(bool bDisplay = true);	// ��֡�ɼ� // Ҷ

	bool StartSnap();	// �����ɼ�
	bool StopSnap();	// ֹͣ�ɼ�	
	
	bool SetTriggerMode(bool Mode);		//���ô���ģʽ	
    bool SetExposureTime(int iValue);	//�����ع�ʱ��	
	bool SetGain(int iValue);			//��������

	int GetExposureTime();		//��ȡ�ع�ʱ��
	int GetGain();				//��ȡ����\

	int GetParameInfo(FG_PARAMETER speciPara,unsigned long &info);
	int SetParameInfo(FG_PARAMETER speciPara,int val);

	static int GetGuidList(list<string> &guidList);         //��ȡGUID�б�;

public:
	CFGCamera m_Camera;
	BYTE* m_pImageBuffer;	
	UINT32 m_iImageWidth;
	UINT32 m_iImageHeight;
	
	void* m_pParent;
	int m_iCamIdx;
	bool m_bBusy;		// �����ɼ�

private:	
	FGNODEINFO m_NodeInfo[3];
	UINT32 m_iResult;	
	UINT32 m_iNodeCount;
	static bool m_b_FGIintFlag;   //��ʼ����־
	
	HANDLE m_hThread;		// �ɼ��߳�
};
