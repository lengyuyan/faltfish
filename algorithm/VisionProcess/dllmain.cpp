// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
//#include "stdafx.h"
//#include "VisionServer.h"
//
//
///**********************************************************************
//����      : InitVision
//����      : ��ʼ������
//�������  : N/A
//�������  : N/A
//����ֵ    : �ɹ���ʧ��
//��ע      : 
//**********************************************************************/
//bool InitVision(void)
//{
//    return true;
//}
//
///**********************************************************************
//����      : OpenWindow
//����      : �󶨴���
//�������  : winID ����ID
//            x,y,width,hight, ����λ�ü����
//�������  : halconWinID halcon����ID
//����ֵ    : �ɹ���ʧ��
//��ע      : 
//**********************************************************************/
//bool OpenWindow(long winID, int x, int y, int width, int hight, long &halconWinID)
//{
//    open_window(y, x,width, hight, winID, "visible", "", &halconWinID);
//    return true;
//}
//
///**********************************************************************
//����      : OpenWindow
//����      : �رմ���
//�������  : halconWinID halcon����ID
//�������  : N/A
//����ֵ    : �ɹ���ʧ��
//��ע      : 
//**********************************************************************/
//bool /*__stdcall*/ CloseHalconWindow(long halconWinID)
//{
//    close_window(halconWinID);
//    return true;
//}
///**********************************************************************
//����      : ReadImageFile
//����      : �󶨴���
//�������  : pFileName ͼƬ·��
//            sceneID   ����ID
//�������  : N/A
//����ֵ    : �ɹ���ʧ��
//��ע      : 
//**********************************************************************/
//bool ReadImage(const char *pFileName, int sceneID, int index)
//{
//    CVisionProcess *pVision = CVisionServer::GetInstance(sceneID);
//    pVision->ReadImage(pFileName, index);
//    
//    return true;
//}
//
///**********************************************************************
//����      : DispImage
//����      : �󶨴���
//�������  : iWinIndex ��ʾ��Ŀ�괰��
//            sceneID   ����ID
//�������  : N/A
//����ֵ    : �ɹ���ʧ��
//��ע      : 
//**********************************************************************/
//bool DispVisionImage(int iWinIndex, int sceneID)
//{
//    CVisionProcess *pVision = CVisionServer::GetInstance(sceneID);
//    pVision->DispImage(iWinIndex);
//    return true;
//}
//
//bool DispObj(int iWinIndex, int sceneID, bool bIsShowTmp)
//{
//    CVisionProcess *pVision = CVisionServer::GetInstance(sceneID);
//    pVision->DispObj(iWinIndex, bIsShowTmp);
//
//    return true;
//}
///**********************************************************************
//����      : VisionProcess
//����      : ��ʼ������
//�������  : N/A
//�������  : N/A
//����ֵ    : �ɹ���ʧ��
//��ע      : 
//**********************************************************************/
//bool VisionProcess(void)
//{
//    return true;
//}
//
// 