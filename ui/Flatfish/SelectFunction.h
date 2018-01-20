#pragma once
#include "DataType.h"

#define  ADD_FUNCTION_MSG    WM_USER +1200  

#define  FUN_TYPE PROCESS_TYPE;
// CSelectFunction �Ի���

class CSelectFunction : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectFunction)

public:
	CSelectFunction(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSelectFunction();

// �Ի�������
	enum { IDD = IDD_DIALOG_FUN_SELECT };

    void UpdateUI(int iSceneID, int index);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    void NotifyAddFunction(LPARAM iType);      //֪ͨ���Դ������ӹ���
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonImage();
    afx_msg void OnBnClickedButtonModel();
    afx_msg void OnBnClickedButtonThreshold();
    afx_msg void OnBnClickedButtonFindLine();
    afx_msg void OnBnClickedButtonFindCircle();

private:
    int m_iCurSceneID;
public:
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedButtonMore();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
