// VisionProcess.cpp : ���� DLL Ӧ�ó���ĵ���������
//



#include "stdafx.h"
#include "VisionProcess.h"
#include <direct.h>
#include "iImageCtrl.h"
#include "common.h"
#include  <io.h>
//#pragma comment(lib,"common.lib")

HTuple  CVisionProcess::m_CamerasHomMat2D[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER];//���Ծ���
HTuple  CVisionProcess::m_CamRotToMotorHomMat2D[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER];//��Ծ���
HTuple  CVisionProcess::m_ImageToMotorRelative[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER];
HTuple  CVisionProcess::m_ImageToMotorAbsolute[CAMERA_NUMBER][AXIS_NUMBER][TOOL_NUMBER];
HTuple  CVisionProcess::m_ScenePosPass[30][30];

class LockStock 
{
public:
	LockStock (CRITICAL_SECTION *LockSection)
	{
		if (NULL != LockSection)
		{
			EnterCriticalSection(LockSection);
		}
		m_LockSection = LockSection;
	}

	~LockStock ()
	{
		if (NULL != m_LockSection)
		{
			LeaveCriticalSection(m_LockSection);          
		}
	}
private:
	CRITICAL_SECTION *m_LockSection;
};

CRITICAL_SECTION *CVisionProcess::m_pSection = NULL;

void CPPExpDefaultExceptionHandler(/*const Halcon::HException& except*/)
{
	// throw except;
}

/////////////////////////////////////////////////////////////////////////

// Chapter: Graphics / Text
// Short Description: This procedure writes a text message.
void disp_message (HTuple WindowHandle, HTuple String, HTuple CoordSystem, 
	HTuple Row, HTuple Column, HTuple Color, HTuple Box)
{
	//using namespace Halcon;

	// Local control variables 
	HTuple  Red, Green, Blue, Row1Part, Column1Part;
	HTuple  Row2Part, Column2Part, RowWin, ColumnWin, WidthWin;
	HTuple  HeightWin, MaxAscent, MaxDescent, MaxWidth, MaxHeight;
	HTuple  R1, C1, FactorRow, FactorColumn, Width, Index, Ascent;
	HTuple  Descent, W, H, FrameHeight, FrameWidth, R2, C2;
	HTuple  DrawMode, Exception, CurrentColor;


	// extern void CPPExpDefaultExceptionHandler(const HException& except);
	// Install default exception handler 
	// HException::InstallHHandler(&CPPExpDefaultExceptionHandler);

	//This procedure displays text in a graphics window.
	//
	//Input parameters:
	//WindowHandle: The WindowHandle of the graphics window, where
	//   the message should be displayed
	//String: A tuple of strings containing the text message to be displayed
	//CoordSystem: If set to 'window', the text position is given
	//   with respect to the window coordinate system.
	//   If set to 'image', image coordinates are used.
	//   (This may be useful in zoomed images.)
	//Row: The row coordinate of the desired text position
	//   If set to -1, a default value of 12 is used.
	//Column: The column coordinate of the desired text position
	//   If set to -1, a default value of 12 is used.
	//Color: defines the color of the text as string.
	//   If set to [], '' or 'auto' the currently set color is used.
	//   If a tuple of strings is passed, the colors are used cyclically
	//   for each new textline.
	//Box: If set to 'true', the text is written within a white box.
	//
	//prepare window


	//  get_rgb(WindowHandle, &Red, &Green, &Blue);
	//get_part(WindowHandle, &Row1Part, &Column1Part, &Row2Part, &Column2Part);
	GetPart(WindowHandle, &Row1Part, &Column1Part, &Row2Part, &Column2Part);
	// get_window_extents(WindowHandle, &RowWin, &ColumnWin, &WidthWin, &HeightWin);
	GetWindowExtents(WindowHandle, &RowWin, &ColumnWin, &WidthWin, &HeightWin);
	//set_part(WindowHandle, 0, 0, HeightWin-1, WidthWin-1);
	SetPart(WindowHandle, 0, 0, HeightWin-1, WidthWin-1);

	//
	//default settings
	if (0 != (Row==-1))
	{
		Row = 12;
	}
	if (0 != (Column==-1))
	{
		Column = 12;
	}
	if (0 != (Color==HTuple()))
	{
		Color = "";
	}
	//Hlong HalconVersion;
	//::get_system("version",&HalconVersion);

	//
	// String = ((""+String)+"").Split("\n");
	//
	//Estimate extentions of text depending on font size.
	//get_font_extents(WindowHandle, &MaxAscent, &MaxDescent, &MaxWidth, &MaxHeight);
	GetFontExtents(WindowHandle, &MaxAscent, &MaxDescent, &MaxWidth, &MaxHeight);
	if (0 != (CoordSystem==HTuple("window")))
	{
		R1 = Row;
		C1 = Column;
	}
	else
	{
		//transform image to window coordinates
		FactorRow = (1.*HeightWin)/((Row2Part-Row1Part)+1);
		FactorColumn = (1.*WidthWin)/((Column2Part-Column1Part)+1);
		R1 = ((Row-Row1Part)+0.5)*FactorRow;
		C1 = ((Column-Column1Part)+0.5)*FactorColumn;
	}
	//
	//display text box depending on text size
	if (0 != (Box==HTuple("true")))
	{
		//calculate box extents
		String = (" "+String)+" ";
		Width = HTuple();
		//for (Index=0; Index<=(String.Num())-1; Index+=1)
		//{
		//    //get_string_extents(WindowHandle, HTuple(((const HTuple&)String)[Index]), &Ascent, 
		//        //&Descent, &W, &H);
		//    Width.Append(W);
		//}
		//FrameHeight = MaxHeight*(String.Num());
		//FrameWidth = (HTuple(0).Concat(Width)).Max();
		//R2 = R1+FrameHeight;
		//C2 = C1+FrameWidth;
		//display rectangles
		//get_draw(WindowHandle, &DrawMode);
		// set_draw(WindowHandle, "fill");
		// set_color(WindowHandle, "light gray");
		// disp_rectangle1(WindowHandle, R1+3, C1+3, R2+3, C2+3);
		//DispRectangle1(WindowHandle, R1+3, C1+3, R2+3, C2+3);

		// set_color(WindowHandle, "white");
		//disp_rectangle1(WindowHandle, R1, C1, R2, C2);
		//DispRectangle1(WindowHandle, R1, C1, R2, C2);
		//set_draw(WindowHandle, DrawMode);
	}
	else if (0 != (Box!=HTuple("false")))
	{
		Exception = "Wrong value of control parameter Box";
		throw HException(Exception);
	}
	//Write text.
	//for (Index=0; Index<=(String.Num())-1; Index+=1)
	//{
	//    CurrentColor = ((const HTuple&)Color)[Index%(Color.Num())];
	//    if (0 != (HTuple(CurrentColor!=HTuple("")).And(CurrentColor!=HTuple("auto"))))
	//    {
	//        set_color(WindowHandle, CurrentColor);
	//    }
	//    else
	//    {
	//        set_rgb(WindowHandle, Red, Green, Blue);
	//    }
	//    Row = R1+(MaxHeight*Index);
	//    set_tposition(WindowHandle, Row, C1);
	//    //write_string(WindowHandle, HTuple(((const HTuple&)String)[Index]));
	//}
	////reset changed window settings
	//set_rgb(WindowHandle, Red, Green, Blue);
	//set_part(WindowHandle, Row1Part, Column1Part, Row2Part, Column2Part);
	SetPart(WindowHandle, Row1Part, Column1Part, Row2Part, Column2Part);
	return;
}

/***********************************************************************/



CVisionProcess::CVisionProcess()
{
	if (NULL == m_pSection)
	{
		m_pSection = new CRITICAL_SECTION;

		InitializeCriticalSection(m_pSection);
	}
	m_HomMat2D = -1;
	m_AngleOffset = 0;
	m_Direction = 0;
	m_MotorAngle = 0.0;
	m_Dispix =0.0;
	m_bIsAppend = false;

	m_modelFindType = 0;
	m_modelBuildType = 0;
	m_modelReadType = 0;
	m_ShapeIdCount = 0;
	m_index = 0;
	for (int i = 0; i < MAXPICNUMEVESCENE; i++)
	{
		m_bIsInit[i] = false;
	}
	m_iValidSize = 0;
	memset(m_PixelArray, 0, CALIB_DATA_MAX_SIZE * sizeof(AF_POINT));
	memset(m_RealArray, 0, CALIB_DATA_MAX_SIZE * sizeof(AF_POINT));


	m_HvMatchModID.Clear();
	//m_HvMatchModID = -1;
	//ReadShapeModel(szModePath,&m_HvMatchModID);
	for (int i = 0;i<sizeof(m_pImage)/sizeof(HObject);i++)
	{
		m_pImage[i].Clear();
	}

	//memset(m_testBuffer,0, strlen(m_testBuffer));


	//��ʼ��halcon�쳣�������
	// HException::InstallHHandler(&CPPExpDefaultExceptionHandler);
}


CVisionProcess::~CVisionProcess(void)
{
	if (NULL != m_pSection)
	{
		delete m_pSection;
		m_pSection = NULL;
	}
	if (-1 != m_HvMatchModID)
	{
		try
		{
			ClearShapeModel(m_HvMatchModID);
		}
		catch (HException e)
		{
			return;
		}
	}
}
//��������߿���ȡָ��ͼ���ڴ��ָ��,��m_pImage[index]֮ǰװ�ع�ͼ��,�������ĳߴ���������Ĳ�һ��,��ô�����µ��ڴ�
char * CVisionProcess::GetImageBuf(int index, int iWidth, int Height)
{
	char *pBuf = NULL;
	HTuple TempPointer;
	try
	{
		Hlong hWidth = 0;
		Hlong hHeight = 0;
		HTuple TupleTemp,H_Width,H_Height;
		if (MAXPICNUMEVESCENE <= index)
		{
			return  pBuf;
		}
		if (m_bIsInit[index])
		{
			GetImageSize(m_pImage[index], &H_Width,  &H_Height);
			hWidth =    H_Width;
			hHeight =  H_Height;
		}

		if (iWidth != hWidth || Height != hHeight)
		{
			m_pImage[index].Clear();
			GenImageConst(&m_pImage[index],"byte",iWidth, Height);
			m_bIsInit[index] = true;
		}
		HTuple ImagePointer;
		GetImagePointer1(m_pImage[index],&ImagePointer, &TupleTemp, &H_Width,  &H_Height);//��ȡͼ��ָ��,����,��,��
		pBuf = const_cast<char*>((char*)((int)(ImagePointer)));
	}
	catch (HalconCpp::HException &hdev_exception)
	{
		hdev_exception = hdev_exception;
		pBuf = NULL;
	}
	return pBuf;
}

bool CVisionProcess::ReadImage(const char *pName, int index, bool bNewPic)
{
	HObject hImage;
	HTuple H_Width,H_Height;
	if (MAXPICNUMEVESCENE <= index)
	{
		return false;
	}
	if (bNewPic && !(m_pImage[index].IsInitialized()))
	{
		return true;
	}
	if(m_bIsInit[index])
	{
		GetImageSize(m_pImage[index], &H_Width,  &H_Height);
		GetImageBuf(index,H_Width, H_Height);
	}
	HalconCpp::ReadImage(&m_pImage[index], pName);
	HObject HO_Image = m_pImage[index];
	m_index = index;
	m_bIsInit[index] = true;
	return true;
}

bool CVisionProcess::DispImage(int winID, int index)
{
	HObject Arrow, ArrowX, ArrowY;
	HTuple width, height, Point1Row, Point1Col, Point2Row, Point2Col;
	HTuple row,col, row1,col1;


	try
	{
		HalconCpp::ClearWindow(winID);
		if (MAXPICNUMEVESCENE <= index || !(m_pImage[index].IsInitialized()))
		{
			return false;
		}
		HalconCpp::ClearWindow(winID);
		HalconCpp::DispImage(m_pImage[index],winID);

	}
	catch(...)
	{
		return false;
	}
	m_index = index;
	return true;
}

bool CVisionProcess::DispObj(long winID, bool bIsShowTmp)
{
	vector<HObject> it;
	HObject obj;
	HTuple count;
	HTuple row,col, row1, col1;
	HTuple Message, Colors;
	try
	{
		m_bIsAppend = false;
		HalconCpp::DispObj(m_vecObj, winID);
		HTuple iWidth = 0;
		HTuple iHeight = 0;

		if (8 <= m_index/* || (H_EMPTY_REGION == m_pImage[m_index].Id() ? true : false)*/)
		{
			return false;
		}
		if (m_bIsPass)
		{
		}
		else
		{
		}
	}
	catch (...)
	{
		return FALSE;
	}
	return true;
}
bool CVisionProcess::ShowWindString(long winID,HTuple StrContent,int topRow,int topCol )
{
	try
	{
		if (topRow>=0&&topCol>=0)
		{
			SetTposition(winID,topRow,topCol);
		}
		WriteString(winID,StrContent);
	}
	catch (...)
	{
		return false;
	}

	return true;

}


bool CVisionProcess::SetDispColorHeight(long winID,HTuple strColor,int FontHeight)
{
	char szText[256] ={0};
	if ( 0 >= FontHeight)
	{
		sprintf_s(szText,"-*-*-*-0-*-*-0-");
	}else
	{
		sprintf_s(szText,"-*-%d-*-0-*-*-0-",FontHeight);
	}

	try
	{
		SetColor(winID,strColor);
		SetFont(winID,szText);

	}
	catch (...)
	{
		return false;
	}

	return true;


}


bool CVisionProcess::SaveImage(int index,char *path, string SNstr)
{

	SYSTEMTIME lt;
	char szPath[_MAX_PATH*2] = {0};
	try 
	{
		GetLocalTime(&lt);
		if (-1 == access(path, 0))//�ж�·���Ƿ���Ч
		{
			_mkdir(path);
		}
		if (SNstr.empty())
		{
			sprintf_s(szPath,"%s/%02d_%02d_%02d.jpg%02d",path,lt.wHour,lt.wMinute,lt.wSecond,lt.wMilliseconds);
		}
		else
		{
			sprintf_s(szPath,"%s/%s",path,SNstr.c_str());
		}
		if (MAXPICNUMEVESCENE <= index)
		{
			return false;
		}
		WriteImage(m_pImage[index], "jpg", 0, szPath);//0��ʶ��ɫ
	}
	catch (...)
	{
		LOG_ERR("����ͼƬʧ��!");
		return false;
	}
	return true;
} 
/**********************************************************************
����      : SaveImagePath
����      : ����ͼ��·��
�������  : index: �����ĵڼ���ͼƬ��������ʱû��
path : ����·��
outpath�����ͼ�񱣴�·��
SNstr��SNstr��
type: ���ͼƬ����ԭʼͼƬ 
�������  : N/A
����ֵ    : bool
��ע      : �Ƿ���ֻ��������һ��ͼƬ���Է����̿ռ����
�޸ļ�¼��
����        �޸���                  �޸�����
2016-08-16  guopengzhou            visionProcessʵ������ͼƬ�������·��,�Ա�ͨѶЭ���ȡ

**********************************************************************/
bool CVisionProcess::saveImagePath(int index,char *path, string SNstr ,unsigned int type)
{
	try 
	{
		if (-1 == access(path, 0))//�ж�·���Ƿ���Ч
		{
			return false;
		}
		if (0 == type)
		{
			memcpy(resultImgPath,path,strlen(path));
			resultImgPath[strlen(path)] = '\0';
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
} 

/**********************************************************************
����      : getImagePath
����      : �õ�����ͼ��·��
�������  : index: �����ĵڼ���ͼƬ��������ʱû��
path : ����·��
outpath�����ͼ�񱣴�·��
SNstr��SNstr��
type: ���ͼƬ����ԭʼͼƬ 
�������  : N/A
����ֵ    : char* ͼƬ·��
��ע      : �Ƿ���ֻ��������һ��ͼƬ���Է����̿ռ����
�޸ļ�¼��
����        �޸���                  �޸�����
2016-08-16  guopengzhou            visionProcessʵ������ͼƬ�������·��,�Ա�ͨѶЭ���ȡ

**********************************************************************/
char* CVisionProcess::getImagePath(int index ,unsigned int type)
{

	// try 
	// {
	if (-1 == access(resultImgPath, 0))//�ж�·���Ƿ���Ч
	{
		return NULL;
	}

	// }
	//catch (...)
	//{
	//    return NULL;
	//}
	if (index >= 8)
	{
		return NULL;
	}
	if (0 == type)///������ͼƬ·��
	{
		return resultImgPath;
	}
	return NULL;

} 
/**********************************************************************
����      : SaveWinImage
����      : CVisionProcess��̬����,����ͼ��
�������  : winID: ��ǰ����ID
path : ����·��
outpath�����ͼ�񱣴�·��
SNstr��SNstr��
�������  : N/A
����ֵ    : �ɹ���ʧ��
��ע      : �Ƿ���ֻ��������һ��ͼƬ���Է����̿ռ����
�޸ļ�¼��
����        �޸���                  �޸�����
2016-08-16  guopengzhou            visionProcessʵ������ͼƬ�������·��,�Ա�ͨѶЭ���ȡ

**********************************************************************/
bool CVisionProcess::SaveWinImage( long winID,char *path,char *outpath,string SNstr )
{
	bool bRet = false;
	SYSTEMTIME lt;
	char szPath[MAX_PATH] = {0};

	strcpy_s(szPath, MAX_PATH, path);
	try
	{
		GetLocalTime(&lt);
		if (-1 == access(szPath, 0))
		{
			_mkdir(szPath);
		}

		if(SNstr.empty())
		{
			sprintf_s(szPath,"%s\\%02d_%02d_%02d.jpg",szPath,lt.wHour,lt.wMinute,lt.wSecond);
		}
		else
		{
			sprintf_s(szPath,"%s\\%02d_%02d_%02d_%s.jpg",szPath,lt.wHour,lt.wMinute,lt.wSecond,SNstr.c_str());
		}
		HalconCpp::DumpWindow(winID, "jpg", szPath);	
		memcpy(outpath,szPath,sizeof(szPath));	
		bRet = true;
	}
	catch (...)
	{
		bRet = false;
	}
	return bRet;
}

void* CVisionProcess::GetWinImage( long winID,double &width,double &heigh )
{
	HObject himage;
	char *buff = NULL;
	HTuple hWidth = 0;
	HTuple hHeight = 0;
	HTuple ImagePointer = 0;
	try
	{
		DumpWindowImage(&himage,winID);
		GetImageSize(himage, &hWidth, &hHeight);
		GetImagePointer1(himage,&ImagePointer, NULL, NULL, NULL);//��������Ϊ����ͼ�������,��ȣ��߶�
		buff = const_cast<char*>((char*)((int)(ImagePointer)));
		width = hWidth;
		heigh = hHeight;
	}
	catch (...)
	{
		return buff;
	}

	return buff;
}
bool CVisionProcess::addAffineData(const char *sceneName, AF_POINT apoint, AF_POINT bpoint,int index,int CameraID, bool bSave)
{
	bool bRet = false;
	if (index > m_iValidSize)
	{
		if (index >= CALIB_DATA_MAX_SIZE)
		{
			return false;
		}
		m_iValidSize = index + 1;
	}
	m_PixelArray[index].x = apoint.x;
	m_PixelArray[index].y = apoint.y;
	m_RealArray[index].x = bpoint.x;
	m_RealArray[index].y = bpoint.y;
	// try
	//{
	if (0 <= CameraID && bSave)
	{
		creat_affine(sceneName, m_PixelArray, m_RealArray, CameraID);
		bRet = true;
	}
	// }
	// catch (Halcon::HException  &hdev_exception)
	// {
	//    LOG_INFO("%s", hdev_exception.message);
	// }
	return bRet;
}

int  CVisionProcess::readAffineData(int CameraID, char *sceneName)
{
	char buf[MAX_PATH] = {0};
	char exePath[MAX_PATH] = {0};
	bool    bResult = false;

	HTuple ID = CameraID;
	HTuple Path;
	HTuple ErrorCode, message;
	char szBuf[BUF_SIZE] = {0};
	HTuple OutValue, Lenght;

	////////////////////////////////
	HTuple InputValue;

	// try
	//{
	DWORD dwLen = GetModuleFileName(NULL, exePath, sizeof(exePath));
	std::string strCommonPath(exePath);
	std::string strExeDir(exePath);
	size_t lastSpit = strCommonPath.find_last_of("\\");
	sprintf_s(buf,"\\%s\\hdvp",sceneName);
	strCommonPath.replace(lastSpit, string::npos, buf);//���صĹ��̴���·��
	strExeDir.replace(lastSpit, string::npos, "");
	Path = strExeDir.c_str();
	InputValue.Append(ID);
	InputValue.Append(Path);
	ToolProcProcess(sceneName, m_hdvAffineProc, -1, InputValue, "read_camera_calib", OutValue, ErrorCode, 0, false);

	if (0 <= ErrorCode)
	{
		//tuple_length(OutValue, &Lenght);
		// tuple_select_range(OutValue, 0, Lenght - 4, &m_HomMat2D);
		m_AngleOffset = OutValue[Lenght - 3];
		m_Direction = OutValue[Lenght - 2];
		m_Dispix = OutValue[Lenght - 1];
		bResult = true;
	}
	else
	{
		bResult = false;
	}
	//��ȡʱ�䵥λΪ��	
	//}
	//catch (HException  &exception)
	//{
	//m_vecMsg.Append(exception.message);
	//LOG_ERR("%s:%s\n",exception.proc, exception.message);
	//bResult = false;
	//}	
	//catch(HDevEngineException &hdev_exception)
	//{
	//	m_vecMsg.Append(hdev_exception.Message());
	//	LOG_ERR("%s %d:%s\n",hdev_exception.ProgLineName(),hdev_exception.ProgLineNum(),
	//		hdev_exception.Message());
	//	bResult = false;
	//}
	return bResult;
}
HTuple  CVisionProcess::GetAffineData()
{
	return m_HomMat2D;
}
int  CVisionProcess::SetAffineData(HTuple HomMat2D)
{
	m_HomMat2D = HomMat2D;
	return 0;
}

int  CVisionProcess::SetAngleOffset(double dAngle)
{
	m_MotorAngle = dAngle;
	return 0;
}

bool  CVisionProcess::creat_affine(const char *sceneName, AF_POINT *apoint, AF_POINT *bpoint,int CameraID)
{
	HTuple  AX, AY, ROW, COL;

	HTuple  ID = CameraID;// = ID;
	HTuple  Path;

	bool    bResult = false;

	HTuple InputValue;
	HTuple ErrorCode, message, OutValue;
	HTuple Lenght;
	//HDevProcedure m_hdvProc;
	//HDevEngine m_HEngine;
	char szBuf[BUF_SIZE] = {0};

	////////////////////////////////
	//  try
	// {
	char exePath[MAX_PATH] = {0};
	DWORD dwLen = GetModuleFileName(NULL, exePath, sizeof(exePath));
	std::string strExeDir(exePath);
	size_t lastSpit = strExeDir.find_last_of("\\");
	strExeDir.replace(lastSpit, string::npos, "");
	Path = strExeDir.c_str();

	//InputValue = m_iValidSize;
	InputValue.Append(ID);
	InputValue.Append(Path);
	for (int i = 0;i <= m_iValidSize;i++)
	{
		AX.Append(m_RealArray[i].x);
		AY.Append(m_RealArray[i].y);
		COL.Append(m_PixelArray[i].x);
		ROW.Append(m_PixelArray[i].y);
	}
	InputValue.Append(ROW);
	InputValue.Append(COL);
	InputValue.Append(AX);
	InputValue.Append(AY);
	ToolProcProcess(sceneName, m_hdvCAffineProc, -1, InputValue, "camera_calib", OutValue, ErrorCode, 0, false);

	if (0 <= ErrorCode)
	{
		// tuple_length(OutValue, &Lenght);
		// tuple_select_range(OutValue, 0, Lenght - 4, &m_HomMat2D);
		m_AngleOffset = OutValue[Lenght - 3];
		m_Direction = OutValue[Lenght - 2];
		m_Dispix = OutValue[Lenght - 1];
		bResult = true;
	}
	else
	{
		bResult = false;
	}   
	// }
	// catch (HException  &exception)
	// {
	//m_vecMsg.Append(exception.message);
	//LOG_ERR("%s:%s\n",exception.proc, exception.message);
	//   bResult = false;
	// }	
	// catch(HDevEngineException &hdev_exception)
	//{
	//m_vecMsg.Append(hdev_exception.Message());
	// LOG_ERR("%s %d:%s\n",hdev_exception.ProgLineName(),hdev_exception.ProgLineNum(),
	//  hdev_exception.Message());
	// bResult = false;
	//}
	return bResult;
}

int CVisionProcess::affine_apply(int ID,char *sceneName, AF_POINT &apoint, AF_POINT &stOutPoint)
{
	int     iResult = -1;
	HTuple  InputRow, InputCol, InputA;
	HTuple  OutputX, OutputY, OutputA;

	////////////////////////////////
	//try
	//  {
	if (-1 == m_HomMat2D)
	{
		/*  try
		{
		readAffineData(ID, sceneName);
		}
		catch (...)
		{
		m_HomMat2D = -1;
		}*/
	}
	//ת��ǰ����
	InputCol = apoint.x;
	InputRow = apoint.y;
	InputA = apoint.angle;

	//affine_trans_point_2d(m_HomMat2D, InputCol, InputRow, &OutputX, &OutputY);

	OutputA = InputA * m_Direction + m_AngleOffset;

	stOutPoint.x = OutputX[0].D();
	stOutPoint.y = OutputY[0].D();
	stOutPoint.angle = OutputA[0].D();
	stOutPoint.dis_pix = OutputA[0].D();
	iResult = 0;
	//  }
	//catch (HException  &exception)
	//{
	//    //m_vecMsg.Append(exception.message);
	//    //LOG_ERR("%s:%s\n",exception.proc, exception.message);
	//    iResult = -1;
	//}	
	if (iResult < 0)
	{
		stOutPoint.x = apoint.x;
		stOutPoint.y = apoint.y;
		stOutPoint.angle = apoint.angle;
	}
	return iResult;
}

//int CVisionProcess::buildModel(int iWinID, ST_RECTANGLE_DATA &modelroi,char *sceneName,char *path)
int CVisionProcess::buildModel(int iWinID, VIRSION_DATA_PKG *pData, int iLength, unsigned int *pType,char *sceneName,char *path)
{
	HTuple InputValue;

	HTuple OutValue, ErrorCode;
	HObject object;
	int iModelID = -1;
	//try
	//{
	InputValue.Append(HTuple(path));
	InputValue.Append(HTuple(iWinID));
	for (int i = VIRSION_DATA_PKG_HEADER_SIZE; i < iLength + VIRSION_DATA_PKG_HEADER_SIZE; i++)
	{
		int iType = pType[i]%100;
		switch (iType)
		{
		case VISION_INT_TYPE:
			{
				InputValue.Append(HTuple(pData[i].iValue));
			}
			break;
		case VISION_DOUBLE_TYPE:
			{
				InputValue.Append(HTuple(pData[i].dValue));
			}
			break;
		default:
			break;
		}

	}
	int iNPos = FindSpecTypePos(pData, pType, SPEC_TYPE);
	int iModelType = pData[iNPos].iValue;
	char szBuf[MAX_PATH] = {0};
	HDevProcedure *BModelProc;
	switch(iModelType)
	{
	case NORMAL_MODEL:
		strncpy(szBuf, "build_model", MAX_PATH - 1);
		BModelProc = &m_hdvBModelProc;
		break;
	case NCC_MODEL:
		strncpy(szBuf, "build_ncc_model", MAX_PATH - 1);
		BModelProc = &m_hdvBNccModelProc;
		break;
	case MANUAL_MODEL:
		strncpy(szBuf, "build_manual_model", MAX_PATH - 1);
		BModelProc = &m_hdvBManualModelProc;
		break;
	default:
		strncpy(szBuf, "build_model", MAX_PATH - 1);
		BModelProc = &m_hdvBModelProc;
		break;
	}
	if (ToolProcProcess(sceneName, *BModelProc, m_index, InputValue, szBuf, OutValue, ErrorCode, 0, false))
	{
		m_modelid = OutValue;
		iModelID = m_modelid[0].I();
	}

	// }
	// catch (HException  &exception)
	//{
	// LOG_ERR("%s:%s\n",exception.proc, exception.message);
	//m_modelid = -1;
	//}
	//catch(HDevEngineException &hdev_exception)
	//{
	//    LOG_ERR("%s %d:%s\n",hdev_exception.ProgLineName(),hdev_exception.ProgLineNum(),
	//        hdev_exception.Message());
	//    m_modelid = -1;
	//}
	return iModelID;
}

bool CVisionProcess::buildModel(int iWinID,int RoiID,char* modePath, int Nevels,double AngleStart,double AngleExtent,
	int Contrast,int index)
{

	HTuple hv_Row,hv_Col,hv_Angle,hv_Score;
	HObject ho_ModContours,ho_AffineContours;
	HTuple hv_Hommat2D;
	HObject ho_ReduceImage;
	HObject ho_ModeContours;
	//HTuple hv_ModleID;
	int ShapeIndex = -1;
	for (int i= 0;i<m_ShapeList.size();i++)
	{
		if (RoiID == m_ShapeList.at(i)->m_ShapeId)
		{
			ShapeIndex = i;
			break;
		}
	}
	if (-1 == ShapeIndex)
	{
		return false;
	}
	HObject ho_Image = m_pImage[index];
	HObject ho_Roi = m_ShapeList.at(ShapeIndex)->m_ShapeObj;
	try
	{

		ReduceDomain(m_pImage[index],m_ShapeList.at(ShapeIndex)->m_ShapeObj,&ho_ReduceImage);
		DispImage(iWinID);
		CreateShapeModel(ho_ReduceImage,Nevels,AngleStart*3.1416/180,AngleExtent*3.1416/180,"auto","auto","use_polarity",Contrast,"auto",&m_HvMatchModID);
		if ( 0 < m_HvMatchModID.Length())
		{
			//��ģ�����,���� ��-0.1��0.2��ģ��
			FindShapeModel(m_pImage[index],m_HvMatchModID,-0.1,0.2,0.5,1,0.5,"least_squares_high",0,0.9,&hv_Row,&hv_Col,&hv_Angle,&hv_Score);
			if (hv_Score.TupleLength()>0)
			{
				GetShapeModelContours(&ho_ModContours,m_HvMatchModID,1);
				VectorAngleToRigid(0,0,0,hv_Row,hv_Col,hv_Angle,&hv_Hommat2D);
				AffineTransContourXld(ho_ModContours,&ho_AffineContours,hv_Hommat2D);
				HalconCpp::DispObj(ho_AffineContours,iWinID); 
			}
			//
			LOG_INFO("дģ�嵽�ļ�(%s)",modePath);
			WriteShapeModel(m_HvMatchModID,modePath);
			ClearShapeModel(m_HvMatchModID);
			m_HvMatchModID.Clear();

		}

	}
	catch(HException e)
	{
		return false;
	}
	return true;
}


bool CVisionProcess::MatchModel(int winId,int index,char* szModePath,int PyramidLevel,int MatchNum,double& Score,
	int ProcIndex,bool IsBulid,double& row,double& col,double& phi)
{
	//���ò���ProcIndex���볡���б��еĴ������

	//���ò���IsBulid���ж��Ƿ����½�ģ���ƥ��  true ��ʾ�½�ģ���ƥ�䣬false��ʾ���½�ģ���ƥ��

	HTuple hv_Row,hv_Col,hv_Angle,hv_Score;
	HTuple hv_AngStart,hv_AngExtent,hv_Tmept;
	HObject ho_ModContours,ho_AffineContours;
	HTuple hv_Hommat2D;
	HTuple hv_ScaleR,hv_ScaleC;
	HObject ho_ImageTemp;
	HObject ho_Cross;
	HTuple ImageWidth,ImageHeight;
	try
	{
		if(m_HvMatchModID.TupleLength()<=0)
		{	
			ReadShapeModel(szModePath,&m_HvMatchModID);
		}
		GetShapeModelParams(m_HvMatchModID,&hv_Tmept,&hv_AngStart,&hv_AngExtent,&hv_Tmept,&hv_Tmept,&hv_Tmept,&hv_Tmept,&hv_Tmept,&hv_Tmept);
		GetShapeModelContours(&ho_ModContours,m_HvMatchModID,1);
		//FindShapeModel(m_pImage[index],m_HvMatchModID,hv_AngStart,hv_AngExtent,Score,MatchNum,0.7,"least_squares",PyramidLevel,0.9,&hv_Row,&hv_Col,&hv_Angle,&hv_Score);
		ho_ImageTemp = m_pImage[index];
		FindAnisoShapeModel(ho_ImageTemp,m_HvMatchModID,hv_AngStart+0.01,hv_AngExtent-0.02,0.95,1.05,0.95,1.05,Score,MatchNum,0.7,"least_squares",PyramidLevel,0.9,
			&hv_Row,&hv_Col,&hv_Angle,&hv_ScaleR,&hv_ScaleC,&hv_Score);

		if (hv_Score.TupleLength()<=0)
		{
			LOG_ERR("����ģ���ƥ��ģ��ʧ��!");
			return false;
		}
		//������´���������£���Ҫ����ƥ������ݸ���ԭʼƥ������,���浽�����ļ��С�
		m_CurMatchTypeProcIndex = ProcIndex;
		m_CurMatchRow[m_CurMatchTypeProcIndex] = hv_Row;
		m_CurMatchCol[m_CurMatchTypeProcIndex] = hv_Col;
		m_CurMatchPhi[m_CurMatchTypeProcIndex] = hv_Angle;

		if (IsBulid)
		{
			m_OrgionMatchRow[m_CurMatchTypeProcIndex] = hv_Row;
			m_OrgionMatchCol[m_CurMatchTypeProcIndex] = hv_Col;
			m_OrgionMatchPhi[m_CurMatchTypeProcIndex] = hv_Angle;
		}else
		{
			//��������´���������������ת������
			//int ProcIndex = static_cast<int>(row);
			m_OrgionMatchRow[m_CurMatchTypeProcIndex] = row;
			m_OrgionMatchCol[m_CurMatchTypeProcIndex] = col;
			m_OrgionMatchPhi[m_CurMatchTypeProcIndex] = phi;


			HTuple Orow = row;
			HTuple OCol = col; 
			HTuple OAng = phi;


			HTuple hv_MatchHomm2D;//�����½�ģ��Ͱ�ǰ�ε�ģ�����ĸ���εĵõ�һ������
			VectorAngleToRigid(Orow,OCol,OAng,hv_Row,hv_Col,hv_Angle,&hv_MatchHomm2D);
			for (int i = (int)m_MatchHomm2DList.size();i<=m_CurMatchTypeProcIndex;i++)
			{
				m_MatchHomm2DList.push_back(hv_MatchHomm2D);
			}
			m_MatchHomm2DList.at(m_CurMatchTypeProcIndex) = hv_MatchHomm2D;
		}

		VectorAngleToRigid(0,0,0,hv_Row,hv_Col,hv_Angle,&hv_Hommat2D);//��000����ε�ģ�彨������
		AffineTransContourXld(ho_ModContours,&ho_AffineContours,hv_Hommat2D);//��ģ���������ݾ�����ӳ��,�Ա���ʾ
		SetDispColorHeight(winId,"red",-1);
		HalconCpp::DispObj(ho_AffineContours,winId);//��ʾ�ڴ�����

		HalconCpp::GetImageSize(m_pImage[index],&ImageWidth,&ImageHeight);//�õ�ͼ��ߴ�
		GenCrossContourXld(&ho_Cross,hv_Row,hv_Col,ImageHeight/8,hv_Angle);//����һ���нǶȵ�ʮ��,ʮ�ֳ���Ϊ8��֮�߶�
		HalconCpp::DispObj(ho_Cross,winId);

		row = hv_Row;
		col = hv_Col;
		phi = hv_Angle;
		Score = hv_Score;
	}
	catch(HException e)
	{
		return false;
	}
	return true;
}
bool CVisionProcess::FindModelRealTime(int winId,int index,char* szModePath,int PyramidLevel,int MatchNum,double& Score,
	int ProcIndex,bool IsBulid,double& row,double& col,double& phi)
{
	HTuple hv_Row,hv_Col,hv_Angle,hv_Score;
	HTuple hv_AngStart,hv_AngExtent,hv_Tmept;
	HObject ho_ModContours,ho_AffineContours;
	HTuple hv_Hommat2D;
	HTuple hv_ScaleR,hv_ScaleC;
	HObject ho_ImageTemp;
	HObject ho_Cross,ho_CenterCross;
	HTuple ImageWidth,ImageHeight;

	try
	{
		ho_ImageTemp = m_pImage[index];
		ClearWindow(winId);
		HalconCpp::DispObj(ho_ImageTemp,winId);//��ʾ�ڴ�����



		if (ProcIndex== false)
		{
			return false;
		}
		//������ʾ��ɫʮ�ֲ�
		HalconCpp::GetImageSize(m_pImage[index],&ImageWidth,&ImageHeight);//�õ�ͼ��ߴ�
		double we =0,le =0;
		GetWinImage(winId,we,le);

		SetDispColorHeight(winId,"green",-1);
		HTuple a = we/2.0,b=le/2.0;
		a = ImageWidth/2.0,b=ImageHeight/2.0;
		
		//HTuple cc,dd,ee,ff;
		//GetWindowExtents(winId,&cc,&dd,&ee,&ff);

		GenCrossContourXld(&ho_CenterCross,b,a,ImageWidth/8,0);//����ʮ�ֲ�

		HalconCpp::DispObj(ho_CenterCross,winId);
		if(m_HvMatchModID.TupleLength()<=0)
		{	
			ReadShapeModel(szModePath,&m_HvMatchModID);//m_HvMatchModID�ڳ�ʼ��ʱӦ�ö�����
		}
		GetShapeModelParams(m_HvMatchModID,&hv_Tmept,&hv_AngStart,&hv_AngExtent,&hv_Tmept,&hv_Tmept,&hv_Tmept,&hv_Tmept,&hv_Tmept,&hv_Tmept);
		GetShapeModelContours(&ho_ModContours,m_HvMatchModID,1);
		FindAnisoShapeModel(ho_ImageTemp,m_HvMatchModID,hv_AngStart+0.01,hv_AngExtent-0.02,0.95,1.05,0.95,1.05,Score,MatchNum,0.7,"least_squares",PyramidLevel,0.9,
			&hv_Row,&hv_Col,&hv_Angle,&hv_ScaleR,&hv_ScaleC,&hv_Score);
		if (hv_Score.TupleLength()<=0)
		{
			LOG_ERR("ƥ��ģ��ʧ��!");
			return false;
		}
		//������´���������£���Ҫ����ƥ������ݸ���ԭʼƥ������,���浽�����ļ��С�
		VectorAngleToRigid(0,0,0,hv_Row,hv_Col,hv_Angle,&hv_Hommat2D);//��000����ε�ģ�彨������
		AffineTransContourXld(ho_ModContours,&ho_AffineContours,hv_Hommat2D);//��ģ���������ݾ�����ӳ��,�Ա���ʾ
		SetDispColorHeight(winId,"red",-1);
		HalconCpp::DispObj(ho_AffineContours,winId);//��ʾ�ڴ�����


		GenCrossContourXld(&ho_Cross,hv_Row,hv_Col,ImageHeight/8,hv_Angle);//����һ���нǶȵ�ʮ��,ʮ�ֳ���Ϊ8��֮�߶�
		HalconCpp::DispObj(ho_Cross,winId);

		HTuple c = hv_Row.ToString();
		HTuple l = hv_Col.ToString();
		HTuple str = "��������:"+c + "," + l;
		ShowWindString(winId,str);
		row = hv_Row;
		col = hv_Col;
		phi = hv_Angle;
		Score = hv_Score;
	}
	catch(HException e)
	{
		return false;
	}
	return true;
}

void  CVisionProcess::MoveAndRotInCamForOriMatch(double PixSize,bool AxeMoveRotDirect,double& MotorRelX,double& MotorRelY,
	int LastSceneId,int CurSceneId,int NextSceneId,double SceneRotPhi,double PicCentX,double PicCentY,double RotCenX,double RotCenY,double CamRotPhi,double CalRotPhi,
	double (*pCamParam),int ParamNum,double (*pCamPose),double CalThickness,int CamIndex,int AxeIndex,
	int ToolIndex)
{
	//CalRotPhi��ָ�о���ʵ����ҵ����ʱ��궨ʱ����ת�Ƕȡ�

	// *** ��Ҫ���������껻���ʵ�ʵ�λ���꣬��Ϊת���ڲ�Ʒ�е���������ʵ�ʵ�λ�ġ�
	HTuple hv_Hommat2d,hv_CurToOrHom;
	HTuple CamRotCenX,CamRotCenY;
	HTuple CurRotX, CurRotY;
	HTuple hv_CamParam,hv_CamPose;
	HTuple hv_TempX,hv_TempY;
	double OrMatchX,OrMatchY ; //use at last step for minus 
	double CurMatchX,CurMatchY ;
	HomMat2dIdentity(&hv_Hommat2d);//����������λ����
	HomMat2dIdentity(&hv_CurToOrHom);
	if (1 == PixSize)//�ñ궨��
	{
		for (int i = 0;i<ParamNum;i++)
		{
			hv_CamParam[i] = pCamParam[i];//�ڲ�12��

		}
		for (int i = 0;i<7;i++)
		{
			hv_CamPose[i] = pCamPose[i];//���6������7 ����Ч
		}

		SetOriginPose(hv_CamPose,0,0,-CalThickness,&hv_CamPose);//���ݱ궨������У�����
		ImagePointsToWorldPlane(hv_CamParam,hv_CamPose,m_OrgionMatchRow[m_CurMatchTypeProcIndex],m_OrgionMatchCol[m_CurMatchTypeProcIndex],
			"mm", &hv_TempX,&hv_TempY );//���ݱ궨���֤��У��process��ԭʼֵ�͵�ǰֵ
		OrMatchX = hv_TempX;
		OrMatchY = hv_TempY;
		ImagePointsToWorldPlane(hv_CamParam,hv_CamPose,m_CurMatchRow[m_CurMatchTypeProcIndex],m_CurMatchCol[m_CurMatchTypeProcIndex],
			"mm", &hv_TempX,&hv_TempY );
		CurMatchX = hv_TempX;
		CurMatchY = hv_TempY;

	}else if (2 == PixSize)//ͼ������궨���������
	{
		HTuple hv_OriMatchX,hv_OriMatchY;
		HTuple hv_CurMatchX,hv_CurMatchY;	
		HTuple hv_OriToCurMetix;
		double RotPhi = CamRotPhi;
		if (CamIndex<0||AxeIndex<0||ToolIndex<0)
		{
			MotorRelX = 0.0;
			MotorRelY = 0.0;
			return;
		}
		//m_ImageToMotorRelative
		//AffineTransPoint2d(m_ImageToMotorRelative[CamIndex][AxeIndex][ToolIndex],m_OrgionMatchRow[m_CurMatchTypeProcIndex],
		//	m_OrgionMatchCol[m_CurMatchTypeProcIndex],&hv_OriMatchX,&hv_OriMatchY);
		AffineTransPoint2d(m_ImageToMotorRelative[CamIndex][AxeIndex][ToolIndex],PicCentX-m_CurMatchRow[m_CurMatchTypeProcIndex],
			PicCentY- m_CurMatchCol[m_CurMatchTypeProcIndex],&hv_CurMatchX,&hv_CurMatchY);
		//HomMat2dIdentity(&hv_OriToCurMetix);
		//HomMat2dRotate(hv_OriToCurMetix,RotPhi,hv_OriMatchX,hv_OriMatchY,&hv_OriToCurMetix) ;
		MotorRelX = hv_CurMatchX ;
		MotorRelY = hv_CurMatchY ;

		return;
	}else if (3 == PixSize) //ͼ��������꣬���ǰѱ궨�ľ������ȥ������
	{
		HTuple hv_OriAbsX,hv_OriAbsY;
		HTuple hv_CurAbsX,hv_CurAbsY;
		HTuple hv_RotPhi =0.0;
		HTuple hv_HomOriToCur;

		AffineTransPoint2d(m_ImageToMotorAbsolute[CamIndex][AxeIndex][ToolIndex],m_OrgionMatchRow[m_CurMatchTypeProcIndex],
			m_OrgionMatchCol[m_CurMatchTypeProcIndex],&hv_OriAbsX,&hv_OriAbsY);
		AffineTransPoint2d(m_ImageToMotorAbsolute[CamIndex][AxeIndex][ToolIndex],m_CurMatchRow[m_CurMatchTypeProcIndex],
			m_CurMatchCol[m_CurMatchTypeProcIndex],&hv_CurAbsX,&hv_CurAbsY);
		MotorRelX = hv_CurAbsX;
		MotorRelY =hv_CurAbsY;


		//int NextSceneID =NextSceneId;

		//if(CurSceneId>=0&&NextSceneId>=0&&CurSceneId!=NextSceneId)
		//{
		//	HTuple hv_DistX = hv_CurAbsX - hv_OriAbsX ;
		//	HTuple hv_DistY = hv_CurAbsY - hv_OriAbsY ;

		//	hv_RotPhi = m_CurMatchPhi[m_CurMatchTypeProcIndex]-m_OrgionMatchPhi[m_CurMatchTypeProcIndex];
		//	//
		//	HomMat2dIdentity(&hv_HomOriToCur);
		//	HomMat2dRotateLocal(hv_HomOriToCur,hv_RotPhi,&hv_HomOriToCur);
		//	HomMat2dRotateLocal(hv_HomOriToCur,SceneRotPhi,&hv_HomOriToCur);

		//	AffineTransPoint2d(hv_HomOriToCur,hv_DistX,hv_DistY,&hv_TempX,&hv_TempY);
		//	HomMat2dTranslate(hv_HomOriToCur,hv_TempX,hv_TempY,&(m_ScenePosPass[CurSceneId][NextSceneID]));
		//}

		//if (LastSceneId>=0&&CurSceneId>=0&&LastSceneId!=CurSceneId)
		//{
		//	AffineTransPoint2d(m_ScenePosPass[LastSceneId][CurSceneId],MotorRelX,MotorRelY,&hv_TempX,&hv_TempY);
		//	MotorRelX = hv_TempX;
		//	MotorRelY =hv_TempY;
		//}
		return;
	}
	else //��Դ���ת ������ת  ��������(������) ������
	{
		OrMatchX = m_OrgionMatchCol[m_CurMatchTypeProcIndex] * PixSize; //�����ر�����س����ش�С
		OrMatchY = m_OrgionMatchRow[m_CurMatchTypeProcIndex] * PixSize;
		CurMatchX = m_CurMatchCol[m_CurMatchTypeProcIndex] * PixSize;
		CurMatchY = m_CurMatchRow[m_CurMatchTypeProcIndex] * PixSize;
	}
	//��ת������任���������ϵ�У�CamRotPhiΪ���ڵļ���ģ��ĽǶ�
	double CoordPhi =  -CamRotPhi;

	HomMat2dRotateLocal(hv_Hommat2d,CoordPhi,&hv_Hommat2d);      //
	HomMat2dRotateLocal(hv_Hommat2d,CalRotPhi,&hv_Hommat2d); //CalRotPhi��ָ��궨ʱ�ĽǶ�ƫ�����Ƕ�
	HomMat2dTranslate(hv_Hommat2d,CurMatchX,CurMatchY,&hv_Hommat2d);
	AffineTransPoint2d(hv_Hommat2d,RotCenX,RotCenY,&CamRotCenX,&CamRotCenY);

	HomMat2dRotate(hv_CurToOrHom, -CoordPhi,CamRotCenX,CamRotCenY,&hv_CurToOrHom); //CoordPhiȡ����Ϊ��У���ص����������ϵƽ�еĽǶȵķ�����ͬ��
	//��ȡ��ǰƥ����ת������ϵ����
	AffineTransPoint2d(hv_CurToOrHom,CurMatchX,CurMatchY,&CurRotX,&CurRotY);
	double Test3= CurRotX;
	double Test4= CurRotY;

	//���ڲ�����ת�ģ�CamRotPhiΪ0������任����Ӱ��,������ǵ�ǰֵ��ȥԭʼֵ,�����Ƕ���ת��Ҳ��Ҫ����

	MotorRelX = CurRotX - OrMatchX;
	MotorRelY = CurRotY - OrMatchY;

}




int CVisionProcess::clearModel(int iModelID, int iType)
{
	Herror error = -1;
	// try
	//  {
	switch (iType)
	{
	case NORMAL_MODEL:
	case MANUAL_MODEL:
		{
			// error = clear_shape_model(iModelID);
			ClearShapeModel(iModelID);
		}
		break;
	case NCC_MODEL:
		{
			//error = clear_ncc_model(iModelID);
			ClearNccModel(iModelID);
		}
	}
	//  }
	//catch (HException  &hdev_exception)
	//{
	//   // LOG_INFO("%s", hdev_exception.message);
	//}
	return error;
}

int  CVisionProcess::readModel(char *sceneName,char *path, int index, int iType)
{
	HTuple ErrorCode, message;

	HTuple InputValue;

	HTuple OutValue;
	//HDevProcedure m_hdvProc;
	//HDevEngine m_HEngine;
	int iModel = -1;

	//try
	//{
	char szBuf[MAX_PATH] = {0};
	HDevProcedure *RModelProc;
	switch(iType)
	{
	case NORMAL_MODEL:
	case MANUAL_MODEL:
		strncpy(szBuf, "read_model", MAX_PATH - 1);
		RModelProc = &m_hdvRModelProc;
		break;
	case NCC_MODEL:
		strncpy(szBuf, "read_nccpro_model", MAX_PATH - 1);
		RModelProc = &m_hdvRNccModelProc;
		break;
	default:
		strncpy(szBuf, "read_model", MAX_PATH - 1);
		RModelProc = &m_hdvRModelProc;
		break;
	}
	InputValue.Append(HTuple(path));

	ToolProcProcess(sceneName, *RModelProc, -1, InputValue, szBuf, OutValue, ErrorCode, 0, false);

	if (0 <= ErrorCode)
	{
		m_modelid[index] = OutValue[0];
		iModel = OutValue[0].I();
	}
	else
	{
		m_modelid[index] = -1;
	}
	//}
	//catch (HException  &exception)
	//{
	//   // m_vecMsg.Append(exception.message);
	//   // LOG_ERR("%s:%s\n",exception.proc, exception.message);
	//    m_modelid[index] = -1;	
	//}	
	//catch(HDevEngineException &hdev_exception)
	//{
	//    m_vecMsg.Append(hdev_exception.Message());
	//    LOG_ERR("%s %d:%s\n",hdev_exception.ProgLineName(),hdev_exception.ProgLineNum(),
	//        hdev_exception.Message());
	//    m_modelid[index] = -1;
	//}
	return iModel;
}

int CVisionProcess::findModel(const char *pSceneName, VIRSION_DATA_PKG *pData, int iLength, unsigned int *pType, ST_RECTANGLE_DATA &rect,unsigned int bDisplay)
{
	HTuple InputValue;
	HTuple OutValue;

	HTuple ErrorCode;
	HTuple row, col, angle;
	int iRet = -1;
	//   try
	//  {
	int iPos = FindSpecTypePos(pData, pType, ID_TYPE);
	for (int i = iPos; i < iLength + VIRSION_DATA_PKG_HEADER_SIZE; i++)
	{
		int iType = pType[i]%100;
		switch (iType)
		{
		case VISION_INT_TYPE:
			{
				InputValue.Append(HTuple(pData[i].iValue));
			}
			break;
		case VISION_DOUBLE_TYPE:
			{
				InputValue.Append(HTuple(pData[i].dValue));
			}
			break;
		default:
			break;
		}
	}
	int iNPos = FindSpecTypePos(pData, pType, SPEC_TYPE);

	int iModelType = 0;
	if (iNPos >= 0)
	{
		iModelType = pData[iNPos].iValue;
	}
	char szBuf[MAX_PATH] = {0};
	HDevProcedure *FModelProc;
	switch(iModelType)
	{
	case NORMAL_MODEL:
	case MANUAL_MODEL:
		strncpy(szBuf, "find_model", MAX_PATH - 1);
		FModelProc = &m_hdvFModelProc;
		break;
	case NCC_MODEL:
		strncpy(szBuf, "find_nccpro_model", MAX_PATH - 1);
		FModelProc = &m_hdvFNccModelProc;
		break;
	default:
		strncpy(szBuf, "find_model", MAX_PATH - 1);
		FModelProc = &m_hdvFModelProc;
		break;
	}

	ToolProcProcess(pSceneName, *FModelProc, m_index, InputValue, szBuf, OutValue, ErrorCode, bDisplay);

	if (0 <= ErrorCode)
	{
		row = OutValue[0];
		col = OutValue[1];
		angle = OutValue[2];
		//HScore = hdvProcCall.GetOutputCtrlParamTuple("score");
		rect.row = row[0].D();
		rect.col = col[0].D();
		rect.angle = angle[0].D();
		//Score = HScore[0].I();
		iRet = 0;
	}
	// }
	//   catch (HException  &exception)
	//   {
	//      // m_vecMsg.Append(exception.message);
	//       //LOG_ERR("%s:%s\n",exception.proc, exception.message);
	//       iRet = -1;
	//   }	
	//   catch(HDevEngineException &hdev_exception)
	//   {
	//       m_vecMsg.Append(hdev_exception.Message());
	//       LOG_ERR("%s %d:%s\n",hdev_exception.ProgLineName(),hdev_exception.ProgLineNum(),
	//           hdev_exception.Message());
	//       iRet = -1;
	//}
	return iRet;		
}

//bool  CVisionProcess::process(int sceneID,SCENE_PKG &param, CHECK_RESULT &result)
bool  CVisionProcess::process(int sceneID,SCENE_PKG *pkg, CHECK_RESULT &result)
{
	//Input
	HTuple  InputTuple;
	HObject InputObj;

	//OutPut
	HTuple  ErrorCode, OutValue;
	HObject OutputObj;

	//tmp
	HTuple TupleLength = HTuple(0);

	bool bRet = false;

	double dTime = 0.0;

	//try
	//{
	m_bIsAppend = true;
	//  m_vecMsg.Reset();
	bRet = ProcProcess(pkg, OutValue, ErrorCode, dTime);

	result.err = ErrorCode[0].I();
	if (0 <= ErrorCode && bRet)
	{
		int iRets = 0;
		result.index = pkg->sceneID;
		// tuple_length(OutValue, &TupleLength);
		result.iCount = TupleLength[0].I();
		for (int i = 0; i < result.iCount; i++)
		{
			//            if (1 == OutValue[i].ValType())
			//            {
			//                sprintf(result.szValue[i], "%d", OutValue[i].I());
			//            }
			//            else if (2 == OutValue[i].ValType())
			//            {
			//                sprintf(result.szValue[i], "%lf", OutValue[i].D());
			//            }
			//            else if (4 == OutValue[i].ValType())
			//            {
			//                strcpy(result.szValue[i], OutValue[i].S());
			//            }

			//iRets = OutValue[i].ValType();
		}
		result.time = dTime;

		HTuple iNumber;
		// count_obj(m_vecObj, &iNumber);
		//�ڴ������������ʾ�� ����Ҫ�ڳ�������ʾ
		if (0 == iNumber)
		{
			result.bIsDisp = false;
		}

		bRet = true;
	}
	else
	{
		result.err = -1;
	}
	//  }  
	//   catch (HException  &exception)
	//   {
	//      // m_vecMsg.Append(exception.message);
	//     //  LOG_ERR("%s:%s\n",exception.proc, exception.message);
	//       //m_hdvProc.~HDevProcedure();
	//	bRet = false;
	//       result.err = -1;
	//   }
	//   catch(HDevEngineException &hdev_exception)
	//   {
	//       m_vecMsg.Append(hdev_exception.Message());
	//       LOG_ERR("%s %d:%s\n",hdev_exception.ProgLineName(),hdev_exception.ProgLineNum(),
	//           hdev_exception.Message());
	//       bRet = false;
	//       result.err = -1;
	//}

	return bRet;
}

int CVisionProcess::GetCurShapeId(void)
{
	return (m_ShapeList.at(m_ShapeList.size()-1))->m_ShapeId;

}

bool CVisionProcess::GetRoiParam(int ShapeId,ST_ROI_INFO* pRoi)
{
	for (int i = 0;i<m_ShapeList.size();i++)
	{
		if (ShapeId == m_ShapeList.at(i)->m_ShapeId)
		{
			switch (m_ShapeList.at(i)->m_ShapeType)
			{
			case HoRecTangleType:
				{
					pRoi->iType = HoRecTangleType;
					pRoi->strect.row = ((ShapeRect*)m_ShapeList.at(i))->CenRow;
					pRoi->strect.col = ((ShapeRect*)m_ShapeList.at(i))->CenCol;
					pRoi->strect.angle = ((ShapeRect*)m_ShapeList.at(i))->pHi;
					pRoi->strect.length1=((ShapeRect*)m_ShapeList.at(i))->Lenth1;
					pRoi->strect.length2=((ShapeRect*)m_ShapeList.at(i))->Lenth2;
				}
				break;
			case HoCircleType:
				{
					pRoi->iType = HoCircleType;
					pRoi->stcircle.row = ((ShapeCircle*)m_ShapeList.at(i))->CenRow;
					pRoi->stcircle.col = ((ShapeCircle*)m_ShapeList.at(i))->CenCol;
					pRoi->stcircle.Radius = ((ShapeCircle*)m_ShapeList.at(i))->CirRii;
				}
				break;		
			}
			return true;
		}
	}
	return false;
}

void CVisionProcess::ShowShap(const Shape& shapObj,long halconID)
{
	HObject hoShape,hoShape1;
	switch (shapObj.m_ShapeType)
	{
	case HoRecTangleType:
		{
			ShapeRect* prectShape = (ShapeRect*)&shapObj;
			//double row = prectShape->CenRow,col = prectShape->CenCol,phi = prectShape->pHi,len1 = prectShape->Lenth1,len2 = prectShape->Lenth2;
			GenRectangle2ContourXld(&hoShape,prectShape->CenRow,prectShape->CenCol,prectShape->pHi,prectShape->Lenth1,prectShape->Lenth2);
			HalconCpp::DispObj(hoShape,halconID);
		}
		break;
	case HoCircleType:
		{
			ShapeCircle* pCircleShape = (ShapeCircle*)&shapObj;
			GenCircleContourXld(&hoShape,pCircleShape->CenRow,pCircleShape->CenCol,pCircleShape->CirRii,HTuple(0), HTuple(6.28318), HTuple("positive"),HTuple(1));
			HalconCpp::DispObj(hoShape,halconID);
		}
		break;

	}


}






bool  CVisionProcess::OndrawRoi(long  halconID , const  ST_ROI_INFO  &roi,RoiCombType CombType,PROCESS_TYPE bflag,vector<int>* pRoiIDList , bool bcolor )
{
	HTuple row,col,phi,width,height,row2,col2,phi2,width2,height2;
	HTuple crow,ccol,radius,crow2,ccol2,radius2, radiuss, radiusb;
	HObject rectangle,rectangleX,circle,circles, circleb,Arrow;
	HTuple Point1Row,Point1Col,Point2Row,Point2Col,Rows,Cols;
	HTuple angleB,angleE,angleRange,Offset_Angle;
	HObject Objects,ContCircle1,ContCircle2,Contour,Contours;
	Shape* pShape = NULL;
	int CurCombIndex = -1;

	try
	{
		//ClearWindow(halconID);
		//DispImage(halconID, m_index);
		//set_color(halconID, "blue");
		// set_draw(halconID, "margin");
		//set_font(halconID,"-Arial-12-*-*-*-*-*-ANSI_CHARSET-");
		//set_line_width(halconID,1);
		SetDispColorHeight(halconID,"red",-1);
		disp_message(halconID, "�����ROI���򣬵���Ҽ���������","", 0, 0, "black", "true");


		if ( LINE_PRO == bflag)
		{
			//ShapeRect* pHoRect = new ShapeRect(++m_ShapeIdCount);
			pShape = new ShapeRect(++m_ShapeIdCount);

			row = HTuple(roi.strect.row);
			col = HTuple(roi.strect.col);
			phi = HTuple(roi.strect.angle);         
			width = HTuple(roi.strect.length1);
			height = HTuple(roi.strect.length2);
			DrawRectangle2Mod(halconID,row,col,phi,width,height,&row2,&col2,&phi2,&width2,&height2);
			if ( row2 == HTuple() )             //roi���򲻱䣬�򱣴�Ϊԭ������
			{
				row2 = row;
				col2 = col;
				phi2 = phi;
				width2 = width;
				height2 = height;

			}
			if (bcolor)
			{
				// set_color(halconID,"green");
			}

			GenRectangle2(&pShape->m_ShapeObj,row2,col2,phi2,width2,height2);
			GenRectangle2ContourXld(&rectangleX,row2,col2,phi2,width2,height2);
			HalconCpp::DispObj(rectangleX,halconID);

			//outroi.strect.row = row2[0].D();
			//outroi.strect.col = col2[0].D();
			//outroi.strect.length1 = width2[0].D();
			//outroi.strect.length2 = height2[0].D();
			//outroi.strect.angle = phi2[0].D();
			//outroi.iType = 0;
			static_cast<ShapeRect*>(pShape)->CenRow = row2;
			static_cast<ShapeRect*>(pShape)->CenCol = col2;
			static_cast<ShapeRect*>(pShape)->pHi = phi2;
			static_cast<ShapeRect*>(pShape)->Lenth1 = width2;
			static_cast<ShapeRect*>(pShape)->Lenth2 = height2;
			//m_ShapeList.push_back(pHoRect);

		} 
		else if(CIRCLE_PRO == bflag)
		{
			//ShapeCircle* pHoCircle = new  ShapeCircle( ++m_ShapeIdCount);
			pShape = new  ShapeCircle( ++m_ShapeIdCount);
			crow = HTuple(roi.stcircle.row);
			ccol=HTuple(roi.stcircle.col);
			radius=HTuple(roi.stcircle.Radius);
			DrawCircleMod(halconID,crow,ccol,radius,&crow2,&ccol2,&radius2);//�ȴ��Ҽ�����ԭλ�ô�С
			if (crow2 == HTuple())//roi���򲻱䣬�򱣴�Ϊԭ������
			{
				crow2 = crow;
				ccol2 = ccol;
				radius2 = radius;
			}


			if (bcolor)
			{
				//set_color(halconID,"green");
			}
			GenCircle(&pShape->m_ShapeObj, crow2, ccol2,radius2);//����Բ����������һ��Բ������,�ӻ���0��ʼ,����Ϊ2*3124,˳ʱ�ӷ���,��Բʱ������������ص�
			GenCircleContourXld(&circles,crow2, ccol2,radius2,HTuple(0), HTuple(6.28318), HTuple("positive"),HTuple(1)); //
			//HalconCpp::DispObj(circles,halconID);

			//outroi.stcircle.row = crow2[0].D();
			//outroi.stcircle.col = ccol2[0].D();
			//outroi.stcircle.Radius =radius2[0].D();
			//outroi.stcircle.width = roi.stcircle.width;
			//outroi.iType = 1;

			static_cast<ShapeCircle*>(pShape)->CenRow = crow2;
			static_cast<ShapeCircle*>(pShape)->CenCol = ccol2;
			static_cast<ShapeCircle*>(pShape)->CirRii = radius2;

		}
		else if( COMBSHAPE_PRO == bflag)
		{
			//pShape = new ComposeShape(++m_ShapeIdCount);
			ComposeShape* pHoCombShape = new ComposeShape(++m_ShapeIdCount);
			m_ShapeList.push_back(pHoCombShape);
			if (pRoiIDList)
			{
				pRoiIDList->push_back(m_ShapeIdCount);
			}

			ClearWindow(halconID);

			DispImage(halconID, m_index);
		}

		if (NULL == pShape) //����ͼԪָ��Ϊ�գ���������һ����ϣ����򷵻ء�
		{
			return true;
		}

		for (int i = (int)m_ShapeList.size()-1;i>=0;i--) //�����������push��ĩβ��,��β�����ҵ����Ǵ��������µ����
		{
			if( HoComposeType == m_ShapeList.at(i)->m_ShapeType)
			{
				CurCombIndex = i;
				break;
			}
		}

		if (NullCombine == CombType)
		{
			//���ݱ��������߼������������Զ��������������棬�Ա���ɾ������ʱ����ɾ��������Ļ���������ɾ�����������
			m_ShapeList.push_back(pShape);

			ClearWindow(halconID);
			DispImage(halconID, m_index);
			//m_CurSceneWnd.OpenWindow(10, 10, 500,400, halconID,"visible","");
			//HalconCpp::DispObj(pShape->m_ShapeObj,halconID);
			ShowShap(*pShape,halconID);
		}else
		{
			switch (CombType)
			{
				//case NullCombine:
				//	{
				//		//���ݱ��������߼������������Զ��������������棬�Ա���ɾ������ʱ����ɾ��������Ļ���������ɾ�����������
				//		m_ShapeList.push_back(pShape);
				//
				//		ClearWindow(halconID);
				//		DispImage(halconID, m_index);
				//		//m_CurSceneWnd.OpenWindow(10, 10, 500,400, halconID,"visible","");
				//		//HalconCpp::DispObj(pShape->m_ShapeObj,halconID);
				//		ShowShap(*pShape,halconID);
				//		
				//	}
				//	break;
			case RoiUnionType:
				{
					if (m_ShapeList.at(CurCombIndex)->m_ShapeObj.IsInitialized())
					{
						Union2(m_ShapeList.at(CurCombIndex)->m_ShapeObj,pShape->m_ShapeObj,&(m_ShapeList.at(CurCombIndex)->m_ShapeObj));
					}
				}
				break;
			case RoiSubType:
				{
					if (m_ShapeList.at(CurCombIndex)->m_ShapeObj.IsInitialized())
					{
						Difference(m_ShapeList.at(CurCombIndex)->m_ShapeObj,pShape->m_ShapeObj,&(m_ShapeList.at(CurCombIndex)->m_ShapeObj));

					}
				}
				break;
			case RoiInsectionType:
				{
					if (m_ShapeList.at(CurCombIndex)->m_ShapeObj.IsInitialized())
					{
						Intersection(m_ShapeList.at(CurCombIndex)->m_ShapeObj,pShape->m_ShapeObj,&(m_ShapeList.at(CurCombIndex)->m_ShapeObj));				
					}
				}
				break;
			}

			//if (NullCombine != CombType) //����ͼ����ϵ�ʱ��Ż᷵�ػ���ͼԪ��ID����Ϣ
			//{
			if (!m_ShapeList.at(CurCombIndex)->m_ShapeObj.IsInitialized()) //���IDΪ�յ�ʱ�򣬼�������ϵĵ�һ��Ԫ��ʱֱ�Ӹ�ֵ��״��
			{
				m_ShapeList.at(CurCombIndex)->m_ShapeObj = pShape->m_ShapeObj;
			}

			m_ShapeList.push_back(pShape);
			if (pRoiIDList) //������ͼԪ��ID������������С�
			{
				pRoiIDList->push_back(m_ShapeIdCount);
			}
			static_cast<ComposeShape*>(m_ShapeList.at(CurCombIndex))->m_VecComponentId.push_back(pShape->m_ShapeId);
			static_cast<ComposeShape*>(m_ShapeList.at(CurCombIndex))->m_VecCombType.push_back(CombType);
			ClearWindow(halconID);
			DispImage(halconID, m_index);
			HalconCpp::DispObj(m_ShapeList.at(CurCombIndex)->m_ShapeObj,halconID);
			//OnShowAllRoi(halconID);	

			//}
		}

		//m_DrawList.push_back(pShape);			
	}
	catch (...)//HException  &hdev_exception
	{
		return FALSE;
		//    //LOG_INFO("%s", hdev_exception.message);
		//    return false;
	}    
	return true;
}

//int CVisionProcess::GetLastShapeId()
//{
//
//	return  (m_ShapeList.at(m_ShapeList.size()-1))->m_ShapeId;
//
//}




void CVisionProcess::GenShape(const ST_ROI_INFO& roi,ShapeType shapeflag,int ShapeId,int Width,int Height)
{

	//GenImageConst(&m_pImage[0],"byte",Width,Height);

	Shape* pShape = NULL;
	if (m_ShapeIdCount<ShapeId)
	{
		m_ShapeIdCount = ShapeId+1;
	}

	switch (shapeflag)
	{
	case HoRecTangleType:
		{
			pShape = new ShapeRect(ShapeId);
			GenRectangle2(&pShape->m_ShapeObj,roi.strect.row,roi.strect.col,roi.strect.angle,roi.strect.length1,roi.strect.length2);
			static_cast<ShapeRect*>(pShape)->CenRow = roi.strect.row;
			static_cast<ShapeRect*>(pShape)->CenCol = roi.strect.col;
			static_cast<ShapeRect*>(pShape)->pHi = roi.strect.angle;
			static_cast<ShapeRect*>(pShape)->Lenth1 = roi.strect.length1;
			static_cast<ShapeRect*>(pShape)->Lenth2 = roi.strect.length2;
		}
		break;
	case HoCircleType:
		{
			pShape = new  ShapeCircle(ShapeId);
			HObject ho_Test ;
			GenCircle(&ho_Test, roi.stcircle.row, roi.stcircle.col,roi.stcircle.Radius);

			pShape->m_ShapeObj =ho_Test;
			static_cast<ShapeCircle*>(pShape)->CenRow = roi.stcircle.row;
			static_cast<ShapeCircle*>(pShape)->CenCol = roi.stcircle.col;
			static_cast<ShapeCircle*>(pShape)->CirRii = roi.stcircle.Radius;
		}
		break;
	case HoComposeType:
		{
			pShape = new ComposeShape(ShapeId);
			((ComposeShape*)pShape)->m_VecComponentId = roi.stCombine.m_ShapIdList;
			((ComposeShape*)pShape)->m_VecCombType = roi.stCombine.m_CombTypeList;
			m_ShapeList.push_back(pShape);
			return; ///���ͼ��ͷ���ȥ��
		}
		break;
	}
	m_ShapeList.push_back(pShape);

	//��������״�ϲ��������״�С�

	for (int i = 0;i<m_ShapeList.size();i++)
	{
		if (HoComposeType == m_ShapeList.at(i)->m_ShapeType)  //�ҵ����ID
		{
			for (int j = 0;j<((ComposeShape*)m_ShapeList.at(i))->m_VecComponentId.size();j++)
			{
				if (ShapeId == ((ComposeShape*)m_ShapeList.at(i))->m_VecComponentId.at(j))  //����ǰ����ͼԪ��ID�Ƿ�����list�У���������У���ɴ���󷵻�
				{

					if (0 == j )  //�������ID�еĵ�0������ͼԪID������Ҫ�ϲ���ֱ�Ӹ�ֵ����
					{
						m_ShapeList.at(i)->m_ShapeObj = pShape->m_ShapeObj;
						// HObject ho_Test = m_ShapeList.at(i)->m_ShapeObj;
						//ho_Test = pShape->m_ShapeObj;
					}else
					{
						switch (((ComposeShape*)m_ShapeList.at(i))->m_VecCombType.at(j)) //
						{
						case RoiUnionType:
							{
								Union2(m_ShapeList.at(i)->m_ShapeObj,pShape->m_ShapeObj,&(m_ShapeList.at(i)->m_ShapeObj));
								// HObject ho_Test = m_ShapeList.at(i)->m_ShapeObj;
							}
							break;
						case RoiSubType:
							Difference(m_ShapeList.at(i)->m_ShapeObj,pShape->m_ShapeObj,&(m_ShapeList.at(i)->m_ShapeObj));
							break;
						case RoiInsectionType:
							Intersection(m_ShapeList.at(i)->m_ShapeObj,pShape->m_ShapeObj,&(m_ShapeList.at(i)->m_ShapeObj));
							break;
						}
					}
					return;
				}
			}
		}
	}
}


void CVisionProcess::OnShowAllRoi(long  halconID,const vector<int>* const pRoiIdList)
{
	ClearWindow(halconID);
	DispImage(halconID, m_index);

	for (int i = 0;i<m_ShapeList.size();i++)
	{
		//�ж�ID�Ƿ���pRoiDdList�У����������ʾ��
		for (int j = 0;j<pRoiIdList->size();j++)
		{
			if (pRoiIdList->at(j) == m_ShapeList.at(i)->m_ShapeId)
			{
				if (m_ShapeList.at(i)->m_ShapeObj.IsInitialized())
				{
					switch (m_ShapeList.at(i)->m_ShapeType)
					{
					case HoRecTangleType:
						ShowShap(*m_ShapeList.at(i),halconID);
						break;
					case HoCircleType:
						ShowShap(*m_ShapeList.at(i),halconID);
						break;
					case HoComposeType:
						{
							HalconCpp::DispObj(m_ShapeList.at(i)->m_ShapeObj,halconID);
							//HObject ho_Test = m_ShapeList.at(i)->m_ShapeObj;
						}

						break;
					default:
						HalconCpp::DispObj(m_ShapeList.at(i)->m_ShapeObj,halconID);
					}

				}
				break;
			}
		}
	}
}


bool  CVisionProcess::drawRoi(long  halconID , const  ST_ROI_INFO  &roi, ST_ROI_INFO &outroi ,bool bflag ,bool bcolor )
{
	HTuple row,col,phi,width,height,row2,col2,phi2,width2,height2;
	HTuple crow,ccol,radius,crow2,ccol2,radius2, radiuss, radiusb;
	HObject rectangle,circle,circles, circleb,Arrow;
	HTuple Point1Row,Point1Col,Point2Row,Point2Col,Rows,Cols;
	HTuple angleB,angleE,angleRange,Offset_Angle;
	HObject Objects,ContCircle1,ContCircle2,Contour,Contours;
	// try
	//{
	//clear_window(halconID);
	ClearWindow(halconID);
	DispImage(halconID, m_index);
	//set_color(halconID, "blue");
	// set_draw(halconID, "margin");
	//set_font(halconID,"-Arial-12-*-*-*-*-*-ANSI_CHARSET-");
	//set_line_width(halconID,1);

	disp_message(halconID, "�����ROI���򣬵���Ҽ���������","", 0, 0, "black", "true");
	if (bflag)
	{
		row = HTuple(roi.strect.row);
		col = HTuple(roi.strect.col);
		phi = HTuple(roi.strect.angle);         
		width = HTuple(roi.strect.length1);
		height = HTuple(roi.strect.length2);
		// draw_rectangle2_mod(halconID,row,col,phi,width,height,&row2,&col2,&phi2,&width2,&height2);
		DrawRectangle2Mod(halconID,row,col,phi,width,height,&row2,&col2,&phi2,&width2,&height2);

		if ( row2 == HTuple() )             //roi���򲻱䣬�򱣴�Ϊԭ������
		{
			row2 = row;
			col2 = col;
			phi2 = phi;
			width2 = width;
			height2 = height;
		}
		if (bcolor)
		{
			// set_color(halconID,"green");
		}
		//gen_rectangle2(&rectangle,row2,col2,phi2,width2,height2);
		//gen_rectangle2_contour_xld(&rectangle,row2,col2,phi2,width2,height2);
		//Point1Row = row2 + width2 * phi2.Sin();
		//Point1Col = col2 - width2 * phi2.Cos();
		//Point2Row = row2 - width2 * phi2.Sin();
		//Point2Col = col2 + width2 * phi2.Cos();
		//gen_arrow_contour_xld(&Arrow, Point1Row,Point1Col,Point2Row,Point2Col,HTuple(40), HTuple(40));
		//concat_obj(rectangle,Arrow,&rectangle);
		//  disp_obj(rectangle,halconID);

		GenRectangle2(&rectangle,row2,col2,phi2,width2,height2);
		GenRectangle2ContourXld(&rectangle,row2,col2,phi2,width2,height2);
		Point1Row = row2 + width2 * phi2.TupleSin();
		Point1Col = col2 - width2 * phi2.TupleCos();
		Point2Row = row2 - width2 * phi2.TupleSin();
		Point2Col = col2 + width2 * phi2.TupleCos();
		//gen_arrow_contour_xld(&Arrow, Point1Row,Point1Col,Point2Row,Point2Col,HTuple(40), HTuple(40));

		//ConcatObj(rectangle,Arrow,&rectangle);
		HalconCpp::DispObj(rectangle,halconID);

		outroi.strect.row = row2[0].D();
		outroi.strect.col = col2[0].D();
		outroi.strect.length1 = width2[0].D();
		outroi.strect.length2 = height2[0].D();
		outroi.strect.angle = phi2[0].D();
		outroi.iType = 0;

	} 
	else
	{
		crow = HTuple(roi.stcircle.row);
		ccol=HTuple(roi.stcircle.col);
		radius=HTuple(roi.stcircle.Radius);
		angleB = 20;//HTuple(roi.stcircle.angleB);
		angleE = 30; HTuple(roi.stcircle.angleE);
		// draw_circle_mod(halconID,crow,ccol,radius,&crow2,&ccol2,&radius2);
		DrawCircleMod(halconID,crow,ccol,radius,&crow2,&ccol2,&radius2);
		if (crow2 == HTuple())//roi���򲻱䣬�򱣴�Ϊԭ������
		{
			crow2 = crow;
			ccol2 = ccol;
			radius2 = radius;
		}
		if (bcolor)
		{
			//set_color(halconID,"green");
		}
		//radiuss = radius2 - HTuple(roi.stcircle.width/2);
		//radiusb = radius2 + HTuple(roi.stcircle.width/2);
		//gen_circle(&circles, crow2, ccol2,radiuss);
		//gen_circle(&circleb, crow2, ccol2,radiusb);
		//concat_obj(circles, circleb, &circle);

		radiuss = radius2 - HTuple(roi.stcircle.width/2);
		radiusb = radius2 + HTuple(roi.stcircle.width/2);
		GenCircle(&circles, crow2, ccol2,radiuss);
		GenCircle(&circleb, crow2, ccol2,radiusb);
		ConcatObj(circles, circleb, &circle);


		if (angleB > angleE)
		{
			angleB = angleB - 360;
		}

		angleRange = angleE - angleB;
		radiuss = radius2 - HTuple(roi.stcircle.width/2);
		if (radius < 1)
		{
			radius = 1;
		}
		radiusb = radius2 + HTuple(roi.stcircle.width/2);

		if (HTuple(360) <= angleRange)
		{
			//gen_circle_contour_xld(&circles,crow2, ccol2,radiuss,HTuple(0), HTuple(6.28318), HTuple("positive"),HTuple(1));
			//gen_circle_contour_xld(&circleb,crow2, ccol2,radiusb,HTuple(0), HTuple(6.28318), HTuple("positive"),HTuple(1));
			//gen_circle(&circles, crow, ccol,radiuss);
			//gen_circle(&circleb, crow, ccol,radiusb);
			//concat_obj(circles, circleb, &circle);

			GenCircleContourXld(&circles,crow2, ccol2,radiuss,HTuple(0), HTuple(6.28318), HTuple("positive"),HTuple(1));
			GenCircleContourXld(&circleb,crow2, ccol2,radiusb,HTuple(0), HTuple(6.28318), HTuple("positive"),HTuple(1));
			GenCircle(&circles, crow, ccol,radiuss);
			GenCircle(&circleb, crow, ccol,radiusb);
			ConcatObj(circles, circleb, &circle);


			//gen_arrow_contour_xld(&Arrow, crow2, ccol2 + radiuss, crow2, ccol2 + radiusb, HTuple(40), HTuple(40));

			//concat_obj(circle,Arrow,&circle);
			//ConcatObj(circle,Arrow,&circle);
			//gen_arrow_contour_xld(&Arrow, crow2 - radiuss, ccol2, crow2 - radiusb, ccol2, HTuple(40), HTuple(40));
			//concat_obj(circle,Arrow,&circle);
			//ConcatObj(circle,Arrow,&circle);
			//gen_arrow_contour_xld(&Arrow, crow2, ccol2 - radiuss, crow2, ccol2 - radiusb, HTuple(40), HTuple(40));
			//concat_obj(circle,Arrow,&circle);
			//ConcatObj(circle,Arrow,&circle);
			//gen_arrow_contour_xld(&Arrow, crow2 + radiuss, ccol2, crow2 + radiusb, ccol2, HTuple(40), HTuple(40));
			//concat_obj(circle,Arrow,&circle);
			//ConcatObj(circle,Arrow,&circle);
		}
		else
		{
			Offset_Angle = angleRange/4.0;
			for (int i = 1; i<4; i++)
			{
				Point1Row = crow2 + radiuss * (-(angleB + Offset_Angle * i).TupleRad()).TupleSin();
				Point1Col = ccol2 + radiuss * (angleB + Offset_Angle * i).TupleRad().TupleCos();
				Point2Row = crow2 + radiusb * (-(angleB + Offset_Angle * i).TupleRad()).TupleSin();
				Point2Col = ccol2 + radiusb * (angleB + Offset_Angle * i).TupleRad().TupleCos();
				//gen_arrow_contour_xld(&Arrow, Point1Row,Point1Col,Point2Row,Point2Col,HTuple(40), HTuple(40));
				//concat_obj(circle,Arrow,&circle);
				//ConcatObj(circle,Arrow,&circle);
			}
			//gen_empty_obj(&Objects);
			//gen_circle_contour_xld (&ContCircle1, crow2, ccol2, radiusb, angleB.Rad(), angleE.Rad(), HTuple("positive"), HTuple(1));
			//concat_obj(Objects,ContCircle1,&Objects);
			//gen_circle_contour_xld (&ContCircle2, crow2, ccol2, radiuss, angleB.Rad(), angleE.Rad(), HTuple("positive"), HTuple(1));
			//concat_obj(Objects,ContCircle2,&Objects);
			//Point1Row = crow2 + radiuss * (-angleB.Rad()).Sin();
			//Point1Col = ccol2 + radiuss * angleB.Rad().Cos();
			//Point2Row = crow2 + radiusb * (-angleB.Rad()).Sin();
			//Point2Col = ccol2 + radiusb * angleB.Rad().Cos();
			//tuple_concat(Point1Row, Point2Row, &Rows);
			//tuple_concat(Point1Col, Point2Col, &Cols);
			//gen_contour_polygon_xld(&Contour,Rows,Cols);
			//concat_obj(Objects, Contour, &Objects);
			//Point1Row = crow2 + radiuss * (-angleE.Rad()).Sin();
			//Point1Col = ccol2 + radiuss * angleE.Rad().Cos();
			//Point2Row = crow2 + radiusb * (-angleE.Rad()).Sin();
			//Point2Col = ccol2 + radiusb * angleE.Rad().Cos();
			//tuple_concat(Point1Row, Point2Row, &Rows);
			//tuple_concat(Point1Col, Point2Col, &Cols);
			//gen_contour_polygon_xld(&Contour,Rows,Cols);
			//concat_obj(Objects, Contour, &Objects);
			//union_adjacent_contours_xld(Objects, &Contours, HTuple(10), HTuple(1), HTuple("attr_keep"));
			//concat_obj(circle, Contours, &circle);


			GenEmptyObj(&Objects);
			GenCircleContourXld (&ContCircle1, crow2, ccol2, radiusb, angleB.TupleRad(), angleE.TupleRad(), HTuple("positive"), HTuple(1));
			ConcatObj(Objects,ContCircle1,&Objects);
			GenCircleContourXld (&ContCircle2, crow2, ccol2, radiuss, angleB.TupleRad(), angleE.TupleRad(), HTuple("positive"), HTuple(1));
			ConcatObj(Objects,ContCircle2,&Objects);
			Point1Row = crow2 + radiuss * (-angleB.TupleRad()).TupleSin();
			Point1Col = ccol2 + radiuss * angleB.TupleRad().TupleCos();
			Point2Row = crow2 + radiusb * (-angleB.TupleRad()).TupleSin();
			Point2Col = ccol2 + radiusb * angleB.TupleRad().TupleCos();
			TupleConcat(Point1Row, Point2Row, &Rows);
			TupleConcat(Point1Col, Point2Col, &Cols);
			GenContourPolygonXld(&Contour,Rows,Cols);
			ConcatObj(Objects, Contour, &Objects);
			Point1Row = crow2 + radiuss * (-angleE.TupleRad()).TupleSin();
			Point1Col = ccol2 + radiuss * angleE.TupleRad().TupleCos();
			Point2Row = crow2 + radiusb * (-angleE.TupleRad()).TupleSin();
			Point2Col = ccol2 + radiusb * angleE.TupleRad().TupleCos();
			TupleConcat(Point1Row, Point2Row, &Rows);
			TupleConcat(Point1Col, Point2Col, &Cols);
			GenContourPolygonXld(&Contour,Rows,Cols);
			ConcatObj(Objects, Contour, &Objects);
			UnionAdjacentContoursXld(Objects, &Contours, HTuple(10), HTuple(1), HTuple("attr_keep"));
			ConcatObj(circle, Contours, &circle);
		}
		//disp_obj(circle,halconID);
		HalconCpp::DispObj(circle,halconID);

		outroi.stcircle.row = crow2[0].D();
		outroi.stcircle.col = ccol2[0].D();
		outroi.stcircle.Radius =radius2[0].D();
		outroi.stcircle.width = roi.stcircle.width;
		outroi.iType = 1;
	}
	// }
	//catch (HException  &hdev_exception)
	//{
	//    //LOG_INFO("%s", hdev_exception.message);
	//    return false;
	//}    

	return true;
}

bool  CVisionProcess::dispRoi(long  halconID , ST_ROI_INFO  roi,bool bflag ,bool bcolor )
{
	HTuple row,col,phi,length1,length2;
	HTuple crow,ccol,radius,radiuss,radiusb,angleB,angleE,angleRange;
	HObject rectangle,circle,circles, circleb, Arrow;
	HTuple Offset_Angle,Point1Row,Point1Col,Point2Row,Point2Col,Rows,Cols;
	HObject Objects,ContCircle1,ContCircle2,Contour,Contours;
	// try
	//{
	// set_draw(halconID, "margin");
	//set_font(halconID,"-Arial-12-*-*-*-*-*-ANSI_CHARSET-");
	// set_line_width(halconID,1);
	if (bflag)
	{
		row = HTuple(roi.strect.row);
		col = HTuple(roi.strect.col);
		phi = HTuple(roi.strect.angle);          
		length1 = HTuple(roi.strect.length1);
		length2 = HTuple(roi.strect.length2);
		//draw_rectangle2(halconID,&row,&col,&phi,&width,&height);
		//DrawRectangle2(halconID,&row,&col,&phi,&width,&height);
		if (bcolor)
		{
			// set_color(halconID,"green");
		}
		// gen_rectangle2_contour_xld(&rectangle,row,col,phi,length1,length2);
		GenRectangle2ContourXld(&rectangle,row,col,phi,length1,length2);
		//Point1Row = row + length1 * phi.TupleSin();
		//Point1Col = col - length1 * phi.TupleCos();
		//Point2Row = row - length1 * phi.TupleSin();
		//Point2Col = col + length1 * phi.TupleCos();
		//gen_arrow_contour_xld(&Arrow, Point1Row,Point1Col,Point2Row,Point2Col,HTuple(40), HTuple(40));
		//concat_obj(rectangle,Arrow,&rectangle);
		// disp_obj(rectangle,halconID);
		//ConcatObj(rectangle,Arrow,&rectangle);
		HalconCpp::DispObj(rectangle,halconID);

	}
	else
	{
		crow = HTuple(roi.stcircle.row);
		ccol=HTuple(roi.stcircle.col);
		radius=HTuple(roi.stcircle.Radius);
		angleB = HTuple(roi.stcircle.angleB);
		angleE = HTuple(roi.stcircle.angleE);
		//draw_circle(halconID,&crow,&ccol,&radius);
		DrawCircle(halconID,&crow,&ccol,&radius);
		if (bcolor)
		{
			//set_color(halconID,"green");
		}
		if (angleB > angleE)
		{
			angleB = angleB - 360;
		}

		angleRange = angleE - angleB;
		radiuss = radius - HTuple(roi.stcircle.width/2);
		radiusb = radius + HTuple(roi.stcircle.width/2);

		if (HTuple(360) <= angleRange)
		{
			//gen_circle_contour_xld(&circles,crow, ccol,radiuss,HTuple(0), HTuple(6.28318), HTuple("positive"),HTuple(1));
			//gen_circle_contour_xld(&circleb,crow, ccol,radiusb,HTuple(0), HTuple(6.28318), HTuple("positive"),HTuple(1));
			//gen_circle(&circles, crow, ccol,radiuss);
			//gen_circle(&circleb, crow, ccol,radiusb);
			//concat_obj(circles, circleb, &circle);

			GenCircleContourXld(&circles,crow, ccol,radiuss,HTuple(0), HTuple(6.28318), HTuple("positive"),HTuple(1));
			GenCircleContourXld(&circleb,crow, ccol,radiusb,HTuple(0), HTuple(6.28318), HTuple("positive"),HTuple(1));
			GenCircle(&circles, crow, ccol,radiuss);
			GenCircle(&circleb, crow, ccol,radiusb);
			ConcatObj(circles, circleb, &circle);

			gen_arrow_contour_xld(&Arrow, crow, ccol + radiuss, crow, ccol + radiusb, HTuple(40), HTuple(40));
			//concat_obj(circle,Arrow,&circle);
			gen_arrow_contour_xld(&Arrow, crow - radiuss, ccol, crow - radiusb, ccol, HTuple(40), HTuple(40));
			//concat_obj(circle,Arrow,&circle);
			gen_arrow_contour_xld(&Arrow, crow, ccol - radiuss, crow, ccol - radiusb, HTuple(40), HTuple(40));
			//concat_obj(circle,Arrow,&circle);
			gen_arrow_contour_xld(&Arrow, crow + radiuss, ccol, crow + radiusb, ccol, HTuple(40), HTuple(40));
			//concat_obj(circle,Arrow,&circle);
		}
		else
		{
			//Offset_Angle = angleRange/4.0;
			//for (int i = 1; i<4; i++)
			//{
			//	Point1Row = crow + radiuss * (-(angleB + Offset_Angle * i).Rad()).Sin();
			//	Point1Col = ccol + radiuss * (angleB + Offset_Angle * i).Rad().Cos();
			//	Point2Row = crow + radiusb * (-(angleB + Offset_Angle * i).Rad()).Sin();
			//	Point2Col = ccol + radiusb * (angleB + Offset_Angle * i).Rad().Cos();
			//	gen_arrow_contour_xld(&Arrow, Point1Row,Point1Col,Point2Row,Point2Col,HTuple(40), HTuple(40));
			//	concat_obj(circle,Arrow,&circle);
			//}
			//gen_empty_obj(&Objects);
			//gen_circle_contour_xld (&ContCircle1, crow, ccol, radiusb, angleB.Rad(), angleE.Rad(), HTuple("positive"), HTuple(1));
			//concat_obj(Objects,ContCircle1,&Objects);
			//   gen_circle_contour_xld (&ContCircle2, crow, ccol, radiuss, angleB.Rad(), angleE.Rad(), HTuple("positive"), HTuple(1));
			//concat_obj(Objects,ContCircle2,&Objects);
			//Point1Row = crow + radiuss * (-angleB.Rad()).Sin();
			//Point1Col = ccol + radiuss * angleB.Rad().Cos();
			//Point2Row = crow + radiusb * (-angleB.Rad()).Sin();
			//Point2Col = ccol + radiusb * angleB.Rad().Cos();
			//tuple_concat(Point1Row, Point2Row, &Rows);
			//tuple_concat(Point1Col, Point2Col, &Cols);
			//gen_contour_polygon_xld(&Contour,Rows,Cols);
			//concat_obj(Objects, Contour, &Objects);
			//Point1Row = crow + radiuss * (-angleE.Rad()).Sin();
			//Point1Col = ccol + radiuss * angleE.Rad().Cos();
			//Point2Row = crow + radiusb * (-angleE.Rad()).Sin();
			//Point2Col = ccol + radiusb * angleE.Rad().Cos();
			//tuple_concat(Point1Row, Point2Row, &Rows);
			//tuple_concat(Point1Col, Point2Col, &Cols);
			//gen_contour_polygon_xld(&Contour,Rows,Cols);
			//concat_obj(Objects, Contour, &Objects);
			//union_adjacent_contours_xld(Objects, &Contours, HTuple(10), HTuple(1), HTuple("attr_keep"));
			//concat_obj(circle, Contours, &circle);
		}


		//gen_circle(&circle,crow,ccol,radius);
		//         disp_obj(circle,halconID);
	}
	// }
	//catch (HException  &hdev_exception)
	//{
	//    //LOG_INFO("%s", hdev_exception.message);
	//    return false;
	//}	
	return true;
}

bool CVisionProcess::Imagebuffer(int index, int sceneID, long *pbuffer)
{
	if (NULL == pbuffer)
	{
		return false;
	}
	memcpy(m_pImage,(HObject*)pbuffer,sizeof(*pbuffer));
	return true;
}


bool CVisionProcess::setpart(long winID,int topx, int topy,int lowx,int lowy)
{
	try
	{
		//SetPart(winID,topy,topx,lowx,lowy);//�������ˣ��ֳ�Ҳ�Ǵ��
		SetPart(winID,topy,topx,lowy,lowx);
	}
	catch (...)  //HException  &hdev_exception
	{
		// LOG_INFO("%s", hdev_exception.message);
		return false;
	}
	return true;
}

bool CVisionProcess::getpart(long winID,int &topx, int &topy,int &lowx,int &lowy)
{
	HTuple hRow1,hRow2,hCol1,hCol2;
	try
	{
		GetPart(winID,&hRow1,&hCol1,&hRow2,&hCol2);
		topx = hCol1;
		topy = hRow1;
		lowx = hCol2;
		lowy = hRow2;
	}
	catch (...)
	{
		// LOG_INFO("%s", hdev_exception.message);
		return false;
	}
	return true;
}
bool CVisionProcess::openWindow(long winID, int x, int y, int width, int hight, long &halconWinID)
{
	try
	{
		HTuple HalWinId;
		HalconCpp::OpenWindow(y, x,width, hight, winID, "visible", "", &HalWinId);
		halconWinID = (long)HalWinId.L();
		return true;

	}
	catch (...)
	{
		LOG_INFO("%s", "�򿪴���ʧ��!");
		return false;
	}

	return true;
}
bool CVisionProcess::closeWindow(long halconWinID)
{
	try
	{
		HalconCpp::CloseWindow(halconWinID);
		return true;

	}
	catch (...)
	{
		LOG_INFO("%s", "�رմ���ʧ��!");
		return false;
	}
	return true;
}

bool CVisionProcess::getimagesize(int &width, int &height)
{
	bool bRet = false;
	HTuple H_width,H_height;
	try
	{
		GetImageSize(m_pImage[0],&H_width,&H_height);
		width = H_width;
		height = H_height;
		bRet = true;
	}
	catch (...)//HException  &hdev_exception
	{
		//LOG_INFO("%s", hdev_exception.message);
		bRet = false;
	}
	return bRet;
}

bool CVisionProcess::getimagesize(int ImageIndex,int &width, int &height)
{
	bool bRet = false;
	HTuple H_width,H_height;
	try
	{
		GetImageSize(m_pImage[ImageIndex],&H_width,&H_height);
		width = H_width;
		height = H_height;
		bRet = true;
	}
	catch (...)//HException  &hdev_exception
	{
		//LOG_INFO("%s", hdev_exception.message);
		bRet = false;
	}
	return bRet;
}

bool CVisionProcess::DebugThreshold(long  halconID, int iMinGray, int iMaxGray)
{
	bool bRet = false;
	HObject RegionObj;
	HTuple Colors;
	//try
	//{
	// set_draw(halconID, "fill");
	// query_color(halconID, &Colors);
	//set_color(halconID, "red");
	//clear_window(halconID);
	DispImage(halconID, m_index);

	if (8 <= m_index/* || (H_EMPTY_REGION == m_pImage[m_index].Id() ? true : false)*/)
	{
		return false;
	}

	//threshold(m_pImage[m_index], &RegionObj, iMinGray, iMaxGray);
	// disp_obj(RegionObj, halconID);
	bRet = true;
	// set_color(halconID, Colors);
	//set_draw(halconID,"margin");
	// }
	//catch (HException  &hdev_exception)
	//{
	//   // LOG_INFO("%s", hdev_exception.message);
	//    bRet = false;
	//}
	return bRet;
}
bool  CVisionProcess::findline(char *pSceneName, VIRSION_DATA_PKG *pData, int iLength, unsigned int *pType, ST_LINE &result, unsigned int IsDisplay)
{
	bool bRet = false;
	char buf[MAX_PATH] = {0};

	HTuple InputValue, ErrorCode;

	HTuple OutValue;

	//	try
	//  {
	for (int i = VIRSION_DATA_PKG_HEADER_SIZE; i < pData[1].iValue + VIRSION_DATA_PKG_HEADER_SIZE; i++)
	{
		int iType = pType[i]%100;
		switch(iType)
		{
		case VISION_INT_TYPE:
			{
				InputValue.Append(HTuple(pData[i].iValue));
			}
			break;
		case VISION_DOUBLE_TYPE:
			{
				InputValue.Append(HTuple(pData[i].dValue));
			}
			break;
		default:
			break;
		}
	}

	ToolProcProcess(pSceneName, m_hdvLineProc, m_index, InputValue, "find_line", OutValue, ErrorCode, IsDisplay);

	if (ErrorCode >= 0)
	{
		result.x_begin = OutValue[0].D();
		result.y_begin = OutValue[1].D();
		result.x_end = OutValue[2].D();
		result.y_end = OutValue[3].D();
		result.phi = OutValue[4].D();
		sprintf_s(buf,"�ߵĶ˵�����:(%f ��%f : %f : %f)\r\n �Ƕȣ�%f��",result.x_begin,result.y_begin,result.x_end,result.y_end, result.phi);
	}
	else
	{
		sprintf_s(buf,"����ʧ��");
	}
	m_vecMsg.Append(buf);
	bRet = true;

	//}
	//  catch (HException  &exception)
	//  {
	//    //  m_vecMsg.Append(exception.message);
	//      //LOG_ERR("%s:%s\n",exception.proc, exception.message);
	//bRet = false;
	//  }
	//  catch(HDevEngineException &hdev_exception)
	//  {
	//      m_vecMsg.Append(hdev_exception.Message());
	//      LOG_ERR("%s %d:%s\n",hdev_exception.ProgLineName(),hdev_exception.ProgLineNum(),
	//          hdev_exception.Message());
	//      bRet = false;
	//  }
	return bRet;
}

bool  CVisionProcess::findcircle(char *pSceneName, VIRSION_DATA_PKG *pData, int iLength, unsigned int *pType, ST_CIRCLE &result, unsigned int IsDisplay)
{
	bool bRet = false;
	char buf[MAX_PATH] = {0};

	HTuple InputValue, ErrorCode;

	HTuple OutValue;
	// 	try
	//     {
	if (NULL == pData)
	{
		return false;
	}
	for (int i = VIRSION_DATA_PKG_HEADER_SIZE; i < pData[1].iValue + VIRSION_DATA_PKG_HEADER_SIZE; i++)
	{
		int iType = pType[i]%100;
		switch(iType)
		{
		case VISION_INT_TYPE:
			{
				InputValue.Append(HTuple(pData[i].iValue));
			}
			break;
		case VISION_DOUBLE_TYPE:
			{
				InputValue.Append(HTuple(pData[i].dValue));
			}
			break;
		default:
			break;
		}
	}

	ToolProcProcess(pSceneName, m_hdvCircleProc, m_index, InputValue, "find_circle", OutValue, ErrorCode, IsDisplay);

	if (ErrorCode >= 0)
	{
		result.x = OutValue[0].D();
		result.y = OutValue[1].D();
		result.radius  = OutValue[2].D();
		sprintf_s(buf,"Բ�����꼰�뾶Ϊ:(%f,%f:%f)",result.x,result.y,result.radius);
	}
	else
	{
		sprintf_s(buf,"����ʧ��");
	}
	m_vecMsg.Append(buf);
	bRet = true;
	//}
	//  catch (HException  &exception)
	//  {
	//      //m_vecMsg.Append(exception.message);
	//    //  LOG_ERR("%s:%s\n",exception.proc, exception.message);
	//bRet = false;
	//  }
	//  catch(HDevEngineException &hdev_exception)
	//  {
	//      m_vecMsg.Append(hdev_exception.Message());
	//      LOG_ERR("%s %d:%s\n",hdev_exception.ProgLineName(),hdev_exception.ProgLineNum(),
	//          hdev_exception.Message());
	//      bRet = false;
	//  }
	return bRet;

}

//�����˲���Row��Rii.
bool  CVisionProcess::MeasureCircle(long iHalconWind,int ImageIndex,int MatchIndex,double& Row,double& Col,double& Rii)
{
	bool IsRealUnit = false;
	HTuple  hv_TransRow,hv_TransCol;
	HObject ho_ModelContour,ho_MeasureContour,ho_Contour,ho_UsedEdges,ho_ResultContours;
	int   ImageWid,ImageHeigh;
	HTuple  hv_MetrologyHandle;
	HTuple hv_Line,hv_LineIndices;//hv_ParamLine;
	HTuple hv_Row,hv_Column,hv_UsedRow,hv_UsedColumn;
	HTuple hv_MearSeqRow, hv_MearSeqColumn, hv_Phi1, hv_MearSeqLength1, hv_MearSeqLength2;
	HTuple hv_RowBegin0,hv_ColBegin0, hv_RowEnd0, hv_ColEnd0, hv_Nr0, hv_Nc0, hv_Dist0;
	HTuple hv_CircleIndices;
	HTuple hv_MeasRes ,hv_arrowR0,hv_arrowC0,hv_Row2,hv_Col2;
	if ( 1 == (int)Row)
	{
		IsRealUnit = true;
	}
	getimagesize(ImageIndex,ImageWid,ImageHeigh);
	CreateMetrologyModel(&hv_MetrologyHandle);
	SetMetrologyModelImageSize(hv_MetrologyHandle, ImageWid, ImageHeigh);
	Shape* pShape = NULL;
	for (int i = 0;i<m_ShapeList.size();i++)
	{
		if ((int)Rii == m_ShapeList.at(i)->m_ShapeId)
		{
			pShape = m_ShapeList.at(i);
			break;
		}
	}
	if (!pShape)
	{
		return false;
	}

	ShapeType typeShape = pShape->m_ShapeType;
	switch (typeShape)
	{
	case HoCircleType:
		{
			Row = ((ShapeCircle*)pShape)->CenRow;
			Col = ((ShapeCircle*)pShape)->CenCol;
			Rii = ((ShapeCircle*)pShape)->CirRii;
		}
		break;

	default:
		return false;
	}
	int no = m_MatchHomm2DList.size();
	if (MatchIndex >= no)
	{
		return false;
	}
	if (MatchIndex>=0)
	{
		AffineTransPixel(m_MatchHomm2DList.at(MatchIndex),Row,Col,&hv_TransRow,&hv_TransCol);
	}else 
	{
		hv_TransRow = Row;
		hv_TransCol = Col;
	}

	AddMetrologyObjectCircleMeasure(hv_MetrologyHandle,hv_TransRow,hv_TransCol,Rii,Rii/2,Rii*6.28/10,0.9,20, HTuple(), HTuple(),&hv_CircleIndices);

	//GetMetrologyObjectModelContour(&ho_ModelContour, hv_MetrologyHandle, "all", 1.5);
	//GetMetrologyObjectMeasures(&ho_MeasureContour, hv_MetrologyHandle, "all", "all", &hv_Row, &hv_Column);
	//HalconCpp::DispObj(ho_MeasureContour,iHalconWind);


	ApplyMetrologyModel(m_pImage[ImageIndex], hv_MetrologyHandle);

	GetMetrologyObjectMeasures(&ho_Contour, hv_MetrologyHandle, "all", "all", &hv_Row, &hv_Column);
	GetMetrologyObjectResult(hv_MetrologyHandle, "all", "all", "used_edges", "row",  &hv_UsedRow);
	GetMetrologyObjectResult(hv_MetrologyHandle, "all", "all", "used_edges", "column", &hv_UsedColumn);
	if (0 == hv_UsedRow.Length())
	{

		return FALSE;
	}

	GenCrossContourXld(&ho_UsedEdges, hv_UsedRow, hv_UsedColumn, 10, HTuple(45).TupleRad());	

	GetMetrologyObjectResultContour(&ho_ResultContours, hv_MetrologyHandle, "all", "all", 1.5);


	//DispObj(ho_MeasureContour,m_Window);
	HalconCpp::DispObj(ho_UsedEdges,iHalconWind);
	//hv_arrowR0 = hv_Row;
	//hv_arrowC0 = hv_Column;
	//TupleSelect(hv_UsedRow, (hv_UsedRow.TupleLength())/2, &hv_Row2);
	//TupleSelect(hv_UsedColumn, (hv_UsedColumn.TupleLength())/2, &hv_Col2);
	//SetTposition(iHalconWind, hv_Row2, hv_Col2);
	GetMetrologyObjectResult(hv_MetrologyHandle, hv_CircleIndices, "all", "result_type", "all_param", &hv_MeasRes);
	Row = hv_MeasRes[0];
	Col = hv_MeasRes[1];
	Rii  = hv_MeasRes[2];
	if (IsRealUnit)
	{
		//Row *= 
	}

	HalconCpp::DispObj(ho_ResultContours,iHalconWind);

	hv_MetrologyHandle.Clear();
	return TRUE;

}

bool CVisionProcess::HalconProcedureLocate(long hWinID,int Imageindex,string ProcedurePath,double (*inParam)[HalProcInParamNum], double& PosX,
	double& PosY,double& PosPhi)
{
	bool res = false;
	char szParamName[128] ={0};
	int hv_ProcState;
	//1������·�� �� ����	
	HTuple hv_ProPathName;
	if (ProcedurePath.empty())
	{
		return false;
	}
	int DivIndex = ProcedurePath.find_last_of('/');

	//1.1����
	char szTemp[512] ={0};
	//char strPath[512] ={0};
	sprintf_s(szTemp,"%s",ProcedurePath.c_str());
	szTemp[DivIndex] = '\0';

	char* pProcPath = szTemp;
	char* pChar = &(szTemp[DivIndex+1]);
	char* pProcName = strtok(pChar,".");

	//2������halcon����
	//2.1����
	HDevEngine HalEngine;
	HDevProcedure HalProc;
	HDevProcedureCall  HalProcCall;
	HalEngine.SetProcedurePath(pProcPath);


	try
	{
		HalEngine.SetProcedurePath(pProcPath);
		HalProc.LoadProcedure(pProcName);
		HalProcCall = HalProc.CreateCall();

	}
	catch (...)
	{
		//string err = "�ļ�����ʧ��";
		throw "�ļ�����ʧ��";
		return false;
	}

	//2.2 �����������
	//HObject ho_Image;
	//HalconCpp::ReadImage(&ho_Image,"F:/vstest/UseHDevEngine/Debug/Test.png");
	HalProcCall.SetInputIconicParamObject(1,m_pImage[Imageindex]);//m_pImage[Imageindex]);
	int ParanNum = sizeof(*inParam)/sizeof(double);

	for (int i = 0;i< ParanNum;i++)
	{
		//memset(szParamName,0,sizeof(szParamName));
		//sprintf_s(szParamName,"Input%d",i);
		double testDouble = (*inParam)[i];
		HalProcCall.SetInputCtrlParamTuple(i+1,(*inParam)[i]);



	}

	//2.3 ����
	try
	{
		HalProcCall.Execute();
		PosX = HalProcCall.GetOutputCtrlParamTuple(1).D();
		PosY = HalProcCall.GetOutputCtrlParamTuple(2).D();
		PosPhi = HalProcCall.GetOutputCtrlParamTuple(3).D();

		hv_ProcState = HalProcCall.GetOutputCtrlParamTuple(4).I();

		if (0 == hv_ProcState)
		{
			res = false;
		}else
		{
			res = true;
		}

		HalEngine.UnloadProcedure(pProcName);
		//HalProc.
		//HalProcCall
		//ClearWindow(hWinID);
		//DispObj(hWinID,true);
		//DispImage(hWinID,Imageindex);

	}
	catch (...)
	{
		//string err = "Halcon�������ʧ��!";
		throw "Halcon�������ʧ��!";
		return false;
	}

	return res;
}

bool CVisionProcess::HalconProcedureProcess(long iWinID,int Imageindex,string ProcedurePath,vector<double> &inparam,vector<double> &outparam,int display,int &error,string &hmessage)
{
	bool res = true;
	char szParamName[256] ={0};
	//Input
	HTuple  InputTuple;
	HObject InputObj;
	HObject ImageA;

	//OutPut
	HTuple  Message;
	HObject OutputObj;
	HTuple  ErrorCode, OutValue;
	LARGE_INTEGER  start,end,litmp;
	//1������·�� �� ����	
	HTuple hv_ProPathName;
	if (ProcedurePath.empty())
	{
		error = -1;
		hmessage = "halcon������򲻴���!";
		return false;
	}
	int DivIndex = ProcedurePath.find_last_of('/');

	//1.1����
	char szTemp[512] ={0};
	sprintf_s(szTemp,"%s",ProcedurePath.c_str());
	szTemp[DivIndex] = '\0';

	char* pProcPath = szTemp;
	char* pChar = &(szTemp[DivIndex+1]);
	char* pProcName = strtok(pChar,".");
	//2������halcon����
	//2.1����
	HDevEngine HalEngine;
	HDevProcedure HalProc;
	HDevProcedureCall  HalProcCall;
	HalEngine.SetProcedurePath(pProcPath);
	try
	{
		HalEngine.SetProcedurePath(pProcPath);
		HalProc.LoadProcedure(pProcName);
		HalProcCall = HalProc.CreateCall();

	}
	catch (...)
	{
		hmessage = "�ļ�����ʧ��";
		error = -2;
		return false;
	}

	//2.2 �����������
	//HObject ho_Image;
	//HalconCpp::ReadImage(&ho_Image,"F:/vstest/UseHDevEngine/Debug/Test.png");
	//HalProcCall.SetInputIconicParamObject(1,m_pImage[Imageindex]);//m_pImage[Imageindex]);

	CopyObj(m_pImage[Imageindex], &ImageA, 1, 1);//Դ��Ŀ��,������1��ʼ,��������
	//ConcatObj(ImageA, m_pImage[Imageindex], &ImageA);//��������ͼ��
	HalProcCall.SetInputIconicParamObject("Image",ImageA);
	int ParanNum = inparam.size();
	for (int i = 0;i< ParanNum;i++)
	{
		//memset(szParamName,0,sizeof(szParamName));
		//sprintf_s(szParamName,"Input%d",i);
		double testDouble = inparam[i];

		InputTuple[i] = testDouble;
	}
	HalProcCall.SetInputCtrlParamTuple("value", InputTuple);
	HalProcCall.SetInputCtrlParamTuple("display", display);

	//�������
	//	HalProcCall.SetOutputIconicParamObject("object",ImageA);

	//2.3 ����
	try
	{
		m_bIsPass = false;
		m_vecMsg.Clear();
		m_vecObj.Clear();

		m_vecMsg = HTuple();
		m_vecObj = HObject();

		QueryPerformanceFrequency(&litmp);//��ȡϵͳʱ��Ƶ��
		QueryPerformanceCounter(&start) ; 

		HalProcCall.Execute();
		ErrorCode = HalProcCall.GetOutputCtrlParamTuple("error");
		m_vecMsg = HalProcCall.GetOutputCtrlParamTuple("message");
		if (0 <= ErrorCode)
		{
			OutValue = HalProcCall.GetOutputCtrlParamTuple("outValue");
			m_vecObj = HalProcCall.GetOutputIconicParamObject("object"); 
			m_bIsPass = true;
			//����
			int outlen = OutValue.TupleLength().I();
			double outtemp = 0.0;
			for (int i = 0;i< outlen ;i++ )
			{
				outtemp = OutValue[i].D();
				outparam.push_back(outtemp);
			}
		}
		else if (0 < ErrorCode)
		{
			m_bIsPass = false;
		}
		else if (1 == display)
		{
			m_vecObj = HalProcCall.GetOutputIconicParamObject("object"); 
		}
		else
		{
			hmessage =  "Halcon������ʧ��!";
			error = -3;
		}

		//ж�س���ÿ�ζ�̬���س����ж�س����ʱ��,���ǲ����¼��س����ֻ���֣��޸�halcon������ܷ�Ӧ����������,
		//���Բ���1������ģʽ�ͣ�2����ģʽѡ��и�ȫ�ֵĵ��Ա�־���Կ���
		HalEngine.UnloadProcedure(pProcName);
		QueryPerformanceCounter(&end) ; 
		double dTime = (double)(end.QuadPart-start.QuadPart) / (double)litmp.QuadPart;    //��ȡʱ�䵥λΪ��	
		char szBuf[125] = {0};
		sprintf_s(szBuf, ",��ʱ��%0.3lf", dTime);
		m_vecMsg.Append(szBuf);
		error = 0;
		hmessage = "OK";
	}
	catch (...)
	{
		hmessage =  "Halcon������ʧ��!";
		error = -3;
		return false;
	}

	return res;
}

bool CVisionProcess::ImageEnhancement(long iWinID,int Imageindex,double& multinum,double& addnum,int display,int &error,string &hmessage)
{
	bool res = true;
	try
	{
		ScaleImage(m_pImage[Imageindex],&m_pImage[Imageindex],multinum,addnum);//multinum = 12.75,addnum= -1224
		if (false != display)//��ʾ
		{
			HalconCpp::DispObj(m_pImage[Imageindex],iWinID);
		}
		error = 0;
		hmessage = "ͼ����ǿ�ɹ�!";
	
	}
	catch(...)
	{
		hmessage = "ͼ����ǿʧ��!";
		error = -1;
		res = false;
	}
	return res;
}



//��ȡ��Ӧ���Ͳ�����pkg->pBody�е�λ��
//input ���ͣ� ������ pkg, 
//�����ҵ�λ��
int CVisionProcess::FindParamData(SCENE_PKG *pkg, int iType, int index, int iGroup)
{
	int iRet = -1;
	int iData = 0;
	int iCount = 0;
	int iGroupT = 0;
	int iTypeT = 0;
	int indexT = 0;
	int itmp = 0;
	if (NULL == pkg)
	{
		return iRet;
	}
	vector<VIRSION_DATA_PKG *>::iterator it;
	VIRSION_DATA_PKG *pData = NULL;
	for (it = pkg->vecBody.begin(); it != pkg->vecBody.end(); it++)
	{
		pData = *it;
		iData = pData[0].iValue; //����
		if(iData < 0)
		{
			break;
		}
		iGroupT = iData / 10000;
		itmp = iData % 10000;
		iTypeT = (itmp)/100;
		indexT = itmp%100;
		if (((iType == iTypeT) && (index == indexT)) || ((iType == iTypeT) && (iGroup == iGroupT))/*(iType * 100 + index) == iData*/)
		{
			iRet = iCount;
			break;
		}
		iCount++;
	}
	return iRet;
}


int CVisionProcess::FindSpecTypePos(const VIRSION_DATA_PKG *pkg, unsigned int *pType, int iType)
{
	int iRet = -1;
	if (NULL != pkg && NULL != pType)
	{
		for (int i = 2; i < pkg[1].iValue + VIRSION_DATA_PKG_HEADER_SIZE; i++)
		{
			if (iType == pType[i] / 100 )
			{
				iRet = i;
				break;
			}
		}
	}
	return iRet;
}
//�Ƚ����������Ƿ����
bool CVisionProcess::DobubleArrayEqueal(double * A,int arraynum)
{
	bool flag = true;
	if (arraynum<=0 || arraynum == 1)
	{
		return flag;
	}
	const double EPSINON = 0.00001;
	double temp = 0.0;
	vector<double>verdouble;
	for (int i = 0;i<arraynum;i++)
	{
		if (i+1<arraynum)
		{		
			temp = A[i] - A[i+1];
			verdouble.push_back(temp);
		}
	}
	for (int i = 0;i<arraynum-1;i++)
	{
		if ((verdouble[i] < - EPSINON)|| (verdouble[i] > EPSINON))
		{
			flag = false;
		}

	}
	return flag;
}

//CalFirstAngle��ʶ�̶������0�� �ƶ����(3.14), CamRotAngle��ʼ���ߺ�����Ĳ� ����
void CVisionProcess::CalCamMetrix(int CamerIndex,int DataCount,double MotorX[],double MotorY[],double CamRow[],double CamCol[],
	double PixSize,double(*pCamRot),double (*pMecAndCam),double(*pImageRela),double (*pImageAbs),double (*pCamParam),
	int ParamNum,double (*pCamPose),int ToolIndex,
	int AxeIndex,double CalFirstX,double CalFirstY,double CalFirstAngle,double ToolFirstX,double ToolFirstY,double CamRotAngle,
	double RotToCamX,double RotToCamY,double CalThickNess)
{
	//�ȼ��ж�,�����������鲻�����
	if (DobubleArrayEqueal(MotorX,DataCount) || DobubleArrayEqueal(MotorY,DataCount))
	{
		return ;
	}
	HTuple hv_MotorX,hv_MotorY,hv_CameraX,hv_CameraY,hv_CamRotX,hv_CamRotY;//hv_Hommat2D;
	HTuple hv_ObsoluteX,hv_ObsoluteY;
	HTuple hv_CamRow,hv_CamCol;
	HTuple hv_Hommat2D,hv_Hommat2DRot;
	HTuple hv_Hommat2DCamRot;
	HTuple hv_CamOriRot = CamRotAngle;  // ����궨ʱ��Լо���ת�ĽǶȣ�����Ի�е����ϵ��ת�ĽǶȣ��ĸ�ֵ,���ڻ����˾���
	//�������ı궨
	HTuple hv_RotToCamX = RotToCamX;  // ��һ�α궨�ó�����ת�������ϵ�е�X��
	HTuple hv_RotToCamY = RotToCamY;  // ��һ�α궨�ó�����ת�������ϵ�е�Y��

	HTuple hv_TempX,hv_TempY; 

	HTuple hv_CalFirstX = CalFirstX;//�����һ����
	HTuple hv_CalFirstY = CalFirstY; //
	HTuple hv_CalFirstAngle = CalFirstAngle;//�ƶ����Ϊ3.14

	HTuple hv_ToolFirstX = ToolFirstX;//���ߵ�һ����
	HTuple hv_ToolFirstY = ToolFirstY;

	//bool CamIsFixed = true;
	HTuple hv_CamParam,hv_CamPose;
	for (int i = 0;i<ParamNum;i++) //��������12��
	{
		hv_CamParam[i] = pCamParam[i];
	}
	for (int i = 0;i<6;i++) //����ڲ���,6��
	{
		hv_CamPose[i] = pCamPose[i];
	}
	hv_CamPose[6] = 0.0;

	SetOriginPose(hv_CamPose,0,0,-CalThickNess/1000,&hv_CamPose);//�����θ��ݱ����ȱ궨�õ���׼ȷ���

	for (int i = 0;i<DataCount;i++)//�궨�����
	{
		hv_MotorX[i] = MotorX[i]; //�Ա궨���һ����λ���
		hv_MotorY[i] = MotorY[i];
		hv_CamRow[i] = CamRow[i];
		hv_CamCol[i] = CamCol[i]; //���ص�����
		hv_CameraX[i] = CamCol[i]*PixSize;
		hv_CameraY[i] = CamRow[i]*PixSize;

	}
	if (1 == PixSize)
	{
		ImagePointsToWorldPlane(hv_CamParam,hv_CamPose,hv_CamRow,hv_CamCol,"mm",&hv_CameraX,&hv_CameraY);
	}
	//1 �������궨
	//1.1 ʹ��ͼ������ϵ�������������ı궨��
	//m_ImageToMotorRelative[CamerIndex][AxeIndex][ToolIndex].Clear();//���������Դ���������õ� �Ե�һ����Ϊ�������������
	//VectorToHomMat2d(hv_CamRow,hv_CamCol,hv_MotorX,hv_MotorY,&(m_ImageToMotorRelative[CamerIndex][AxeIndex][ToolIndex]));
	//for (int i = 0;i<(m_ImageToMotorRelative[CamerIndex][AxeIndex][ToolIndex]).TupleLength();i++)
	//{
	//	pImageRela[i] = m_ImageToMotorRelative[CamerIndex][AxeIndex][ToolIndex][i];
	//}

	if (DataCount>3) //��������4��ͼ,�������յ��ͼ
	{	
		//�ò�ֵ���궨�����ƶ����
		m_ImageToMotorRelative[CamerIndex][AxeIndex][ToolIndex].Clear();
		//�����յ��һ���궨��(100,100),Ȼ��õ�����һ��(100,100),(101,101),(102,103)...��
		HTuple Chv_MotorX,Chv_MotorY;
		HTuple Chv_CamRow,Chv_CamCol;
		for (int i = 0;i<DataCount-1;i++)//����DataCount�գ�����ֻ�ܵõ�DataCount-1����
		{
			Chv_MotorX[i] = -MotorX[i]; //�Ա궨���һ����λ���
			Chv_MotorY[i] = -MotorY[i]; //���������ص��ȥ�����㣬����֤
			Chv_CamRow[i] =  CamRow[0]-CamRow[i+1];
			Chv_CamCol[i] =  CamCol[0] -CamCol[i+1]; 
		}
		VectorToHomMat2d(Chv_CamRow,Chv_CamCol,Chv_MotorX,Chv_MotorY,&(m_ImageToMotorRelative[CamerIndex][AxeIndex][ToolIndex]));
		for (int i = 0;i<(m_ImageToMotorRelative[CamerIndex][AxeIndex][ToolIndex]).TupleLength();i++)
		{
			pImageRela[i] = m_ImageToMotorRelative[CamerIndex][AxeIndex][ToolIndex][i];
		}
	}


	//1.2 ʹ���������ϵ�������������ı궨��
	HomMat2dIdentity(&hv_Hommat2D);
	VectorToHomMat2d( hv_MotorX,hv_MotorY,hv_CameraX,hv_CameraY,&hv_Hommat2D);// m_CamerasHomMat2D[CamerIndex][ToolIndex]
	AffineTransPoint2d(hv_Hommat2D,0,0,&hv_TempX,&hv_TempY);//�ɻ�е����0,0�õ���������
	m_CamRotToMotorHomMat2D[CamerIndex][AxeIndex][ToolIndex].Clear();
	HomMat2dTranslate(hv_Hommat2D,-hv_TempX,-hv_TempY,&hv_Hommat2D);
	HomMat2dInvert(hv_Hommat2D,&(m_CamRotToMotorHomMat2D[CamerIndex][AxeIndex][ToolIndex]));
	for (int i = 0;i<(m_CamRotToMotorHomMat2D[CamerIndex][AxeIndex][ToolIndex]).TupleLength();i++)
	{
		pCamRot[i] = m_CamRotToMotorHomMat2D[CamerIndex][AxeIndex][ToolIndex][i];
	}
	//----end relative-----
	//2 ʹ�þ����������
	for (int i = 0;i<DataCount;i++)//��������ΪToolFirstX����������������������ϵ
	{
		if(0 == i)
		{
			hv_ObsoluteX[i] = ToolFirstX;
			hv_ObsoluteY[i] = ToolFirstY;
		}
		else
		{
			hv_ObsoluteX[i] = MotorX[i-1]+ToolFirstX;
			hv_ObsoluteY[i] = MotorY[i-1]+ToolFirstY;
		}
	}

	//2.1 ֱ��ʹ��ͼ������ϵrow,col�������������ı궨
	VectorToHomMat2d(hv_CamRow,hv_CamCol,hv_ObsoluteX,hv_ObsoluteY,&(m_ImageToMotorAbsolute[CamerIndex][AxeIndex][ToolIndex]));
	//��ʱ����ʹ�ã���������	
	for (int i = 0;i<(m_ImageToMotorAbsolute[CamerIndex][AxeIndex][ToolIndex]).TupleLength();i++)
	{
		pImageAbs[i] = m_ImageToMotorAbsolute[CamerIndex][AxeIndex][ToolIndex][i];
	}


	//2.2 ʹ���������ϵ,���ڼ����������ı궨��
	for (int i = 0;i<DataCount;i++)
	{
		hv_MotorX[i] = MotorX[i]+CalFirstX;
		hv_MotorY[i] = MotorY[i]+CalFirstY;
	}

	//���������ͼ���������������������߸����Ϊ0��hv_CamOriRot=0,hv_RotToCamX=hv_RotToCamY=0ʱ
	//�����ƶ��������ת180��,Ȼ���ȡ����������������ش�С��Ϊ����

	//����������궨λ�Ļ�е����任��
	//����ת�Ƕ�Ϊ0ʱ,�൱��ʡ����һ��
	HomMat2dIdentity(&hv_Hommat2DCamRot);
	HomMat2dRotate(hv_Hommat2DCamRot,hv_CamOriRot,hv_RotToCamX,hv_RotToCamY,&hv_Hommat2DCamRot);
	AffineTransPoint2d(hv_Hommat2DCamRot,hv_CameraX,hv_CameraY,&hv_CamRotX,&hv_CamRotY);

	VectorToHomMat2d(hv_CamRotX,hv_CamRotY,hv_MotorX,hv_MotorY,&hv_Hommat2D);// m_CamerasHomMat2D[CamerIndex][ToolIndex]
	//2.2.1,���ݱ궨��һ����(�������λ�õ�0,0��)��Ӧ�Ļ�е������ʾֵ���任��ʵ�ʻ�еϵ��
	//��һ������ʡ��,��������̶������ƶ�����.
	HomMat2dRotate(hv_Hommat2D,hv_CalFirstAngle,hv_CalFirstX,hv_CalFirstY,&hv_Hommat2DRot);//hv_CalFirstAngle=3.1416��=0.0


	//2.2.2�����ݱ궨��һ�����е������ʾֵ��о��ڵ�һ���궨�������ֵ�������ϵ�任��ʵ�ʼо߶�Ӧ�Ļ�еϵ��
	//���ƶ�Ϊ0ʱ,�൱��ʡ����һ��
	HomMat2dTranslate(hv_Hommat2DRot,ToolFirstX-CalFirstX,ToolFirstY-CalFirstY,&(m_CamerasHomMat2D[CamerIndex][AxeIndex][ToolIndex]));


	for (int i = 0;i<(m_CamerasHomMat2D[CamerIndex][AxeIndex][ToolIndex]).TupleLength();i++)
	{
		pMecAndCam[i] = m_CamerasHomMat2D[CamerIndex][AxeIndex][ToolIndex][i];
	}



}

void CVisionProcess::AffineCoorPoint(double CoordX,double CoordY,double CoordAng,double& PointX,double& PointY)
{
	HTuple hv_Hommat2D,hv_Hommat2DInverse;
	HTuple hv_Qx,hv_Qy;
	// double OrginalX= PointX;
	//double OrginalY= PointY;
	HomMat2dIdentity(&hv_Hommat2D);
	HomMat2dRotateLocal(hv_Hommat2D,CoordAng,&hv_Hommat2D);
	HomMat2dTranslate(hv_Hommat2D,CoordX,CoordY,&hv_Hommat2D);  
	HomMat2dInvert(hv_Hommat2D,&hv_Hommat2DInverse);
	AffineTransPoint2d(hv_Hommat2DInverse,PointX,PointY,&hv_Qx,&hv_Qy);
	PointX = hv_Qx;
	PointY = hv_Qy;

}


void CVisionProcess::AffToRobotWorld(int CamIndex,int AxisIndex,int ToolIndex,double PixSize,double ModRotPhi,double& ToolWorldX,
	double& ToolWorldY,bool WithCamParam,double CalThickness,double* pCamParam,int ParamNum,double* CamPos)
{
	HTuple TempX,TempY;
	//�����Ʒ��ģ���ڻ�еϵ�е�����
	HTuple PartmodWorldX ;
	HTuple PartmodWorldY ;

	HTuple hv_CamParam,hv_CamPose;
	for (int i = 0;i<ParamNum;i++)
	{
		hv_CamParam[i] = pCamParam[i];
	}
	for (int i = 0;i<6;i++)
	{
		hv_CamPose[i] = CamPos[i];
	}
	hv_CamPose[6] = 0.0;

	SetOriginPose(hv_CamPose,0,0,-CalThickness/1000,&hv_CamPose);
	if (WithCamParam)
	{
		ImagePointsToWorldPlane(hv_CamParam,hv_CamPose,m_OrgionMatchRow[m_CurMatchTypeProcIndex],m_OrgionMatchCol[m_CurMatchTypeProcIndex],
			"mm",&TempX,&TempY);
	}else
	{
		TempX = m_OrgionMatchCol[m_CurMatchTypeProcIndex] * PixSize;
		TempY = m_OrgionMatchRow[m_CurMatchTypeProcIndex] * PixSize;
	}

	AffineTransPoint2d(m_CamerasHomMat2D[CamIndex][AxisIndex][ToolIndex],TempX,TempY,&PartmodWorldX,
		&PartmodWorldY);

	//��ȡʾ��ʱ�оߵĻ�е����,��ת���ĵ�����.
	double ToolModWorldX = ToolWorldX;
	double ToolModWorldY = ToolWorldY;

	//��ȡ��ǰ��Ʒ�Ļ�е�������ת�ĽǶ�
	HTuple PartCurWorldX = 0.0;
	HTuple PartCurWorldY = 0.0;
	double PartCurWorldPhi = ModRotPhi;

	if (WithCamParam)
	{
		ImagePointsToWorldPlane(hv_CamParam,hv_CamPose,m_CurMatchRow[m_CurMatchTypeProcIndex],m_CurMatchCol[m_CurMatchTypeProcIndex],
			"mm",&TempX,&TempY);
	}else
	{
		TempX = m_CurMatchCol[m_CurMatchTypeProcIndex] * PixSize;
		TempY = m_CurMatchRow[m_CurMatchTypeProcIndex] * PixSize;
	}

	AffineTransPoint2d(m_CamerasHomMat2D[CamIndex][AxisIndex][ToolIndex],TempX,TempY,&PartCurWorldX,
		&PartCurWorldY);

	ToolWorldX = PartCurWorldX.D();
	ToolWorldY = PartCurWorldY.D();

	return;


	//����о�ʵ��Ӧ�õĻ�е���ꡣ 
	HTuple ToolCurWorldX ; // =?
	HTuple ToolCurWorldY ; // =?

	HTuple hv_ModRot;

	HomMat2dIdentity(&hv_ModRot);
	//�ƶ�����ǰ��Ʒ��е����λ��
	HomMat2dTranslate(hv_ModRot,PartCurWorldX - PartmodWorldX,PartCurWorldY-PartmodWorldY,&hv_ModRot);
	//��ת��ǰ��Ʒ����ת�Ƕ�
	HomMat2dRotate(hv_ModRot,PartCurWorldPhi,PartCurWorldX,PartCurWorldY,&hv_ModRot);
	AffineTransPoint2d(hv_ModRot,ToolModWorldX,ToolModWorldY,&ToolCurWorldX,&ToolCurWorldY);
	ToolWorldX = ToolCurWorldX.D();
	ToolWorldY = ToolCurWorldY.D();


}


bool  CVisionProcess::ProcProcess(SCENE_PKG *param, HTuple &OutValue, HTuple &ErrorCode, double &dOutTime)
{
	//Input
	HTuple  InputTuple;
	HObject InputObj;
	HObject ImageA;

	//OutPut
	HTuple  Message;
	HObject OutputObj;

	//tmp
	HTuple TupleLength = HTuple(0);
	HDevEngine    HEngine;

	//SCENE_PKG pkg; 
	bool bRet = false;

	LARGE_INTEGER  start,end,litmp;
	char exePath[MAX_PATH] = {0};
	char buf[MAX_PATH * 2] = {0};

	//  try
	//{
	m_bIsPass = false;
	// m_vecMsg.Reset();
	//m_vecObj.Reset();
	m_vecMsg = HTuple();
	m_vecObj = HObject();

	HDevProcedureCall hdvProcCall;
	//  try
	//{
	LockStock Stock(m_pSection);
	if (!m_hdvMainProc.IsLoaded())
	{
		//��ȡ��ǰ·���� GetCurrentDirectory��仯
		DWORD dwLen = GetModuleFileName(NULL, exePath, sizeof(exePath));
		std::string strCurPath(exePath);
		size_t lastSpit = strCurPath.find_last_of("\\");
		strCurPath.replace(lastSpit, string::npos, "");	

		sprintf_s(buf,"%s\\%s\\hdvp",strCurPath.c_str(), param->szSceneName);
		std::string strProName(buf);
		strProName += "\\process.hdvp";
		HEngine.SetProcedurePath(buf); //add spe sence;

		memset(buf, 0, MAX_PATH * 2);
		sprintf_s(buf,"%s\\halcon",strCurPath.c_str(), param->szSceneName);
		HEngine.AddProcedurePath(buf); //add commom

		//���process�������ڣ� ��ʹ��ģ�崦��
		if (-1 != access(strProName.c_str(), 0))
		{
			m_hdvMainProc.LoadProcedure("process");
		}
		else
		{
			m_hdvMainProc.LoadProcedure("processdemo");
		}
	}
	hdvProcCall = HDevProcedureCall(m_hdvMainProc);
	//}
	//catch (...)
	//{
	//          throw;
	//}



	QueryPerformanceFrequency(&litmp);//��ȡϵͳʱ��Ƶ��
	QueryPerformanceCounter(&start) ; 
	//��ͼƬ������ӵ�һ��ͼ�������
	for (int iI = 0; iI < param->imageCount; iI++)
	{
		if (0 == iI)
		{
			// copy_obj(m_pImage[iI], &ImageA, 1, 1);
			CopyObj(m_pImage[iI], &ImageA, 1, 1);
		}
		else
		{
			if (8 <= iI/* || (H_EMPTY_REGION == m_pImage[iI].Id() ? true : false)*/)
			{
				return false;
			}

			//Halcon::concat_obj(ImageA, m_pImage[iI], &ImageA);
			ConcatObj(ImageA, m_pImage[iI], &ImageA);

		}
	}
	hdvProcCall.SetInputIconicParamObject("Image",ImageA);

	//�������
	vector<VIRSION_DATA_PKG *>::iterator it;
	VIRSION_DATA_PKG *pData = NULL;
	int index = 0;
	int iCurItem = 0;
	for (it = param->vecBody.begin(); it != param->vecBody.end(); it++)
	{
		pData = *it;
		if (MAIN_PRO != (pData[0].iValue%10000)/100)
		{
			InputTuple[index] = "$";
			index++;
			InputTuple[index] = pData[0].iValue;
			index++;

			InputTuple[index] = pData[1].iValue;
			index++;
			for (int i = VIRSION_DATA_PKG_HEADER_SIZE; i < pData[1].iValue + VIRSION_DATA_PKG_HEADER_SIZE; i++)
			{
				int iT = (param->vecDataType[iCurItem][i])%100;
				switch(iT)
				{
				case 0:
					{
						InputTuple[index] = pData[i].iValue;
					}
					break;
				case 1:
					{
						InputTuple[index] = pData[i].dValue;
					}
					break;
				default:
					break;
				}
				index++;
			}
		}
		iCurItem++;
	}
	InputTuple[index] = "$";
	InputTuple[index+1] = (int)param->halconID;
	if (0 != param->display)
	{
		try
		{
			char szBuf[256] = {0};
			sprintf_s(szBuf, "./data/Pro%d.tup", param->sceneID);
			// write_tuple(InputTuple, szBuf);
			WriteTuple(InputTuple, szBuf);
		}
		catch (...)
		{
		}


	}

	hdvProcCall.SetInputCtrlParamTuple("value", InputTuple);
	hdvProcCall.SetInputCtrlParamTuple("display", param->display);
	hdvProcCall.Execute();
	ErrorCode = hdvProcCall.GetOutputCtrlParamTuple("error");
	m_vecMsg = hdvProcCall.GetOutputCtrlParamTuple("message");
	if (0 == ErrorCode)
	{
		OutValue = hdvProcCall.GetOutputCtrlParamTuple("outValue");
		m_vecObj = hdvProcCall.GetOutputIconicParamObject("object"); 
		m_bIsPass = true;

	}
	else if (0 < ErrorCode)
	{
		OutValue = hdvProcCall.GetOutputCtrlParamTuple("outValue");
		m_vecObj = hdvProcCall.GetOutputIconicParamObject("object"); 
		m_bIsPass = false;
	}
	else if (0 != param->display)
	{
		m_vecObj = hdvProcCall.GetOutputIconicParamObject("object"); 
	}

	QueryPerformanceCounter(&end) ; 
	//result.time = (double)(end.QuadPart-start.QuadPart) / (double)litmp.QuadPart;    //��ȡʱ�䵥λΪ��	
	double dTime = (double)(end.QuadPart-start.QuadPart) / (double)litmp.QuadPart;    //��ȡʱ�䵥λΪ��	
	char szBuf[125] = {0};
	sprintf_s(szBuf, "��ʱ��%0.3lf", dTime);
	dOutTime = dTime;
	m_vecMsg.Append(szBuf);
	bRet = true;
	//  }  
	//catch (HException  &exception)
	//{
	//    //m_vecMsg.Append(exception.message);
	//    //LOG_ERR("%s:%s\n",exception.proc, exception.message);
	//    //m_hdvProc.~HDevProcedure();
	//    bRet = false;
	//    ErrorCode = -1;
	//}
	//catch(HDevEngineException &hdev_exception)
	//{
	//    m_vecMsg.Append(hdev_exception.Message());
	//    LOG_ERR("%s %d:%s\n",hdev_exception.ProgLineName(),hdev_exception.ProgLineNum(),
	//        hdev_exception.Message());
	//    bRet = false;
	//    ErrorCode = -1;
	//}

	return bRet;
}


bool  CVisionProcess::ToolProcProcess(const char *pSceneName, HDevProcedure &hdvpProc,int index, HTuple InputValue,
	char *szProcName, HTuple &OutValue, HTuple &ErrorCode, unsigned int bDisplay, bool bIsUserObj)
{
	//Input
	HTuple  InputTuple;
	HObject InputObj;
	HObject ImageA;

	//OutPut
	HTuple  Message;
	HObject OutputObj;

	//tmp
	HTuple TupleLength = HTuple(0);
	HDevEngine    HEngine;
	int  IsDisplay = bDisplay;

	//SCENE_PKG pkg; 
	bool bRet = false;

	LARGE_INTEGER  start,end,litmp;
	char exePath[MAX_PATH] = {0};
	char buf[MAX_PATH * 2] = {0};

	//   try
	// {
	m_bIsPass = false;
	if (!m_bIsAppend)
	{
		//m_vecMsg.Reset();
		//m_vecObj.Reset();
	}

	HDevProcedureCall hdvProcCall;
	//EnterCriticalSection(m_pSection);
	//try
	//{
	LockStock Stock(m_pSection);
	if (!hdvpProc.IsLoaded())
	{
		//��ȡ��ǰ·���� GetCurrentDirectory��仯
		DWORD dwLen = GetModuleFileName(NULL, exePath, sizeof(exePath));
		std::string strCurPath(exePath);
		size_t lastSpit = strCurPath.find_last_of("\\");
		strCurPath.replace(lastSpit, string::npos, "");	

		sprintf_s(buf,"%s\\%s\\hdvp",strCurPath.c_str(), pSceneName);
		std::string strPrcessFile(buf);
		strPrcessFile.append("\\");
		strPrcessFile.append(szProcName);
		strPrcessFile.append(".hdvp");
		HEngine.SetProcedurePath(buf); //add spe sence;

		//���process�������ڣ� ��ʹ��ģ�崦��
		if (-1 == access(strPrcessFile.c_str(), 0))
		{
			sprintf_s(buf,"%s\\halcon",strCurPath.c_str());
			HEngine.AddProcedurePath(buf); //add commom
		}
		hdvpProc.LoadProcedure(szProcName);
	}
	hdvProcCall = HDevProcedureCall(hdvpProc);
	//}
	//catch(...)
	//{
	//     throw;
	//}
	//LeaveCriticalSection(m_pSection);

	QueryPerformanceFrequency(&litmp);//��ȡϵͳʱ��Ƶ��
	QueryPerformanceCounter(&start) ; 
	//��ͼƬ������ӵ�һ��ͼ�������
	if (0 > index)
	{
		index = 0;
	}
	//disp_obj(m_pImage[index], 3600);
	if (8 <= index/* || (H_EMPTY_REGION == m_pImage[index].Id() ? true : false)*/)
	{
		return false;
	}
	//copy_obj(m_pImage[index], &ImageA, 1, 1);//gpz ���������˵ImageAû�г�ʼ��

	hdvProcCall.SetInputIconicParamObject("Image",m_pImage[index]);
	//�������        
	hdvProcCall.SetInputCtrlParamTuple("value", InputValue);
	hdvProcCall.SetInputCtrlParamTuple("display", IsDisplay);
	if (0 != IsDisplay)
	{
		try
		{
			char path[128] = {0};
			sprintf_s(path, "./data/%s.tup", szProcName);
			//write_tuple(InputValue, path);
			WriteTuple(InputValue,path);
		}
		catch (...)
		{
		}
		//tuple_length(InputValue,&TupleLength);
	}
	hdvProcCall.Execute();
	ErrorCode = hdvProcCall.GetOutputCtrlParamTuple("error");
	Message = hdvProcCall.GetOutputCtrlParamTuple("message");
	m_vecMsg.Append(Message);
	//gen_empty_obj(&OutputObj);
	GenEmptyObj(&OutputObj);

	if (0 <= ErrorCode)
	{
		OutValue = hdvProcCall.GetOutputCtrlParamTuple("outValue");
		if (bIsUserObj)
		{
			OutputObj = hdvProcCall.GetOutputIconicParamObject("object");
		}
		bRet = true;
		m_bIsPass = true;

	}
	else if (0 != IsDisplay)
	{
		if (bIsUserObj)
		{
			OutputObj = hdvProcCall.GetOutputIconicParamObject("object"); 
		}
	}
	//count_obj(m_vecObj,&TupleLength);
	if (0 < TupleLength)
	{
		//concat_obj(m_vecObj, OutputObj, &m_vecObj);
	}
	else
	{
		// copy_obj(OutputObj, &m_vecObj,1, -1);
	}

	QueryPerformanceCounter(&end) ; 
	//result.time = (double)(end.QuadPart-start.QuadPart) / (double)litmp.QuadPart;    //��ȡʱ�䵥λΪ��	
	double dTime = (double)(end.QuadPart-start.QuadPart) / (double)litmp.QuadPart;    //��ȡʱ�䵥λΪ��	
	char szBuf[125] = {0};
	sprintf_s(szBuf, "��ʱ��%0.3lf", dTime);
	m_vecMsg.Append(szBuf);
	// }  
	//catch (HException  &exception)
	//{
	//    //m_vecMsg.Append(exception.message);
	//    //LOG_ERR("%s:%s\n",exception.proc, exception.message);
	//    //m_hdvProc.~HDevProcedure();
	//    bRet = false;
	//    ErrorCode = -1;
	//}
	//catch(HDevEngineException &hdev_exception)
	//{
	//    m_vecMsg.Append(hdev_exception.Message());
	//    LOG_ERR("%s %d:%s\n",hdev_exception.ProgLineName(),hdev_exception.ProgLineNum(),
	//        hdev_exception.Message());
	//    bRet = false;
	//    ErrorCode = -1;
	//}

	return bRet;
}

void CVisionProcess::gen_arrow_contour_xld (HObject *Arrow, HTuple Row1, HTuple Column1, HTuple Row2,HTuple Column2,HTuple HeadLength,HTuple HeadWidth)
{
	//using namespace Halcon;

	// Local iconic variables 

	HObject  TempArrow;


	// Local control variables 
	HTuple  Length, ZeroLengthIndices, DR, DC, HalfHeadWidth;
	HTuple  RowP1, ColP1, RowP2, ColP2, Index;

	//This procedure generates arrow shaped XLD contours,
	//pointing from (Row1, Column1) to (Row2, Column2).
	//If starting and end point are identical, a contour consisting
	//of a single point is returned.
	//
	//input parameteres:
	//Row1, Column1: Coordinates of the arrows' starting points
	//Row2, Column2: Coordinates of the arrows' end points
	//HeadLength, HeadWidth: Size of the arrow heads in pixels
	//
	//output parameter:
	//Arrow: The resulting XLD contour
	//
	//The input tuples Row1, Column1, Row2, and Column2 have to be of
	//the same length.
	//HeadLength and HeadWidth either have to be of the same length as
	//Row1, Column1, Row2, and Column2 or have to be a single element.
	//If one of the above restrictions is violated, an error will occur.
	//
	//
	//Init
	//gen_empty_obj(&(*Arrow));
	GenEmptyObj(&(*Arrow));
	//
	//Calculate the arrow length
	//distance_pp(Row1, Column1, Row2, Column2, &Length);
	DistancePp(Row1, Column1, Row2, Column2, &Length);

	//Mark arrows with identical start and end point
	//(set Length to -1 to avoid division-by-zero exception)
	ZeroLengthIndices = Length.TupleFind(0);
	if (0 != (ZeroLengthIndices!=-1))
	{
		Length[ZeroLengthIndices] = -1;
	}
	//
	//Calculate auxiliary variables.
	DR = (1.0*(Row2-Row1))/Length;
	DC = (1.0*(Column2-Column1))/Length;
	HalfHeadWidth = HeadWidth/2.0;
	//
	//Calculate end points of the arrow head.
	RowP1 = (Row1+((Length-HeadLength)*DR))+(HalfHeadWidth*DC);
	ColP1 = (Column1+((Length-HeadLength)*DC))-(HalfHeadWidth*DR);
	RowP2 = (Row1+((Length-HeadLength)*DR))-(HalfHeadWidth*DC);
	ColP2 = (Column1+((Length-HeadLength)*DC))+(HalfHeadWidth*DR);
	//
	//Finally create output XLD contour for each input point pair
	for (Index=0; Index<=(Length.TupleNumber())-1; Index+=1)
	{
		if (0 != (HTuple(Length[Index])==-1))
		{
			//Create_ single points for arrows with identical start and end point
			//gen_contour_polygon_xld(&TempArrow, HTuple(Row1[Index]), HTuple(Column1[Index]));
			GenContourPolygonXld(&TempArrow, HTuple(Row1[Index]), HTuple(Column1[Index]));

		}
		else
		{
			//Create arrow contour
			//gen_contour_polygon_xld
			GenContourPolygonXld(&TempArrow, ((((HTuple(Row1[Index]).TupleConcat(HTuple(Row2[Index]))).TupleConcat(HTuple(RowP1[Index]))).TupleConcat(HTuple(Row2[Index]))).TupleConcat(HTuple(RowP2[Index]))).TupleConcat(HTuple(Row2[Index])), 
				((((HTuple(Column1[Index]).TupleConcat(HTuple(Column2[Index]))).TupleConcat(HTuple(ColP1[Index]))).TupleConcat(HTuple(Column2[Index]))).TupleConcat(HTuple(ColP2[Index]))).TupleConcat(HTuple(Column2[Index])));
		}
		ConcatObj((*Arrow), TempArrow, &(*Arrow));
	}
	return;
}

void CVisionProcess::get_mesage_information (HTuple Message, HTuple *String, HTuple *Color)
{
	//using namespace Halcon;

	// Local control variables 
	HTuple  Index, str, SubString;

	(*Color) = HTuple();
	(*String) = HTuple();
	for (Index=0; Index<=(Message.TupleNumber())-1; Index+=1)
	{
		str = Message[Index];
		if (0 != ((str.TupleStrBitSelect(0))==HTuple("$")))
		{
			(*Color).Append("blue");
			//tuple_str_last_n(str, 1, &SubString);
			TupleStrLastN(str, 1, &SubString);
		}
		else if (0 != ((str.TupleStrBitSelect(0))==HTuple("&")))
		{
			(*Color).Append("yellow");
			TupleStrLastN(str, 1, &SubString);
		}
		else
		{
			(*Color).Append("black");
			TupleStrLastN(str, 0, &SubString);
		}
		(*String).Append(SubString);
	}
	return;
}



