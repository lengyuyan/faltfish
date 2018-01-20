#pragma once
#include "afxcmn.h"


// CLightDebugDlg �Ի���

class CLightDebugDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLightDebugDlg)

public:
	CLightDebugDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLightDebugDlg();

// �Ի�������
	enum { IDD = IDD_LIGHTDEBUG_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedButtonOpLight();
    afx_msg void OnNMCustomdrawSliderLightValue(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedButtonSetLight();
    afx_msg void OnDblclkListDoport(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnClickListDoport(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDblclkListSerialport(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnClickListSerialport(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedButtonUpsortport();
    afx_msg void OnBnClickedButtonDownsortport();
    afx_msg void OnBnClickedButtonAddport();
    afx_msg void OnBnClickedButtonUpdataport();
    afx_msg void OnBnClickedButtonLightnummodify();
    afx_msg void OnBnClickedButtonDeletePort();
    afx_msg void OnBnClickedButtonResetLight();

protected:
    void InitLightComBox();
    void SetLightParam();
    void InitAllListctrl();
    void InitListctrl(CListCtrl *Listctrl);
    void SortPortInstall();				//������������
    void SortPortSave();              //��������ı���
    CString GetPortNum();				//��ȡ�������õĴ��ں�
    void SortShowBut();
public:
    CComboBox m_CommList;
    CSpinButtonCtrl m_SpinValue;
    CSliderCtrl m_SliderChanl;
    CListCtrl m_DoPort;				//�������ô���ö��
    CListCtrl m_SortPort;				//����ѡ������ 
    CEdit m_PortModiEdit;
    int                 m_DoPortChoose;                //����ö���б��ѡ����
    int                 m_SortPortItem;                //����������б��ѡ����  
    int                 m_SortPortSubItem;             //����������б��ѡ����
    int m_LightValue;
    int m_OpenStatus;
    afx_msg void OnCbnSelchangeComboChannelIndex();
};
