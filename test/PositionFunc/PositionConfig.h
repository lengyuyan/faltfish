#pragma once
#include "ApplyFunParam.h"


// PositionConfig �Ի���

class PositionConfig : public ApplyFunParam
{
	DECLARE_DYNAMIC(PositionConfig)

public:
	PositionConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PositionConfig();

// �Ի�������
	//enum { IDD = IDD_POSITIONCONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
