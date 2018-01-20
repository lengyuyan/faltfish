#pragma once
#ifndef FLATFISH_INCLUDE_GLOBALERROR_H
#define FLATFISH_INCLUDE_GLOBALERROR_H

//�������
enum EM_ERR_CODE
{
    ERR_INPUT_PARAM = -1000,        //���Ϊ�ջ��߲��Ϸ�
    ERR_PARAM_FORMAT,               //������ʽ����
    ERR_VERSION,                    //�汾����
    ERR_TIMEOUT,                    //�ȴ���ʱ
    ERR_NO_INIT,                    //δ��ʼ��
    ERR_REPEAT_INIT,                //�ظ���ʼ��
    ERR_NO_CONNECT,                 //δ����
    ERR_NO_REG,                     //δע��
    ERR_REPEAT_REG,                 //�ظ�ע��
    ERR_DATA_OVERLIMIT,             //�������
    ERR_INVALID_ID,                 //��ID��Ч
    ERR_INVALID_HANDLE,             //��Ч���
    ERR_INVALID_NAME,               //��Ч����
    ERR_NAME_EXIST,                 //�������Ѿ�����
    ERR_NOREADY,                    //���ھ���״̬,���ܽ��ܸ�ָ��

    //File error
    ERR_CREATE_FILE,                //�����ļ�ʧ��
    ERR_OPEN_FILE,                  //���ļ�ʧ��
    ERR_WRITE_FILE,                 //д���ļ�ʧ��
    ERR_READ_FILE,                  //��ȡ�ļ�ʧ��
    ERR_DEL_FILE,                   //ɾ���ļ�ʧ��
    ERR_GET_ATTRIBUTES,             //��ȡ�ļ�����ʧ��
    ERR_FILE_FORMAT,                //�ļ���ʽ����

    //commport error
    ERR_OPEN_COM,                   //�򿪴���ʧ��
    ERR_COM_WRITE,                  //д�봮�ڴ���
    ERR_COM_READ,                   //��ȡ���ڴ���
    ERR_GET_COMSTATE,               //��ȡ���ڲ�������
    ERR_SET_COMSTATE,               //���ô��ڲ�������
    ERR_CLEAR_COMSTACK,             //���������ʧ��

    //sysapi
    ERR_INIT_NET,                   //��������Э��ջʧ��
    ERR_ABANDONED,                  //��ռģʽ���޷�����
    ERR_CLIENT_NORESPONSE,          //�ͻ�������Ӧ
    ERR_CREATE_SOCKET,              //����socketʧ��
    ERR_BIND_SOCKET,                //�󶨶˿ڴ���
    ERR_OPERATOR_THREAD,            //�����߳�ʧ��
    ERR_OPERATOR_EVENT,             //�����¼�ʧ��
    ERR_ABANDONED_EVENT,            //�¼�������, �޷�����
    ERR_OPERATOR_MUTEX,             //��������������
    ERR_LOAD_DLL,                   //����dllʧ��
    ERR_LOAD_DLLFUN,                //����dllĳ���ӿ�ʧ��

    //skt
    ERR_INVALID_IP,                 //��ЧIP
    ERR_INVALID_PORT,               //��Ч�˿�
    ERR_LISTEN_SKT,                 //socket����ʧ��
    ERR_ACCEPT_SKT,                 //��������ʧ��
    ERR_CONNECT_SKT,                //���ӷ����ʧ��
    ERR_RECV_MSG,                   //������Ϣ����
    ERR_SEND_MSG,                   //������Ϣ����
    ERR_GET_SKTINFO,                //��ȡsocket��Ϣ(ip,port)

    //user
    ERR_NO_LOGIN,                   //δ��¼
    ERR_NO_LIMIT,                   //��Ȩ��
    ERR_INVALID_PWD,                //������Ч
    ERR_LIMIT_OVER,                 //Ȩ������ֻ��32λ��������

    //motor err
    ERR_NOINIT_CARD,                //δ��ʼ�����ƿ�
    ERR_MOTOR_API,                  //���ƿ��ӿ�����ش���
    ERR_NO_CARD,                    //���ƿ������ڣ������޿�
    ERR_CARD_EXIST,                 //���ƿ��Ѿ�����
    ERR_LOAD_CFG,                   //���ؿ������ļ�ʧ��
    ERR_INIT_MOTOR,                 //��ʼ�����ƿ�ʧ��
    ERR_INVALID_AXIS,               //��Ч��
    ERR_AXIS_HOME,                  //���ԭʧ��
    ERR_AXIS_MOVING,                //�������˶���
    ERR_GET_CURPOS,                 //��ȡ��ǰλ��ʧ��
    ERR_CARD_RESET,                 //����λʧ��
    ERR_AXIS_RESET,                 //�Ḵλʧ��
    ERR_AXIS_TRAP,                  //���λ�˶�ʧ��
    ERR_SET_MOVETYPE,               //�����˶�ģʽʧ��
    ERR_SET_CRD,                    //��������ϵʧ��
    ERR_GET_CRD,                    //��ȡ����ϵʧ��
    ERR_ARC_MOVE,                   //Բ���岹ʧ��
    ERR_CRD_MOVE,                   //����ϵ�˶�ʧ��
    ERR_IO_TYPE,                    //io���ʹ���
    ERR_WRITE_IO,                   //д��ioʧ��
    ERR_OCCUPY_IO,                  //IO��ռ��
    ERR_OCCUPY_AXIS,                //�ᱻռ��

    //no init library
    ERR_NOINIT_COMM,                //δ��ʼ��comm��
    ERR_NOINIT_NET,                 //δ��ʼ������Э��ջ
    ERR_NOINIT_SKT,                 //δ��ʼ��skt��
    ERR_NOINIT_USER,                //δ��ʼ��user��
    ERR_NOINIT_CONTROL,             //δ��ʼ��control��
    ERR_NOINIT_OPS,                 //opsδ��ʼ��
    ERR_NOINIT_VISION,              //δ��ʼ���Ӿ���
    ERR_START_SAFEMONITOR,          //������ȫ���ʧ��
    ERR_SAFE_CHK,                   //��ȫ�ռ�����

    ERR_NO_HOMEP,                   //��ԭ���ź�
    ERR_NO_LIMITN,                  //�޸���λ�ź�
    ERR_NO_LIMITP,                  //������λ�ź�

    ERR_VISION_RES,                 //�Ӿ��������
    ERR_FORMAT_MSG,                 //��Ϣ��ʽ����
    ERR_NETCALL,                    //������ó���

    ERR_EM_MAX,                     //���ֵ,���Ǵ�����
    RETURN_OK = 0,                  //��������
};

#define ERR_NO_NAME ERR_INVALID_NAME

#endif  //��ֹ�ظ�����