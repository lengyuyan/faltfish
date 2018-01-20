
// ImageIntefaceTestDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "HalconCpp.h"
#include <list>
#include "afxcmn.h"
using namespace Halcon;
using namespace std;

// CImageIntefaceTestDlg �Ի���
class CImageIntefaceTestDlg : public CDialogEx
{
// ����
public:
	CImageIntefaceTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_IMAGEINTEFACETEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonGetimageFirstr();
	CStatic m_static_windfirst;
	CStatic m_static_windsecond;
	afx_msg void OnBnClickedButtonGetimageFirst();

	Hobject m_hImage;
	Hlong m_diswin_first;
	Hlong m_diswin_second;
	Hlong m_imageHeight;
	Hlong m_imageWidth;
	void (*callback)(void *context);
	void DispImage();
	//void VedioCallback(void *context);
	list<string>::iterator m_list_str_it;
	string m_name;
public:
	afx_msg void OnBnClickedButtonVedioFirst();
	

	afx_msg void OnNMCustomdrawSliderExt(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_slider_exm;
	CSliderCtrl m_slider_gain;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnBnClickedButtonVedioSecond();
};
