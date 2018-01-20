#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "FlatfishDlg.h"

// SystemConfig �Ի���
struct SceneName 
{
    CString Oldname;
    CString Updataname;
};

class SystemConfig : public CDialogEx
{
	DECLARE_DYNAMIC(SystemConfig)

public:
	SystemConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SystemConfig();

 
// �Ի�������
	enum { IDD = IDD_DIALOG_GUIDE };
	void InitListctrl(CListCtrl *Listctrl);   //����ListCtrl���
protected:
    static SystemConfig *m_pInstance;
	virtual BOOL OnInitDialog();
	bool DeleteDirFile(char* psDirName);				//ɾ���ļ��еò���
	void AddScenePross();                    //���ӳ���  
	void InitAllListctrl();				//��ʼ���������е�ListCtrl
    static int WINAPI OnConfigMessage( unsigned int wParam, void * lParam, SYSTEM_MSG_TYPE msgType);
	CString GetPortNum();				//��ȡ�������õĴ��ں�
    void SceneInstall();                  //ϵͳ�������õ��ṹ��
	virtual void SceneModifyInstall();				//�����޸�����
	virtual void SortPortInstall();				//������������
    virtual void SceneSave();				//���������ı���
	virtual void SortPortSave();              //��������ı���
    void Deletecamera();                      //ɾ��������
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:

	CListCtrl           m_Scene;				//�����б�
	CListCtrl           m_Camera;				//���������ʼ�б�
	CListCtrl           m_Cameralist;				//���������б�
	int                 SceneChoose;                 //�����޸��б��ѡ����
	int                 CameraChoose;                //���ö���б��ѡ����
	int                 reCameraChoose;              //���������б��ѡ����
	int                 DoPortChoose;                //����ö���б��ѡ����
	int                 SortPortItem;                //����������б��ѡ����  
	int                 SortPortSubItem;             //����������б��ѡ����
    int                 SceneItem;                   //�����޸��б��ѡ����
    int                 SceneSubItem;             //�����޸��б��ѡ����

    CString strFilename;    //���ӵ��ļ����Ĵ���
    //static bool         LandType;
	afx_msg void OnClickListSceneModify(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonAddscene();    
	afx_msg void OnBnClickedButtonDeletescene();    
	afx_msg void OnBnClickedButtonUpscene();      
	afx_msg void OnBnClickedButtonDownscene();        
	afx_msg void OnClickListCameraCurrent(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAddCamera();      
	afx_msg void OnClickListCameraModify(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonUpcamera();
	afx_msg void OnBnClickedButtonDowncamera();
	afx_msg void OnBnClickedButtonCameralistInstall();
	afx_msg void OnDblclkListCameraCurrent(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListCameraModify(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonScenesave();
    afx_msg void OnBnClickedButtonCameraseave();
	CListCtrl m_DoPort;				//�������ô���ö��
	CListCtrl m_SortPort;				//����ѡ������
	afx_msg void OnDblclkListDoport(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickListDoport(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListSerialport(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickListSerialport(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonUpsortport();
	afx_msg void OnBnClickedButtonDownsortport();
	afx_msg void OnClose();
    afx_msg void OnBnClickedButtonResetdisplay();
    afx_msg void OnBnClickedButtonAlterscenename();
    afx_msg void OnBnClickedButtonUpdatacamera();
    afx_msg void OnBnClickedButtonAddport();
    afx_msg void OnBnClickedButtonLightnummodify();
    afx_msg void OnBnClickedButtonUpdataport();
    virtual BOOL DestroyWindow();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnBnClickedCheckLandtype();
    CButton m_CheckLandType;
    CEdit m_SceneModiEdit;
    CEdit m_PortModiEdit;
    afx_msg void OnBnClickedButtonSavepath();
};

