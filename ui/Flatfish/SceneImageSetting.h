#pragma once


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
};
