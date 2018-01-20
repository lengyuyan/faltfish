#pragma once
#include "visionprocbase.h"
#include "HalProcProcessSetting.h"

class HalProcProcess :
	public VisionProcBase
{
public:
	HalProcProcess(void);
    HalProcProcess(CString ConfigPath,int ProcID);
	~HalProcProcess(void);

public:
	virtual void BuildInstance(void);
	virtual void InitInstance(CWnd* pParentWnd);
	//virtual BOOL IsInitialConfigWnd();
	virtual  void DeleteSelf();

	virtual void HideConfigWnd();
	virtual void ShowConfigWnd();

public:
	virtual void SaveProcConfig(int ProcIndex);
	virtual void ReadProcConfig(int ProcIndex);
	virtual BOOL VisionProcRun(CHECK_RESULT& ProcRes); //ִ���Ӿ��Ĵ���
public:
	bool CommonProcess(string pathName,vector<double>&inparam,vector<double>&outparam,int display,int &error,string &hmessage);//����


public:

	HalProcProcessSetting m_HalCommomPosSet;




};

