#pragma once
#include "afxwin.h"
#include "resource.h"

// SceneLogicSetting �Ի���

class SceneLogicSetting : public CDialogEx
{
	DECLARE_DYNAMIC(SceneLogicSetting)

public:
	SceneLogicSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SceneLogicSetting();

// �Ի�������
	enum { IDD = IDD_SCENE_LOGIC };

	enum  LOGIC_TYPE
	{
       Logic_Loop = 0,
	   Logic_Branch = 1
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ѡ���߼�����
	CComboBox m_SelLogicType;
	// ѭ���߼���ѭ������
	CEdit m_ELoopTimes;
	// ѭ���߼��Ŀ�ʼģ�飨�����е�ĳ������
	CEdit m_ELoopBegin;
	// ѭ���߼��Ľ���ģ�� �������е�ĳ������
	CEdit m_ELoopEnd;

public:
	void UpdateUI();
	void GetDlgLoopParam();
	LOGIC_TYPE m_CurLogicType;
	int m_LoopTimes;
	int m_LoopBeginIndex;
	int m_LoopEndIndex;

	 


public:

	afx_msg void OnCbnSelchangeScenelogicLogictype();
	afx_msg void OnBnClickedScenelogicSetting();
};
