#pragma once
#include "VisionProcBase.h"
#include "ScenePositionSettin.h"

class ScenePositon:public VisionProcBase
{
public:
	ScenePositon(void);
	ScenePositon(CString ConfigPath,int ProcID);
	~ScenePositon(void);
public:
	virtual void BuildInstance(void);
	virtual void InitInstance(CWnd* pParentWnd);
	virtual void HideConfigWnd();
	virtual void ShowConfigWnd();
	virtual void DeleteSelf();


public:
	virtual void SaveProcConfig(int ProcIndex);
	virtual void ReadProcConfig(int ProcIndex);
	virtual BOOL VisionProcRun(CHECK_RESULT& ProcRes); //ִ���Ӿ��Ĵ���

public:
	void ShowAllShape(void);
	void AddCurShapeId();
	void DelCoordinateShapeId();

	bool PosFitCircle(int MatchIndex,double& row,double& col,double& Rii); //ƥ���Ŀ���Ǵֶ�λ
	bool PosFitLine(int MatchIndex,double& row1,double& col1,double& row2,double& col2);

	
	
public:
    

	ScenePositionSettin m_ScenePositionSetting;


};

