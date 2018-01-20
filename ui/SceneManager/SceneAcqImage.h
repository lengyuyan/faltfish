#pragma once

#include "VisionProcBase.h"
#include "SceneImageSetting.h"

class SceneAcqImage:public VisionProcBase
{
public:
	SceneAcqImage(void);
	SceneAcqImage(CString ConfigPath,int ProcID);
	~SceneAcqImage(void);

public:
	virtual void BuildInstance(void);  //������ m_SceneAcqImageConfig�趨�Ի���;
	virtual void InitInstance(CWnd* pParentWnd);
	//virtual BOOL IsInitialConfigWnd();
	virtual void DeleteSelf();


	virtual void HideConfigWnd();
	virtual void ShowConfigWnd();

	//void GetInstance(int ProcId);// 

public:
	virtual void SaveProcConfig(int ProcIndex); 
	virtual void ReadProcConfig(int ProcIndex);
	virtual BOOL VisionProcRun(CHECK_RESULT& ProcRes); //ִ���Ӿ��Ĵ���


public:
	bool ProcGrabImage(bool sendflag);

    
public:

	int m_CamIndex;   //�����������ϵͳ�����е����������Ӧ��
	CSceneImageSetting m_SceneAcqImageConfig;


	

};

