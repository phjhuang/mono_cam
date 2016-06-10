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
	
	//��ʼ�����
	BOOL CacheMapCom_Init(string sProgID,IUnknown* pOwner );
    BOOL CacheCom_uninit();//�ͷ����
	static long MAP_EnumPlugINS(CStringArray* pArr);

	//��ʼ��
	BOOL CreatMapFile(string path,string pDevcename,string pPortname);//��ʼ������·���Ͳ���
	string GetSaveFilePath();
	BOOL SetLoopEnable(BOOL IsLoop,long nLoopCount);//�Ƿ�ѭ���洢

	BOOL SetHeaderInfo(CCASHEMAPLib::MAPFILEHEAD* pHead); //�����ļ�ͷ
	CCASHEMAPLib::MAPFILEHEAD* GetHeaderInfo();           //���ļ�ͷ

	long GetFreeDisk(string pPath);  //ʣ�������

	BOOL SaveToMapFile(string pFileName);  //���

	BOOL SetMinDiskSize(long nSize); //��С�ڶ�����

	BOOL SetCalbackInfo(void* pMsgCallBack,void* pHwn);//��Ϣ����

    BOOL WriteData(BYTE* data,int nSize,BYTE* pframhear,int nnhear);//дͼƬ����

	void CloseMapFile(); //д�ļ�ͷ���ر��ļ�

	BOOL OpenMapFile(string pFileName);
	BOOL IsOpen(){return m_IsOpenOK;};
	BOOL SetDataType(int nRotateType,int nShiningType);
	BOOL GetDataType(long& nRotateType,long& nShiningType);
	PBYTE GetMapData(int nImagePos,int nDataFormat,int& nLen);
	BOOL SaveToData(int nBeginPos,int nEndPos,int nFrame,int nType,string pSavePath,int nfps);
	PBYTE GetComData(int nCompos,int nMovepos,int nDataType);
	void SetSaveCtrl(BOOL enable);//�����м���û��ֹͣ

	//�õ�ʱ��

public:
	IBaseCasheMap* m_pSaver;
	CCASHEMAPLib::MAPFILEHEAD* m_pHead;
};