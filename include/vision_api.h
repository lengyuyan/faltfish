/*********************************************************************
    *�ļ�˵��: �Ӿ�����ӿڣ������Ӿ����ݲ����������˽ӿ��ĵ�
    *����: �޾�
    *����: 2017/02/07
    *�޸���־:  2017/02/07 CW4945 ����

    ***ע������***
    1. ���ز��ʱ����ӿڣ����ӿڼ�ⲻͨ�����������ػ�ʧ��
    2. ���нӿڶ������Ƿ�����ʵ�֣����ǰ�����ʱ����
    3. ��ʼ���������������get_vision_info�ӿڷ��أ������������Сƥ��
    4. �������ϲ����init_visionʱ���Զ���̬�ۼӣ���һ�ſ�Ϊ0���Դ�����.
***********************************************************************/
#pragma once
#ifndef _INCLUDE_VISION_API_H
#define _INCLUDE_VISION_API_H


#define VISION_API_VERSION "vision_api_1.1.1.20170207_beta"

#define EXPORT_C extern "C"
#define WINAPI __stdcall

#define VISION_MAX_NAME     32      //������󳤶�
#define VISION_ID_LEN       16      //���ID����
#define VISION_ARRAY_CNT    8       //��������
#define VISION_RES_CNT      16      //���������֧�ֽ������

enum EMVision_Status
{//�Ӿ�����״̬
    EMVISION_NOINIT = 0,    //δ��ʼ��
    EMVISION_ERROR,         //�쳣
    EMVISION_NORMAL,        //OK
};

struct TVisionInitParam
{//��ʼ�������ṹ��
    char data[VISION_ARRAY_CNT][VISION_MAX_NAME];   //����
};

struct TVisionPluginInfo
{//�Ӿ������Ϣ
    char id[VISION_ID_LEN];         //�������id -- ��Ϊ�Ӿ����ͱ�ʶ��,�����ظ�
    char desc[VISION_MAX_NAME];     //Ӳ������
    char initParamDesc[VISION_ARRAY_CNT][VISION_MAX_NAME];  //��ʼ����������
    unsigned short  paramCnt;       //��������
    char reserved[6];               //��֤8λ����
};

struct TVisionResult
{//�Ӿ�����ṹ��
    int     res;        //��� 0 - ok
    int     resCnt;     //�������
    double  val[VISION_RES_CNT];    //���
};

#ifndef VISION_FUN_IMPORT    //��ֹ���ú�������

/***********************************************
    *Function: ��ȡ�Ӿ������Ϣ����
    *Intput:   pInfo �Ӿ������Ϣ�ṹ��
    *Output:   pInfo �Ӿ������Ϣ������
    *Return:   �ο�EM_ERR_CODE
*********************************************/
EXPORT_C int WINAPI get_vision_info(TVisionPluginInfo* pInfo);

/***********************************************
    *Function: ��ʼ��һ���Ӿ�����
    *Intput:   visionID �Ӿ�����,��̬�ۼ�,��0��ʼ
               param ��ʼ������ ���֧��8������
    *Output:   NULL
    *Return:   �ο�EM_ERR_CODE
*********************************************/
EXPORT_C int WINAPI init_vision(unsigned short visionID, TVisionInitParam* param);

/***********************************************
    *Function: �ͷ�һ���Ӿ�����
    *Intput: visionID �Ӿ�����
    *Output: ��
    *Return: �ο�EM_ERR_CODE
    *********************************************/
EXPORT_C int WINAPI uninit_vision(unsigned short visionID);

/****************************************************
    *Function: ��ȡһ���Ӿ����������״̬
    *Intput:  visionID �Ӿ�����
    *Return: EMVision_Status
******************************************************/
EXPORT_C int WINAPI vision_status(unsigned short visionID);

/***********************************************
    *Function: ��ָ���Ӿ��������Զ���������ܽӿڣ�
    *Intput: visionID �Ӿ�����
             cmd ָ��
		     msg ָ����� ����Ϊ��
    *Output: NULL
    *Return: EM_ERR_CODE
*********************************************/
EXPORT_C int WINAPI send_custom_cmd(unsigned short visionID, const char* cmd, const char* msg/* = 0*/);

/***********************************************
    *Function: ����ָ���Ӿ������Զ�����Ϣ
    *Intput: visionID �Ӿ�����
             cmd ָ��
		     recvMsg���������ݻ�����,����Ϊ��
             timeout ��ʱ���� Ĭ�ϲ��ȴ�
    *Output: recvMsg �����ֵ
    *Return: EMCARD_STATUS ��ʱ����ERR_TIMEOUT
*********************************************/
EXPORT_C int WINAPI recv_custom_cmd(unsigned short visionID, const char* cmd, char* recvMsg/* = 0*/, int timeout/* = 0*/);

/****************************************************
    *Function: �Ӿ�����ָ�������ɷ���
    *Intput:  visionID  �Ӿ�����
              secne     ����ID��������Ӿ�ͨѶЭ�鴫��
              timeout   ��ʱ����
    *Output:  NULL
    *Return: EM_ERR_CODE
******************************************************/
EXPORT_C int WINAPI vision_process(unsigned short visionID, int secne);

/****************************************************
    *Function: ��ȡ�Ӿ�������
    *Intput:  visionID �Ӿ�����
              secne    ����ID��������Ӿ�ͨѶЭ�鴫��
              timeout  ��ʱ���� -1���ߵȴ�
    *Output:  pres    ���ؽ��
    *Return: EM_ERR_CODE ��ʱ����ERR_TIMEOUT
******************************************************/
EXPORT_C int WINAPI recv_process_result(unsigned short visionID, int secne, TVisionResult* pres, int timeout);

/****************************************************
    *Function: ��ȡ�Ӿ�����ͼ��������
    *Intput:  visionID �Ӿ�����
              secne    ����ID��������Ӿ�ͨѶЭ�鴫��
              pdata ���ȴ�NULL��ȡ��������С��new�ڴ���ٴε���
    *Output:  pdata    ͼ�񻺳���
    *Return: �ɹ�������������С��ʧ�ܷ��ش����� < 0
******************************************************/
EXPORT_C int WINAPI get_process_image(unsigned short visionID, int secne, char* pdata);

/****************************************************
    *Function: ��ȡ�Ӿ�����ͼ��·��
    *Intput:  visionID �Ӿ�����
              secne    ����ID��������Ӿ�ͨѶЭ�鴫��
    *Output:  path    ͼ��·�� MAX_PATH
    *Return: EM_ERR_CODE
******************************************************/
EXPORT_C int WINAPI get_process_image_path(unsigned short visionID, int secne, char* path);

/****************************************************
    *Function: �����Ӿ��궨���ݸ��Ӿ����󣨻�е���꣩
    *Intput:  visionID  �Ӿ�����
              secne     �궨����ID��������Ӿ�ͨѶЭ�鴫��
              x��y  ��е����
    *Output:  NULL
    *Return: EM_ERR_CODE
******************************************************/
EXPORT_C int WINAPI send_calib_data(unsigned short visionID, int secne, double x, double y);

/****************************************************
    *Function: ���Ӿ���������Ӿ��궨���ݣ����أ�,�Լ��������ϵ
    *Intput:  visionID  �Ӿ�����
              secne     �궨����ID��������Ӿ�ͨѶЭ�鴫��
              x��y  ��е����
    *Output:  NULL
    *Return: EM_ERR_CODE
******************************************************/
EXPORT_C int WINAPI recv_calib_data(unsigned short visionID, int secne, double x, double y);

/****************************************************
    *Function: �Ѵ��Ӿ����յ��Ľ��ʹ�ñ궨���ݽ���ת��
    *Intput:  visionID  �Ӿ�����
              secne     �궨����ID���궨ʱ�õ��ĸ�����
    *Output:  NULL
    *Return: EM_ERR_CODE
******************************************************/
EXPORT_C int WINAPI convert_calib_data(unsigned short visionID, int secne, TVisionResult* pres);

#endif
#endif      //��ֹ�ظ�����