#pragma once
#include "DataType.h"
#include "resource.h"

#define  ADD_FUNCTION_MSG    WM_USER +1200  

#define  FUN_TYPE PROCESS_TYPE;
// CSelectFunction �Ի���
/*
���ܣ��Ի�����ʾ
1����Ӵ����ܰ�ť
2���°汾������һ�����ṹ�������˾ɰ汾�еĹ��ܰ�ť
3,����Ҫ����һЩ������Ӧ�¼���Ȼ��ͨ��ADD_FUNCTION_MSG,��Ϣ���׸��������Դ��ڴ���
*/

#define STR_FUN_IMAGE_READ       "ͼ�����"
#define STR_FUN_IMAGE_ACQ        "ͼ��ɼ�"
#define STR_FUN_IMAGE_ENHANCEMENT "ͼ����ǿ"
#define STR_FUN_MATCH            "ģ��ƥ��"
#define STR_CONTOUR_POSITION     "������λ"
#define STR_HALPROC_POSITION     "HAL��λ"
#define STR_HALPROC_PROCESS      "HALͨ�ô���"
#define STR_NO_PROC              "δ֪����"


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

	virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedButtonImage();//ͼ��ɼ� //һ�¶��Ǿɰ�Ĺ��ܰ�ť����
    afx_msg void OnBnClickedButtonModel();//ģ��ƥ��
    afx_msg void OnBnClickedButtonThreshold();//��ֵ����
    afx_msg void OnBnClickedButtonFindLine();//����
    afx_msg void OnBnClickedButtonFindCircle();//��Բ

private:
    int m_iCurSceneID;//����id
	int m_CurProcIndex; //��ǰ������Ŀ������
	CTreeCtrl m_FunTree;
	CString m_ToolItemStr;
	BOOL m_ItemChange;
public:
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized); //�����
	afx_msg void OnBnClickedButtonMore();  //���ܰ�ť
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus); //��ʾ����
	afx_msg void OnNMDblclkScenefuncTree(NMHDR *pNMHDR, LRESULT *pResult);//˫������ѡ���¼�
	afx_msg void OnTvnSelchangedScenefuncTree(NMHDR *pNMHDR, LRESULT *pResult);//����������ͻ���Ӧ�¼�,û�õ�
	afx_msg void OnBnClickedScenefunPosition();//������λ����
};
