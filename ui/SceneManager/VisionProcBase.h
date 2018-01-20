#pragma once


#include "DataType.h"
#include "iManagerCtrl.h"
#include "MyFunction.h"
#define  SCENE_PROC_ID_FACTOR 10000
/*
 *��������Ӿ�������ַ����Ļ��࣬������ �Ӿ�����ģ�鶼�̳����������������̬�⣬���ڱ������У�����Ϊ�������ṩ�����Ĵ���ӿڣ��ӿ��������
 *��ͼ���Լ����ֿɵ���������������Ǵ����Ľ�����������ꡢ�������֡��ȡ�
 *
 **/
class ProcList;

class  VisionProcBase
{
public:
	VisionProcBase(void);
	VisionProcBase(CString ConfigPath,int ProcID);
	~VisionProcBase(void);
public:
	enum ProcResultType
	{
		//�ֳ��õ��Ǳ궨��������,�����Ƕȵ�������꣬ͼ������
		NOMAL_RESULT=0, //
		REAL_UNIT_RESULT =1,//ʹ�û�е��λ��ͬʱ����ǶȺ�λ��ƫ��
		NO_MATCH_ROT = 2,   //ƥ��󲻴�����ת��ֻ����λ��ƫ��
		MODLE_DISTENCE,    //��������
		TEST_REPOSITION,   //�����ظ���λ  IMAGE_RELA   ͼ������궨���������
		IMAGE_ABS,       //ͼ������궨����������
	};

public:
    //�ӿں���
	virtual void VisionProc(...);									//��ʱû��
	virtual void BuildInstance(void) ;								// ֻ���������������У�����ʼ������ ���˺���Ӧ�ÿ��Բ�Ϊ�麯����
	virtual void InitInstance(CWnd* pParentWnd);					//�������󣬲���ʼ�����棬�����˻�������
	virtual BOOL IsInitialConfigWnd();								//�Ƿ��ǳ�ʼ������,�и�ȫ��g_SceneInitialCount����Աm_SceneInitialCount�����Ⱦ��ǳ�ʼ����
	virtual void HideConfigWnd()=0;									//�ж��Ƿ��������ô���
	virtual void ShowConfigWnd() = 0;								//�ж��Ƿ���ʾ�����Ӧ����������ô���
	virtual void SaveProcConfig(int ProcIndex);						//����ô������������ļ���ProcIndex����λ��
	virtual void ReadProcConfig(int ProcIndex);						//��ȡ����
	virtual BOOL VisionProcRun(CHECK_RESULT& ProcRes);				//ÿ��������ĺ������к���
	CString m_ConfigPath;											//�������·��
	void ReadAndCreateRect(CString ProcIndexName,int ShapeIndex);	//��������һ������
	void ReadAndCreateCircle(CString ProcIndexName,int ShapeIndex); //ProcIndexName��ʶ�ڼ���������Ŀ,��������һ��Բ
	int GetProcIdBase();											//��ȡ����������
	int GetProcId();												//��ȡ������ID
	int GetProcTypeID();											//������������
	virtual void DeleteSelf();										//ɾ�������౾����
	int GetSceneId();												//��������ID/10000;
	int GetProcIndexById(int ProcID);								//����ID����ȡ����
	int GetProcTypeIndex();											//����ID�ҵ���������,Ȼ����������ҵ���������ͬ�����е�����
	ProcList* GetCurScene();										//�ҵ��������ڵĴ����
	void AfterCreateComb();											//���ͼԪid�б� ���id�б�,���Է���ģ������
	CWnd* m_pParentWnd;												//������������ϵ����ÿ�,�ϷŴ�����Ŀ��Ӧ��������
	PROCESS_TYPE m_ProcTypeId;										//��������ͣ�
    int m_VisonProcId;												// �Ӿ�������ı�ʶ ID = ������*10000+��������;
	int m_ProcNo;													//�������ڶ�̬����ɾʱ��̫��ά����ֻ�����ڶ�ȡ�����ļ���ʱ�����������Զ��ʹ�øó�Ա������
																	//���Ҫʹ�ã���һ��Ҫ���κθĶ�������ɾ���ƣ�ʱ���������������д���ģ�����¸���:ReOrderIndex()�����ʹ�á�	                 
    static int VisionProcIdCount;									//ID����������δ�õ�;
	int m_RoiID;													//������Ψһ��ROI��ID     ��id
	vector<int> m_RoiIdList;										//ROI��ɵĻ���ͼԪ��ID  List   ��ͼԪid
	vector<int> m_RoiComIdList;										//ROI����ͼԪ����Ϸ���ID list; ����
	//vector<int> m_ShapeIdList;									//����ͼԪ��ID������һЩ��Ҫ�ĳ��ϡ�
	int m_SceneInitialCount;										//�ж��Ƿ�򿪳�������
	ProcResultType m_ResultType;									//ȷ�������� ���Ҫ����ģ������ȥ��ģ�崦��ʱҪ���͵���������
protected:
	int m_CurSceneCamIndex; //���Ҫ���ڲɼ�������ȥ
};



//  **************************  ��������  ******************//

class ProcList
{
public:
  ProcList();
public:	
	void AddProc(VisionProcBase* pProc);							//���һ��������
	void InsertProc(int Index,VisionProcBase* pProc);				//��ָ������֮�����һ�������� 
	void RemoveProc(int Index);										//�Ƴ�ָ���Ĵ�����,���ǲ���delete��������ڴ漰���������ȣ�����
	void delProc(int Index);										//ɾ��ָ�������༰����
	int GetProcCount();												//��ȡ����������
	int GetProcIndex(int ProcID);									//��������һ��ID=������*10000+�������е�λ��(��0��ʼ)��������ݴ�����id����ȡ���ڴ����е�����,����-1
	int GetProcType(int Index);										//������������ȡ��������������,��ɼ�,ģ��
	VisionProcBase* GetProcByIndex(int Index);						//������������ȡ������
	VisionProcBase* GetProcByTypeIndex(int ProcTypeID,int TypeIndex);//��ȡ��Ӧ���͵Ķ�Ӧ����������
	int GetSceneID();												//ÿ�����������һ��������
	void SetSceneID(int SceneID);									//���ó�����
	BOOL RunSceneSingle(CHECK_RESULT& ProcRes);						//���ĺ���,���г������е����д�����,�ɹ�����ture,ʧ�ܷ���false
	int GetTypeIndex(int ProcType,void* pProc);						//��ȡָ�����ͺ�ָ��������ָ�� �ڴ�����е�����
	int GetProcImageIndex(int ProcId);								//��ȡָ��������IDǰ������������ɼ��������
    void ReOrderIndex();											//���� ÿ���������������һ�����,��ɾ��һ���,�����е�����Ҫ�仯
	void SetAxeIndex(int Index);									//����
	void SetToolIndex(int Index);									//����
	void SetRelaSceneID(int RelativeId);							//���� 	������س�����ID
	int GetRelaSceneID(void);										//���� 	��ȡ��س�����ID
	void SetNextSceneID(int NextSceneId);							//���� 	�����¸�������ID
	int GetNextSceneID(void);										//���� 	��ȡ�¸�������ID
	void SetNextRotAngle(double RotAngle);							//���� 	���ó����ĽǶ�
	double GetNextRotAngle(void);									//���� 	��ȡ�����Ƕ�

private:
	CArray <VisionProcBase*> m_ProcList; 
	int m_SceneID;
	int m_AxeIndex;
	int m_ToolIndex;
	int m_RelativeSceneId; 
	int m_NextScenenId;    
	double m_NextSceneRotAngle;
};


//***********************   ���������� *********************//

class SceneList
{
public:
	void AddScene(ProcList* pScene);					//���ӳ�����ȫ�ֱ�
	int GetIndexBySceneID(int SceneID);					//��ȡ��Ӧ����id�Ĵ������ȫ�ֱ��е�����
	ProcList* GetSceneBySceneID(int SceneID);			//��ȡ��Ӧ����id��Ӧ�����
	ProcList* GetSceneByIndex(int SceneIndex);			//��ȡ��Ӧ�����Ĵ����
	int GetSceneCount();								//��ȡ����
private:
	CArray<ProcList*> m_SceneList;


};

