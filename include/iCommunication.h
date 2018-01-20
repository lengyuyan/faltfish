#ifndef _I_COMMUNICATION_H_
#define _I_COMMUNICATION_H_
#include "DataType.h"

#ifndef EXPORT_C
#define EXPORT_C extern "C"
#endif
#ifndef DLL_EXPORT
#define DLL_EXPORT _declspec(dllexport)
#endif

typedef  bool (*pFunCallback)(int index, int ires);
typedef  void (*pFunCloseSys)(int iType, void *param);

/**********************************************************************
����      : InitConnect
����      : ��ʼ������
�������  : pCount ��ʼ��������
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : N/A
**********************************************************************/
EXPORT_C DLL_EXPORT int InitConnect(void *pCount);

/**********************************************************************
����      : InitConnect
����      : ��ʼ������
�������  : pCount ��ʼ��������
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : N/A
**********************************************************************/
EXPORT_C DLL_EXPORT int UnInitConnect();

EXPORT_C DLL_EXPORT void ReleaseConnectInstance();
/**********************************************************************
����      : RegesterCallBack
����      : ע�ᴥ���ӿ�
�������  : �ص�����
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : N/A
**********************************************************************/
EXPORT_C DLL_EXPORT bool RegesterCallBack(pFunCallback pfun);

/**********************************************************************
����      : RegesterCloseSys
����      : ֪ͨ���̹ر�
�������  : �ص�����
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : N/A
**********************************************************************/
EXPORT_C DLL_EXPORT bool RegesterCloseSys(pFunCloseSys pfun);


/**********************************************************************
����      : 
����      : ͼ��ɼ����
�������  : iSceneID ����ID
iErrorCode ������
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : N/A
**********************************************************************/
EXPORT_C DLL_EXPORT int GrabFinished(int iSceneID,CHECK_RESULT *pResult, int iErrorCode);

/**********************************************************************
����      : getImagePathFinished
����      : ͼ��ɼ����
�������  : iSceneID ����ID
			path  ͼƬ·��
            iErrorCode ������
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
�޸ļ�¼��
����		�޸���       �޸�����
2016-08-16 guopengzhou   ���ͼƬ·��
**********************************************************************/
EXPORT_C DLL_EXPORT int getImagePathFinished(int iSceneID, char* path,int iErrorCode = 0);

/**********************************************************************
����      : ProcessFinished
����      : �������
�������  : pResult ������
            iErrorCode ������
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT int CommunicationProcessFinished(int iSceneID, CHECK_RESULT *pResult, int iErrorCode = 0);

/**********************************************************************
����      : ProcessFinished
����      : �������
�������  : pResult ������
            iErrorCode ������
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : 
**********************************************************************/
EXPORT_C DLL_EXPORT int SendImage(int iSceneID, void *pBuffer, int size,int w,int h,int iErrorCode = 0);
/**********************************************************************
����      : OpenLight
����      : ��Դ����
�������  : index ��Դ����
            type  ��������, 1Ϊ���� 0Ϊ��
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : ��Դͨ��IO�ڲ����� IO�ڶ�ռ
**********************************************************************/
EXPORT_C DLL_EXPORT bool OpenLight(int index, int type);
/**********************************************************************
����      : ResetFinished
����      : ����ϵͳ������Ϣ
�������  : iSceneID ����ID
            iErrorCode ������
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : N/A
**********************************************************************/
EXPORT_C DLL_EXPORT int ResetFinished(int iErrorCode = 0);

/**********************************************************************
����      : PantCheckSend
����      : ������ⷢ�����ݺ���
�������  :  
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : N/A
**********************************************************************/
EXPORT_C DLL_EXPORT int PantCheckSend();


EXPORT_C DLL_EXPORT void SetCommuniWebProctocol(const PROCTOCOLDATA& ProctoData);
EXPORT_C DLL_EXPORT void GetCommuniWebProctocol(PROCTOCOLDATA& ProctoData);


#endif 
