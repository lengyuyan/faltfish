#pragma once


// CSceneModelSetting �Ի���

class CSceneModelSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CSceneModelSetting)

public:
	CSceneModelSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSceneModelSetting();

// �Ի�������
	enum { IDD = IDD_DIALOG_SCENE_MODEL };

    void UpdateUI(int iSceneID, int index, CWnd *hROI);
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonSetting();
    afx_msg void OnBnClickedBtnModelRoi();
    afx_msg void OnBnClickedBtnCreatModel();
    afx_msg void OnBnClickedBtnCheckModel();
public:
    void SetDisplayWindowID(int winID);
private:
    int m_iCurSceneID;
    int m_iCurIndex;
    int m_winID;
    CWnd  *m_hROI;
public:
    afx_msg void OnBnClickedButtonModelScoreA();
    afx_msg void OnBnClickedButtonModelScoreS();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
