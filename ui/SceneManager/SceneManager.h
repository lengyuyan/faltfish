// SceneManager.h : SceneManager DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "VisionProcBase.h"

// CSceneManagerApp
// �йش���ʵ�ֵ���Ϣ������� SceneManager.cpp
//

class CSceneManagerApp : public CWinApp
{
public:
	CSceneManagerApp();

// ��д
public:
	virtual BOOL InitInstance();

public:
	
	DECLARE_MESSAGE_MAP()
};

void BuildProc(VisionProcBase** ppProcBase,CString configPath,int ProcType,int SceneID,int ProcIdBase);

extern "C"  void PASCAL EXPORT InitGrabWay2Sceneimage(int SceneID,bool sendflag);

extern "C"  BOOL  SceneVisionProcess(int iSceneID,CHECK_RESULT& ProcRes);
//LIBRARY
//extern  CArray<CArray<VisionProcBase*>*> g_SceneList;
extern SceneList g_SceneList;
