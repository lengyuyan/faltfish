#pragma once

#include "common.h"
#include "LightDebugDlg.h"

#include "DataType.h"
#include "afxwin.h"


// CSystemManagerDlg �Ի���
struct SceneName 
{
    CString Oldname;
    CString Updataname;
};

class CSystemManagerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSystemManagerDlg)

public:
	CSystemManagerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSystemManagerDlg();

// �Ի�������
	enum { IDD = IDD_SYSTEMSETTING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL OnInitDialog();
    virtual BOOL DestroyWindow();
    virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickListSceneModify(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonAddscene();    
	afx_msg void OnBnClickedButtonDeletescene();    
	afx_msg void OnBnClickedButtonUpscene();      
    afx_msg void OnBnClickedButtonDownscene();   
    afx_msg void OnBnClickedButtonScenesave();     
	afx_msg void OnClickListCameraCurrent(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAddCamera();      
	afx_msg void OnClickListCameraModify(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonUpcamera();
	afx_msg void OnBnClickedButtonDowncamera();
	afx_msg void OnBnClickedButtonCameralistInstall();
	afx_msg void OnDblclkListCameraCurrent(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDblclkListCameraModify(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedButtonCameraseave();
    afx_msg void OnBnClickedButtonUpdatacamera();
	//afx_msg void OnDblclkListDoport(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnClickListDoport(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnDblclkListSerialport(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnClickListSerialport(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnBnClickedButtonUpsortport();
	//afx_msg void OnBnClickedButtonDownsortport();
 //   afx_msg void OnBnClickedButtonAddport();
    //   afx_msg void OnBnClickedButtonUpdataport();
    //afx_msg void OnBnClickedButtonLightnummodify();
    afx_msg void OnBnClickedButtonResetdisplay();
    afx_msg void OnBnClickedButtonAlterscenename();
    afx_msg void OnBnClickedCheckLandtype();
    afx_msg void OnBnClickedButtonSavepath();
    
    afx_msg void  CSystemManagerDlg::OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnClose();

private:
    void AddScenePross();                    //���ӳ���  
    virtual void SceneSave();				//���������ı���
    void Deletecamera();                      //ɾ��������
    void InitAllListctrl();
    void InitListctrl(CListCtrl *Listctrl);
    void InitPasswordDll();
    void SceneInstall();                  //ϵͳ�������õ��ṹ��
    virtual void SceneModifyInstall();				//�����޸�����
    //virtual void SortPortInstall();				//������������
    bool DeleteDirFile(char* psDirName);				//ɾ���ļ��еò���
    static int WINAPI OnConfigMessage( unsigned int wParam, void * lParam, void* otherparam);

public:
    static CSystemManagerDlg *m_pInstance;
    CButton m_CheckLandType;
    CEdit m_SceneModiEdit;
	CEdit m_AxisNum;

    CListCtrl           m_Scene;				//�����б�
    CListCtrl           m_Camera;				//���������ʼ�б�
    CListCtrl           m_Cameralist;				//���������б�
    int                 SceneChoose;                 //�����޸��б��ѡ����
    int                 CameraChoose;                //���ö���б��ѡ����
    int                 reCameraChoose;              //���������б��ѡ����
    int                 SceneItem;                   //�����޸��б��ѡ����
    int                 SceneSubItem;             //�����޸��б��ѡ����

    CString strFilename;    //���ӵ��ļ����Ĵ���
    
	  //����ͨѶЭ���趨������
	CString m_CalRevHead;
	CString m_WorkRevHead;
	CString m_CalSendHead;
	CString m_WorkSendHead;
	CString m_IPDivid;
	CString m_IPEnd;

	void SetCommunitProctocol(const PROCTOCOLDATA&  SetProData);
	void GetCommunitProctocol(PROCTOCOLDATA&  SetProData);
	void GetCamCalData(int CamIndex,int AxiIndex);

	//��Ʒ���趨���������ڻ���Ʒʱһ���л�
	void SetSysProductMap(void* pMapData);
	void GetSysProductMap(void* pMapData);
	void SetCurProductName(string ProductName);



public:
    int m_LoginLevels;
    afx_msg void OnBnClickedButtonLightnummodify();
    afx_msg void OnBnClickedCheckIsUserComm();
    CButton m_AutoOperLine;
    afx_msg void OnBnClickedButtonPasswordalter();
	afx_msg void OnBnClickedSyssetdlgIpprotocol();
	afx_msg void OnBnClickedSysconfigCalcam();


private:
        HINSTANCE m_hPasswordDLL;
public:

	afx_msg void OnBnClickedSyssettingProduct();
	afx_msg void OnBnClickedSysAxistool();
	afx_msg void OnBnClickedSimulateWork();
	CButton m_WorkSimulate;
};
