#pragma once
class MyFunction
{
public:
	MyFunction(void);
	~MyFunction(void);

static	int   MyGetSubstrNum(CString str,CString invStr)  ////����:1_2_3�򷵻�3,��ʶ��3����״ _1_2_3_Ҳ����3
	{
		int strNum = 0;
		if (str.IsEmpty())
		{
			return strNum ;
		}else
		{
			strNum = 1;
			
		}

		if (invStr == str.GetAt(str.GetLength()-1))  //���������з��ţ���ȥ��
		{
			str.Delete(str.GetLength()-1);
		}
		if (invStr == str.GetAt(0))   //�����ǰ���з��ţ���ȥ��
		{
			str.Delete(0);
		}


		CString strTemp;
		while (-1 != str.Find(invStr))
		{
			strTemp = str.Left(str.Find(invStr)+1);
			str.Delete(0,strlen(strTemp));
			strNum++;
		}
		return strNum;
	}



//��BaseStr���ҵ�invStr,BeginNumΪ�ڼ���(�ӵ�һ����ʼ),InvNumΪ�Ҽ���,invStrΪ���ַ���,����Ϊ�ҵ���
static	CString MyGetSubStr(CString BaseStr,int BeginNum,int InvNum,CString invStr)   
	{
		CString strTemp,strTemp1;

		if (BaseStr.IsEmpty())
		{
			return  BaseStr;
		}
		if (BeginNum+InvNum-1> MyGetSubstrNum(BaseStr,invStr))
		{
			strTemp.Empty();
			return strTemp;
		}
		int numTemp = 1;
		while (numTemp < BeginNum)//���ǵ�2��ɾ����һ��
		{
			strTemp = BaseStr.Left(BaseStr.Find(invStr)+1);
			int len = strlen(strTemp);
			BaseStr.Delete(0,strlen(strTemp));
			numTemp++;
		}

		numTemp = 0;
		strTemp.Empty();
		while (numTemp < InvNum)
		{

			strTemp1 = BaseStr.Left(BaseStr.Find(invStr)+1);
			if (strTemp1.IsEmpty())
			{
				return strTemp+BaseStr;
			} 
			else
			{
				strTemp+=strTemp1;
			}

			BaseStr.Delete(0,strlen(strTemp1));
			numTemp++;
		}
		return strTemp;

	}


};

