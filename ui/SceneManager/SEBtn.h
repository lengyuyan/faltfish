#pragma once
#include "DataType.h"


#define  BTN_ADD_SUB       WM_USER + 1001    //��ť�Ӽ���Ϣ 
enum BTN_ALGORITHM	//��ť���㷨��
{
	BTN_ADD = 0,	//�ӷ�
	BTN_SUB,		//����
	BTN_INIT,		//��ʼ��
	BTN_DELETE,		//ɾ��
};
// CSEBtn �Ի���

class CSEBtn : public CDialogEx
{
	DECLARE_DYNAMIC(CSEBtn)

public:
	CSEBtn(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSEBtn();

// �Ի�������
	enum { IDD = IDD_DIALOG_SEBTN };

	void UpdateUI(int iSceneID, int index, CWnd *hROI, int revise = 0);
	void getInterfaceSetData(INTERFACE_SET_DATA interfaceSetData);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg LRESULT OnBtnMessage( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButtonSebAdd();
	afx_msg void OnClickedButtonSebSub();
	//afx_msg void OnBnClickedBtnCheckOne();
	//bool SetSceneParamItemValue(int index, int iType, int iValue, double dValue);
	void SetDisplayWindowID(int winID);
	void UpdateItem(CString str, BTN_ALGORITHM btnAlgorithm, bool insert = false);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	INTERFACE_SET_DATA m_interfaceSetData;
	int m_iCurSceneID;
	int m_iCurIndex;
	int m_winID;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditSeb();
	int CurSceneID();
	string BtnType();
};
