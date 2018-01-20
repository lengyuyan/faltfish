#pragma once
//


//#include "stdafx.h"
//#include "VisionProcBase.h"


//#include <afxtempl.h>

class VisionProcBase;

typedef class  OpticProcManange  //�������� SceneManager
{
public:
    OpticProcManange();
  	~OpticProcManange();

public:
	void FreeSceneProc(void);
	//void InsertProcItem(int Index,VisionProcBase*);
	void InsertProcItem(int Index,VisionProcBase*);
	void DelProcItem(int ProcId);
	void PushBackProcItem(VisionProcBase*);
	int GetProcCount();
	void RunProc();
	void SaveProcItem();
	void LoadProcItem();
	void SetSceneId(int SceneId);
	int GetSceneId();
	void SetProcNum(int ProcNum);
	int GetProcNum();

	BOOL CmpSceneId(int SceneId);


protected:


private:
	 int m_ProcNum;
     int m_SceneId;
	 CArray<void*> m_ScenceProcArr;
	 //CArray<VisionProcBase*> m_ScenceProcArr;


}OPTICPROCMANANGE;







class AssistantApply
{
public:
	AssistantApply(void);
	~AssistantApply(void);

public:	
	OpticProcManange* GetSceneManaById(int SceneId);
	void AddSceneManager(OpticProcManange* pSceneMana); //��ӳ�������
	void InsertSceneManager(int Index,OpticProcManange* pSceneMana); // ���볡������
	void DelSceneManager(int Index);   //ɾ����������
	void SwapSceneManager(int Index1,int Index2);  //��������λ��

public:
	//CArray<OpticProcManange*> m_SceneManaList; 
	int m_SceneManaList;

};


