
// BaseCasheMap.h : Declaration of the CBaseCasheMap

#ifndef __BASECASHEMAP_H_
#define __BASECASHEMAP_H_

#define IDS_PROJNAME                    100
#define IDR_BASECASHEMAP                101

#include<string>
using namespace std;

#define DATALEN 8192;
#define BUFNUM 500
#define GAMMAMAX 40
#include "Str_Tools.h"
#include "FSO_Tools.h"
#include "..\\..\\include\\Bayer.h"
typedef void(__stdcall *MSG_CALLBACK)(void* pData, long savecout, long cashenum, int msgtype);

//#define S_OK                                   ((HRESULT)0L)
//#define S_FALSE                                ((HRESULT)1L)

 
namespace CCASHEMAPLib
{ 


enum FileMode
{
	File_Single = 0,
	File_Multi = 1,
	File_Men = 2
};
/* [v1_enum] */
enum FilePos
{
	START_FILE = 0,
	CUR_FILE = 1,
	END_FILE = 2
};
typedef struct _SYSTEMTIME_CACHEMAP
{
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
} 	SYSTEMMAP;

struct MAPFILEHEAD
{

	MAPFILEHEAD(){};


	int m_HeadFormat;
	BYTE pDeviceName[256];
	BYTE pPortName[256];
	int nHeadLen;
	int nFileNum;
	BYTE *pMapFile[10];
	int nWidth;
	int nHeight;
	int nPixel;
	int nFrame;
	LONGLONG lpFrequency;
	int nSaveCount;
	int nStratPos;
	long m_nSize;
	long m_CasheSzie;
	SYSTEMMAP pStart;
	SYSTEMMAP pEnd;
	int nFormat;
	int nBayerFilter;
	double nBayRedGain;
	double nBayBlueGain;
	double nBayGreenGain;
	int nBayRedGramma;
	int nBayBlueGramma;
	int nBayGreenGramma;
};












class  CBaseCasheMap 
{
public:
	CBaseCasheMap()
	{
		m_IsWrite = TRUE;
		m_StoreStage = 0;
		m_nShiningType = 0;
		m_nRotateType = 0;
		m_IsOpen = FALSE;
		m_ShowData = NULL;
		m_OutData = NULL;
		m_MapData = NULL;
		m_TempData = NULL;
		m_BayerData = NULL;

		m_nPixelByte = 1;
		m_SaveToBigen = 0;
		m_SaveToEnd = 0;
		m_SaveToFrame = 0;
		m_SaveToType = 0;
		m_SaveToPath = "";
		m_Buffnum = 0;
		m_Buffpos = 0;

		for (int i = 0; i < 500; i++)
		{
			m_ImageBuf[i] = NULL;
			m_HeadBuf[i] = NULL;
			m_ImageStrats[i] = FALSE;
		}

		m_Imagepos = NULL;
		m_CasheImage = 0;
		m_Imagesize = 0;
		m_ComBigenPos = 0;
		//m_CasheSzie=0;m_CurrCashepos
		m_ComBigenPos = -1;
		m_SaveImageSzie = 0;
		m_IsOpenThread = FALSE;

		m_MovPos = 0;
		m_IsComData = FALSE;
		m_CamData = NULL;
		m_Comsize = 0;
		m_MapFilename = "";
		m_NewFilename = "";
		m_SavePath = "";
		m_hFactFile = NULL;
		m_pHead = NULL;

		m_IsMinBuf = FALSE;
		m_IsLoop = 0;
		m_nImageSize = 0;
		m_nLoopCount = 0;

		m_nCulPos = 0;
		m_nMinDiskSize = 0;
		m_nFreeDiskSize = 0;
		m_nFileSize = 0;

		m_OldCashepos = -1;
		m_hSaveThr = NULL;
		m_hSaveToThr = NULL;
		m_SaveEnable = TRUE;

		m_MsgFun = NULL;
		m_pMsgHwn = 0;
		m_pFramHear = NULL;

		if (!m_pHead)
			m_pHead = new MAPFILEHEAD();

		m_pHead->m_HeadFormat = 0;
		memset(m_pHead->pDeviceName, 0, 256);
		memset(m_pHead->pPortName, 0, 256);
		m_pHead->nHeadLen = DATALEN;
		m_pHead->nFileNum = 0;
		m_pHead->nWidth = 0;
		m_pHead->nHeight = 0;
		m_pHead->nPixel = 0;
		m_pHead->nFrame = 0;
		m_pHead->nSaveCount = 0;
		m_pHead->nStratPos = 0;
		m_pHead->m_nSize = 0;
		m_pHead->nBayerFilter = 0;
		m_pHead->nBayRedGain = 0.0;
		m_pHead->nBayGreenGain = 0.0;
		m_pHead->nBayBlueGain = 0.0;
		m_pHead->nBayRedGramma = 0;
		m_pHead->nBayBlueGramma = 0;
		m_pHead->nBayGreenGramma = 0;
		m_pHead->nFormat = 0;

		m_nSaveCount = 0;
		m_nOff = 0;
		m_nAlign = 0;
		m_SaveFps = 25;
		m_ShowLen = 0;
		m_DoubleBuf = NULL;
	}


public:
	BOOL m_IsWrite;
	BYTE m_fhear[DATHDLEN];
	int m_nShiningType;
	int m_nRotateType;
	BOOL m_IsOpen;
	PBYTE m_ShowData;
	PBYTE m_OutData;
	PBYTE m_BayerData;
	/*20140219 dlp**/
	PBYTE m_MapData;
	PBYTE m_TempData;
	PBYTE m_DoubleBuf;
	int m_nPixelByte;

	int m_SaveToBigen;
	int m_SaveToEnd;
	int m_SaveToFrame;
	int m_SaveToType;
	string m_SaveToPath;
	BOOL m_SaveEnable;

	CBayerFilter m_pBayer;
	//  BayerInfo   m_Bayerinfo;

	////////////串口数据///
	BOOL m_IsComData;
	PBYTE m_CamData;
	long m_ComBigenPos;
	int m_MovPos;
	long m_Comsize;
	BYTE m_CurrtComdata[512];
	//////////////////////////写参数
	BOOL m_IsOpenThread;
	int m_StoreStage;
	PBYTE m_Imagepos;
	PBYTE m_ImageBuf[500];
	PBYTE m_HeadBuf[500];
	BOOL  m_ImageStrats[500];
	BOOL m_IsMinBuf;
	int  m_Buffnum;
	int m_Buffpos;
	//PBYTE m_TimeBuf[500];
	int m_CasheImage;
	int m_Imagesize;
	int m_SaveImageSzie;
	long m_CurrCashepos;
	long m_OldCashepos;
	int m_ImageCount;

	////////////////////////////读参数  

	HRESULT SaveToData(/*[in]*/int nBeginPos,/*[in]*/int nEndPos, /*[in]*/int nFrame, /*[in]*/int  nType, /*[in]*/BYTE *pSavePath,/*[in]*/ int nFps);
	BYTE*  GetMapData(/*[in]*/int nImagePos,/*[in]*/int nDataFormat,/*[out]*/int* nLen);
	HRESULT SetDataType(/*[in]*/int nRotateType,/*[in]*/int nShiningType);
	HRESULT GetDataType(/*[out]*/long* nRotateType,/*[out]*/long*  nShiningType);
	HRESULT OpenMapFile(/*[in]*/BYTE* pFileName);
	HRESULT SetCalbackInfo(/*[in]*/void* pMsgCallBack,/*[in]*/void* pHwn);
	ULONGLONG GetMapFileSize();
	HRESULT SetMapFileHead(/*[in]*/struct MAPFILEHEAD*  phead);
	struct MAPFILEHEAD*  GetMapFileHead ();
	HRESULT SaveToMapFile(/*[in]*/BYTE* pFileName);
	HRESULT SetLoopEnable(/*[in]*/BOOL IsLoop,/*[in]*/long nLoopCount);
	HRESULT SetMinDiskSize(/*[in]*/ULONGLONG nSize);
	HRESULT DeleteMapFile();
	ULONGLONG GetFreeDisk(/*[in]*/BYTE* pPath);
	long WriteData(/*[in]*/BYTE*pData, /*[in]*/int nData,/*[in]*/BYTE *pFramHear,/*[in]*/int nHear);
	void  CloseMapFile();
	HRESULT CreatMapFile(/*[in]*/BYTE* pFilePath,/*[in]*/BYTE *pDevcename,/*[in]*/BYTE *pPortname);

	void CloseMapFile2();
	//////////////////////////写函数
	int WriteDATHead();
	int WriteData2(BYTE *pData, int nData, BYTE *pFramHear, int nHear);
	////////////////////////读函数
	BOOL SetMapFilePtrPos(ULONGLONG nPos);
	int ReadFileHeadData();
	long ReadData(BYTE *pData, int nData);
	int CountWidth(int Winput);

protected:
	CriticalSection m_SaveLock;
	string m_NewFilename;
	string m_MapFilename;
	string m_SavePath;
	PBYTE m_pFramHear;
	HANDLE m_hFactFile;

	MAPFILEHEAD* m_pHead;

	BOOL m_IsLoop;
	long m_nImageSize;
	long m_nLoopCount;

	ULONGLONG m_nCulPos;
	ULONGLONG m_nFileSize;
	ULONGLONG m_nFreeDiskSize;
	ULONGLONG m_nMinDiskSize;

	HANDLE m_hSaveToThr;
	HANDLE m_hSaveThr;

	MSG_CALLBACK m_MsgFun;
	void* m_pMsgHwn;

	long m_nSaveCount;

	int m_SaveFps;
	int m_ShowLen;
	int m_nOff;
	int m_nAlign;
private:
	static unsigned __stdcall ThreadSaveFuc(LPVOID lpParam);//实际处理函数
	static unsigned __stdcall SaveToThrd(void * pParam);

public:
	HRESULT SetSaveCtrl (BOOL enable);
	 BYTE* GetSaveFilePath (void);
	 BYTE* GetComData (int nCompos, int nMovepos, int nDataType);
	LONGLONG GetMapTime(int nImagePos);

	long GetShowData(BYTE *PImageData, PBYTE* PShowData);
};


}   //namesp



#endif //__BASECASHEMAP_H_
