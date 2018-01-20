// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� DEVICEINFO_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// DEVICEINFO_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef DEVICEINFO_EXPORTS
#define DEVICEINFO_API __declspec(dllexport)
#else
#define DEVICEINFO_API __declspec(dllimport)
#endif


enum    {DES_ENCRYPT,DES_DECRYPT};

//ȡ��12λ�Ĺ���CPU��Ϣ�����ڼ��ܣ�
DEVICEINFO_API int GetCpuInfo(char *szCpuInfo);

//ȡ��12λ�Ĺ���CPU4�������������Ϣ�����ڼ��ܣ�
DEVICEINFO_API int GetCpuALLInfo(char *szCpuInfo);

//ȡ��Ӳ�̵�Ӳ�����кţ�
DEVICEINFO_API int GetHDInfo(char *szHDInfo);

//ȡ������ MAC ��ַ��ע��: MAC ��ַ�ǿ�����ע������޸ĵģ�
DEVICEINFO_API int GetMACInfo(char *szMACInfo);

//ȡ��bios�������Ϣ,�п���ȡ����
DEVICEINFO_API int GetBISOInfo(char *szBIOSDnfo);

//ȡ������������Ϣ
DEVICEINFO_API int GetCARDInfo(char *szCARDnfo);

/****************************************************
*Function: ��/�����ַ�
*Intput:  In         �����ַ���
          datalen    ���볤��
          Key        ��Կ
          keylen     ��Կ����
          Type       ����/����
*Output: Out
*Return: ��
******************************************************/
DEVICEINFO_API bool Execution_3DES(char *Out, char *In, long datalen,const char *Key, int keylen, bool Type);

/****************************************************
*Function: ��/�����ַ�
*Intput:  In         �����ַ���
          datalen    ���볤��
*Output:  Out        MD5����������
*Return: ��
******************************************************/
DEVICEINFO_API void Execution_MD5(char *Out,const char *In, long datalen);

/****************************************************
*Function: ��ȡ���ܺ���ַ���
*Intput:  
          
*Output:  pData  �� ����ֵ [���ܺ�buffer��СΪ32���ַ�]
*Return: ��
******************************************************/
DEVICEINFO_API bool GetLicenseBuf(char *pStrMd5, int iLength);