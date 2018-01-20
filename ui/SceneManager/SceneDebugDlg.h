#pragma once
#include "afxcmn.h"
#include "iManagerCtrl.h"
#include "DataType.h"
#include "SetSceneDlg.h"
//#include "SceneImageSetting.h"
#include "SceneAcqImage.h"
#include "SceneModelSetting.h"
#include "ScenePositon.h"
#include "SceneThresholdSetting.h"
#include "SceneFindLine.h"
#include "SceneFindCircle.h"
#include "SelectFunction.h"
#include "InterfaceSetDlg.h"
#include "SEBtn.h"
#include "SRBtn.h"
#include "common.h"
#include <vector>
#include <map>

#include "VisionProcBase.h"
#include "SceneMatchModel.h"
#include "afxwin.h"

using namespace std;

// CSceneDebugDlg �Ի���
#define FILE_LIST_CTRL_COUNT   4   //��ʾͼ���б����


class CSceneDebugDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSceneDebugDlg)

public:
	CSceneDebugDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSceneDebugDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SCENE_DEBUG };
	//CVisionServer m_visions;

protected:
    // ���ɵ���Ϣӳ�亯��
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    afx_msg LRESULT OnManualMessage( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnSetScene( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT OnAddFunction( WPARAM wParam, LPARAM lParam );
    afx_msg void OnBnClickedBtnCrabpic();
    afx_msg void OnBnClickedBtnOpenFolder();
    afx_msg void OnBnClickedBtnOpenShow();
    //afx_msg void OnBnClickedBtnCloseShow();
    afx_msg void OnBnClickedBtnSaveoripic();
    afx_msg void OnBnClickedBtnReultpic();
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton12();
    afx_msg void OnBnClickedBtnOpenPicfloder();
    afx_msg void OnBnClickedBtnClear();
    afx_msg void OnBnClickedBtnDelete();
    afx_msg void OnBnClickedBtnAllSelect();
    afx_msg void OnBnClickedCheckPic();
    afx_msg void OnBnClickedBtnStart();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedBtnEdgeadd2();
    afx_msg void OnBnClickedBtnEdgeminus2();
    afx_msg void OnBnClickedCheckIsShowTmp();
    afx_msg void OnClickListFunction(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDblclkListFunction(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDblclkListPicture(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedBtnSaveParam();
    afx_msg void OnBnClickedButtonAddFunction();
    afx_msg void OnBnClickedButtonSubFunction();
    afx_msg void OnDblclkStaticBind();
    afx_msg void OnBnClickedButtonRestore();
    afx_msg void OnBnClickedCheckIsAffine();
    virtual void OnCancel();
    virtual BOOL DestroyWindow();
    afx_msg void OnBnClickedCheckIsTop();
    afx_msg void OnBnClickedRadioDisAll();
    afx_msg void OnBnClickedRadioDis1();
    afx_msg void OnBnClickedRadioDis2();
    afx_msg void OnBnClickedRadioDis3();
    afx_msg void OnBnClickedButtonI1();
    afx_msg void OnBnClickedButtonI2();
    afx_msg void OnBnClickedButtonI3();
    afx_msg void OnBnClickedButtonI4();
	afx_msg void OnBnClickedButtonMoreConttrol();
	afx_msg void OnBnClickedButtonDeleteConttrol();
    afx_msg void OnBnClickedCheckIsSimulate();

	DECLARE_MESSAGE_MAP()
private:
public:
    void init_scene_info();
	void SetShowScene(int SceneID);
    void init_list();
    void updata_ui_data(int iType = 0, int index = 0);
	void SceneInitGrab(int SceneID,int ProcessType,bool sendflag); //���·�װ��ʼ���պ�����
    void update_list_info();
	void updat_way2_list();
    void updata_list();
    void find(char * lpPath,bool bflag);
    void UpdateMapIndex(int iSel);    //����map�б�ɾ��������������
    static int WINAPI OnRemoteMessage( unsigned int wParam, void *lParam,void* otherparam);
    void ShowFileListCtrl(int index);


	int AddWay2Function(int selPos,int FunType);
	int AddProcessFunction(int FunType);

	void DelSceneFunc(int selIndex);

	void ClickProcessFunc();//�����б�����  �ɷ���
	void ClickWay2Func(); //�����б�����    �·���

	void DBClickProcessFunc();
	void DBClickWay2Func();
	int GetSceneFirstCamIndex(int SceneID);

	//��ͼ����
	//void CombineDrawShape();




private:
    CListCtrl m_listPicture[FILE_LIST_CTRL_COUNT];
    int       m_iCurGroup;
    CListCtrl m_listFunction;
	CButton m_checkbox;
	CComboBox m_RoiCombineType;  
	CButton m_RoiCombFlag;
	BOOL m_IsRoiComboin;
	
	//IDC_CBSCENDEBUG_COMPOSETYPE
	//CStatic m_static_scene;
	//CStatic m_static_index;
	long m_winID;
    CString m_strTemp;
    int          m_iTestImageIndex;     //�����ļ�����
	CHECK_RESULT m_strsult;
	CSetSceneDlg m_setScene;
    CSelectFunction         m_SelectFunction;
//#ifndef MYFUNC
    CSceneImageSetting      m_SceneImageSetting;
    CSceneModelSetting      m_SceneModelSetting;
    CSceneThresholdSetting  m_SceneThresholdSetting;
    CSceneFindLine          m_SceneFindLine;
    CSceneFindCircle        m_SceneFindCircle;
//#else
	 //SceneMatchModel* m_pMatchModel;
	CArray <VisionProcBase*> m_pFunArr;


//#endif
	CInterfaceSetDlg		m_InterfaceSet;
	vector<INTERFACE_SET_DATA>		m_interfaceSetData;
	vector<CSEBtn*>			m_SEBtn;
	vector<CSRBtn*>			m_SRBtn;

	bool m_sceneChange;
    bool m_initControl[SCENE_SIZE];
	double m_height;
	int m_iT; //�ϰ汾�пؼ���صĲ���
	int m_index; //�ϰ汾�пؼ���صĲ���
    int m_iCurSceneID; //��ǰ����
	int m_CurProcIndex;//��ǰ�����еĵ�ǰ��������
	int m_CurCamIndex;      // ��ǰ�������
    int m_iCurIndex;   // ��ǰ�����еĵڼ���ͼ��������Ǵ�����������Ҳ�������������������ȷ��ͼ������ͼ�������е�ͼ��λ�á�


    CWnd  *m_hROI; //ͼ�񴰿�ָ��
	CWnd* m_pFunSettingWnd;
	bool m_loop;
	int m_loopcount[16];
	int m_time;

	vector<CString> m_vecorpic;
    vector<unsigned int> m_vecListInfo;
    static CSceneDebugDlg *m_pInstance;
    bool   m_bIsContinousGrab;  //�Ƿ���ʵʱ��ʾ״̬
    void DrawControl();
	//
	RoiCombType m_CombType;   // ����ROI���������
	int m_ProcProcess;        //

public:
    int m_LoginLevels;
	afx_msg void OnBnClickedButtonloop();
	static DWORD WINAPI CalGrabThread(void* pThis);

	static void LoopSenceTest(void * param);
	void  DispLoolCount();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeCbscendebugComposetype();
	afx_msg void OnBnClickedBtscendebugDrawcompose();
	afx_msg void OnBnClickedBtscendebugDrawcircle();
	afx_msg void OnBnClickedBtscendebugDrawrect();	
	afx_msg void OnBnClickedBtscendebugShowall();

    void SaveSceneWay2Config();


	CButton m_BAddFun;
	CButton m_BDelProc;
	// //ѡ����ŵĿؼ�
	CComboBox m_SelAxisIndex;
	// ѡ��оߺŵĿؼ�
	CComboBox m_SelToolIndex;



	//CComboBox m_CB_SceneNo;
	CComboBox m_CB_RelaSceneNo;
	CEdit m_ED_SceneNo;
	afx_msg void OnCbnSelchangeScenedlgRelasceneno();
	CComboBox m_CB_NextSceneId;
	CEdit m_NextSceneRot;
	afx_msg void OnCbnSelchangeScenedlgNextsceneno();
};
