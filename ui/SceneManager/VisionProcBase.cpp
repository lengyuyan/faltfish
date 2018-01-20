#include "StdAfx.h"
#include "VisionProcBase.h"
#include "SceneLogic.h"




VisionProcBase::VisionProcBase(void)
{
	BuildInstance();
}

VisionProcBase::VisionProcBase(CString ConfigPath,int ProcID):m_ConfigPath(ConfigPath)
{
     BuildInstance();
	 m_VisonProcId = ProcID; //Ϊ����Id*10000+index  �ڴ���
	 VisionProcIdCount = ProcID; //��ʱû���õ�
	 m_ProcNo = ProcID%SCENE_PROC_ID_FACTOR; //�����е�����,Ҳ�Ǵ��������е�����
	 
}

VisionProcBase::~VisionProcBase(void)
{
	
}

int VisionProcBase::VisionProcIdCount = -1;



void VisionProcBase::VisionProc(...)
{

}

void VisionProcBase::BuildInstance(void)   //�ú���ֻ���ڹ��캯���е��ã�ֻ�ܵ���һ��
{
	m_pParentWnd = NULL;
	m_VisonProcId = -1;
	m_SceneInitialCount = 0;//�ж��Ƿ�򿪳�������
	m_CurSceneCamIndex = 0; //��ǰ�������
}

void  VisionProcBase::InitInstance(CWnd* pParentWnd)
{
	m_pParentWnd = pParentWnd;//����debu���и����������Ϊ����
}

 BOOL VisionProcBase::IsInitialConfigWnd()
{
	//g_SceneInitialCountȫ�ֳ�ʼΪ0,����dug���������ʱ++
	//m_SceneInitialCount��vp���ǲ����ӵ�

	return  (m_SceneInitialCount!=g_SceneInitialCount);  // !g_IsSceneDlgInitial;
} 



int  VisionProcBase::GetProcId()
{
	return m_VisonProcId;
}

int  VisionProcBase::GetProcTypeID()
{
	return m_ProcTypeId;
}

void VisionProcBase::DeleteSelf()
{



}

int VisionProcBase::GetProcIdBase()
{
	return m_VisonProcId%SCENE_PROC_ID_FACTOR;
}


int VisionProcBase::GetSceneId()
{
	 return m_VisonProcId/SCENE_PROC_ID_FACTOR;

}
int VisionProcBase::GetProcIndexById(int ProcID)
{
	return (g_SceneList.GetSceneBySceneID(GetSceneId()))->GetProcIndex(ProcID);
}

int VisionProcBase::GetProcTypeIndex()
{
	int TypeIndex = -1;
	for (int i =0,k = 0; i<=GetProcIndexById(GetProcId());i++)//����ID�ҵ���������,Ȼ����������ҵ���������ͬ�����е�����
	{
		if( m_ProcTypeId == GetCurScene()->GetProcType(i))
		{	
			TypeIndex = k;
			k++;
		}
	}
	return TypeIndex;
}


ProcList* VisionProcBase::GetCurScene()
{
	return g_SceneList.GetSceneBySceneID(GetSceneId());
}


void VisionProcBase::ReadAndCreateRect(CString ProcIndexName,int ShapeIndex)
{
	CString strTemp;
	char szResValue[256]={0};
	ST_ROI_INFO RectRoi;

	strTemp.Format("ShapeID%d",ShapeIndex);
	int ShapeId = GetPrivateProfileInt(ProcIndexName,strTemp,-1,m_ConfigPath);
	m_RoiIdList.push_back(ShapeId);

	strTemp.Format("ShapeParam%d_Row",ShapeIndex);
	memset(szResValue,0,sizeof(szResValue));
	GetPrivateProfileStringA(ProcIndexName,strTemp,"-1",szResValue,sizeof(szResValue),m_ConfigPath);
	RectRoi.strect.row = atof(szResValue);

	strTemp.Format("ShapeParam%d_Col",ShapeIndex);
	memset(szResValue,0,sizeof(szResValue));
	GetPrivateProfileStringA(ProcIndexName,strTemp,"-1",szResValue,sizeof(szResValue),m_ConfigPath);
	RectRoi.strect.col = atof(szResValue);

	strTemp.Format("ShapeParam%d_Angle",ShapeIndex);
	memset(szResValue,0,sizeof(szResValue));
	GetPrivateProfileStringA(ProcIndexName,strTemp,"-1",szResValue,sizeof(szResValue),m_ConfigPath);
	RectRoi.strect.angle = atof(szResValue);

	strTemp.Format("ShapeParam%d_Len1",ShapeIndex);
	memset(szResValue,0,sizeof(szResValue));
	GetPrivateProfileStringA(ProcIndexName,strTemp,"-1",szResValue,sizeof(szResValue),m_ConfigPath);
	RectRoi.strect.length1 = atof(szResValue);

	strTemp.Format("ShapeParam%d_Len2",ShapeIndex);
	memset(szResValue,0,sizeof(szResValue));
	GetPrivateProfileStringA(ProcIndexName,strTemp,"-1",szResValue,sizeof(szResValue),m_ConfigPath);
	RectRoi.strect.length2 = atof(szResValue);
	//����SDK�Ĺ������ڳ�����Ӧ��ͼ�δ��������������״��
	GenShape( GetSceneId(),RectRoi, HoRecTangleType,ShapeId,m_CurSceneCamIndex);

}

void  VisionProcBase::ReadAndCreateCircle(CString ProcIndexName,int ShapeIndex)
{
	CString strTemp;
	char szResValue[256]={0};
	ST_ROI_INFO CircleRoi;

	strTemp.Format("ShapeID%d",ShapeIndex);
	int ShapeId = GetPrivateProfileInt(ProcIndexName,strTemp,-1,m_ConfigPath);
	m_RoiIdList.push_back(ShapeId);

	strTemp.Format("ShapeParam%d_Row",ShapeIndex);
	memset(szResValue,0,sizeof(szResValue));
	GetPrivateProfileStringA(ProcIndexName,strTemp,"-1",szResValue,sizeof(szResValue),m_ConfigPath);
	CircleRoi.stcircle.row = atof(szResValue);

	strTemp.Format("ShapeParam%d_Col",ShapeIndex);
	memset(szResValue,0,sizeof(szResValue));
	GetPrivateProfileStringA(ProcIndexName,strTemp,"-1",szResValue,sizeof(szResValue),m_ConfigPath);
	CircleRoi.stcircle.col = atof(szResValue);

	strTemp.Format("ShapeParam%d_Rii",ShapeIndex);
	memset(szResValue,0,sizeof(szResValue));
	GetPrivateProfileStringA(ProcIndexName,strTemp,"-1",szResValue,sizeof(szResValue),m_ConfigPath);
	CircleRoi.stcircle.Radius = atof(szResValue);
	//����SDK�Ĺ������ڳ�����Ӧ��ͼ�δ��������������״��
	GenShape( GetSceneId(),CircleRoi, HoCircleType,ShapeId,m_CurSceneCamIndex);


}

void VisionProcBase::AfterCreateComb()
{
	m_RoiID = m_RoiIdList.at(m_RoiIdList.size()-1);
	m_RoiIdList.clear();
	m_RoiComIdList.clear();

}


void VisionProcBase::ReadProcConfig(int ProcIndex)
{
	//��ȡ�������������ϢӦ�÷��ڸ�����������ȥ,��Ӧ�÷��ڻ�����

	//��ȡ��Roi��ز���������Shape��ROI,����ROI��Id��������m_RoiIdList�С�
	//��Ϊ����һ��ģ�壬ROIֻ����һ����
	//��ȡ���ͼ�ε���Ϣ�����ɶ���
	// 
	char szValue[256] = {0};
	CString ProcIndexName,strTemp;
	int CombShapeNum = 0;
	int ShapeId = -1;
	ST_ROI_INFO RoiInfor;
	ProcIndexName.Format("%d",ProcIndex);

	ShapeId = GetPrivateProfileInt(ProcIndexName,"CombShapeID", -1, m_ConfigPath);//�����ID:207
	GetPrivateProfileStringA(ProcIndexName, "CombParamID", "",szValue,sizeof(szValue), m_ConfigPath);//����е���״ID: 208_209
	CombShapeNum =  MyFunction::MyGetSubstrNum(szValue,"_");//����:1_2_3�򷵻�3,��ʶ��3����״
	for (int i = 0;i<CombShapeNum;i++)
	{
		strTemp = MyFunction::MyGetSubStr(szValue,i+1,1,"_");
		RoiInfor.stCombine.m_ShapIdList.push_back(atoi(strTemp));
	}
	GetPrivateProfileStringA(ProcIndexName, "CombParamType", "",szValue,sizeof(szValue), m_ConfigPath);
	for (int i = 0;i< MyFunction::MyGetSubstrNum(szValue,"_");i++)
	{
		strTemp =  MyFunction::MyGetSubStr(szValue,i+1,1,"_");      //
		RoiInfor.stCombine.m_CombTypeList.push_back(atoi(strTemp));//����в������� �� �� ��
		m_RoiComIdList.push_back(atoi(strTemp));//���ͷŵ�visionProcBase�ĳ�Աm_RoiComIdList��
	}
	//����Ľ������ �������
	m_ResultType =(ProcResultType)GetPrivateProfileInt(ProcIndexName,"ResultType", 0, m_ConfigPath);



	m_RoiID = ShapeId;
	//manager�����ṩ�ĺ���
	GenShape( GetSceneId(),RoiInfor, HoComposeType,ShapeId,m_CurSceneCamIndex);  //�������������ID���������ͼԪID list����û���ƣ�
	//m_RoiIdList.push_back(ShapeId);
	//��ȡ����ͼԪ��Ϣ������
	//int ShapeNum = GetPrivateProfileInt(ProcIndexName, "RoiShapeNum", -1, m_ConfigPath);
	for (int i = 0;i<CombShapeNum ;i++)
	{
		strTemp.Format("ShapeType%d",i);
		int ShapeType = GetPrivateProfileInt(ProcIndexName,strTemp, -1, m_ConfigPath);
		switch (ShapeType)  //��ȡ������ͼ��
		{
		case HoRecTangleType:
			ReadAndCreateRect(ProcIndexName,i);
			break;
		case HoCircleType:
			ReadAndCreateCircle(ProcIndexName,i);
			break;
		}	

	}

}

void VisionProcBase::SaveProcConfig(int ProcIndex)
{
	CString ProcIndexName, StrTemp,StrTemp1;
	ProcIndexName.Format("%d",ProcIndex);
	StrTemp.Format("%d",m_ProcTypeId);
	WritePrivateProfileStringA(ProcIndexName, "ProcType",StrTemp, m_ConfigPath);
	StrTemp.Format("%d",m_VisonProcId);
	WritePrivateProfileStringA(ProcIndexName, "ProcId",StrTemp, m_ConfigPath);

	StrTemp.Format("%d",m_ResultType);
	WritePrivateProfileStringA(ProcIndexName, "ResultType",StrTemp, m_ConfigPath);
	
	//StrTemp.Format("%d",m_AxisIndex);
	//WritePrivateProfileStringA(ProcIndexName, "AxisIndex",StrTemp, m_ConfigPath);

	//��ROI����Ϣ����
	ProcIndexName.Format("%d",ProcIndex);
	StrTemp.Format("%d",m_RoiID);
	WritePrivateProfileStringA(ProcIndexName,"CombShapeID", StrTemp, m_ConfigPath);
	StrTemp.Empty();
	for (int i = 0;i<m_RoiIdList.size();i++)
	{
		StrTemp1.Format("%d_",m_RoiIdList.at(i));
		StrTemp +=StrTemp1;
	}
	WritePrivateProfileStringA(ProcIndexName,"CombParamID", StrTemp, m_ConfigPath);  

	StrTemp.Empty();
	for (int i = 0;i<m_RoiComIdList.size();i++)
	{
		StrTemp1.Format("%d_",m_RoiComIdList.at(i));
		StrTemp +=StrTemp1;
	}
	WritePrivateProfileStringA(ProcIndexName,"CombParamType", StrTemp, m_ConfigPath);  
   //�������ͼԪ�Ĳ���
	  ST_ROI_INFO RoiParam;
	  for (int i = 0;i<m_RoiIdList.size();i++)
	  {
		  StrTemp1.Format("%d",m_RoiIdList.size());
		  WritePrivateProfileStringA(ProcIndexName,"RoiShapeNum", StrTemp1, m_ConfigPath);
		  StrTemp.Format("ShapeID%d",i);
		  StrTemp1.Format("%d",m_RoiIdList.at(i));
		  WritePrivateProfileStringA(ProcIndexName,StrTemp, StrTemp1, m_ConfigPath);

		  StrTemp.Format("ShapeType%d",i);
		  GetRoiParam(GetSceneId(),m_RoiIdList.at(i),&RoiParam);
		  switch (RoiParam.iType)
		  {
		  case HoRecTangleType:
			  {
				   StrTemp1.Format("%d",HoRecTangleType);
				   WritePrivateProfileStringA(ProcIndexName,StrTemp, StrTemp1, m_ConfigPath);
				   StrTemp.Format("ShapeParam%d_Row",i);
				   StrTemp1.Format("%.6lf",RoiParam.strect.row);
				   WritePrivateProfileStringA(ProcIndexName,StrTemp, StrTemp1, m_ConfigPath);
				   StrTemp.Format("ShapeParam%d_Col",i);
				   StrTemp1.Format("%.6lf",RoiParam.strect.col);
				    WritePrivateProfileStringA(ProcIndexName,StrTemp, StrTemp1, m_ConfigPath);
				   StrTemp.Format("ShapeParam%d_Angle",i);
				   StrTemp1.Format("%.6lf",RoiParam.strect.angle);
				   WritePrivateProfileStringA(ProcIndexName,StrTemp, StrTemp1, m_ConfigPath);
				   StrTemp.Format("ShapeParam%d_Len1",i);
				   StrTemp1.Format("%.6lf",RoiParam.strect.length1);
				   WritePrivateProfileStringA(ProcIndexName,StrTemp, StrTemp1, m_ConfigPath);
				   StrTemp.Format("ShapeParam%d_Len2",i);
				   StrTemp1.Format("%.6lf",RoiParam.strect.length2);
				   WritePrivateProfileStringA(ProcIndexName,StrTemp, StrTemp1, m_ConfigPath);

			  }
		  	break;
		  case HoCircleType:
			  {
				  StrTemp1.Format("%d",HoCircleType);
				  WritePrivateProfileStringA(ProcIndexName,StrTemp, StrTemp1, m_ConfigPath);
				  StrTemp.Format("ShapeParam%d_Row",i);
				  StrTemp1.Format("%.6lf",RoiParam.stcircle.row);
				  WritePrivateProfileStringA(ProcIndexName,StrTemp, StrTemp1, m_ConfigPath);
				  StrTemp.Format("ShapeParam%d_Col",i);
				   StrTemp1.Format("%.6lf",RoiParam.stcircle.col);
				   WritePrivateProfileStringA(ProcIndexName,StrTemp, StrTemp1, m_ConfigPath);
				  StrTemp.Format("ShapeParam%d_Rii",i);
				   StrTemp1.Format("%.6lf",RoiParam.stcircle.Radius);
				   WritePrivateProfileStringA(ProcIndexName,StrTemp, StrTemp1, m_ConfigPath);
			  }
			  break;

		  }
	  }
}


 BOOL VisionProcBase::VisionProcRun(CHECK_RESULT& ProcRes)
 {


	 return TRUE;

 }


//*****************************************   �������� ***************************************//

 ProcList:: ProcList()
 {
	 m_RelativeSceneId = -1;
	 m_NextScenenId = -1;
	 m_NextSceneRotAngle = 0.0;
 }




void ProcList::AddProc(VisionProcBase* pProc)
{
	m_ProcList.Add(pProc);

}
void ProcList:: InsertProc(int Index,VisionProcBase* pProc)
{
	m_ProcList.InsertAt(Index,pProc);
}
void ProcList:: RemoveProc(int Index)
{
	
	if (Index >=m_ProcList.GetCount())
	{
		return ;
	}
	m_ProcList.RemoveAt(Index);
}
void  ProcList::delProc(int Index)
{
	if (Index >=m_ProcList.GetCount())
	{
		return ;
	}

	m_ProcList.GetAt(Index)->DeleteSelf();
	//delete m_ProcList.GetAt(Index);
	RemoveProc(Index);
	
}
int  ProcList::GetProcCount()
{
	return (int)m_ProcList.GetCount();
}

int ProcList::GetProcIndex(int ProcID)
{
	for (int i = 0;i< m_ProcList.GetCount();i++)
	{
		if (ProcID == m_ProcList.GetAt(i)->GetProcId())
		{
			return i;
		}
	}
	return -1;

}

VisionProcBase* ProcList::GetProcByIndex(int Index)
{
	if (Index<0)
	{
		return NULL;
	}
	if (Index<m_ProcList.GetCount())
	{
	     return m_ProcList.GetAt(Index);
	}else
	{
		return NULL;
	}
}


VisionProcBase* ProcList::GetProcByTypeIndex(int ProcTypeID,int TypeIndex)
{
	int Index = -1;
	for (int i = 0;i<m_ProcList.GetSize();i++)
	{
		if (ProcTypeID == m_ProcList.GetAt(i)->m_ProcTypeId)
		{
			Index++;
			if ( Index == TypeIndex)
			{
				return m_ProcList.GetAt(i);
			}
		}

	}
	return NULL;

}

int ProcList::GetSceneID()
{
	return m_SceneID;
}
void ProcList::SetSceneID(int SceneID)
{
	m_SceneID = SceneID;
}

int  ProcList::GetProcType(int Index)
{
	if (Index >=m_ProcList.GetCount())
	{
		return -1;
	}
	return m_ProcList.GetAt(Index)->m_ProcTypeId;
}

int ProcList::GetTypeIndex(int ProcType,void* pProc)
{
	int Index = -1;
	for (int i = 0;i<m_ProcList.GetCount(); i++)
	{
		if (GetProcType(i) == ProcType)//ͬ���͵ĵڼ��� ��ɼ�ͼƬ������,���п�����012
		{
			Index++;
			if (GetProcByIndex(i) ==  pProc)
			{
				return Index;
			}
		}
	}
	return -1;
}

BOOL ProcList::RunSceneSingle(CHECK_RESULT& ProcRes)
{
	int LoopCount = 0 ,LoopBeginIndex = 0,LoopEndIndex = 0;
	for (int i  = 0;i<m_ProcList.GetCount(); i++)
	{
		if (LOGIC_CTR == m_ProcList.GetAt(i)->m_ProcTypeId)
		{
			LoopCount = ((SceneLogic*)m_ProcList.GetAt(i))->GetLoopTimes();
			LoopBeginIndex = ((SceneLogic*)m_ProcList.GetAt(i))->GetLoopBeginIndex();
			LoopEndIndex =  ((SceneLogic*)m_ProcList.GetAt(i))->GetLoopEndIndex();
		}else
		{
			 LoopCount = 1;
             LoopBeginIndex = i;
			 LoopEndIndex = i;
		}

		for (int j = 0;j< LoopCount;j++)
		{
		    for (int k = LoopBeginIndex;k<=LoopEndIndex;k++ )
		    {
				if (!m_ProcList.GetAt(k)->VisionProcRun(ProcRes))
				{
					return FALSE;
				}else
				{
					//ÿһ��ִ�������е�����������ڴ����

				}
				i = k;  //i��ѭ���������λ�ÿ�ʼִ�С�
			}
		}
	}
	return TRUE;

}
int ProcList::GetProcImageIndex(int ProcId)
{
	int ImageIndex = -1;
	for (int i = 0;i<m_ProcList.GetCount();i++)
	{
		if (IMAGE_ACQISITION == m_ProcList.GetAt(i)->m_ProcTypeId) //||IMAGE_READ == m_ProcList.GetAt(i)->m_ProcTypeId
		{
			ImageIndex++;
		}
		if (ProcId == m_ProcList.GetAt(i)->GetProcId()) //���ҵ�����������ͼ��ɼ�����,ֱ���ҵ���ǰ��һ����ҵ���
		{
			break;
		}
	}
	return ImageIndex;
}

 void  ProcList::ReOrderIndex()
 {
	 for (int i = 0;i<m_ProcList.GetCount();i++)
	 {
		 m_ProcList.GetAt(i)->m_ProcNo = i;
	 }
 }

 void ProcList::SetAxeIndex(int Index)
 {
	 m_AxeIndex = Index;
	 
 }

 void ProcList::SetToolIndex(int Index)
 {
	  m_ToolIndex = Index;
 }

 void ProcList::SetRelaSceneID(int RelativeId)
 {
	 m_RelativeSceneId = RelativeId;
 }
 int ProcList::GetRelaSceneID(void)
 {
	 return m_RelativeSceneId;
 }

 void ProcList::SetNextSceneID(int NextSceneId)
 {
	 m_NextScenenId = NextSceneId ;
 }

 int  ProcList::GetNextSceneID(void)
 {
	 return m_NextScenenId;
 }

 void  ProcList::SetNextRotAngle(double RotAngle)
 {
	 m_NextSceneRotAngle = RotAngle;
 }
 double  ProcList::GetNextRotAngle(void)
 {
	 return m_NextSceneRotAngle;
 }



 //*****************************************   ���������� ***************************************//

void SceneList::AddScene(ProcList* pScene)
{
	m_SceneList.Add(pScene);

}

ProcList* SceneList::GetSceneBySceneID(int SceneID)
{

	for (int i = 0;i<m_SceneList.GetCount();i++)
	{
		if ( m_SceneList.GetAt(i)->GetSceneID() == SceneID)
		{
			return  m_SceneList.GetAt(i);
		}
	}
	return NULL;

}

ProcList*  SceneList::GetSceneByIndex(int SceneIndex)
{
	if (SceneIndex >=m_SceneList.GetCount())
	{
		return NULL;
	}
	return m_SceneList.GetAt(SceneIndex);
}



int SceneList::GetSceneCount()
{
	return (int)m_SceneList.GetCount();
}


int SceneList::GetIndexBySceneID(int SceneID)
{
	for (int i = 0;i<m_SceneList.GetCount();i++)
	{
		if ( m_SceneList.GetAt(i)->GetSceneID() == SceneID)
		{
			return i;
		}
	}
	return -1;
}

	

