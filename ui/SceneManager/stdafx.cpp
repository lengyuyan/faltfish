// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// SceneManager.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"



int  g_SceneInitialCount = 0;
long g_SceneDlgHwinId = -1 ;
SceneList  g_SceneList;
SceneList* SceneGetSceneList()
{
	return &g_SceneList;
}

