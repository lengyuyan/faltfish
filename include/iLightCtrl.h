#include "common.h"
#include "DataType.h"
#ifndef _I_LIGHT_CTRL_H_
#define _I_LIGHT_CTRL_H_

#ifndef OUTTYPE_H
#define OUTTYPE_H extern "C" _declspec(dllexport)
#endif OUTTYPE_H

enum OPER_LIGHT_TYPE
{
    TYPE_OPEN = 0,         //�򿪹�Դ
    TYPE_CLOSE,        //�رչ�Դ
    TYPE_SET_VAULE,    //���ù�Դ����
    TYPE_RESET,    //���ô���
};

/**********************************************************************
����      : iLightInstance
����      : ��ʼ����Դ������
�������  : seriaInfo:��Դ��������Ϣ��
�������  : N/A
����ֵ    : LIGHTCTL_ERRORCODE ������
��ע      : 
**********************************************************************/
OUTTYPE_H int iLightInstance(LightInfo seriaInfo[],int cout, LightTypeInfo *pType = NULL, int iTypeCount = 1) ; 

/**********************************************************************
����      : destroyLightInstance
����      : ȥ��ʼ����Դ������
�������  : N/A
�������  : N/A
����ֵ    : N/A
��ע      : 
**********************************************************************/
OUTTYPE_H void destroyLightInstance() ; 

/**********************************************************************
����      : initLightCtrl
����      : �ͷŹ�Դ��������Դ
�������  : comName:������
�������  : N/A
����ֵ    : LIGHTCTL_ERRORCODE ������
��ע      : 
**********************************************************************/
OUTTYPE_H int deInitLightCtrl(string comName);

/**********************************************************************
����      : openLight
����      : ��ָ��Ƶ����Դ
�������  : comName:������;index,ָ���Ĺ�Դͨ��1��ʼ;
�������  : N/A
����ֵ    : LIGHTCTL_ERRORCODE ������
��ע      : 
**********************************************************************/
OUTTYPE_H int openLight(string comName,int index);

/**********************************************************************
����      : closeLight
����      : �ر�ָ��Ƶ����Դ
�������  : comName:������;index,ָ���Ĺ�Դͨ��1��ʼ;
�������  : N/A
����ֵ    : LIGHTCTL_ERRORCODE ������
��ע      : 
**********************************************************************/
OUTTYPE_H int closeLight(string comName,int index);

/**********************************************************************
����      : setLightValue
����      : ����ָ��Ƶ����Դ����
�������  : comName:������;index,ָ���Ĺ�Դͨ��1��ʼ;value,����ֵ;
�������  : N/A
����ֵ    : LIGHTCTL_ERRORCODE ������
��ע      : 
**********************************************************************/
OUTTYPE_H int setLightValue(string comName,int index ,unsigned int value);

/**********************************************************************
����      : getLightValue
����      : ��ȡָ��Ƶ����Դ����ֵ
�������  : comName:������;index,ָ���Ĺ�Դͨ��1��ʼ;value,����ֵ;
�������  : N/A
����ֵ    : LIGHTCTL_ERRORCODE ������
��ע      : 
**********************************************************************/
OUTTYPE_H int getLightValue(string comName,int index ,unsigned int &val);
/**********************************************************************
����      : JudgeLightConnect
����      : �жϹ�Դ�������Ƿ����ӳɹ�
�������  : ��������
�������  : N/A
����ֵ    : LIGHTCTL_ERRORCODE ������
��ע      : 
**********************************************************************/
OUTTYPE_H int JudgeLightConnect(string comName);

#endif