#include "common.h"
#include "vision_api.h"
#include "manage.h"
#include "cvision.h"

#include "GolobalError.h"

const char* g_genvisionDesc[] = {
    "������IP:",
    "�˿�:",
    "��ʶ(S/C):",
};

/***********************************************
    *Function: ��ȡ�Ӿ������Ϣ����
    *Intput:   pInfo �Ӿ������Ϣ�ṹ��
    *Output:   pInfo �Ӿ������Ϣ������
    *Return:   �ο�EM_ERR_CODE
*********************************************/
EXPORT_C int WINAPI get_vision_info(TVisionPluginInfo* pInfo)
{
	RETURN_CHK(pInfo, ERR_INPUT_PARAM);

	memset(pInfo, 0, sizeof(TVisionPluginInfo));    //��Щ��ϢҪ�������ļ����һ��
	strcpy_s(pInfo->id, "genvision");               //ע��idΪ���id
	strcpy_s(pInfo->desc, "ͨ���Ӿ�");
    pInfo->paramCnt = sizeof(g_genvisionDesc) / sizeof(char*);
	for (int i = 0; i < pInfo->paramCnt; i++)
	{
		strcpy_s(pInfo->initParamDesc[i], g_genvisionDesc[i]);
	}
	return 0;
}

/***********************************************
    *Function: ��ʼ��һ���Ӿ�����
    *Intput:   visionID �Ӿ�����,��̬�ۼ�,��0��ʼ
               param ��ʼ������ ���֧��8������
    *Output:   NULL
    *Return:   �ο�EM_ERR_CODE
*********************************************/
EXPORT_C int WINAPI init_vision(unsigned short visionID, TVisionInitParam* param)
{
    return INSTANCE->InitVision(visionID, param);
}

/***********************************************
    *Function: �ͷ�һ���Ӿ�����
    *Intput: visionID �Ӿ�����
    *Output: ��
    *Return: �ο�EM_ERR_CODE
    *********************************************/
EXPORT_C int WINAPI uninit_vision(unsigned short visionID)
{
	return INSTANCE->UnInitVision(visionID);
}

/****************************************************
    *Function: ��ȡһ���Ӿ����������״̬
    *Intput:  visionID �Ӿ�����
    *Return: EMVision_Status
******************************************************/
EXPORT_C int WINAPI vision_status(unsigned short visionID)
{
	return INSTANCE->GetVisionStatus(visionID);
}

/***********************************************
    *Function: ��ָ���Ӿ��������Զ���������ܽӿڣ�
    *Intput: visionID �Ӿ�����
             cmd ָ��
		     msg ָ����� ����Ϊ��
    *Output: NULL
    *Return: EM_ERR_CODE
*********************************************/
EXPORT_C int WINAPI send_custom_cmd(unsigned short visionID, const char* cmd, const char* msg/* = 0*/)
{
	return INSTANCE->SendCustomCmd(visionID,cmd, msg);
}

/***********************************************
    *Function: ����ָ���Ӿ������Զ�����Ϣ
    *Intput: visionID �Ӿ�����
             cmd ָ��
		     recvMsg���������ݻ�����,����Ϊ��
             timeout ��ʱ���� Ĭ�ϲ��ȴ�
    *Output: recvMsg �����ֵ
    *Return: EMCARD_STATUS ��ʱ����ERR_TIMEOUT
*********************************************/
EXPORT_C int WINAPI recv_custom_cmd(unsigned short visionID, const char* cmd, char* recvMsg/* = 0*/, int timeout/* = 0*/)
{
	return INSTANCE->RecvCustomCmd(visionID, cmd, recvMsg, timeout);
}

/****************************************************
    *Function: �Ӿ�����ָ�������ɷ���
    *Intput:  visionID  �Ӿ�����
              secne     ����ID��������Ӿ�ͨѶЭ�鴫��
              timeout   ��ʱ����
    *Output:  NULL
    *Return: EM_ERR_CODE
******************************************************/
EXPORT_C int WINAPI vision_process(unsigned short visionID, int secne)
{
	return INSTANCE->VisionProcess(visionID,secne);
}

/****************************************************
    *Function: ��ȡ�Ӿ�������
    *Intput:  visionID �Ӿ�����
              secne    ����ID��������Ӿ�ͨѶЭ�鴫��
              timeout  ��ʱ���� -1���ߵȴ�
    *Output:  pres    ���ؽ��
    *Return: EM_ERR_CODE ��ʱ����ERR_TIMEOUT
******************************************************/
EXPORT_C int WINAPI recv_process_result(unsigned short visionID, int secne, TVisionResult* pres, int timeout)
{
	return INSTANCE->RecvProcessResult(visionID,secne,pres,timeout);
}

/****************************************************
    *Function: ��ȡ�Ӿ�����ͼ��������
    *Intput:  visionID �Ӿ�����
              secne    ����ID��������Ӿ�ͨѶЭ�鴫��
              pdata ���ȴ�NULL��ȡ��������С��new�ڴ���ٴε���
    *Output:  pdata    ͼ�񻺳���
    *Return: �ɹ�������������С��ʧ�ܷ��ش����� < 0
******************************************************/
EXPORT_C int WINAPI get_process_image(unsigned short visionID, int secne, char* pdata)
{
    return ERR_INPUT_PARAM;
}

/****************************************************
    *Function: ��ȡ�Ӿ�����ͼ��·��
    *Intput:  visionID �Ӿ�����
              secne    ����ID��������Ӿ�ͨѶЭ�鴫��
    *Output:  path    ͼ��·�� MAX_PATH
    *Return: EM_ERR_CODE
******************************************************/
EXPORT_C int WINAPI get_process_image_path(unsigned short visionID, int secne, char* path)
{
    return ERR_INPUT_PARAM;
}

/****************************************************
    *Function: �����Ӿ��궨���ݸ��Ӿ����󣨻�е���꣩
    *Intput:  visionID  �Ӿ�����
              secne     �궨����ID��������Ӿ�ͨѶЭ�鴫��
              x��y  ��е����
    *Output:  NULL
    *Return: EM_ERR_CODE
******************************************************/
EXPORT_C int WINAPI send_calib_data(unsigned short visionID, int secne, double x, double y)
{
	return INSTANCE->SendCalibData(visionID, secne, x, y);
}

/****************************************************
    *Function: ���Ӿ���������Ӿ��궨���ݣ����أ�,�Լ��������ϵ
    *Intput:  visionID  �Ӿ�����
              secne     �궨����ID��������Ӿ�ͨѶЭ�鴫��
              x��y  ��е����
    *Output:  NULL
    *Return: EM_ERR_CODE
******************************************************/
EXPORT_C int WINAPI recv_calib_data(unsigned short visionID, int secne, double x, double y)
{
    return ERR_INPUT_PARAM;
}

/****************************************************
    *Function: �Ѵ��Ӿ����յ��Ľ��ʹ�ñ궨���ݽ���ת��
    *Intput:  visionID  �Ӿ�����
              secne     �궨����ID���궨ʱ�õ��ĸ�����
    *Output:  NULL
    *Return: EM_ERR_CODE
******************************************************/
EXPORT_C int WINAPI convert_calib_data(unsigned short visionID, int secne, TVisionResult* pres)
{
	return ERR_INPUT_PARAM;
}