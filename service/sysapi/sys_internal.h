/*��ͷ�ļ���Ҫ��������ϵͳ��غ�������sysapi�ڲ�ʹ�ã���Ӧ��������ģ�����ô��ļ�
*/
#pragma once
#ifndef _INCLUDE_SYS_H
#define _INCLUDE_SYS_H

//����
#define ___WINDOWS
//#define ___MAC

#ifdef ___WINDOWS       //windows
#include <windows.h>
#include <IPHlpApi.h>
#include <IcmpAPI.h>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")
#endif  //end of window include and comment

#ifdef ___MAC           //MAC

#endif

#endif   //��ֹ�ظ�����