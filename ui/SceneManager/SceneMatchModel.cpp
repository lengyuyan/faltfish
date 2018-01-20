
#pragma  once

#include "StdAfx.h"
#include "SceneMatchModel.h"
#include "SceneAcqImage.h"
#include "io.h"
#include "direct.h"

SceneMatchModel::SceneMatchModel(void)
{
	 //m_ProcTypeId = MODEL_PRO;
	BuildInstance();
}
SceneMatchModel::SceneMatchModel(CString ConfigPath,int ProcID):VisionProcBase(ConfigPath,ProcID)
{
	BuildInstance();
}

//SceneMatchModel::SceneMatchModel(CWnd*pParentWnd):VisionProcBase(pParentWnd)
//{
//
//}

SceneMatchModel::~SceneMatchModel(void)
{
	m_SceneModelSetting.CloseSettingDlg();

}




void SceneMatchModel::BuildInstance(void)
{
		CString szTemp;
		m_CalModRotAng = 0.0;
	    m_ProcTypeId = MODEL_PRO;
		m_SceneModelSetting.m_ProcProcess = 0;
		m_SceneModelSetting.GetContainer(this);
		ReadProcConfig(m_ProcNo);
		
		//�õ�ģ��·������
		//char ScenePath[256] = {0};
		memset(m_ModelName,0,sizeof(m_ModelName));
		memcpy(m_ModelName, GetScenePath(GetSceneId()),sizeof(m_ModelName));
		if(-1 == access(m_ModelName,0))
			    _mkdir(m_ModelName);
		strcat_s(m_ModelName,"\\");
		szTemp.Format("Scene%d_shapeMod_%d.mod",GetSceneId(),m_ProcNo);
		strcat_s(m_ModelName,szTemp);
		
		
}




void SceneMatchModel::InitInstance(CWnd* pParentWnd)
{	
	CRect rtS;
	if (NULL == pParentWnd)
	{
		return;
	}
	m_pParentWnd = pParentWnd;

	assert(pParentWnd->IsKindOf( RUNTIME_CLASS(CWnd)));
	m_pParentWnd->GetClientRect(&rtS);
	m_SceneModelSetting.Create(IDD_DIALOG_SCENE_MODEL, m_pParentWnd);
	m_SceneModelSetting.SetWindowPos(NULL,rtS.left,rtS.top,rtS.right,rtS.bottom,SWP_NOZORDER);
	m_SceneModelSetting.ShowWindow(SW_SHOW);
	m_SceneModelSetting.UpdateUI();
	//m_SceneModelSetting.GetContainer(this);

	m_SceneInitialCount = g_SceneInitialCount;

}

void SceneMatchModel::DeleteSelf()
{
	delete this;

}

void SceneMatchModel::HideConfigWnd()
{
    if(g_SceneInitialCount==m_SceneInitialCount)
	{
		m_SceneModelSetting.ShowWindow(SW_HIDE);
	}
}

void SceneMatchModel::ShowConfigWnd()
{
	if(g_SceneInitialCount==m_SceneInitialCount)
		m_SceneModelSetting.ShowWindow(SW_SHOW);
}

void SceneMatchModel::SaveProcConfig(int ProcIndex)
{
	VisionProcBase::SaveProcConfig(ProcIndex);
  //����������
	char szValue[256] = {0};
	CString ProcIndexName;
	ProcIndexName.Format("%d",ProcIndex);
	memset(szValue,0,sizeof(szValue));
	sprintf_s(szValue,"%.6lf",m_CalModRotAng);
	WritePrivateProfileStringA(ProcIndexName,"CalModRotAng",szValue, m_ConfigPath);	

	 //����ROI�Ĳ���
	m_SceneModelSetting.SaveProcConfig(m_ConfigPath,ProcIndex);

}



void SceneMatchModel::ShowAllShape(void)
{
	OnShowAllRoi(g_SceneDlgHwinId,GetSceneId(),&m_RoiIdList);

}
void SceneMatchModel::CreateModel(int Nevels,double AngleStart,double AngleExtent,int Contrast,double& ModRow,double ModCol,
	double& ModPhi)
{
	//�õ������ͼ�����
	 long HalWinId = -1;

	 int ImageIndex = g_SceneList.GetSceneBySceneID(GetSceneId())->GetProcImageIndex(GetProcId());
	 if (FindHalconWinID(g_SceneDlgHwinId,HalWinId))
	 {
	      CreateWay2VisionModel(HalWinId,GetSceneId(),m_RoiID,m_ModelName,Nevels,AngleStart,AngleExtent,Contrast,ImageIndex);
     }

	 //�´���ƥ������ݱ��浽�����ļ��С�
	 
	double Score = 0.5,Row,Col,phi ;
	bool IsBuild = true ; //���ò���IsBuild���ж��Ƿ����½�ƥ��

	//��ȡ  ƥ�����
	int ProcIndex = GetProcTypeIndex();


	// Row = GetProcIndexById(GetProcId());
	MatchModel(m_SceneModelSetting.m_MatchLevel, m_SceneModelSetting.m_MatchNum,Score,IsBuild,Row,Col,phi);
    
	char szName[256] ={0};
	char szValue[256]={0};
	CString ProcIndexName,strTemp;
	ProcIndexName.Format("%d",GetProcIndexById(GetProcId()));
	//�洢ģ������
	memset(szName,0,sizeof(szValue));
	memset(szValue,0,sizeof(szValue));
	sprintf_s(szName,"Match%d_OriRow",GetProcIndexById(GetProcId()));	
	sprintf_s(szValue,"%.6lf",Row);
	WritePrivateProfileStringA(ProcIndexName,szName,szValue, m_ConfigPath);

	memset(szName,0,sizeof(szValue));
	memset(szValue,0,sizeof(szValue));
	sprintf_s(szName,"Match%d_OriCol",GetProcIndexById(GetProcId()));	
	sprintf_s(szValue,"%.6lf",Col);
	WritePrivateProfileStringA(ProcIndexName,szName,szValue, m_ConfigPath);

	memset(szName,0,sizeof(szValue));
	memset(szValue,0,sizeof(szValue));
	sprintf_s(szName,"Match%d_OriPhi",GetProcIndexById(GetProcId()));	
	sprintf_s(szValue,"%.6lf",phi);
	WritePrivateProfileStringA(ProcIndexName,szName,szValue, m_ConfigPath);	
}

BOOL  SceneMatchModel::MatchModel(int PyramidLevel,int MatchNum,double& Score,bool IsBulid,double& row,double& col,double& phi)
{
	 int ImageIndex = g_SceneList.GetSceneBySceneID(GetSceneId())->GetProcImageIndex(GetProcId());
	 int ProcIndex  = GetProcTypeIndex();//���������Ǵ������ͬ����ĵڼ���
	 SceneAcqImage* pAcqImage = dynamic_cast<SceneAcqImage*>(g_SceneList.GetSceneBySceneID(GetSceneId())->GetProcByTypeIndex(IMAGE_ACQISITION,ImageIndex));
	 int CamIndex = pAcqImage->m_CamIndex; //�������
	
	 long HalWinId = -1;
	 if(g_SceneInitialCount==m_SceneInitialCount&&g_SceneInitialCount!=0)
	 {
		 FindHalconWinID(g_SceneDlgHwinId,HalWinId);
	 }else
	 {
		 SYSTEM_PARAM pSysParam;
		 GetSysParam(&pSysParam);
		 vector<CStatic *>* pImageWnd = ((vector<CStatic *>*)(pSysParam.MainImageWndVec));
		 if (CamIndex < pImageWnd->size())
		 {
			 FindHalconWinID((long)(pImageWnd->at(CamIndex)->m_hWnd),HalWinId);//��ʾ�������Ӧ�Ĵ���

		 }else
		 {
			  FindHalconWinID((long)(pImageWnd->at(0)->m_hWnd),HalWinId);//��ʾ�ڵ�һ����
		 }

	}
	 if (-1 == HalWinId )
	 {
		 return false;
	 }


    if(!FindWay2VisionModel(GetSceneId(),HalWinId,ImageIndex,m_ModelName,PyramidLevel,MatchNum,Score,ProcIndex,IsBulid,row,col,phi))
	{
		return false;
	}
	
	return true;

}


void SceneMatchModel::ReadProcConfig(int ProcIndex)
{
	VisionProcBase::ReadProcConfig(ProcIndex);
	//����������
	char szValue[256] = {0};
	CString ProcIndexName,strTemp;
	int CombShapeNum = 0;
	int ShapeId = -1;
	ST_ROI_INFO RoiInfor;
	ProcIndexName.Format("%d",ProcIndex);
	memset(szValue,0,sizeof(szValue));
	GetPrivateProfileStringA(ProcIndexName, "CalModRotAng", "",szValue,sizeof(szValue), m_ConfigPath);
	m_CalModRotAng = atof(szValue);
	m_SceneModelSetting.ReadProcConfig(m_ConfigPath,ProcIndex);

}





BOOL SceneMatchModel::VisionProcRun(CHECK_RESULT& ProcRes)
{
	bool ResRun = false;
	SCENE_PKG *config = NULL;
	double CamRotPhi = 0.0,MoveX = 0.0,MoveY =0.0;
	SYSTEM_PARAM *sysParam = NULL;
	sysParam = (SYSTEM_PARAM *)GetSysParamPoint();
	if (NULL == sysParam)
	{
		AfxMessageBox("ϵͳ������ȡʧ��!");
		return false;
	}
	config = (SCENE_PKG *)GetSceneParamHandle(GetSceneId());
	int AxisIndex = config->AxisIndex;
	if (AxisIndex<0)
	{
		AfxMessageBox("��������ô���");
		return false;
	}
	int imageIndex = (g_SceneList.GetSceneBySceneID(GetSceneId()))->GetProcImageIndex(GetProcId());
	SceneAcqImage* pAcqImage = dynamic_cast<SceneAcqImage*>(g_SceneList.GetSceneBySceneID(GetSceneId())->GetProcByTypeIndex(IMAGE_ACQISITION,imageIndex));
	int CamIndex = pAcqImage->m_CamIndex; //�������
	int LastId = -1,NextId = -1;
	double SceneRotPhi = 0.0;

	ProcList* pScene = g_SceneList.GetSceneBySceneID(GetSceneId());
	if (pScene)
	{
		LastId = pScene->GetRelaSceneID();
		NextId = pScene->GetNextSceneID();
		SceneRotPhi = pScene->GetNextRotAngle()*31.1416/180;
	}

	try
	{
		if(m_SceneModelSetting.VisionProcRun(ProcRes))//�������������˽��,�����ȷ�Ż��߽�if
		{
			if (IMAGE_ABS == m_ResultType || MODLE_DISTENCE == m_ResultType)//��������ֵ�����ֵ���Ǿ���ֵ
			{
				ProcRes.resultType = 0;
			}
			else
			{
				ProcRes.resultType = 1;//1Ϊ�������
			}//�궨�����߲�����,���ص�����������
			if (m_ResultType > 0) //���µ�����������㣬���û�б궨����ͼоߣ�����ʹ�õĸ���ֵ�ǳ�ֵ�����㽫�Ǵ���ġ�
			{		
				int ToolIndex = config->MotorToolIndex; //�о�����							
				CamRotPhi =  atof(ProcRes.szValue[3]);  //��3����Ϊ�Ƕ�
				double RotAxeX = sysParam->RotX[AxisIndex][ToolIndex] + sysParam->RotX_Adjust[AxisIndex][ToolIndex];//�ò���
				double RotAxeY = sysParam->RotY[AxisIndex][ToolIndex] + sysParam->RotY_Adjust[AxisIndex][ToolIndex];;
				bool AxeMoveRotDirect = sysParam->AxeRotDirect[CamIndex][AxisIndex][ToolIndex];//��ת����ϵͳ��������
				int  ResType = sysParam->PosMoveType[CamIndex][AxisIndex][ToolIndex];//λ�����ͣ��������껹����ת���������,ϵͳ��������
				double picCentX = sysParam->PicCenterPointX[CamIndex][AxisIndex][ToolIndex]; //�������ص�x
				double picCentY = sysParam->PicCenterPointY[CamIndex][AxisIndex][ToolIndex]; //�������ص�y
				
				//2����Ҫ�������������ϵ��λ�˻�ԭʱ���������ϵ�µ�����˶���			
				switch (m_ResultType)
				{
				case REAL_UNIT_RESULT:  //������ģ���Ӧ�����ƫ��ֵ��������ת
					{
						MoveAndRotInCamForOriMatch(GetSceneId(),sysParam->CameraPixSize[CamIndex][AxisIndex][ToolIndex],AxeMoveRotDirect,MoveX,MoveY,
							        LastId,NextId,SceneRotPhi,picCentX,picCentY,sysParam->CamParam[CamIndex][AxisIndex][ToolIndex],12,sysParam->CamPose[CamIndex][AxisIndex][ToolIndex],
									sysParam->CalThickNess[CamIndex][AxisIndex][ToolIndex],
									sysParam->RotX[AxisIndex][ToolIndex]+sysParam->RotX_Adjust[AxisIndex][ToolIndex],
									sysParam->RotY[AxisIndex][ToolIndex]+sysParam->RotY_Adjust[AxisIndex][ToolIndex],
									CamRotPhi,m_CalModRotAng*3.1416/180.0);	//
					}
					break;
				case NO_MATCH_ROT: //������ģ���Ӧ�����ƫ��ֵ,��������ת
					{
						CamRotPhi = 0.0;
						//����,����id,���ش�С,��ת����,ģ��x,y����һ������id����һ������id,��һ��������ת�Ƕ�,����ڲ�,������,���,��ת����x��y,z,�˴μ��Ƕȸ����νǶȵĲ�,ģ��Ƕ�(),���,��,��������
						//MoveAndRotInCamForOriMatch(GetSceneId(),sysParam->CameraPixSize[CamIndex][AxisIndex][ToolIndex],AxeMoveRotDirect,MoveX,MoveY,
						//	LastId,NextId,SceneRotPhi,picCentX,picCentY,sysParam->CamParam[CamIndex][AxisIndex][ToolIndex],12,sysParam->CamPose[CamIndex][AxisIndex][ToolIndex],
						//	sysParam->CalThickNess[CamIndex][AxisIndex][ToolIndex]);
						//ģ���ƶ��˶��٣������ƶ�����
						MoveX = atof(ProcRes.szValue[1]) *sysParam->CameraPixSize[CamIndex][AxisIndex][ToolIndex];
						MoveY = atof(ProcRes.szValue[2]) *sysParam->CameraPixSize[CamIndex][AxisIndex][ToolIndex];


					}
					break;
				case TEST_REPOSITION: //������ģ���Ӧ�����ƫ��ֵ,��������ת//�����ظ���λ  IMAGE_RELA   ͼ������궨���������
					{
		
						MoveAndRotInCamForOriMatch(GetSceneId(),2,AxeMoveRotDirect,MoveX,MoveY,
							LastId,NextId,SceneRotPhi,picCentX,picCentY,sysParam->CamParam[CamIndex][AxisIndex][ToolIndex],12,sysParam->CamPose[CamIndex][AxisIndex][ToolIndex],
							sysParam->CalThickNess[CamIndex][AxisIndex][ToolIndex],
							sysParam->RotX[AxisIndex][ToolIndex]+sysParam->RotX_Adjust[AxisIndex][ToolIndex],
							sysParam->RotY[AxisIndex][ToolIndex]+sysParam->RotY_Adjust[AxisIndex][ToolIndex],
							CamRotPhi,m_CalModRotAng*3.1416/180.0,CamIndex,AxisIndex,ToolIndex);
					}
					break;
				case IMAGE_ABS: //������ģ���Ӧ�����ƫ��ֵ,��������ת
					{
						MoveAndRotInCamForOriMatch(GetSceneId(),3,AxeMoveRotDirect,MoveX,MoveY,LastId,NextId,SceneRotPhi,picCentX,picCentY,
							sysParam->CamParam[CamIndex][AxisIndex][ToolIndex],12,sysParam->CamPose[CamIndex][AxisIndex][ToolIndex],
							sysParam->CalThickNess[CamIndex][AxisIndex][ToolIndex],
							sysParam->RotX[AxisIndex][ToolIndex]+sysParam->RotX_Adjust[AxisIndex][ToolIndex],
							sysParam->RotY[AxisIndex][ToolIndex]+sysParam->RotY_Adjust[AxisIndex][ToolIndex],
							CamRotPhi,m_CalModRotAng*3.1416/180.0,CamIndex,AxisIndex,ToolIndex);
						
					}
					break;
				case MODLE_DISTENCE: //��Ի�е�֣������������ֵ��
					{
						//MoveAndRotInCamForOriMatch(GetSceneId(),sysParam->CameraPixSize[CamIndex][ToolIndex],MoveX,MoveY);
						int ResCount = ProcRes.iCount; 
						MoveX = sysParam->CameraPixSize[CamIndex][AxisIndex][ToolIndex]*atof(ProcRes.szValue[1]);
						MoveY = sysParam->CameraPixSize[CamIndex][AxisIndex][ToolIndex]*atof(ProcRes.szValue[2]);
						//��������λ����궨ʱ�ĳ�ʼλ���ƶ��仯����Ҫ��������仯�� 
						double CurCamX = 0.0;
						double CurCamY = 0.0;
						double CurCamPhi = 0.0;

						double OriCamX  = sysParam->CamCalOriX[CamIndex][AxisIndex][ToolIndex];
						double OriCamY  = sysParam->CamCalOriY[CamIndex][AxisIndex][ToolIndex];
						double OriCamPhi  = sysParam->CamCalOriPhi[CamIndex][AxisIndex][ToolIndex];

						if (1) //���ʵ������λ���뽨��ģ���λ��һ�¡�
						{
							CurCamX = OriCamX;
							CurCamY = OriCamY;
							CurCamPhi = OriCamPhi;
						}else
						{
							//���ʵ������λ���뽨��ģ���λ�ò�һ�£���Ҫͨ��Э�����ʵ������λ�ã���ʵ�֡�
						 
						}
						double TcpRot = 0.0;
						if (AxeMoveRotDirect)//true = �෴
						{
							TcpRot =  -CamRotPhi; //ȡ����Ϊ�˾����ص�ԭ��ģ��ĽǶȣ������ƽ�У���
						}else
						{
							TcpRot = CamRotPhi;
						}

						MoveX = sysParam->ToolCalOriX[CamIndex][AxisIndex][ToolIndex];//TCP Mod 
						MoveY = sysParam->ToolCalOriY[CamIndex][AxisIndex][ToolIndex];
						//����Ϊ:����Ṥ������,������ش�С���Ƕ� ��һ���� �Ƿ��ñ궨 ���,�ڲ�12 ��� 
						AffToRobotWorld(GetSceneId(),CamIndex,AxisIndex,ToolIndex,sysParam->CameraPixSize[CamIndex][AxisIndex][ToolIndex],
							TcpRot,MoveX,MoveY,sysParam->CalWithCamParam[CamIndex][AxisIndex][ToolIndex],sysParam->CalThickNess[CamIndex][AxisIndex][ToolIndex],
							sysParam->CamParam[CamIndex][AxisIndex][ToolIndex],12,sysParam->CamPose[CamIndex][AxisIndex][ToolIndex]);

					}
					break;
					

				}
				//3,�����ϵ�е��ƶ��任����еϵ�У�����任��ʹ�õľ���ֻ����ת���󣬲����ƶ���
				if (REAL_UNIT_RESULT == m_ResultType)
				{
					AffinePointByCamInMecRot(CamIndex,AxisIndex,ToolIndex,MoveX,MoveY);
					MoveX = -MoveX;
					MoveY = -MoveY;
					
				}

				//4������Ҫ�ı��ֵ�����������У�����m_SceneModelSetting������ʹ�õĽ�����ݲ������˴���ò�Ҫ���ǣ���Ӧʹ������������
				if (AxeMoveRotDirect)//true = �෴
				{
					CamRotPhi = CamRotPhi;
				}else
				{
					CamRotPhi = -CamRotPhi;
				}
				//ת����Ľ��

				sprintf(ProcRes.szValue[1],"%0.4lf",MoveX);
				sprintf(ProcRes.szValue[2],"%0.4lf",MoveY);
				double angleA = CamRotPhi*180/3.1415926;
				if (angleA>=180.000)
				{
					angleA -= 180.000;
				}
				sprintf(ProcRes.szValue[3],"%0.4lf",angleA);	
	
			}	
			ResRun = true;
		}else
		{
			 ProcRes.err = MODEL_PRO;
			 ResRun = false;
		}
	}
	catch(...)
	{
		ResRun = false;
	}

	vector<CStatic *>* pImageWnd = ((vector<CStatic *>*)(sysParam->MainImageWndVec));
	long HalconWinID = -1;
	if(-1 == g_SceneDlgHwinId)
	{
		FindHalconWinID((int)(pImageWnd->at(CamIndex)->m_hWnd),HalconWinID);
	}
	else
	{
		FindHalconWinID(g_SceneDlgHwinId,HalconWinID);
	}

	if (ResRun)
	{
		SetVisionColorHeight(HalconWinID,"green");//������ɫ,Ĭ������
		ShowVisionString(HalconWinID,"ƥ�� OK");//Ĭ����0,0λ��д����

	}else 
	{
		SetVisionColorHeight(HalconWinID,"red");
		ShowVisionString(HalconWinID,"ƥ�� NG");
	}

	//���ֻ�������̣��򷵻�OK
	if (sysParam->IsSimulate)
	{
		ResRun = true;
	}

	return ResRun;
}



