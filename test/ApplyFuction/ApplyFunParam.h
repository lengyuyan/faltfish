#pragma once


// ApplyFunParam �Ի���

class   /*_declspec(dllexport)*/ ApplyFunParam : public CDialogEx
{
	DECLARE_DYNAMIC(ApplyFunParam)

public:
	ApplyFunParam(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ApplyFunParam();

// �Ի�������
	enum { IDD = IDD_APPLYFUC_PARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	



};
