#pragma once

#include "math.h"
#include "atltypes.h"
#include "string.h"

#include "common.h"

#define MATH_PI 3.1415926535

using namespace std;
// ��������ϵ��ԭ��+����
typedef struct FuncCoordinate
{
	double  OriginalX;
	double  OriginalY;
	double  OriginalA;
	FuncCoordinate()
	{
		OriginalX = 0.0;
		OriginalY = 0.0;
		OriginalA = 0.0;
	}
	void operator=(const FuncCoordinate& sourse)
	{ 
		OriginalX = sourse.OriginalX;
		OriginalY = sourse.OriginalY;
		OriginalA = sourse.OriginalA;
	}


}FUNCCOORDINATE,*LPFUNCCOORDINATE;


class  MyFunClass
{

public:
	 static double CalDistance(double x,double y)
	 {
		 return sqrt((x*x)+(y*y));
	 }


// �������ص�ʵ�ʴ�С��
  static double CalPixSize(double(*MotorDataX),double(*MotorDataY),double (*CamDataCol), double (*CamDataRow),int DataNum)
	{
		if (DataNum<2)
		{
			LOG_WARN("�������ػ�е����ʱС��2����");
			return 0.0;
		}
		double DistanceMotor = 0.0;
		double DistancePix = 0.0;

		double PixSize = 0;
		int CalNum = 0;
		for (int i = 0;i<DataNum;i++)
		{
			for (int j = i+1;j<DataNum;j++)
			{
				DistanceMotor = CalDistance((MotorDataX[j]-MotorDataX[i]),MotorDataY[j] - MotorDataY[i]);
				DistancePix = CalDistance((CamDataCol[j]-CamDataCol[i]),CamDataRow[j] - CamDataRow[i]);
				if (DistanceMotor<2)  //��е����仯<1mm����������仯С��3�����أ�������
				{
					LOG_WARN("������е�������С��2����λ");
					//continue;
				}
				if (DistancePix<10)
				{
					LOG_WARN("���������������С��10����λ");
				}
				if (DistancePix == 0)
				{
					LOG_WARN("�����������������ڵ���0");
					continue;
				}

				PixSize += DistanceMotor/DistancePix;	 
				CalNum++;
			}
		 }
		if (CalNum == 0) //��ĸ����Ϊ0
		{
			return 0.0;
		}
		return PixSize/CalNum;
	}

	//��������������ӵ�ֱ�ߵ�ϵ����
  static void Point2ToLine( const double x1,const double y1,const double x2,const double y2,double& K,double& phi,double& B)
  {
	  K = (y2-y1)/(x2-x1);
	  phi = atan(K);
	  B =y1 - x1*K;

  }

  //��ֱ�߽���
  static void GetInsectionPoint(const double K1,const double B1 ,const double K2,const double B2,double& x,double& y)
  {
	  x = (B2 - B1)/(K1 - K2);   //�л�X���꣨���У�
	  y = K1*x + B1;             //�л�Y���꣨���У�   

  }

  //�����������ת�Ƕȵõ���ת����,Ϊ����ʹ�ã���ת�Ƕȵ�λΪ�ȡ�
 static void CalRotCenterTool(double& x1,double& y1,double x2,double y2,double rotPhi)
  {
	  double LineK,LinePhi,LineB;
	  double LineK1,LineB1,LineK2,LineB2;
	  double addPhi = 0.0;
	  Point2ToLine( x1, y1,  x2,  y2, LineK,LinePhi,LineB);

	  //rotAng = rotAng*MATH_PI/180;
	  addPhi = rotPhi<0?(-rotPhi/2 - MATH_PI/2):(MATH_PI/2-rotPhi/2);


	  LineK1 = tan(LinePhi+addPhi);
	  LineK2 = tan(LinePhi-addPhi);
	  
	  LineB1 = y1 - LineK1*x1;
	  LineB2 = y2 - LineK2*x2;

	  GetInsectionPoint(LineK1,LineB1,LineK2,LineB2,x1,y1);
  }

 static void TransMetrixData( double PreSoData, double soMetix,double&  DisMetix)
 {
	 DisMetix = soMetix+PreSoData;

 }

 //����0-1-1 ��� 0-1-2
 static void ParshRelativeToObsolute(double (*RelativeMetric),double (*ObsoluteMetric),int MetricNum)
 {

	 ObsoluteMetric[0] = RelativeMetric[0];
	 for (int i = 1 ;i < MetricNum;i++)
	 {
		 ObsoluteMetric[i] =ObsoluteMetric[i-1]+ RelativeMetric[i];
	 }

 }

  

};




