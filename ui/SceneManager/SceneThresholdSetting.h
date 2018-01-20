#pragma once


// CSceneThresholdSetting �Ի���

class CSceneThresholdSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CSceneThresholdSetting)

public:
	CSceneThresholdSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSceneThresholdSetting();

// �Ի�������
	enum { IDD = IDD_DIALOG_SCENE_THRESHOLD };

    void UpdateUI(int iSceneID, int index);
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
    afx_msg void OnBnClickedButtonAdd();
    afx_msg void OnBnClickedButtonSub();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
