#include "Camer.h"
#include <string>
#include <list>
#include <hash_map>
#include "iImageCtrl.h"
using namespace std;

class Camer;
class  TCamera
{
public:
		TCamera(void);

        static TCamera* GetInstance();
        static void ReleaseInstance();
		static TCamera *m_this;

		//��ʼ��,����ͼ��ɼ���
		virtual int Init(int dllnum, string dlllist[]); 

		//�ͷ�ͼ��ɼ�����Դ
		virtual int Deinit();	

		// ��ȡ���guid�б�
		virtual int GetGUIDList(list<string> &guidlist,string &type);

		//��ȡ�������
		virtual int GetParameInfo(string name,CAMARA_PARA_ITEM speciPara,unsigned long &info);

		//�����������
		virtual int SetParameInfo(string name,CAMARA_PARA_ITEM speciPara,int val);

		//�����
		virtual int Open (string name );

		//�ر����
		virtual int Close (string name);

		//��ȡ���ͼ���С
		virtual int GetImageSize(string name ,int &w, int &h);

		//��ȡ���ͼ��
		virtual int GetImage(string name, void *pImage);

		// �����������
        virtual int SetGain(string name,unsigned int value); 
        virtual int GetGain(string name); 

		// ��������ع�ʱ��
        virtual int SetExposureTime(string name,unsigned int value);
        // ��ȡ����ع�ʱ��
        virtual int GetExposureTime(string name);

		//��ʼʵʱȡ��
		virtual int StartSnap(string name,void *pImage,void(*Callback)(void* context),void *pParent);

		//ֹͣʵʱȡ��
		virtual int StopSnap(string name);

private:
	//��ȡ�������
	Camer *GetCamer(string name);

private:
	hash_map<string,Camer*> m_hm_camera;
	hash_map<string,Camer*>::iterator m_it_camera;
	int m_iresult;
	CAMARA_ADN_CONNECT_TYPE m_en_camaratype;


};