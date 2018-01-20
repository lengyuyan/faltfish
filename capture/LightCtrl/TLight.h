#include <string>
#include <hash_map>
using namespace std;
#include "SerialPort.h"
#include "iLightCtrl.h"
#include "sysapi.h"

#define RETURN_NULLPTR(p,v) if (NULL == p) \
{	\
	return v; \
}


enum CTR_TYPE
{
	LIGHT_OFF = 0,
	LIGHT_OPEN = 1,	
	LIGHT_SET = 2,
};

typedef struct lightpara
{
	unsigned int channalVal[8];//һ����Դ���������8��ͨ��
	LIGHTCTL_TYPE lighttype;

    lightpara()
    {
        memset(channalVal, 0, sizeof(channalVal));
        lighttype = LIGHTCTL_TYPE_NDPS;
    }

    lightpara operator=(lightpara pa)
    {
        memcpy(channalVal, pa.channalVal, sizeof(channalVal));
        this->lighttype = pa.lighttype;
        return *this;
    }
}LightPara;


class  TLight
{
public:
    TLight();
    ~TLight();

    static TLight* getInstance();
    static void destroyInstance();
	static TLight* m_this;

	//��ʼ����Դ������,name:�������б�
	virtual int Init(LightInfo seriaInfo[],int cout, LightTypeInfo *pType, int iTypeCount = 1) ; 

	//�ͷŹ�Դ��������Դ
	virtual int DeInit(string comName);

	//��ָ��Ƶ����Դ
	virtual int Open(string comName,int index);

	//�ر�ָ��Ƶ����Դ
	virtual int Close(string comName,int index);

	//����ָ��Ƶ����Դ����
	virtual int SetLightValue(string comName,int index ,unsigned int value);

	//��ȡָ��Ƶ����Դ����ֵ
	virtual int GetLightValue(string comName,int index ,unsigned int &val);

    //�жϹ�Դ�����������Ƿ�ɹ�
    virtual int JudgeLightConnect(string comName);
private:
	//����ӹ�����
	int CommandProcess(int index ,CTR_TYPE ctrtype,unsigned int lvalue);

	//�������ָ�����ڣ�comName����������comm������
	int SendCommd(string comName,string comm);

	//��ȡcom����
	int GetComer(string name);
	string ChangeCharstr2Hexstr(string Charstr);
	void HexStringFilter(string &str);
	char CombineHexChar(char CharH,char CharL);
	string DevideHexChar(char HexChar);

	LIGHTCTL_TYPE m_LightType;//��Դ����
	hash_map<string,SerialPort *> *m_hs_NameToCom;//�����б�
	hash_map<string,SerialPort *>::iterator m_hs_NamToCom_it;
	hash_map<string,LightPara> *m_hs_NameToLightPa;//�������Դ������ϵ��
	hash_map<string,LightPara>::iterator m_hs_NamToLightP_it;
	int m_i_result;			//�����ķ��ؽ��

	string m_command;       //��������

	SerialPort *m_serialp;
	SerialPortInfo *m_serialinfo;
	LightPara *m_lightp;

	void testmyself();

	HANDLE  m_Section;
};