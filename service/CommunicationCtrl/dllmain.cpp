// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include "iCommunication.h"
#include "NetworkCtrl.h"

//BOOL APIENTRY DllMain( HMODULE hModule,
//                       DWORD  ul_reason_for_call,
//                       LPVOID lpReserved
//					 )
//{
//	switch (ul_reason_for_call)
//	{
//	case DLL_PROCESS_ATTACH:
//	case DLL_THREAD_ATTACH:
//	case DLL_THREAD_DETACH:
//	case DLL_PROCESS_DETACH:
//		break;
//	}
//	return TRUE;
//}

/**********************************************************************
����      : InitConnect
����      : ��ʼ������
�������  : pCount ��ʼ��������
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : SDK��ں���
**********************************************************************/
int InitConnect(void *pCount)
{
    int iRet = 0;
    SOCKET_PARAM *pParam = NULL;
    CNetworkCtrl *pInstance = NULL;
    if (NULL == pCount)
    {
        return -1;
    }
    pParam = (SOCKET_PARAM *)pCount;
    pInstance = CNetworkCtrl::GetInstance(pParam->strServerIPAdd.c_str());
    if (NULL != pInstance)
    {
        iRet = pInstance->InitConnect(pParam);
    }
    return iRet;
}
void ReleaseConnectInstance()
{
    CNetworkCtrl::ReleaseInstance(NULL);
}

/**********************************************************************
����      : UnInitConnect
����      : ��ʼ������
�������  : pCount ��ʼ��������
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : N/A
**********************************************************************/
int UnInitConnect(void)
{
    int iRet = 0;
    return iRet;
}
/**********************************************************************
����      : RegesterCallBack
����      : ע�ᴥ���ӿ�
�������  : �ص�����
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : N/A
**********************************************************************/
bool RegesterCallBack(pFunCallback pfun)
{
    CNetworkCtrl *pInstance = NULL;
    pInstance = CNetworkCtrl::GetInstance();
    if (NULL != pInstance)
    {
        pInstance->RegesterCallBack(pfun);
    }
    return true;
}
/**********************************************************************
����      : RegesterCloseSys
����      : ֪ͨ���̹ر�
�������  : �ص�����
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : N/A
**********************************************************************/
bool RegesterCloseSys(pFunCloseSys pfun)
{
    CNetworkCtrl *pInstance = NULL;
    pInstance = CNetworkCtrl::GetInstance();
    if (NULL != pInstance)
    {
        pInstance->RegesterCloseSys(pfun);
    }
    return true;
}

int GrabFinished(int iSceneID,CHECK_RESULT *pResult, int iErrorCode)
{
    int iRet = 0;
    CNetworkCtrl *pInstance = NULL;
    pInstance = CNetworkCtrl::GetInstance();
    if (NULL != pInstance)
    {
        iRet = pInstance->GrabFinished(iSceneID, pResult,iErrorCode);
    }
    return iRet;
}

int getImagePathFinished(int iSceneID, char* path,int iErrorCode)
{
    int iRet = 0;
    CNetworkCtrl *pInstance = NULL;
    pInstance = CNetworkCtrl::GetInstance();
    if (NULL != pInstance)
    {
        iRet = pInstance->getImagePathFinished(iSceneID, path,iErrorCode);
    }
    return iRet;
}
int CommunicationProcessFinished(int iSceneID, CHECK_RESULT *pResult, int iErrorCode)
{
    int iRet = 0;
    CNetworkCtrl *pInstance = NULL;
    pInstance = CNetworkCtrl::GetInstance();
    if (NULL != pInstance)
    {
        iRet = pInstance->ProcessFinished(iSceneID, pResult, iErrorCode);
    }
    return iRet;
}
  /**********************************************************************
    ����      : SendImage
    ����      : ����ͼ��
    �������  : pResult ������
                iErrorCode ������
    �������  : N/A
    ����ֵ    : �ɹ���ʧ��
    ��ע      : 
  **********************************************************************/

EXPORT_C DLL_EXPORT int SendImage( int iSceneID, void *pBuffer, int size,int w,int h,int iErrorCode /*= 0*/ )
{
	int iRet = 0;
	CNetworkCtrl *pInstance = NULL;
	pInstance = CNetworkCtrl::GetInstance();
	if (NULL != pInstance)
	{
		iRet = pInstance->SendImage(iSceneID,pBuffer,size,w,h,iErrorCode);
	}
	return iRet;
}

/**********************************************************************
����      : OpenLight
����      : ��Դ����
�������  : index ��Դ����
            type  ��������
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : ��Դͨ��IO�ڲ����� IO�ڶ�ռ
**********************************************************************/
bool OpenLight(int index, int type)
{
    bool bRet = false;
    CNetworkCtrl *pInstance = NULL;
    pInstance = CNetworkCtrl::GetInstance();
    if (NULL != pInstance)
    {
        bRet = pInstance->OpenLight(index, type);
    }
    return bRet;
}
/**********************************************************************
����      : ResetFinished
����      : ����ϵͳ������Ϣ
�������  :  iErrorCode ������
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : N/A
**********************************************************************/
int ResetFinished(int iErrorCode)
{
    int iRet = 0;
    CNetworkCtrl *pInstance = NULL;
    pInstance = CNetworkCtrl::GetInstance();
    if (NULL != pInstance)
    {
        iRet = pInstance->ResetFinished( iErrorCode);
    }
    return iRet;
}
/**********************************************************************
����      : PantCheckSend
����      : ������ⷢ�����ݺ���
�������  :  
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : N/A
**********************************************************************/
int PantCheckSend()
{
    int iRet = 0;
    CNetworkCtrl *pInstance = NULL;
    pInstance = CNetworkCtrl::GetInstance();
    if (NULL != pInstance)
    {
        iRet = pInstance->PantCheckSend();
    }
    else
    {
        iRet = 1;  //ϵͳ������
    }
    return iRet;
}

void SetCommuniWebProctocol(const PROCTOCOLDATA& ProctoData)
{
	CNetworkCtrl *pInstance = NULL;
	pInstance = CNetworkCtrl::GetInstance();
	if (NULL != pInstance)
	{
		pInstance->SetCommuniWebProctocol(ProctoData);
	}
}

void GetCommuniWebProctocol( PROCTOCOLDATA& ProctoData)
{
	CNetworkCtrl *pInstance = NULL;
	pInstance = CNetworkCtrl::GetInstance();
	if (NULL != pInstance)
	{
		pInstance->GetCommuniWebProctocol(ProctoData);
	}
}




