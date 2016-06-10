#pragma once

//#import "..\\..\\CCasheMapMem\\Debug\\CCasheMap.dll"
//D:\hj\s6\HMap\ReleaseMinDependency


// #import "C:\\CCasheMap.dll"
#include "hCCasheMap.h"

#include "..\\..\\include\\CAT_Tools.h"
#include "..\\..\\include\\FSO_Tools.h"
#include <string>

using namespace std;

using namespace CCASHEMAPLib;

#define DATALEN 8192;

class CMapFileCtrl  
{
	BOOL m_IsMapInitOK;
	BOOL m_IsCreateOK;
	BOOL m_IsOpenOK;
	string m_SavePath;
	long m_SaveCount;
public:
	CMapFileCtrl();
	virtual ~CMapFileCtrl();
	
	//初始化组件
	BOOL CacheMapCom_Init(string sProgID,IUnknown* pOwner );
    BOOL CacheCom_uninit();//释放组件
	static long MAP_EnumPlugINS(CStringArray* pArr);

	//初始化
	BOOL CreatMapFile(string path,string pDevcename,string pPortname);//初始化保存路径和参数
	string GetSaveFilePath();
	BOOL SetLoopEnable(BOOL IsLoop,long nLoopCount);//是否循环存储

	BOOL SetHeaderInfo(CCASHEMAPLib::MAPFILEHEAD* pHead); //设置文件头
	CCASHEMAPLib::MAPFILEHEAD* GetHeaderInfo();           //读文件头

	long GetFreeDisk(string pPath);  //剩余多少兆

	BOOL SaveToMapFile(string pFileName);  //另存

	BOOL SetMinDiskSize(long nSize); //不小于多少兆

	BOOL SetCalbackInfo(void* pMsgCallBack,void* pHwn);//消息传递

    BOOL WriteData(BYTE* data,int nSize,BYTE* pframhear,int nnhear);//写图片数据

	void CloseMapFile(); //写文件头，关闭文件

	BOOL OpenMapFile(string pFileName);
	BOOL IsOpen(){return m_IsOpenOK;};
	BOOL SetDataType(int nRotateType,int nShiningType);
	BOOL GetDataType(long& nRotateType,long& nShiningType);
	PBYTE GetMapData(int nImagePos,int nDataFormat,int& nLen);
	BOOL SaveToData(int nBeginPos,int nEndPos,int nFrame,int nType,string pSavePath,int nfps);
	PBYTE GetComData(int nCompos,int nMovepos,int nDataType);
	void SetSaveCtrl(BOOL enable);//保存中间有没有停止

	//得到时间

public:
	IBaseCasheMap* m_pSaver;
	CCASHEMAPLib::MAPFILEHEAD* m_pHead;
};