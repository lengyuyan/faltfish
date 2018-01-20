#pragma once
#include "resource.h"
#include "afxwin.h"
#include "FuncDefine.h"

// ScenePositionSettin �Ի���

class ScenePositionSettin : public CDialogEx
{
	DECLARE_DYNAMIC(ScenePositionSettin)

public:
	ScenePositionSettin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ScenePositionSettin();

// �Ի�������
	enum { IDD = IDD_SCENE_POSITION };
	enum coordinateType
	{
		Point_To_Point =0 ,
		Point_To_Line =1,
		Line_To_Line = 2,

	};
	struct DoublePoint
	{
		double x;
		double y;
		DoublePoint()
		{
			x = 0.0;
			y = 0.0;
		}
		DoublePoint(double InX,double InY)
		{
			x = InX;
			y = InY;
		}
		void operator=(const DoublePoint& src)
		{
			x = src.x;
			y = src.y;

		}

	};


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnBnClickedScenepositionCommit();
	void CloseSettingDlg();


	CEdit m_OriginalX;
	CEdit m_OriginalY;
	CEdit m_OriginalA;
	CComboBox m_CoordinateType;
	CComboBox m_CMatchSel;

	void UpdateUI();
	void GetContainer(void* Pcontainer);
	void SetDisplayWindowID(int winID);
	void SetCoordinate();
	
	void ReadProcConfig(CString ConfigPath,int ProcIndex);
	void SaveProcConfig(CString ConfigPath,int ProcIndex);
	BOOL VisionProcRun( CHECK_RESULT& ProcRes);

public:
	int m_iCurSceneID;
	int m_iCurIndex;
	int m_winID;
	int m_MatchIndex;

	coordinateType m_CoordType;

	int m_ProcProcess ;
	void* m_Pcontainer;
	

	CArray<DoublePoint> m_CoordinatePoint; ////��������ϵ���Ƕ������Ա��Ԫ��ˢ�¡�

	//���ʵʱ��������Ϣ��ÿ��λһ�ζ���ˢ��һ�Ρ�
	FUNCCOORDINATE m_Coodinate;
	//vector<int> m_ShapeIdList;  //����ͼԪ��ID������һЩ��Ҫ�ĳ��ϡ�

	afx_msg void OnBnClickedScenedlgPositionSearchcontour();
	afx_msg void OnCbnSelchangeScenepositionCoordinateway();
	afx_msg void OnBnClickedScenepositionCancel();
	afx_msg void OnCbnDropdownScenepositionSelmatch();
	afx_msg void OnCbnSelchangeScenepositionSelmatch();

};
