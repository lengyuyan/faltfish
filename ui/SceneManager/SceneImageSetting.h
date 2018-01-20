#pragma once
#include "resource.h"
#include "iManagerCtrl.h"
//#include "ProcConfigBase.h"
// CSceneImageSetting �Ի���

class CSceneImageSetting : public CDialogEx 
{
	DECLARE_DYNAMIC(CSceneImageSetting)

public:
	CSceneImageSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSceneImageSetting();

// �Ի�������
	enum { IDD = IDD_DIALOG_SCENE_IMAGE };

public:
    void UpdateUI(int iSceneID, int index);
	void UpdateUI();  //�°汾�ĺ���
	void CloseSettingDlg();
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonSetting();
public:
    void SetDisplayWindowID(int winID);

	 
private:
    int m_iCurSceneID;
    int m_iCurIndex;
    int m_winID;
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnBnClickedBtnLightadd();
    afx_msg void OnBnClickedBtnLightminus();
    afx_msg void OnBnClickedBtnGainAdd();
    afx_msg void OnBnClickedBtnGainMinus();

//�µķ�ʽ ���ӵĲ���
public:
	int m_ShutterTime;
	int m_Gain;
	int m_LightIndex;
	int m_LightValue;
	int m_CameraIndex;
	int m_Interval;  //���ò������ʱ��,ms
	VisionProcBase* m_pContainer;


	bool ConfigGrabImage(bool sendflag);
	void GetContainer(VisionProcBase* pContainer);

	void SaveProcConfig(CString ConfigPath,int ProcIndex);
	void ReadProcConfig(CString ConfigPath,int ProcIndex);
	BOOL VisionProcRun(CHECK_RESULT& ProcRes); 

	int m_ProcProcess;
	CString m_ConfigPath;
	int m_ProcIndex;



};
