#pragma once

/*
 *��������Ӿ�������ַ����Ļ��࣬������ �Ӿ�����ģ�鶼�̳����������������̬�⣬���ڱ������У�����Ϊ�������ṩ�����Ĵ���ӿڣ��ӿ��������
 *��ͼ���Լ����ֿɵ���������������Ǵ����Ľ�����������ꡢ�������֡��ȡ�
 *
 **/
class  VisionProcBase
{
public:
	VisionProcBase(void);
	~VisionProcBase(void);
public:
    //�ӿں���
	virtual void VisionProc(...);
	virtual VisionProcBase* CreateProcInstance(char* szName);

};

