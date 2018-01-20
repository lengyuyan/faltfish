// DeviceInfo.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "DeviceInfo.h"
#include "cpuid.h"
#include "GetOtherInfo.h"


DEVICEINFO_API int GetCpuALLInfo(char *szCpuInfo)
{
    int num = 0;
    for (int i=0;i<5;i++)
    {
        char buf[1024];
        int count = GetEXAKEYCpu(i,buf);
        CopyMemory( szCpuInfo + num, buf, count);
        num += count;
    }
    return num;
}
//ȡ��12λ�Ĺ���CPU��Ϣ�����ڼ��ܣ�
DEVICEINFO_API int GetCpuInfo(char *szCpuInfo)
{
    WORD index = wincpuidext();
    DWORD features = wincpufeatures();	
    sprintf(szCpuInfo,"%.4x%.8x",index,features);
    return 12;

}
//ȡ��Ӳ�̵�Ӳ�����кţ�ͬʱ�����˵��ո����
DEVICEINFO_API int GetHDInfo(char *szHDInfo)
{
    BYTE szSystemInfo[4096]; 
    UINT uSystemInfoLen = 0;
    {
        OSVERSIONINFO ovi = { 0 };
        ovi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
        GetVersionEx( &ovi );

        if( ovi.dwPlatformId != VER_PLATFORM_WIN32_NT )
        {
            // Only Windows 2000, Windows XP, Windows Server 2003...
            //return FALSE;
        }
        else
        {
            if( !WinNTHDSerialNumAsPhysicalRead( szSystemInfo, &uSystemInfoLen, 1024 ) )
            {
                WinNTHDSerialNumAsScsiRead( szSystemInfo, &uSystemInfoLen, 1024 );
            }
        }

    }	
    int count = 0;
    for (int i=uSystemInfoLen-1;i>0;i--)
    {
        if (szSystemInfo[i]!=' ')
        {
            szHDInfo[count++] = szSystemInfo[i];
        }
    }	
    szHDInfo[count]= '\0';	
    return count;
}
//ȡ������ MAC ��ַ��ע��: MAC ��ַ�ǿ�����ע������޸ĵģ�
//int GetMACInfo(char *szMACInfo)
//{
//    NCB ncb;
//    UCHAR uRetCode;
//    LANA_ENUM lana_enum;
//    memset( &ncb, 0, sizeof(ncb) );
//    ncb.ncb_command = NCBENUM;
//    ncb.ncb_buffer = (unsigned char *) &lana_enum;
//    ncb.ncb_length = sizeof(lana_enum);
//    // ����������NCBENUM����,�Ի�ȡ��ǰ������������Ϣ,���ж��ٸ�������ÿ�������ı�ŵ�
//    uRetCode = Netbios( &ncb );
//    int number = 0;
//    if ( uRetCode == 0 )
//    {
//        // ��ÿһ������,�����������Ϊ������,��ȡ��MAC��ַ
//        for ( int i=0; i<lana_enum.length; ++i)
//        {
//            char buf[1024];
//            int count = getmac_one(lana_enum.lana[i],buf);
//            CopyMemory( szMACInfo + number, buf, count );
//            number +=count;
//        }
//    }
//    szMACInfo[number] = '\0';
//    return number;
//}
//ȡ��bios�������Ϣ,�п���ȡ����
DEVICEINFO_API int GetBISOInfo(char *szBIOSDnfo)
{
    int uSystemInfoLen = 0;
    SIZE_T ssize; 

    LARGE_INTEGER so; 
    so.LowPart=0x000f0000;
    so.HighPart=0x00000000; 
    ssize=0xffff; 
    wchar_t strPH[30]=L"\\device\\physicalmemory"; 

    DWORD ba=0;

    UNICODE_STRING struniph; 
    struniph.Buffer=(wchar_t *)strPH; 
    struniph.Length=0x2c; 
    struniph.MaximumLength =0x2e; 

    OBJECT_ATTRIBUTES obj_ar; 
    obj_ar.Attributes =64;
    obj_ar.Length =24;
    obj_ar.ObjectName=&struniph;
    obj_ar.RootDirectory=0; 
    obj_ar.SecurityDescriptor=0; 
    obj_ar.SecurityQualityOfService =0; 

    HMODULE hinstLib = LoadLibrary("ntdll.dll"); 
    ZWOS ZWopenS=(ZWOS)GetProcAddress(hinstLib,"ZwOpenSection"); 
    ZWMV ZWmapV=(ZWMV)GetProcAddress(hinstLib,"ZwMapViewOfSection"); 
    ZWUMV ZWunmapV=(ZWUMV)GetProcAddress(hinstLib,"ZwUnmapViewOfSection"); 
    //���ú������������ڴ����ӳ�� 
    HANDLE hSection; 
    if( 0 == ZWopenS(&hSection,4,&obj_ar))
        if ( 
            0 == ZWmapV( 
            ( HANDLE )hSection,   //��Sectionʱ�õ��ľ�� 
            ( HANDLE )0xFFFFFFFF, //��Ҫӳ����̵ľ���� 
            &ba,                  //ӳ��Ļ�ַ 
            0,
            0xFFFF,               //����Ĵ�С 
            &so,                  //�����ڴ�ĵ�ַ 
            &ssize,               //ָ���ȡ�ڴ���С��ָ�� 
            1,                    //�ӽ��̵Ŀɼ̳����趨 
            0,                    //�������� 
            2                     //�������� 
            ) )
            //ִ�к���ڵ�ǰ���̵Ŀռ俪��һ��64k�Ŀռ䣬����f000:0000��f000:ffff��������ӳ�䵽���� 
            //ӳ��Ļ�ַ��ba����,���ӳ�䲻������,Ӧ����ZwUnmapViewOfSection�Ͽ�ӳ�� 
        {		
            BYTE* pBiosSerial = ( BYTE* )ba;
            UINT uBiosSerialLen = FindAwardBios( &pBiosSerial );
            if( uBiosSerialLen == 0U )
            {
                uBiosSerialLen = FindAmiBios( &pBiosSerial );
                if( uBiosSerialLen == 0U )
                {
                    uBiosSerialLen = FindPhoenixBios( &pBiosSerial );
                }
            }
            if( uBiosSerialLen != 0U )
            {
                CopyMemory( szBIOSDnfo + uSystemInfoLen, pBiosSerial, uBiosSerialLen );
                uSystemInfoLen += uBiosSerialLen;
            }
            ZWunmapV( ( HANDLE )0xFFFFFFFF, ( void* )ba );
        }
        szBIOSDnfo[uSystemInfoLen] = '\0';
        return uSystemInfoLen;
}
////ȡ������������Ϣ
//int GetCARDInfo(char *szCARDnfo)
//{
//    DWORD  serailno;
//    int flag = GetPciBoardSerailNo(0,&serailno);
//    if (flag==0)
//    {
//        if (serailno!=0)		
//        {
//            sprintf(szCARDnfo,"%.8x",serailno);
//            return 8;
//        }
//    }
//    return 0;
//}

DEVICEINFO_API  bool GetLicenseBuf(char *pStrMd5, int iLength)
{
    bool bRet = false;
    char skey[16] = {"TestDES"};  //��Կ
    char *pDesEncrypt = NULL;   //����ǰ�ڴ�
    char *pDesDecrypt = NULL;   //���ܺ��ڴ�
    //char szMd5[33] = {0};       //ת��md5������
    char szMd5FildData[33] = {0};       //�ļ��е�md5����

    char szMID[1024] = {0};    //�洢Ӳ���ڴ����� 
    do 
    {
        if (NULL == pStrMd5)
        {
            break;
        }
        //Step 1 ��ȡMID
        GetCpuInfo(pStrMd5);
        GetHDInfo(pStrMd5+strlen(pStrMd5));
        GetBISOInfo(pStrMd5+strlen(pStrMd5));

        //��ֹԽ��
        if (iLength < strlen(pStrMd5))
        {
            memset(pStrMd5, 0, iLength);
            break;
        }
        
        //Step 2 ���ܴ���
        int iLength = (strlen(szMID)+7)&0xfffffff8;
        pDesEncrypt = new char[iLength + 1];
        if (NULL == pDesEncrypt)
        {
            break;
        }
        memset(pDesEncrypt, 0, iLength+1);
        strcpy(pDesEncrypt, szMID);

        pDesDecrypt = new char[iLength + 1];
        if (NULL == pDesDecrypt)
        {
            break;
        }
        memset(pDesEncrypt, 0, iLength+1);
        if (!Execution_3DES(pDesDecrypt, pDesEncrypt, iLength,skey, strlen(skey), DES_DECRYPT))
        {
            break;
        }
        delete pDesEncrypt;
        pDesEncrypt = NULL;

        //����md5ֵ
        Execution_MD5(pStrMd5, pDesDecrypt, iLength);
        delete pDesDecrypt;
        pDesDecrypt = NULL;

        bRet = true;
    } while (0);
    return bRet;
}