
// FlatFishDlg.h : ͷ�ļ�
//

#pragma once

#include <vector>
#include "common.h"
#include "iManagerCtrl.h"
using namespace std;
// CFlatFishDlg �Ի���
class CFlatFishDlg : public CDialogEx
{
// ����
public:
	CFlatFishDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FLATFISH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
    static DWORD WINAPI  CheckStateThread(LPVOID lpParameter);

    afx_msg LRESULT OnManualMessage( WPARAM wParam, LPARAM lParam );
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL DestroyWindow();
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();

    //afx_msg LRESULT OnSDKMessage( WPARAM wParam, LPARAM lParam );

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

    void initLayout();
    void initStatusBar();
    void initSettingDll();
	void initSceneList();
	void GrabSceneInit();

    void unLoadDll();
    static int WINAPI OnSDKMessage( unsigned int wParam, void * lParam, void* otherparam);
    virtual void OnOK();
    virtual void OnCancel();
	void *GetImageBufByFile(char *path,int &size,int &x,int &h);
public:
    vector<CStatic *>m_vecPicItem;   //���ڱ��������ڽ���ͼ��
	


private:
    static CFlatFishDlg *m_pInstance;
    //״̬����Ϣ
    CStatusBar m_statusBar;    
	static bool isstatusready;//�ж�״̬���Ƿ�׼���ã��Ȳ�����

    UINT      *m_pIndicators;             
    int       m_LoginLevel;   
    UINT      m_IndicatorsSize;   //״̬��״̬����
	bool      m_IsSceneInit;
	int     m_LayRow;
	int     m_LayCol;

    
    HINSTANCE m_hSystemSettingDLL;
    HINSTANCE m_hSceneDebugDLL;
    HINSTANCE m_hLoginDLL;
public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	CString m_TitleName;
	
	
};
