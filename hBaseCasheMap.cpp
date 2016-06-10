// BaseCasheMap.cpp : Implementation of CBaseCasheMap
 

#include "stdafx.h"
#include "hCCasheMap.h"
#include "hBaseCasheMap.h"
#include <process.h>  
//#include "include\\pfDoubleRate.h"
#include "..\..\include\YUV2RGB.h"
#include "..\..\include\Avi_Tools.h"
/////////////////////////////////////////////////////////////////////////////
// CBaseCasheMap
#include <string>
#include <sstream>
using namespace std;

#define COMLEN 15;

HRESULT CBaseCasheMap::CreatMapFile(BYTE *pFilePath, BYTE *pDevcename, BYTE *pPortname)
{
	m_IsWrite=TRUE;
	if(m_IsOpenThread)
		return S_FALSE;

	if(m_hFactFile)
		CloseMapFile();

	m_StoreStage=0;
	m_hFactFile=NULL;

	HANDLE hFile = INVALID_HANDLE_VALUE;
	DWORD nAccess = 0, nShareMode = 0;
	DWORD nCreate = 0, nFlag = FILE_FLAG_RANDOM_ACCESS;
	DWORD nProtect = 0,nMapAccess = 0;
	ULONGLONG nTmp = 0;

	nAccess = GENERIC_READ|GENERIC_WRITE;
	nShareMode = FILE_SHARE_WRITE | FILE_SHARE_READ;
	nCreate = CREATE_ALWAYS;
	nProtect = PAGE_READWRITE;
	nMapAccess = FILE_MAP_WRITE;
	nFlag = FILE_FLAG_NO_BUFFERING;

	if(!pFilePath) return S_FALSE;

	string path=(char*)pFilePath;

	if(Str_Tools::Find(path,',')>0)
	{
		stringstream strstr( path);
		string pos="";	
		getline(strstr,pos,',');

		path=pos;
	}

	//if(!FSO_Tools::isFsoExists(path))
	//	return S_FALSE;

	m_SavePath=path;

	SYSTEMTIME savetime;
	GetLocalTime(&savetime);

	string datname="";

	char ss[512];

	memset(ss,0,512);
	sprintf(ss,"%s%s_%s_%0.4d%0.2d%0.2d%0.2d%0.2d%0.2d.dat",m_SavePath.c_str(),pDevcename,pPortname,savetime.wYear, savetime.wMonth,savetime.wDay,savetime.wHour,savetime.wMinute,savetime.wSecond);

	m_MapFilename=ss;

	nFlag = FILE_FLAG_NO_BUFFERING;
	m_hFactFile = CreateFile((char*)m_MapFilename.c_str(), nAccess, nShareMode, NULL, nCreate, nFlag, NULL);
	if(m_hFactFile == INVALID_HANDLE_VALUE) 
	{
		DWORD nErr = GetLastError();	return S_FALSE;
	}

	m_nFreeDiskSize =GetFreeDisk((BYTE*)m_SavePath.c_str());
	m_nFileSize=GetMapFileSize();

	if(!m_pFramHear)
		m_pFramHear=new BYTE[FRAMEHEADLEN];

	memset(m_pFramHear,0,FRAMEHEADLEN);
	if(!m_pHead)
		m_pHead=new MAPFILEHEAD();

	m_pHead->nHeadLen = DATALEN;
	m_pHead->nFileNum = 0;
	m_pHead->pStart.wYear=savetime.wYear;
	m_pHead->pStart.wMonth=savetime.wMonth;
	m_pHead->pStart.wDay=savetime.wDay;
	m_pHead->pStart.wHour=savetime.wHour;
	m_pHead->pStart.wMinute=savetime.wMinute;
	m_pHead->pStart.wSecond=savetime.wSecond;
	m_pHead->pStart.wMilliseconds=savetime.wMilliseconds;

	LARGE_INTEGER fre;
	QueryPerformanceFrequency(&fre);

	m_pHead->lpFrequency=fre.QuadPart;
	/*2014 0219 dlp*/
	char tmps[256];
	memset(tmps,0,256);
	sprintf(tmps,"%s",pDevcename);
	memcpy(m_pHead->pDeviceName,tmps,strlen(tmps));

	memset(tmps,0,256);
	sprintf(tmps,"%s",pPortname);
	memcpy(m_pHead->pDeviceName,tmps,strlen(tmps));
	//memcpy(m_pHead->pPortName,pPortname,strlen(pPortname));
	m_nSaveCount=0;

	WriteDATHead();

	m_nCulPos=0;

	m_nFileSize=0;
	m_ImageCount=0;

	m_CurrCashepos=0;
	m_Imagepos=m_ImageBuf[0];

	m_StoreStage=1;

	//if(m_IsMinBuf){
		m_hSaveThr = (HANDLE)_beginthreadex(0,0,ThreadSaveFuc,this,0,0); 
	//}
	return S_OK;
}

void CBaseCasheMap::CloseMapFile2()
{
	if(m_IsWrite)
	{
		SYSTEMTIME savetime;
		GetLocalTime(&savetime);
		m_pHead->pEnd.wYear=savetime.wYear;
		m_pHead->pEnd.wMonth=savetime.wMonth;
		m_pHead->pEnd.wDay=savetime.wDay;
		m_pHead->pEnd.wHour=savetime.wHour;
		m_pHead->pEnd.wMinute=savetime.wMinute;
		m_pHead->pEnd.wSecond=savetime.wSecond;
		m_pHead->pEnd.wMilliseconds=savetime.wMilliseconds;

		// TODO: Add your implementation code here
		int HeadWriteSign = WriteDATHead();

		DWORD nMove = FILE_BEGIN;
		ULONGLONG nPos=0;
		DWORD nRet = 0;
		LONG nLow = 0, nHigh = 0;
		LONG *pHigh = &nHigh;
		nLow = (LONG)(nPos & 0xffffffff);
		nHigh = (LONG)((nPos >> 32) & 0xffffffff);

		nRet = SetFilePointer(m_hFactFile, nLow, pHigh, nMove);

		m_nCulPos=nPos;
		if(nRet == 0xFFFFFFFF)
			m_nCulPos=0;

		for(int i=0;i<BUFNUM;i++)
		{
			if(m_ImageBuf[i])
			{
				if(m_IsMinBuf)
					delete m_ImageBuf[i];
				m_ImageBuf[i]=NULL;
			}

			if(m_HeadBuf[i])
			{
				delete m_HeadBuf[i];
				m_HeadBuf[i]=NULL;

			}
		}
	}
	else{

		if(m_ShowData)
			delete m_ShowData;

		m_ShowData=NULL;

		if(m_OutData)
			delete m_OutData;
		m_OutData=NULL;


		if(m_MapData)
			delete m_MapData;
		m_MapData=NULL;

		if(m_DoubleBuf)
		  delete m_DoubleBuf;
		m_DoubleBuf=NULL;

		if(m_TempData)
			delete m_TempData;
		m_TempData=NULL;

		if(m_BayerData)
			delete m_BayerData;
		m_BayerData=NULL;

		if(m_CamData)
			delete m_CamData;

		m_CamData=NULL;


	}

	if(m_hFactFile != INVALID_HANDLE_VALUE) 
		CloseHandle(m_hFactFile);

	m_hFactFile = NULL; 

	if(m_pFramHear)
		delete m_pFramHear;
	m_pFramHear=NULL;
	/* 20140211 dlp
	if(m_pHead)
	delete m_pHead;
	m_pHead=NULL;
	*/
	m_IsOpen=FALSE;
}
 void CBaseCasheMap::CloseMapFile()
{
	if(m_IsWrite)
	{
		if(m_StoreStage==1)
			m_StoreStage=2;

	
	}
	else CloseMapFile2();
}

int CBaseCasheMap::WriteData2(BYTE *pData,int nData,BYTE *pFramHear,int nHear)
{
	if(!m_IsWrite)
		return 0;

	long nlen=0;
	DWORD nRet = 0;
	int tpval = 0;
	if(m_hFactFile == NULL)
	{
		return 0;
	}

	//if(nHear<FRAMEHEADLEN)
	//	memcpy(m_pFramHear,pFramHear,nHear);
	//else memcpy(m_pFramHear,pFramHear,FRAMEHEADLEN);

	ULONGLONG nsize=nData;
	ULONGLONG frameHead=nHear;

	m_nFreeDiskSize-=(nsize+frameHead);   //GetFreeDisk((unsigned char *)m_MapFilename.c_str());
	//DEBUG_INFO1(" Free:%64d",m_nFreeDiskSize/(1048576));
	//DEBUG_INFO1(" Disk:%64d",m_nMinDiskSize/1048576);
	if(m_nMinDiskSize != 0 && (m_nFreeDiskSize<m_nMinDiskSize))
	{
		return 1;
	}

	if(m_nLoopCount>0)
	{
		if(m_ImageCount>m_nLoopCount)
		{
			return 1;
		}
	}

	if(pFramHear)
	{
		if( !WriteFile(m_hFactFile, pFramHear, FRAMEHEADLEN, &nRet, NULL))
		{
			DWORD nErr = GetLastError(); 

			return 0;
		}
	}

	if( !WriteFile(m_hFactFile, pData, nData, &nRet, NULL))
	{
		DWORD nErr = GetLastError(); 

		return 0;
	}
	else
	{
		m_nCulPos+=(nsize+frameHead);

		m_ImageCount+=m_Buffnum;

		nlen=nData+nHear;
	}

	return nlen;

}

 long CBaseCasheMap::WriteData(BYTE *pData,int nData,BYTE *pFramHear,int nHear)
{
	if(!m_IsWrite)
		return 0;

	int ste=0;
	static long noldtime=0;

	if(m_StoreStage!=1)
		return 0;

	//if((!m_ImageBuf))
	//	return 0;

	//LARGE_INTEGER lpPerformanceCount;
	//QueryPerformanceCounter(&lpPerformanceCount);

	if((m_Imagesize!=nData)&&(nData!=0))
		m_Imagesize=nData;

	if(m_IsMinBuf)
	{
		memcpy(m_Imagepos+m_Buffpos*(m_SaveImageSzie+FRAMEHEADLEN), pFramHear, nHear);
		memcpy(m_Imagepos+m_Buffpos*(m_SaveImageSzie+FRAMEHEADLEN)+FRAMEHEADLEN, pData, m_Imagesize);
	}
	else
	{
		m_ImageBuf[ m_CurrCashepos]=pData;
		memcpy(m_HeadBuf[m_CurrCashepos], pFramHear, nHear);
	/*
		ste=WriteData2(pData,m_SaveImageSzie, pFramHear,nHear);
	
		if(ste<=1)
		{
			if(m_MsgFun)
				m_MsgFun(m_pMsgHwn,m_ImageCount,0,ste);
		}


		long newtime=::GetTickCount();


		if((newtime-noldtime)>=1000)
		{
			if(m_MsgFun){
				//	DEBUG_INFO1("count:%d",p->m_ImageCount);
				m_MsgFun(m_pMsgHwn,m_ImageCount,0,2);
			}

			noldtime=newtime;
		}
		
		*/
		//////////////
		/*
		int estroe=m_StoreStage;

		if((estroe==2))
		{
		CloseMapFile2();
		m_StoreStage=0;

		if(m_MsgFun)
		m_MsgFun(m_pMsgHwn,m_ImageCount,0,3);


		}
		*/
		//memcpy(m_HeadBuf[m_CurrCashepos], &m_nSaveCount, 8);

	}

	m_Buffpos++;

	if(m_Buffpos>(m_Buffnum-1))
	{
		m_ImageStrats[m_CurrCashepos]=TRUE;

		m_OldCashepos=m_CurrCashepos;

		m_CurrCashepos++;

		if(m_CurrCashepos>=m_CasheImage)
			m_CurrCashepos=0;

		m_Imagepos=m_ImageBuf[ m_CurrCashepos];
		m_Buffpos=0;

		//if(m_ImageStrats[m_CurrCashepos])
		//	DEBUG_INFO1("m_CurrCashepos:%d",m_CurrCashepos);

	}

	m_nSaveCount++;
	/*LARGE_INTEGER lpPerformanceCount1;
	QueryPerformanceCounter(&lpPerformanceCount1);

	double t1=lpPerformanceCount.QuadPart;
	double t2=lpPerformanceCount1.QuadPart;

	double ss=(t2-t1)*1000.0/(m_pHead->lpFrequency);

	if(ss>3.0)
	DEBUG_INFO1("time:%f",ss);*/

	return nData+FRAMEHEADLEN;
}

 ULONGLONG CBaseCasheMap::GetFreeDisk(BYTE *pPath)
{
	if(!m_IsWrite)
		return 0;

	int nFind = 0;
	string pRoot =(char*)pPath;
	ULARGE_INTEGER uFree;
	if( pRoot.empty() ) pRoot = m_MapFilename;
	if( pRoot.empty() ) return 0;
	nFind = pRoot.find("\\");
	if(nFind == -1)
		return 0;
	else
		pRoot = pRoot.substr(0,nFind+1);
	if(!GetDiskFreeSpaceEx(pRoot.c_str(), &uFree, NULL, NULL)) return 0;
	return uFree.QuadPart;
}

 HRESULT  CBaseCasheMap::DeleteMapFile()
{
	// TODO: Add your implementation code here
	DeleteFile((char*)m_MapFilename.c_str());
	return S_OK;
}

 HRESULT CBaseCasheMap::SetMinDiskSize(ULONGLONG nSize)
{
	if(!m_IsWrite)
		return S_FALSE;
	// TODO: Add your implementation code here
	m_nMinDiskSize=nSize;

	return S_OK;
}

 HRESULT CBaseCasheMap::SetLoopEnable(BOOL IsLoop, long nLoopCount)
{
	if(!m_IsWrite)
		return S_FALSE;

	m_IsLoop=IsLoop;
	m_nLoopCount=nLoopCount;
	return S_OK;
}

 HRESULT CBaseCasheMap::SaveToMapFile(BYTE *pFileName)
{
	// TODO: Add your implementation code here
	m_NewFilename = (char*)pFileName;
	m_hSaveToThr = (HANDLE)_beginthreadex(0,0,SaveToThrd,this,0,0); 
	return S_OK;
}

 MAPFILEHEAD* CBaseCasheMap::GetMapFileHead()
{
	return m_pHead;
}

 HRESULT CBaseCasheMap::SetMapFileHead(struct MAPFILEHEAD *phead)
{
	if(!m_IsWrite)
	{
		m_pHead->nBayerFilter=phead->nBayerFilter;
		m_pHead->nBayRedGain=phead->nBayRedGain;
		m_pHead->nBayBlueGain=phead->nBayBlueGain;
		m_pHead->nBayGreenGain=phead->nBayGreenGain;
		m_pHead->nBayRedGramma=phead->nBayRedGramma;
		m_pHead->nBayBlueGramma=phead->nBayBlueGramma;
		m_pHead->nBayGreenGramma=phead->nBayGreenGramma;
		return S_FALSE;

	}

	if((!phead)||(m_StoreStage>0))
		return S_FALSE;

	m_Imagesize=m_pHead->m_nSize;
	memcpy(m_pHead,phead,sizeof(MAPFILEHEAD));

	m_SaveImageSzie=m_pHead->m_nSize;
	if((m_pHead->m_nSize%512)>0)
		m_SaveImageSzie=(m_pHead->m_nSize/512+1)*512;

	m_CasheImage=(m_pHead->m_CasheSzie*1024*1024)/BUFFERSIZE;

	if(m_CasheImage>500)
		m_CasheImage=500;

	if(m_SaveImageSzie<=1024*512)
	{
		for(int i=0;i<m_CasheImage;i++)
		{
			if(m_ImageBuf[i])
				delete m_ImageBuf[i];

			m_ImageBuf[i]=NULL;

			m_ImageBuf[i]=new BYTE[BUFFERSIZE];
			memset(m_ImageBuf[i],0,BUFFERSIZE);

			long szie=m_SaveImageSzie+FRAMEHEADLEN;

			m_Buffnum=BUFFERSIZE/szie;
			m_Buffpos=0;
		}

		m_IsMinBuf=TRUE;

	}
	else 
	{
		for(int i=0;i<m_CasheImage;i++)
		{
			if(m_ImageBuf[i])
				delete m_ImageBuf[i];

			m_ImageBuf[i]=NULL;

			if(m_HeadBuf[i])
				delete m_HeadBuf[i];

			m_HeadBuf[i]=new BYTE[FRAMEHEADLEN];
			memset(m_HeadBuf[i],0,FRAMEHEADLEN);
		}

		m_Buffnum=1;
		m_Buffpos=0;

		m_IsMinBuf=FALSE;
	}

	return S_OK;
}

 ULONGLONG CBaseCasheMap::GetMapFileSize()
{
	if(m_hFactFile == NULL) return 0;

	ULONG lpFileSizeHigh=0;
	ULONG lowsize=GetFileSize(m_hFactFile,&lpFileSizeHigh); 

	if(lowsize==0xffffffff)
	{
		if(lpFileSizeHigh == 0) return 0;
		if(GetLastError() != NO_ERROR) return 0;
	}

	ULONGLONG nHsize=lpFileSizeHigh;
	ULONGLONG nLSize=lowsize;
	ULONGLONG npos=0;



	npos=nHsize*(ULONGLONG)4096*(ULONGLONG)1024*(ULONGLONG)1024+nLSize;

	return npos;
}

HRESULT CBaseCasheMap::SetCalbackInfo(void* pMsgCallBack,/*[in]*/void* pHwn)
{
	m_MsgFun=(MSG_CALLBACK)pMsgCallBack;
	m_pMsgHwn=pHwn;
	return S_OK;
}

HRESULT CBaseCasheMap::OpenMapFile(BYTE* pFileName)
{
	if(!pFileName) return S_FALSE;

	m_IsWrite=FALSE;
	if(m_hFactFile)
		CloseMapFile();

	m_hFactFile=NULL;

	memset(m_fhear,0,DATHDLEN);
	HANDLE hFile = INVALID_HANDLE_VALUE;
	DWORD nAccess = 0, nShareMode = 0;
	DWORD nCreate = 0, nFlag = FILE_FLAG_RANDOM_ACCESS;
	DWORD nProtect = 0,nMapAccess = 0;
	ULONGLONG nTmp = 0;

	nAccess = GENERIC_READ;
	nShareMode = FILE_SHARE_READ;
	nCreate = OPEN_ALWAYS;
	nProtect = PAGE_READONLY;
	nMapAccess = FILE_MAP_READ;


	nFlag = FILE_FLAG_NO_BUFFERING;
	m_hFactFile = CreateFile((char*)pFileName, nAccess, nShareMode, NULL, nCreate, nFlag, NULL);
	if(m_hFactFile == INVALID_HANDLE_VALUE) 
	{
		DWORD nErr = GetLastError();	return S_FALSE;
	}

	m_MapFilename=(char*)pFileName;

	if(!m_pFramHear)
		m_pFramHear=new BYTE[FRAMEHEADLEN];

	memset(m_pFramHear,0,FRAMEHEADLEN);

	if(!m_pHead)
		m_pHead=new MAPFILEHEAD();

	if(ReadFileHeadData()<=0)
	{
		CloseMapFile();

		return S_FALSE;
	}

	//知道图像位深

	if(m_pHead->nFormat==0){
		if (m_pHead->nPixel<=8)
			m_nPixelByte = 1;
		else if (m_pHead->nPixel==32)
			m_nPixelByte = 4;
		else if (m_pHead->nPixel==24)
			m_nPixelByte = 3;
		else if (m_pHead->nPixel<=16)
			m_nPixelByte = 2;
	}
	else if((m_pHead->nFormat==1)||(m_pHead->nFormat==3))//bayer格式
	{

		m_pBayer.SetBayFilter(m_pHead->nBayerFilter);
		m_pBayer.SetGain(m_pHead->nBayRedGain, m_pHead->nBayBlueGain, m_pHead->nBayGreenGain);
		m_pBayer.SetGamma(m_pHead->nBayRedGramma, m_pHead->nBayBlueGramma, m_pHead->nBayGreenGramma);

	}
	else if(m_pHead->nFormat==4)
		m_nPixelByte = 3;


	long savesize=m_pHead->nWidth*m_pHead->nHeight;

	//20150810 dlp当文件没有正常的关闭，需要手动计算图像存储个数

	long savesize1=m_pHead->nWidth*m_pHead->nHeight*m_nPixelByte;
	if((savesize1%512)!=0)
		savesize1=(savesize1/512+1)*512;


		if(savesize1>0&&m_pHead->nSaveCount==0){
	
		ULONGLONG titlesize=8192;
		ULONGLONG fsize=512;
		ULONGLONG framesize=savesize1+fsize;

		ULONGLONG filesize=GetMapFileSize();

		long countsavenum=(filesize-titlesize)/framesize;
		if(countsavenum>0)
			m_pHead->nSaveCount=countsavenum;

	}


	if(!m_ShowData)
		m_ShowData=new BYTE[savesize*4];

	if(!m_OutData)
		m_OutData=new BYTE[savesize*4];


	if(!m_MapData)
		m_MapData=new BYTE[savesize*4];  

	if(!m_DoubleBuf)
		m_DoubleBuf=new BYTE[savesize*4]; 
	
	if(!m_TempData)
		m_TempData=new BYTE[savesize*4];  

	if(!m_BayerData)
		m_BayerData=new BYTE[savesize*4];

	



	if(m_pHead->nPixel>8&&m_pHead->nFormat==0)
	{
		m_nAlign=0;
		m_nOff=m_pHead->nPixel-(m_nPixelByte-1)*8;
	}


	///////////同步串口数据//////////////////
	string conpath=m_MapFilename+".txt";
	m_IsComData=FALSE;
	long nszie=0;
	if(FSO_Tools::isFsoExists(conpath,NULL,&nszie))
	{
		if(nszie>0)
		{

			HANDLE hComFile = INVALID_HANDLE_VALUE;

			hComFile = CreateFile((char*)conpath.c_str(), nAccess, nShareMode, NULL, nCreate, nFlag, NULL);
			if(hComFile == INVALID_HANDLE_VALUE) 
				m_IsComData=FALSE;
			else
			{
				if(m_CamData)
					delete m_CamData;

				m_CamData=new BYTE[nszie];
				DWORD nRet = 0;
				if( !ReadFile(hComFile, m_CamData, nszie, &nRet, NULL) )
				{
					CloseHandle(hComFile);

					hComFile=NULL;

					if(m_CamData)
						delete m_CamData;

					m_CamData=NULL;

					m_Comsize=0;

				}
				else
				{
					m_IsComData=TRUE;
					m_Comsize=nszie;
				}
			}
		}

	}
	else 
	{
		m_IsComData=FALSE;
		m_CamData=NULL;
		m_Comsize=0;
	}

	m_IsOpen=TRUE;
	//

/*	double t2=0;
	for(int i=0;i<m_pHead->nSaveCount;i++)
	{
		double t1=GetMapTime(i);
		double ss=(t1*1000.0)/(m_pHead->lpFrequency);

		if((ss-t2)>3.0)
			DEBUG_INFO2("time:%f   %d",(ss-t2),i);

		t2=ss;
	}*/


	return S_OK;
}

HRESULT CBaseCasheMap::SetDataType(int nRotateType, int nShiningType)
{   
	if(m_IsWrite)
		return S_FALSE;

	m_nRotateType = nRotateType;
	m_nShiningType = nShiningType;

	return S_OK;
}

HRESULT CBaseCasheMap::GetDataType(long* nRotateType, long*  nShiningType)
{
	if(m_IsWrite)
		return S_FALSE;

	if(nRotateType)
		*nRotateType=(long)m_nRotateType;

	if(nShiningType)
		*nShiningType=(long)m_nShiningType;

	return S_OK;
}

 BYTE*  CBaseCasheMap::GetMapData(int nImagePos,int nDataFormat, int* nLen)
{
	if((!m_IsOpen)||m_IsWrite)
		return NULL;

	if ((nImagePos>=m_pHead->nSaveCount)||(nImagePos<0))
		return NULL;

	DWORD nRet = 0;
	long savesize=m_pHead->m_nSize;

	if((savesize%512)!=0)
		savesize=(savesize/512+1)*512;

	ULONGLONG hearlen=FRAMEHEADLEN;
	ULONGLONG nsize=(ULONGLONG)savesize+hearlen;
	ULONGLONG imagenum=nImagePos;
	ULONGLONG len=DATHDLEN;


	ULONGLONG npos=nsize*imagenum+len;

	SetMapFilePtrPos(npos);

	memset(m_TempData,0,nsize*2);
	nRet = ReadData(m_TempData,nsize);//512
	*nLen = (int)nRet;
	memset(m_pFramHear,0,FRAMEHEADLEN);
	memcpy( m_pFramHear, m_TempData, FRAMEHEADLEN);

	memset(m_MapData,0,nsize*3);
	memset(m_DoubleBuf,0,nsize*3);

	int newByte=1;

	if (nDataFormat == 1)
	{
		int NewWidth=m_pHead->nWidth;

		if (m_pHead->nFormat==3)
		{
			int nWidth1=0;
			//pfDoubleRate_GetModulatedWidthColor(m_pHead->nWidth,&nWidth1);

			//int demod = pfDoubleRate_DeModulateImageColor(m_DoubleBuf, m_TempData+FRAMEHEADLEN, m_pHead->nWidth , m_pHead->nHeight, nWidth1);
			
			int nFactWidth =m_pHead->nWidth * 3;
			newByte=3;
			NewWidth = m_pHead->nWidth;
			m_pBayer.SetSize(m_pHead->nWidth, nFactWidth, m_pHead->nHeight);

			m_pBayer.SetBayFilter(m_pHead->nBayerFilter);
		    m_pBayer.SetGain(m_pHead->nBayRedGain, m_pHead->nBayBlueGain, m_pHead->nBayGreenGain);
		    m_pBayer.SetGamma(m_pHead->nBayRedGramma, m_pHead->nBayBlueGramma, m_pHead->nBayGreenGramma);

			m_pBayer.bayer_to_rgb24(m_DoubleBuf,m_MapData);

			*nLen=m_pHead->nWidth*newByte*m_pHead->nHeight;
		}
		else if(m_pHead->nFormat==2)
		{
			int nWidth1=0;
			//pfDoubleRate_GetModulatedWidth(m_pHead->nWidth,&nWidth1);

			NewWidth = m_pHead->nWidth;//
			newByte=3;
			//int demod = pfDoubleRate_DeModulateImage(m_DoubleBuf, m_TempData+FRAMEHEADLEN, NewWidth , m_pHead->nHeight, nWidth1);

			CYUV2RGB::convert_to_gray8(m_DoubleBuf,m_MapData,m_pHead->nWidth,m_pHead->nHeight,m_nPixelByte,0,m_nOff);
			*nLen = (int)(m_pHead->nWidth*m_pHead->nHeight*3);
		}
		else if(m_pHead->nFormat==0)
		{
			NewWidth = m_pHead->nWidth;

			//20150810 dlp图像宽度不是4的倍数
			if(NewWidth%4>0){

				NewWidth=(NewWidth/4+1)*4;
			}
			newByte=3;
			*nLen = (int)(NewWidth*m_pHead->nHeight*3);

			CYUV2RGB::convert_to_gray8(m_TempData+FRAMEHEADLEN,m_MapData,m_pHead->nWidth,m_pHead->nHeight,m_nPixelByte,0,m_nOff);
			//memcpy(m_pTempImage,m_pSingleImage,NewWidth*m_pHead->nHeight);
		}
		else if(m_pHead->nFormat==1){

			int nFactWidth =m_pHead->nWidth * 3;
			newByte=3;
			NewWidth = m_pHead->nWidth;
			m_pBayer.SetSize(m_pHead->nWidth, nFactWidth, m_pHead->nHeight);

			m_pBayer.SetBayFilter(m_pHead->nBayerFilter);
		    m_pBayer.SetGain(m_pHead->nBayRedGain, m_pHead->nBayBlueGain, m_pHead->nBayGreenGain);
		    m_pBayer.SetGamma(m_pHead->nBayRedGramma, m_pHead->nBayBlueGramma, m_pHead->nBayGreenGramma);

			m_pBayer.bayer_to_rgb24(m_TempData+FRAMEHEADLEN,m_MapData);

			*nLen=m_pHead->nWidth*newByte*m_pHead->nHeight;
			//	memcpy(m_MapData,m_BayerData,m_pHead->nWidth*newByte*m_pHead->nHeight);
		}
		else if(m_pHead->nFormat==4)
		{

			NewWidth = m_pHead->nWidth;
			newByte=3;
			*nLen = (int)nRet;
			CYUV2RGB::convert_yuv_to_rgb_buffer(m_TempData+FRAMEHEADLEN,m_MapData,m_pHead->nWidth,m_pHead->nHeight);
			//memcpy(m_pTempImage,m_pSingleImage,NewWidth*m_pHead->nHeight);
		}



		if ((m_nRotateType==0)&&(m_nShiningType==0))
			return m_MapData;
		else 
		{
			//轴映射
			if (m_nShiningType==1)
			{

				for (int i = 0; i<m_pHead->nHeight; i++)
				{
					memcpy(m_TempData+NewWidth*newByte*i,m_MapData+NewWidth*newByte*(m_pHead->nHeight-i-1),NewWidth*newByte);
				}

			}
			else if (m_nShiningType==2)
			{

				int Tcount = 0;
				for (int i = 0; i< m_pHead->nHeight; i++)
				{		
					for (int j = NewWidth-1; j >=0 ; j --)
					{
						for(int byte=0;byte<newByte;byte++)
						{
							m_TempData[i*newByte*NewWidth+newByte*(NewWidth-1-j)+byte] = m_MapData[i*NewWidth*newByte+newByte*j+byte];
							Tcount++;
						}
					}
				}
			}
			else if (m_nShiningType==3)
			{
				for (int i = 0; i<m_pHead->nHeight; i++)
				{
					memcpy(m_TempData+NewWidth*newByte*i,m_MapData+NewWidth*newByte*(m_pHead->nHeight-i-1),NewWidth*newByte);
				}

				memcpy(m_MapData,m_TempData,*nLen);

				int Tcount = 0;
				for (int i = 0; i< m_pHead->nHeight; i++)
				{		
					for (int j = NewWidth-1; j >=0 ; j --)
					{
						for(int byte=0;byte<newByte;byte++)
						{
							m_TempData[i*newByte*NewWidth+newByte*(NewWidth-1-j)+byte] = m_MapData[i*NewWidth*newByte+newByte*j+byte];
							Tcount++;
						}
					}
				}
			}
			else 
			{
				memcpy(m_TempData,m_MapData,NewWidth*newByte*m_pHead->nHeight);
			}

			if (m_nRotateType==1)//旋转
			{
				int Tcount = 0;
				
				int RWidth=(m_pHead->nHeight/4)*4;
			
				for (int j = 0; j < NewWidth ; j ++)
				{

				// for (int i = 0;i<m_pHead->nHeight; i++)
					for (int i = RWidth; i>=0; i--)
					{
						for(int byte=0;byte<newByte;byte++){

							m_MapData[j*RWidth*newByte+newByte*i+byte] = m_TempData[i*NewWidth*newByte+newByte*j+byte];

							//Tcount++;
						}
					}
				}
			}
			else if (m_nRotateType==2)
			{
				int Tcount = 0;
				for (int i = m_pHead->nHeight-1; i>=0; i--)
				{

					for (int j = NewWidth-1; j >=0  ; j --)
					{
						for(int byte=0;byte<newByte;byte++){ 

							m_MapData[Tcount] = m_TempData[i*NewWidth*newByte+newByte*j+byte];
							Tcount++;
						}
					}
				}
			}
			else if (m_nRotateType==3)
			{
				int Tcount = 0;
				
				int RWidth=(m_pHead->nHeight/4)*4;
				for (int j = NewWidth-1; j >=0 ; j --)
				{		
					for (int i = 0; i< RWidth; i++)
					{
						for(int byte=0;byte<newByte;byte++){ 

							m_MapData[(NewWidth-1-j)*RWidth*newByte+newByte*i+byte] = m_TempData[i*NewWidth*newByte+newByte*j+byte];

							//Tcount++;
						}
					}
				}
			}
			else 
			{
				memcpy(m_MapData,m_TempData,NewWidth*newByte*m_pHead->nHeight);	
			}

			return m_MapData;
		}

	}
	else return m_TempData;
}

HRESULT CBaseCasheMap::SaveToData(int nBeginPos, int nEndPos, int nFrame, int  nType, BYTE *pSavePath, int nFps)
{
	if((!m_IsOpen)||(m_IsWrite))
		return S_FALSE;

	if(!m_pHead)
		return S_FALSE;

	if ((nEndPos>=m_pHead->nSaveCount)||(nBeginPos<0)||(nBeginPos>nEndPos))
		return S_FALSE;

	if (!pSavePath)
		return S_FALSE;

	m_SaveToBigen=nBeginPos;
	m_SaveToEnd=nEndPos;
	m_SaveToFrame=nFrame;
	m_SaveToType=nType;
	m_SaveToPath = (char *)pSavePath;

	m_SaveFps=nFps;

	if(!FSO_Tools::isFsoExists(m_SaveToPath))
		FSO_Tools::CreateDirectory2(m_SaveToPath);

	if(!FSO_Tools::isFsoExists(m_SaveToPath))
		return S_FALSE;

	m_hSaveToThr = (HANDLE)_beginthreadex(0,0,SaveToThrd,this,0,0); 

	return S_OK;
}

int CBaseCasheMap::WriteDATHead()
{
	if(!m_pHead)
		return -2;
	if(m_hFactFile == NULL) 
		return -1;

	DWORD nMove = FILE_BEGIN;
	ULONGLONG nPos=0;
	DWORD nRet = 0;
	LONG nLow = 0, nHigh = 0;
	LONG *pHigh = &nHigh;
	nLow = (LONG)(nPos & 0xffffffff);
	nHigh = (LONG)((nPos >> 32) & 0xffffffff);

	nRet = SetFilePointer(m_hFactFile, nLow, pHigh, nMove);

	m_nCulPos=0;

	string tempstring = "";

	char * TempChar = new char[DATHDLEN];

	BYTE * HeadBuf = new BYTE[DATHDLEN];

	memset (HeadBuf,0,DATHDLEN);

	SYSTEMTIME ss;
	m_pHead->nSaveCount = m_ImageCount;

	sprintf(TempChar,"%d||RocketechDatFile||-HeadLen:%d-FileNum:%d-MapFile:",m_pHead->m_HeadFormat,m_pHead->nHeadLen,m_pHead->nFileNum);
	tempstring += TempChar;
	for (int i =0;i<m_pHead->nFileNum;i++)
	{
		//if(m_pHead->pMapFile[i]!="")
		//{
		string s = "";
		s = (char*)m_pHead->pMapFile[i];
		tempstring += s;
		tempstring +=":";
		//}
		//else {break;}

	}
	//
	tempstring += "-";

	sprintf(TempChar,"DeviceName:%s-PortName:%s-",m_pHead->pDeviceName,m_pHead->pPortName);
	tempstring += TempChar;


	sprintf(TempChar,"Width:%d-Height:%d-Pixels:%d-FrameRate:%d-ImageCount:%d-StartPos:%d-",m_pHead->nWidth,m_pHead->nHeight,m_pHead->nPixel,
		m_pHead->nFrame,m_pHead->nSaveCount,m_pHead->nStratPos);
	tempstring += TempChar;

	sprintf(TempChar,"nSize:%ld-StartTime:%d_%d_%d_%d_%d_%d-EndTime:%d_%d_%d_%d_%d_%d-",
		m_pHead->m_nSize,m_pHead->pStart.wYear,m_pHead->pStart.wMonth,m_pHead->pStart.wDay,m_pHead->pStart.wHour,m_pHead->pStart.wMinute,m_pHead->pStart.wSecond,
		m_pHead->pEnd.wYear,m_pHead->pEnd.wMonth,m_pHead->pEnd.wDay,m_pHead->pEnd.wHour,m_pHead->pEnd.wMinute,m_pHead->pEnd.wSecond);
	tempstring += TempChar;

	sprintf(TempChar,"Format:%d-BayerFilter:%d-RedGama:%d-BlueGama%d-GreenGama:%d-RedGain:%lf-BlueGain:%lf-GreenGain:%lf:-",
		m_pHead->nFormat,m_pHead->nBayerFilter,m_pHead->nBayRedGramma,m_pHead->nBayBlueGramma,m_pHead->nBayGreenGramma,
		m_pHead->nBayRedGain,m_pHead->nBayBlueGain,m_pHead->nBayGreenGain);
	tempstring += TempChar;



	int strlength = tempstring.length();

	//TempChar = tempstring.c_str();

	memcpy( HeadBuf, &(m_pHead->lpFrequency), 8);

	memcpy( HeadBuf+8, tempstring.c_str(), strlength);

	strlength+=8;

	if( !WriteFile(m_hFactFile, HeadBuf, DATHDLEN, &nRet, NULL))
	{
		DWORD nErr = GetLastError(); 
		return 0;
	}

	m_nCulPos=DATHDLEN;
	//WriteData( HeadBuf, DATHDLEN);

	if (TempChar)
	{
		delete[] TempChar;
		TempChar = NULL;
	}
	if (HeadBuf)
	{
		delete[] HeadBuf;
		HeadBuf = NULL;
	}

	return 1;					
}

unsigned __stdcall CBaseCasheMap::SaveToThrd(void * pParam)
{
	CBaseCasheMap * p = (CBaseCasheMap *) pParam;

	if(p->m_IsWrite)
	{
		if(p->m_MsgFun)
			p->m_MsgFun(p->m_pMsgHwn,0,0,4);

		BOOL X = CopyFile(p->m_MapFilename.c_str(),p->m_NewFilename.c_str(),0);
		BOOL Y = DeleteFile(p->m_MapFilename.c_str());
		if (X)
		{
			if(p->m_MsgFun)
				p->m_MsgFun(p->m_pMsgHwn,0,0,5);
		}
		else
		{
			if(p->m_MsgFun)
				p->m_MsgFun(p->m_pMsgHwn,0,0,6);
		}
	}
	else 
	{
		SYSTEMTIME Time;
		GetLocalTime(&Time);

		string savestr="";

		char s[256]; 
		memset(s,0,256);
		sprintf(s,"%s\\%s_%s_%d_%d_%d_%d_%d_%d",p->m_SaveToPath.c_str(),p->m_pHead->pDeviceName,p->m_pHead->pPortName,Time.wMonth,Time.wDay,Time.wHour,Time.wMinute,Time.wSecond,Time.wMilliseconds);

		savestr=s;

		int nBitDep=8;
		if(p->m_pHead->nFormat>0)
			nBitDep=24;

		long savesize=p->m_pHead->m_nSize;

		if((savesize%512)!=0)
			savesize=(savesize/512+1)*512;

		if (p->m_SaveToType == 1)
		{
			savestr+=".avi";

			//		CAVIFile file(str,p->nWidth,p->nHeight);

			PAVIFILE    m_pAVIFile=NULL;     ///< AVI文件的句柄
			PAVISTREAM	m_ps=NULL;           ///< 通过AVI文件创建的视频流
			PAVISTREAM	m_psCompressed=NULL; ///< 通过视频流和编码器创建的压缩视频流
			int         m_nTimeFrame=0;   ///< 写入avi文件需要的帧ID号



			bool B_AVI = Avi_Tools::InitAviCreate(&m_pAVIFile,&m_ps,&m_psCompressed,savestr,p->m_pHead->nWidth,p->m_pHead->nHeight,p-> m_SaveFps,nBitDep);

			//if (p->nFrameRate==0)
			//	file.m_FramRat = m_pHead->nFrame;
			//else file.m_FramRat = m_FrameRate;

			for(int i=p->m_SaveToBigen;i<=p->m_SaveToEnd;i++)
			{

				if(!p->m_SaveEnable){
					break;
				}
				ULONGLONG nsize=savesize+FRAMEHEADLEN;
				ULONGLONG imagenum=i;
				ULONGLONG len=DATHDLEN;
				ULONGLONG hearlen=FRAMEHEADLEN;

				ULONGLONG npos=nsize*imagenum+len;

				p->SetMapFilePtrPos(npos);
				//p->SetMapFilePtrPos(DATHDLEN+p->nImageSize*i, START_FILE);
				long nRet = p->ReadData(p->m_ShowData,savesize+FRAMEHEADLEN);

				if(nRet <= 0)
				{
					return -1;
				}

				if (p->m_pHead->nFormat==3)
				{
					int nWidth1=0;
					//pfDoubleRate_GetModulatedWidthColor(p->m_pHead->nWidth,&nWidth1);

					//int demod = pfDoubleRate_DeModulateImageColor(p->m_DoubleBuf, p->m_ShowData+FRAMEHEADLEN, p->m_pHead->nWidth , p->m_pHead->nHeight, nWidth1);

					int nFactWidth =p->m_pHead->nWidth * 3;

					p->m_pBayer.SetSize(p->m_pHead->nWidth, nFactWidth, p->m_pHead->nHeight);
					p->m_pBayer.SetBayFilter(p->m_pHead->nBayerFilter);
		            p->m_pBayer.SetGain(p->m_pHead->nBayRedGain, p->m_pHead->nBayBlueGain, p->m_pHead->nBayGreenGain);
		            p->m_pBayer.SetGamma(p->m_pHead->nBayRedGramma, p->m_pHead->nBayBlueGramma, p->m_pHead->nBayGreenGramma);
					p-> m_pBayer.bayer_to_rgb24(p->m_DoubleBuf,p->m_OutData);

					memcpy(p->m_ShowData,p->m_OutData,(p->m_pHead->nWidth)*(p->m_pHead->nHeight)*3);

					Avi_Tools::SaveAvi(&m_psCompressed,p->m_ShowData,p->m_pHead->nWidth,p->m_pHead->nHeight,m_nTimeFrame,nBitDep);
				}
				else if (p->m_pHead->nFormat==2)
				{
					int nWidth1=0;
					//pfDoubleRate_GetModulatedWidth(p->m_pHead->nWidth,&nWidth1);

					//int demod = pfDoubleRate_DeModulateImage(p->m_DoubleBuf, p->m_ShowData+FRAMEHEADLEN, p->m_pHead->nWidth , p->m_pHead->nHeight, nWidth1);

					memcpy(p->m_ShowData,p->m_DoubleBuf,(p->m_pHead->nWidth)*(p->m_pHead->nHeight));

					Avi_Tools::SaveAvi(&m_psCompressed,p->m_ShowData,p->m_pHead->nWidth,p->m_pHead->nHeight,m_nTimeFrame,nBitDep);
				}
				else if(p->m_pHead->nFormat==4){
					CYUV2RGB::convert_yuv_to_rgb_buffer(p->m_ShowData+FRAMEHEADLEN,p->m_OutData,p->m_pHead->nWidth,p->m_pHead->nHeight);
					memcpy(p->m_ShowData,p->m_OutData,(p->m_pHead->nWidth)*(p->m_pHead->nHeight)*3);

					Avi_Tools::SaveAvi(&m_psCompressed,p->m_ShowData,p->m_pHead->nWidth,p->m_pHead->nHeight,m_nTimeFrame,nBitDep);
				}
				else if(p->m_pHead->nFormat==0){
					CYUV2RGB::convert_to_src(p->m_ShowData+FRAMEHEADLEN,p->m_OutData,p->m_pHead->nWidth,p->m_pHead->nHeight,p->m_nPixelByte,0,p->m_nOff,p->m_pHead->nFormat);
					memcpy(p->m_ShowData,p->m_OutData,(p->m_pHead->nWidth)*(p->m_pHead->nHeight));

					Avi_Tools::SaveAvi(&m_psCompressed,p->m_ShowData,p->m_pHead->nWidth,p->m_pHead->nHeight,m_nTimeFrame,nBitDep);
				}
				else if(p->m_pHead->nFormat==1){

					//Bayer格式
					int nFactWidth =p->m_pHead->nWidth * 3;

					p->m_pBayer.SetSize(p->m_pHead->nWidth, nFactWidth, p->m_pHead->nHeight);
					p->m_pBayer.SetBayFilter(p->m_pHead->nBayerFilter);
		            p->m_pBayer.SetGain(p->m_pHead->nBayRedGain, p->m_pHead->nBayBlueGain, p->m_pHead->nBayGreenGain);
		            p->m_pBayer.SetGamma(p->m_pHead->nBayRedGramma, p->m_pHead->nBayBlueGramma, p->m_pHead->nBayGreenGramma);
					p-> m_pBayer.bayer_to_rgb24(p->m_ShowData+FRAMEHEADLEN,p->m_OutData);

					memcpy(p->m_ShowData,p->m_OutData,(p->m_pHead->nWidth)*(p->m_pHead->nHeight)*3);

					Avi_Tools::SaveAvi(&m_psCompressed,p->m_ShowData,p->m_pHead->nWidth,p->m_pHead->nHeight,m_nTimeFrame,nBitDep);
				}

				m_nTimeFrame++;

				//file.AddFrame((char*)p->m_pSingleImage);
				p->m_MsgFun(p->m_pMsgHwn,i-p->m_SaveToBigen,0,1);

			}
			Avi_Tools::StopAvi(&m_pAVIFile, &m_ps, &m_psCompressed);
		}
		else if (p->m_SaveToType == 0)
		{
			if(!FSO_Tools::isFsoExists(savestr))
				FSO_Tools::CreateDirectory2(savestr);

			if(!FSO_Tools::isFsoExists(savestr))
			{
				//::MessageBox(Ptr->m_MsgHWND,"Save Path Failure","Error",MB_OK);
				return -1;
			}

			for(int i=p->m_SaveToBigen;i<=p->m_SaveToEnd;i++)
			{


				if(!p->m_SaveEnable){
					break;
				}
				string savepath;
				sprintf(s,"\\File_%0.8d.raw",i);savepath = savestr + s;

				ULONGLONG nsize=savesize+FRAMEHEADLEN;
				ULONGLONG imagenum=i;
				ULONGLONG len=DATHDLEN;
				ULONGLONG hearlen=FRAMEHEADLEN;

				ULONGLONG npos=nsize*imagenum+len;

				p->SetMapFilePtrPos(npos);

				long nRet = p->ReadData(p->m_ShowData,savesize+FRAMEHEADLEN);

				if(nRet <= 0)
				{
					return -1;
				}

				if (p->m_pHead->nFormat==3)
				{
					int nWidth1=0;
					//pfDoubleRate_GetModulatedWidthColor(p->m_pHead->nWidth,&nWidth1);

					//int demod = pfDoubleRate_DeModulateImageColor(p->m_DoubleBuf, p->m_ShowData+FRAMEHEADLEN, p->m_pHead->nWidth , p->m_pHead->nHeight, nWidth1);

					int nFactWidth =p->m_pHead->nWidth * 3;

					p->m_pBayer.SetSize(p->m_pHead->nWidth, nFactWidth, p->m_pHead->nHeight);
					p->m_pBayer.SetBayFilter(p->m_pHead->nBayerFilter);
		            p->m_pBayer.SetGain(p->m_pHead->nBayRedGain, p->m_pHead->nBayBlueGain, p->m_pHead->nBayGreenGain);
		            p->m_pBayer.SetGamma(p->m_pHead->nBayRedGramma, p->m_pHead->nBayBlueGramma, p->m_pHead->nBayGreenGramma);
					p-> m_pBayer.bayer_to_rgb24(p->m_DoubleBuf,p->m_OutData);

					memcpy(p->m_ShowData,p->m_OutData,(p->m_pHead->nWidth)*(p->m_pHead->nHeight)*3);

					ofstream fout(savepath.c_str(),ios::binary);
					fout.write((char *)p->m_ShowData,(p->m_pHead->nWidth)*(p->m_pHead->nHeight)*3);

					fout.close();
				}
				else if (p->m_pHead->nFormat==2)
				{
					int nWidth1=0;
					//pfDoubleRate_GetModulatedWidth(p->m_pHead->nWidth,&nWidth1);

					//int demod = pfDoubleRate_DeModulateImage(p->m_DoubleBuf, p->m_ShowData+FRAMEHEADLEN, p->m_pHead->nWidth , p->m_pHead->nHeight, nWidth1);

					memcpy(p->m_ShowData,p->m_DoubleBuf,(p->m_pHead->nWidth)*(p->m_pHead->nHeight));

			
					ofstream fout(savepath.c_str(),ios::binary);
					fout.write((char *)p->m_ShowData,(p->m_pHead->nWidth)*(p->m_pHead->nHeight));

					fout.close();
				}			
				else if(p->m_pHead->nFormat==4)
				{

					CYUV2RGB::convert_yuv_to_rgb_buffer(p->m_ShowData+FRAMEHEADLEN,p->m_OutData,p->m_pHead->nWidth,p->m_pHead->nHeight);
					memcpy(p->m_ShowData,p->m_OutData,(p->m_pHead->nWidth)*(p->m_pHead->nHeight)*3);
					ofstream fout(savepath.c_str(),ios::binary);
					fout.write((char *)p->m_ShowData,(p->m_pHead->nWidth)*(p->m_pHead->nHeight)*3);
					fout.close();
				}
				else if(p->m_pHead->nFormat==0||p->m_pHead->nFormat==1)
				{
					CYUV2RGB::convert_to_src(p->m_ShowData+FRAMEHEADLEN,p->m_OutData,p->m_pHead->nWidth,p->m_pHead->nHeight,p->m_nPixelByte,0,p->m_nOff,p->m_pHead->nFormat);
					memcpy(p->m_ShowData,p->m_OutData,(p->m_pHead->nWidth)*(p->m_pHead->nHeight));
					ofstream fout(savepath.c_str(),ios::binary);
					fout.write((char *)p->m_ShowData,(p->m_pHead->nWidth)*(p->m_pHead->nHeight));
					fout.close();
				}

				p->m_MsgFun(p->m_pMsgHwn,i-p->m_SaveToBigen,0,0);
			}

		}
		else if(p->m_SaveToType == 2)
		{
			BITMAPFILEHEADER pfileInfo;
			BITMAPINFOHEADER pheadInfo;
			RGBQUAD m_color[256];

			if(!FSO_Tools::isFsoExists(savestr))
				FSO_Tools::CreateDirectory2(savestr);

			if(!FSO_Tools::isFsoExists(savestr))
			{
				//::MessageBox(Ptr->m_MsgHWND,"Save Path Failure","Error",MB_OK);
				return -1;
			}


			int filelen=sizeof(BITMAPFILEHEADER);

			int headlen=sizeof(BITMAPINFOHEADER);
			int rgblen=0;
			if(p->m_pHead->nFormat==0)
				rgblen=sizeof(RGBQUAD)*256;

			int totallen = filelen+headlen+rgblen;



			//	m_headInfo.biBitCount =p->nBitDep;//
			//int NewWidth = p->CountWidth(p->m_pHead->nWidth);
			pheadInfo.biCompression = BI_RGB;

			if(p->m_pHead->nFormat==3)
			{
				pheadInfo.biSizeImage=(p->m_pHead->nWidth)*(p->m_pHead->nHeight)*3;
				pheadInfo.biBitCount =24;
				pheadInfo.biWidth = p->m_pHead->nWidth;

			}
			else if(p->m_pHead->nFormat==2)
			{
				pheadInfo.biSizeImage=(p->m_pHead->nWidth)*(p->m_pHead->nHeight);
				pheadInfo.biBitCount =8;
				pheadInfo.biWidth = p->m_pHead->nWidth;

			}
			else if(p->m_pHead->nFormat==4)
			{
				pheadInfo.biSizeImage=(p->m_pHead->nWidth)*(p->m_pHead->nHeight)*3;
				pheadInfo.biBitCount =24;
				pheadInfo.biWidth = p->m_pHead->nWidth;

			}
			else if(p->m_pHead->nFormat==1)
			{
				pheadInfo.biSizeImage=(p->m_pHead->nWidth)*(p->m_pHead->nHeight)*3;
				pheadInfo.biBitCount =24;
				pheadInfo.biWidth = p->m_pHead->nWidth;
			}
			else
			{
				pheadInfo.biSizeImage=(p->m_pHead->nWidth)*(p->m_pHead->nHeight);
				pheadInfo.biBitCount =8;
				pheadInfo.biWidth = p->m_pHead->nWidth;
			}

			pfileInfo.bfType		= ((WORD) ('M' << 8) | 'B');	// always is "BM"
			pfileInfo.bfReserved1 = 0;
			pfileInfo.bfReserved2 = 0;
			pfileInfo.bfOffBits	= (DWORD) (54+rgblen);
			pfileInfo.bfSize      =pheadInfo.biSizeImage +filelen+headlen+ rgblen;

			pheadInfo.biHeight = p->m_pHead->nHeight;
			pheadInfo.biXPelsPerMeter = 0;
			pheadInfo.biYPelsPerMeter = 0 ;
			pheadInfo.biClrImportant = 0;
			pheadInfo.biClrUsed = 0;
			pheadInfo.biPlanes = 1;
			pheadInfo.biSize = 40;

			for(int j=0;j<256;j++)
			{
				m_color[j].rgbBlue=(BYTE)j;
				m_color[j].rgbGreen=(BYTE)j;
				m_color[j].rgbRed=(BYTE)j;
				m_color[j].rgbReserved=(BYTE)0;
			}

			//FILEHEADLEN
			CopyMemory( p->m_OutData, &pfileInfo ,  14);
			CopyMemory( p->m_OutData+14,&pheadInfo , 40);

			if(p->m_pHead->nFormat==0)
				CopyMemory( p->m_OutData+54, m_color ,  sizeof(RGBQUAD)*256);


			for(int i=p->m_SaveToBigen;i<=p->m_SaveToEnd;i++)
			{

				if(!p->m_SaveEnable){
					break;
				}
				string savepath;
				sprintf(s,"\\File_%0.8d.bmp",i);
				savepath = savestr + s;

				ULONGLONG nsize=savesize+FRAMEHEADLEN;
				ULONGLONG imagenum=i;
				ULONGLONG len=DATHDLEN;
				ULONGLONG hearlen=FRAMEHEADLEN;

				ULONGLONG npos=nsize*imagenum+len;

				p->SetMapFilePtrPos(npos);

				long nRet = p->ReadData(p->m_ShowData,savesize+FRAMEHEADLEN);

				if(nRet <= 0)
				{
					return -1;
				}

				if(p->m_pHead->nFormat==0)
				{
					CYUV2RGB::convert_to_src(p->m_ShowData+FRAMEHEADLEN,p->m_OutData+totallen,p->m_pHead->nWidth,p->m_pHead->nHeight,p->m_nPixelByte,0,p->m_nOff,p->m_pHead->nFormat);
					//memcpy(p->m_ShowData,p->m_OutData+totallen,p->m_pHead->nWidth*p->m_pHead->nHeight);
					ofstream fout(savepath.c_str(),ios::binary);
					fout.write((char *)p->m_OutData,totallen+(p->m_pHead->nWidth*p->m_pHead->nHeight));
					fout.close();
				}
				else if(p->m_pHead->nFormat==1){

					//Bayer格式
					int nFactWidth =p->m_pHead->nWidth * 3;

					p->m_pBayer.SetSize(p->m_pHead->nWidth, nFactWidth, p->m_pHead->nHeight);
					p->m_pBayer.SetBayFilter(p->m_pHead->nBayerFilter);
		            p->m_pBayer.SetGain(p->m_pHead->nBayRedGain, p->m_pHead->nBayBlueGain, p->m_pHead->nBayGreenGain);
		            p->m_pBayer.SetGamma(p->m_pHead->nBayRedGramma, p->m_pHead->nBayBlueGramma, p->m_pHead->nBayGreenGramma);

					p-> m_pBayer.bayer_to_rgb24(p->m_ShowData+FRAMEHEADLEN,p->m_OutData+totallen);

					//memcpy(p->m_OutData+totallen,p->m_ShowData,(p->m_pHead->nWidth)*(p->m_pHead->nHeight)*3);

					ofstream fout(savepath.c_str(),ios::binary);
					fout.write((char *)p->m_OutData,totallen+(p->m_pHead->nWidth*p->m_pHead->nHeight*3));
					fout.close();
				}
				else if (p->m_pHead->nFormat==2)
				{
					int nWidth1=0;
					//pfDoubleRate_GetModulatedWidth(p->m_pHead->nWidth,&nWidth1);

					//int demod = pfDoubleRate_DeModulateImage(p->m_OutData+totallen,p->m_ShowData+FRAMEHEADLEN, p->m_pHead->nWidth , p->m_pHead->nHeight, nWidth1);

					ofstream fout(savepath.c_str(),ios::binary);
					fout.write((char *)p->m_OutData,totallen+(p->m_pHead->nWidth*p->m_pHead->nHeight));
					fout.close();
				}
				else if (p->m_pHead->nFormat==3)
				{
					int nWidth1=0;
					//pfDoubleRate_GetModulatedWidthColor(p->m_pHead->nWidth,&nWidth1);

					//int demod = pfDoubleRate_DeModulateImageColor(p->m_DoubleBuf, p->m_ShowData+FRAMEHEADLEN, p->m_pHead->nWidth , p->m_pHead->nHeight, nWidth1);

					int nFactWidth =p->m_pHead->nWidth * 3;

					p->m_pBayer.SetSize(p->m_pHead->nWidth, nFactWidth, p->m_pHead->nHeight);
					p->m_pBayer.SetBayFilter(p->m_pHead->nBayerFilter);
		            p->m_pBayer.SetGain(p->m_pHead->nBayRedGain, p->m_pHead->nBayBlueGain, p->m_pHead->nBayGreenGain);
		            p->m_pBayer.SetGamma(p->m_pHead->nBayRedGramma, p->m_pHead->nBayBlueGramma, p->m_pHead->nBayGreenGramma);
					p-> m_pBayer.bayer_to_rgb24(p->m_DoubleBuf,p->m_OutData+totallen);

					ofstream fout(savepath.c_str(),ios::binary);
					fout.write((char *)p->m_OutData,totallen+((p->m_pHead->nWidth)*(p->m_pHead->nHeight)*3));
					fout.close();
				}
				else if(p->m_pHead->nFormat==4){

					CYUV2RGB::convert_yuv_to_rgb_buffer(p->m_ShowData+FRAMEHEADLEN,p->m_OutData+totallen,p->m_pHead->nWidth,p->m_pHead->nHeight);
					//memcpy(Ptr->m_pSrcData+totallen,Ptr->m_pData,p->nWidth*p->nHeight*3);
					ofstream fout(savepath.c_str(),ios::binary);
					fout.write((char *)p->m_OutData,totallen+(p->m_pHead->nWidth)*(p->m_pHead->nHeight)*3);
					fout.close();
				}
				p->m_MsgFun(p->m_pMsgHwn,i-p->m_SaveToBigen,0,2);
			}
		}

		else if(p->m_SaveToType==3){

			savestr += ".dat";

			DWORD nAccess = 0, nShareMode = 0;
			DWORD nCreate = 0, nFlag = FILE_FLAG_RANDOM_ACCESS;
			DWORD nProtect = 0,nMapAccess = 0;
			ULONGLONG nTmp = 0;
			//		m_MapMode=nmode;

			nAccess = GENERIC_READ|GENERIC_WRITE;
			nShareMode = FILE_SHARE_WRITE | FILE_SHARE_READ;
			nCreate = CREATE_ALWAYS;
			nProtect = PAGE_READWRITE;
			nMapAccess = FILE_MAP_WRITE;

			char * filepathstr = new char[256];
			filepathstr = (char *)savestr.c_str();


			//头文件保存
			MAPFILEHEAD* m_HeadPart=new MAPFILEHEAD();

			m_HeadPart->m_HeadFormat=p->m_pHead->m_HeadFormat;
			memcpy(m_HeadPart->pDeviceName,p->m_pHead->pDeviceName,256);
			memcpy(m_HeadPart->pPortName,p->m_pHead->pPortName,256);
			m_HeadPart->nHeadLen=p->m_pHead->nHeadLen ;
			m_HeadPart->nFileNum = p->m_pHead->nFileNum;
			m_HeadPart->nWidth = p->m_pHead->nWidth ;
			m_HeadPart->nHeight = p->m_pHead->nHeight;
			m_HeadPart->nPixel =p-> m_pHead->nPixel;
			m_HeadPart->nFrame =p->m_pHead->nFrame;
			m_HeadPart->nSaveCount =p->m_SaveToEnd-p->m_SaveToBigen+1;
			m_HeadPart->nStratPos = p->m_pHead->nStratPos;
			m_HeadPart->m_nSize = p->m_pHead->m_nSize;
			m_HeadPart->nBayerFilter =p->m_pHead->nBayerFilter;
			m_HeadPart->nBayRedGain = p->m_pHead->nBayRedGain;
			m_HeadPart->nBayGreenGain = p->m_pHead->nBayGreenGain;
			m_HeadPart->nBayBlueGain = p->m_pHead->nBayBlueGain;
			m_HeadPart->nBayRedGramma =p->m_pHead->nBayRedGramma;
			m_HeadPart->nBayBlueGramma =p-> m_pHead->nBayBlueGramma;
			m_HeadPart->nBayGreenGramma =p-> m_pHead->nBayGreenGramma;
			m_HeadPart->nFormat =p-> m_pHead->nFormat;
		
			m_HeadPart->lpFrequency=p->m_pHead->lpFrequency;
			//memcpy(m_HeadPart)
			// memcpy(m_HeadPart,p->m_pHead,DATALEN);

			ULONGLONG hourofms=3600000;
			ULONGLONG dayofhour=24;
			ULONGLONG minofms=60000;
			ULONGLONG secofms=1000;
			ULONGLONG starttime = (ULONGLONG)(p->m_pHead->pStart.wDay*hourofms*dayofhour+p->m_pHead->pStart.wHour*hourofms+p->m_pHead->pStart.wMinute* minofms+p->m_pHead->pStart.wSecond*secofms+p->m_pHead->pStart.wMilliseconds);
			ULONGLONG endtime = (ULONGLONG)(p->m_pHead->pEnd.wDay*hourofms*dayofhour+p->m_pHead->pEnd.wHour*hourofms+p->m_pHead->pEnd.wMinute*minofms+p->m_pHead->pEnd.wSecond*secofms+p->m_pHead->pEnd.wMilliseconds); 
			ULONGLONG framegap = (endtime - starttime)/(ULONGLONG)p->m_pHead->nSaveCount; 
			ULONGLONG newstart = p->m_SaveToBigen*framegap+starttime;
			ULONGLONG newend = p->m_SaveToEnd*framegap+starttime;


			m_HeadPart->pStart.wYear =m_HeadPart->pEnd.wYear =p->m_pHead->pStart.wYear;
			m_HeadPart->pStart.wMonth  = m_HeadPart->pStart.wMonth  = p->m_pHead->pStart.wMonth;


			m_HeadPart->pStart.wDay =(int) (newstart/(hourofms*dayofhour));
			newstart-=(dayofhour*hourofms)*(ULONGLONG)m_HeadPart->pStart.wDay;
			m_HeadPart->pStart.wHour =(int) (newstart/(hourofms));
			newstart-=(hourofms)*(ULONGLONG)m_HeadPart->pStart.wHour;
			m_HeadPart->pStart.wMinute = int(newstart/(minofms));
			newstart-=(minofms)*(ULONGLONG)m_HeadPart->pStart.wMinute;
			m_HeadPart->pStart.wSecond = int(newstart/(secofms));
			newstart-=(secofms)*(ULONGLONG)m_HeadPart->pStart.wSecond;
			m_HeadPart->pStart.wMilliseconds = int(newstart);

			m_HeadPart->pEnd.wDay = (int)(newend/(hourofms*dayofhour));
			newend-=(hourofms*dayofhour)*(ULONGLONG)m_HeadPart->pEnd.wDay ;
			m_HeadPart->pEnd.wHour = (int)(newend/(hourofms));
			newend-=(hourofms)*(ULONGLONG)m_HeadPart->pEnd.wHour;
			m_HeadPart->pEnd.wMinute =(int)(newend/(minofms));
			newend-=(minofms)*(ULONGLONG)m_HeadPart->pEnd.wMinute;
			m_HeadPart->pEnd.wSecond =(int) (newend/(secofms));
			newend-=(secofms)*(ULONGLONG)m_HeadPart->pEnd.wSecond;
			m_HeadPart->pEnd.wMilliseconds =(int) newend;

			HANDLE New_hFactFile = CreateFile( filepathstr, nAccess, nShareMode, NULL, nCreate, nFlag, NULL);

			if(New_hFactFile == INVALID_HANDLE_VALUE) 
			{
				return -1;
			}
			SetFilePointer(New_hFactFile, 0, 0, FILE_BEGIN);

			string tempstring = "";
			char * TempChar = new char[DATHDLEN];
			BYTE * HeadBuf = new BYTE[DATHDLEN];
			memset (HeadBuf,0,DATHDLEN);	

			sprintf(TempChar,"%d||RocketechDatFile||-HeadLen:%d-FileNum:1-MapFile:",m_HeadPart->m_HeadFormat,m_HeadPart->nHeadLen);
			tempstring += TempChar;

			tempstring += savestr;

			
			/*for (int i =0;i<1;i++)
			{
				string s = "";
				s = (char*)m_HeadPart->pMapFile[i];
				tempstring += s;
				tempstring +=":";
			}*/
			//
			tempstring += "-";

			sprintf(TempChar,"DeviceName:%s-PortName:%s-",m_HeadPart->pDeviceName,m_HeadPart->pPortName);
			tempstring += TempChar;


			sprintf(TempChar,"Width:%d-Height:%d-Pixels:%d-FrameRate:%d-ImageCount:%d-StartPos:%d-",m_HeadPart->nWidth,m_HeadPart->nHeight,m_HeadPart->nPixel,
				m_HeadPart->nFrame,m_HeadPart->nSaveCount,m_HeadPart->nStratPos);
			tempstring += TempChar;

			sprintf(TempChar,"nSize:%ld-StartTime:%d_%d_%d_%d_%d_%d-EndTime:%d_%d_%d_%d_%d_%d-",
				m_HeadPart->m_nSize,m_HeadPart->pStart.wYear,m_HeadPart->pStart.wMonth,m_HeadPart->pStart.wDay,m_HeadPart->pStart.wHour,m_HeadPart->pStart.wMinute,m_HeadPart->pStart.wSecond,
				m_HeadPart->pEnd.wYear,m_HeadPart->pEnd.wMonth,m_HeadPart->pEnd.wDay,m_HeadPart->pEnd.wHour,m_HeadPart->pEnd.wMinute,m_HeadPart->pEnd.wSecond);
			tempstring += TempChar;

			sprintf(TempChar,"Format:%d-BayerFilter:%d-RedGama:%d-BlueGama%d-GreenGama:%d-RedGain:%lf-BlueGain:%lf-GreenGain:%lf::",
				m_HeadPart->nFormat,m_HeadPart->nBayerFilter,m_HeadPart->nBayRedGramma,m_HeadPart->nBayBlueGramma,m_HeadPart->nBayGreenGramma,
				m_HeadPart->nBayRedGain,m_HeadPart->nBayBlueGain,m_HeadPart->nBayGreenGain);
			tempstring += TempChar;

			int strlength = tempstring.length();

			//TempChar = tempstring.c_str();

			//memcpy( HeadBuf, tempstring.c_str(), strlength);
			memcpy( HeadBuf, &(m_HeadPart->lpFrequency), 8);
			memcpy( HeadBuf+8, tempstring.c_str(), strlength);
		
			strlength+=8;

			DWORD nRet = 0;
			if( !WriteFile(New_hFactFile, HeadBuf, DATHDLEN, &nRet, NULL))
			{
				DWORD nErr = GetLastError(); 
				return 0;
			}



			for(int i=p->m_SaveToBigen;i<=p->m_SaveToEnd;i++)
			{

				if(!p->m_SaveEnable){
					break;
				}
				ULONGLONG nsize=savesize+FRAMEHEADLEN;
				ULONGLONG imagenum=i;
				ULONGLONG len=DATHDLEN;
				ULONGLONG hearlen=FRAMEHEADLEN;

				ULONGLONG npos=nsize*imagenum+len;

				p->SetMapFilePtrPos(npos);

				long nRet1 = p->ReadData(p->m_ShowData,savesize+FRAMEHEADLEN);

				if(nRet1<=0)
				{
					return -1;
				}
				if( !WriteFile(New_hFactFile, p->m_ShowData,savesize+FRAMEHEADLEN, &nRet, NULL))

				{ 
					return -2;
				}

				p->m_MsgFun(p->m_pMsgHwn,i-p->m_SaveToBigen,0,3);

			}


			DWORD nMove = FILE_BEGIN;
			ULONGLONG nPos=0;

			LONG nLow = 0, nHigh = 0;
			LONG *pHigh = &nHigh;
			nLow = (LONG)(nPos & 0xffffffff);
			nHigh = (LONG)((nPos >> 32) & 0xffffffff);


			nRet = SetFilePointer(New_hFactFile , nLow, pHigh, nMove);

			if(New_hFactFile != INVALID_HANDLE_VALUE) 
			{
				CloseHandle(New_hFactFile);
				New_hFactFile = INVALID_HANDLE_VALUE;
			}


			if(m_HeadPart){
				delete m_HeadPart;
				m_HeadPart=NULL;			 
			}

			if(HeadBuf){
				delete HeadBuf;
				HeadBuf=NULL;		
			}


			if(TempChar){
				delete TempChar;
				TempChar=NULL;

			}


		}

	}

	return 0;
}

unsigned CBaseCasheMap::ThreadSaveFuc(LPVOID lpParam)
{
	static long noldtime=0;
	CBaseCasheMap* p=(CBaseCasheMap*)lpParam;
	p->m_IsOpenThread=TRUE;
	int nszie=0;
	int cashnum=0;
	PBYTE pdata=NULL;
	long imageszie=p->m_Imagesize;
	long savesize=(p->m_SaveImageSzie+FRAMEHEADLEN)*(p->m_Buffnum);
	long savesize2=p->m_SaveImageSzie;
	while(p->m_IsOpenThread)
	{

		int bigenpos=p->m_CasheImage-1;

		if(p->m_ImageStrats[p->m_CasheImage-1])
		{
			for(int i=p->m_CasheImage-1;i>=0;i--)
			{
				if(!p->m_ImageStrats[i])
				{
					bigenpos=i+1;
					break;
				}
			}

			for(int k=bigenpos;k<p->m_CasheImage;k++)
			{
				if(p->m_ImageStrats[k])
				{
					//存储
					int ste=0;

					if(p->m_IsMinBuf)
						ste=p->WriteData2(p->m_ImageBuf[k],savesize,NULL,0);
					else ste=p->WriteData2(p->m_ImageBuf[k],savesize2,p->m_HeadBuf[k],FRAMEHEADLEN);

					if(ste<=1)
					{
						if(p->m_MsgFun)
							p->m_MsgFun(p->m_pMsgHwn,p->m_ImageCount,0,ste);
					}

					nszie++;
				}

				p->m_ImageStrats[k]=FALSE;

			}

		}

		for(int i=0;i<bigenpos;i++)
		{
			if(p->m_ImageStrats[i])
			{
				//存储
				int ste=0;

				if(p->m_IsMinBuf)
					ste=p->WriteData2(p->m_ImageBuf[i],savesize,NULL,0);
				else ste=p->WriteData2(p->m_ImageBuf[i],savesize2,p->m_HeadBuf[i],FRAMEHEADLEN);

				if(ste<=1)
				{
					if(p->m_MsgFun)
						p->m_MsgFun(p->m_pMsgHwn,p->m_ImageCount,0,ste);
				}
				//存储
				p->m_ImageStrats[i]=FALSE;

				nszie++;
				//if(nszie>100)

			}
		}

		long newtime=::GetTickCount();

		if(nszie>0)
			cashnum=nszie;

		if((newtime-noldtime)>=1000)
		{
			if(p->m_MsgFun){
				//	DEBUG_INFO1("count:%d",p->m_ImageCount);
				p->m_MsgFun(p->m_pMsgHwn,p->m_ImageCount,cashnum,2);
			}
			
			p->m_nFreeDiskSize =p->GetFreeDisk((BYTE*)(p->m_SavePath.c_str()));
			noldtime=newtime;
		}

		int estroe=p->m_StoreStage;

		if((estroe==2)&&(nszie==0))
		{
			p->CloseMapFile2();
			p->m_StoreStage=0;

			if(p->m_MsgFun)
				p->m_MsgFun(p->m_pMsgHwn,p->m_ImageCount,0,3);

			break;
		}

		//Sleep(1);
		nszie=0;

	}

	p->m_IsOpenThread=FALSE;

	return 0;
}

BOOL CBaseCasheMap::SetMapFilePtrPos(ULONGLONG nPos)
{
	LONG nLow = 0, nHigh = 0;
	LONG *pHigh = &nHigh;
	DWORD nMove = FILE_CURRENT;
	if(m_hFactFile == NULL) return FALSE;

	nMove = FILE_BEGIN;

	DWORD nRet = 0;
	nLow = (LONG)(nPos & 0xffffffff);
	nHigh = (LONG)((nPos >> 32) & 0xffffffff);

	nRet = SetFilePointer(m_hFactFile, nLow, pHigh, nMove);

	if(nRet == 0xFFFFFFFF)
	{
		if(nHigh == 0) return FALSE;
		if(GetLastError() != NO_ERROR) return FALSE;
	}


	m_nCulPos=nPos;

	return TRUE;
}

int CBaseCasheMap::ReadFileHeadData()
{
	if(!m_fhear)
		return 0;

	SetMapFilePtrPos(0);

	long nlen=ReadData(m_fhear,DATHDLEN);

	if(nlen<=0)
		return 0;

	if(m_fhear[8]!='0')
		return 0;

	string HDstring = (char *)(m_fhear+9);

	list<string> strlist=Str_Tools::Split(HDstring,"-");

	int k=strlist.size();
	if(strlist.size()<23)
		return 0;

	list<string>::iterator i1=strlist.begin();
	string tempste=*i1;
	if(tempste.find("RocketechDatFile",0)<0)
		return 0;

	i1++;

	int pos=0;
	string tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("HeadLen:",0);//-HeadLen:%d-FileNum:%d-MapFile:
	tempstring = Str_Tools::Mid(tempste,pos+sizeof("HeadLen:")-1);
	m_pHead->nHeadLen = atoi(tempstring.c_str());

	pos=0;
	tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("FileNum:",0);
	tempstring = Str_Tools::Mid(tempste,pos+sizeof("FileNum:")-1);
	m_pHead->nFileNum = atoi(tempstring.c_str());

	pos=0;
	tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("MapFile:",0);
	tempstring = Str_Tools::Mid(tempste,pos+sizeof("MapFile:")-1);
	//m_pHead->nFileNum = atoi(tempstring.c_str());
	pos=0;
	tempstring = "";
	string str = "";
	for (int i = 0;i<m_pHead->nFileNum; i++)
	{
		pos = tempstring.find(":",0);
		str = Str_Tools::Mid(tempstring,0,pos);
		m_pHead->pMapFile[i] = (BYTE*)str.c_str();
		tempstring.erase(0,pos+1);		
	}

	pos=0;
	tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("DeviceName:",0);//-HeadLen:%d-FileNum:%d-MapFile:
	tempstring=Str_Tools::Mid(tempste,pos+sizeof("DeviceName:")-1);
	memcpy(m_pHead->pDeviceName,tempstring.c_str(),tempstring.size());

	pos=0;
	tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("PortName:",0);//-HeadLen:%d-FileNum:%d-MapFile:
	tempstring=Str_Tools::Mid(tempste,pos+sizeof("PortName:")-1);
	memcpy(m_pHead->pPortName,tempstring.c_str(),tempstring.size());

	pos=0;
	tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("Width:",0);  
	tempstring = Str_Tools::Mid(tempste,pos+sizeof("Width:")-1);
	m_pHead->nWidth = atoi(tempstring.c_str());

	pos=0;
	tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("Height:",0);
	tempstring = Str_Tools::Mid(tempste,pos+pos+sizeof("Height:")-1);
	m_pHead->nHeight = atoi(tempstring.c_str());

	pos=0;
	tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("Pixels:",0);
	tempstring = Str_Tools::Mid(tempste,pos+sizeof("Pixels:")-1);
	m_pHead->nPixel = atoi(tempstring.c_str());
	m_nPixelByte=m_pHead->nPixel;

	pos=0;
	tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("FrameRate:",0);  
	tempstring = Str_Tools::Mid(tempste,pos+sizeof("FrameRate:")-1);
	m_pHead->nFrame = atoi(tempstring.c_str());

	pos=0;
	tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("ImageCount:",0);
	tempstring = Str_Tools::Mid(tempste,pos+sizeof("ImageCount:")-1);
	m_pHead->nSaveCount = atoi(tempstring.c_str());

	pos=0;
	tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("StartPos:",0);
	tempstring = Str_Tools::Mid(tempste,pos+sizeof("StartPos:")-1);
	m_pHead->nStratPos = atoi(tempstring.c_str());

	pos=0;
	tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("nSize:",0);
	tempstring = Str_Tools::Mid(tempste,pos+sizeof("nSize:")-1);
	m_pHead->m_nSize = atol(tempstring.c_str());
	///////////////////////////////////////////////////

	pos=0;
	tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("StartTime:",0);
	tempstring = Str_Tools::Mid(tempste,pos+sizeof("StartTime:")-1);
	m_pHead->pStart.wYear = atoi(tempstring.c_str());
	pos = tempstring.find("_",0);
	tempstring = tempstring.erase(0,pos+1);
	m_pHead->pStart.wMonth = atoi(tempstring.c_str());
	pos = tempstring.find("_",0);
	tempstring = tempstring.erase(0,pos+1);
	m_pHead->pStart.wDay = atoi(tempstring.c_str());
	pos = tempstring.find("_",0);
	tempstring = tempstring.erase(0,pos+1);
	m_pHead->pStart.wHour = atoi(tempstring.c_str());
	pos = tempstring.find("_",0);
	tempstring = tempstring.erase(0,pos+1);
	m_pHead->pStart.wMinute = atoi(tempstring.c_str());
	pos = tempstring.find("_",0);
	tempstring = tempstring.erase(0,pos+1);
	m_pHead->pStart.wSecond = atoi(tempstring.c_str());

	pos=0;
	tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("EndTime:",0);
	tempstring = Str_Tools::Mid(tempste,pos+sizeof("EndTime:")-1);
	m_pHead->pEnd.wYear = atoi(tempstring.c_str());
	pos = tempstring.find("_",0);
	tempstring = tempstring.erase(0,pos+1);
	m_pHead->pEnd.wMonth = atoi(tempstring.c_str());
	pos = tempstring.find("_",0);
	tempstring = tempstring.erase(0,pos+1);
	m_pHead->pEnd.wDay = atoi(tempstring.c_str());
	pos = tempstring.find("_",0);
	tempstring = tempstring.erase(0,pos+1);
	m_pHead->pEnd.wHour = atoi(tempstring.c_str());
	pos = tempstring.find("_",0);
	tempstring = tempstring.erase(0,pos+1);
	m_pHead->pEnd.wMinute = atoi(tempstring.c_str());
	pos = tempstring.find("_",0);
	tempstring = tempstring.erase(0,pos+1);
	m_pHead->pEnd.wSecond = atoi(tempstring.c_str());

	pos=0;
	tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("Format:",0);
	tempstring = Str_Tools::Mid(tempste,pos+sizeof("Format:")-1);
	m_pHead->nFormat = atoi(tempstring.c_str());

	pos=0;
	tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("BayerFilter:",0);
	tempstring = Str_Tools::Mid(tempste,pos+sizeof("BayerFilter:")-1);
	m_pHead->nBayerFilter = atoi(tempstring.c_str());

	pos=0;
	tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("RedGama:",0);
	tempstring = Str_Tools::Mid(tempste,pos+sizeof("RedGama:")-1);
	m_pHead->nBayRedGramma = atoi(tempstring.c_str());

	pos=0;
	tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("BlueGama:",0);
	tempstring = Str_Tools::Mid(tempste,pos+sizeof("BlueGama:")-1);
	m_pHead->nBayBlueGramma = atoi(tempstring.c_str());

	pos=0;
	tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("GreenGama:",0);
	tempstring = Str_Tools::Mid(tempste,pos+sizeof("GreenGama:")-1);
	m_pHead->nBayGreenGramma = atoi(tempstring.c_str());

	pos=0;
	tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("RedGain:",0);
	tempstring = Str_Tools::Mid(tempste,pos+sizeof("RedGain:")-1);
	m_pHead->nBayRedGain = atof(tempstring.c_str());

	pos=0;
	tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("BlueGain:",0);
	tempstring = Str_Tools::Mid(tempste,pos+sizeof("BlueGain:")-1);
	m_pHead->nBayBlueGain = atof(tempstring.c_str());

	pos=0;
	tempstring = "";
	tempste=*i1;
	i1++;
	pos = tempste.find("GreenGain:",0);
	tempstring = Str_Tools::Mid(tempste,pos+sizeof("GreenGain:")-1);
	Str_Tools::Remove(tempstring,':');
	m_pHead->nBayGreenGain = atof(tempstring.c_str());


	memcpy(&(m_pHead->lpFrequency),m_fhear, 8);


	//////////////////////////////////////////////////

	return HDstring.size();
}

long CBaseCasheMap::ReadData(BYTE *pData, int nData)
{
	DWORD nRet = 0;
	if(m_hFactFile == NULL) return 0;
	if( !ReadFile(m_hFactFile, pData, nData, &nRet, NULL) )
	{
		DWORD nErr = GetLastError();  
		return 0;
	}
	return nRet;
}



int  CBaseCasheMap::CountWidth(int Winput)
{
	int Woutput = 0, Tempval = 0;
	if (Winput==680)
		Tempval = 24;
	else if (Winput>=614)
		Tempval = 22;
	else if (Winput>=548)
		Tempval = 20;
	else if (Winput>=482)
		Tempval = 18;
	else if (Winput>=416)
		Tempval = 16;
	else if (Winput>=350)
		Tempval = 14;
	else Tempval = 12;
	Woutput = (Winput-Tempval)*2;
	return Woutput;
}


HRESULT CBaseCasheMap::SetSaveCtrl(BOOL enable)
{
	// TODO: 在此添加实现代码

	m_SaveEnable=enable;

	return S_OK;
}


 BYTE* CBaseCasheMap::GetSaveFilePath(void)
{
	// TODO: 在此添加实现代码
	return (BYTE*)m_MapFilename.c_str();
}


 BYTE* CBaseCasheMap::GetComData(int nCompos,int nMovepos,int nDataType)
{
	// TODO: 在此添加实现代码
	if((!m_IsComData)||(!m_CamData)||(m_Comsize<=0))
		return NULL;

	memset(m_CurrtComdata,0,512);
	m_MovPos=nMovepos;

	BYTE dd[15];
	memset(dd,0,15);

	if(m_ComBigenPos==-1)
	{
		LONGLONG nimagetime=GetMapTime(0);
		LONGLONG congtime=0;
		BOOL ret=FALSE;
		for(int i=0;i<1000;i++)
		{
			if((i*23+8)>(m_Comsize-23))
				break;

			memcpy(dd,m_CamData+i*23+8,15);
			memcpy(&congtime,m_CamData+i*23,8);

			int nFrame=(dd[12]/8)*25;

			double step=1000.0/nFrame;

			double t1=congtime;
			double t2=nimagetime;

			double ss=(t2-t1)*1000.0/m_pHead->lpFrequency;

			if(fabs(ss)<=step)
			{
				m_ComBigenPos=i;
				ret=TRUE;
				break;
			}
		}

		if(!ret)
			m_ComBigenPos=0;
	}

	int pos=(m_ComBigenPos+nCompos+nMovepos)*23;

	if((pos<0)||(pos>(m_Comsize-23)))
		return NULL;

	memcpy(dd,m_CamData+pos+8,15);

	if(nDataType==0)
	{
		memcpy(m_CurrtComdata,dd,15);

		return m_CurrtComdata;
	}

	static UINT tt=0;
	static UINT old=0;
	PBYTE pata=dd+1;

	UINT nTmp = 0;
	BYTE nVal = 0;
	int nCount = 0, nnTmp = 0;
	int nHour  = 0, nMinute = 0, nSec = 0, nMinSec = 0;

	nHour = (pata[0] & 0xfc) >> 2;
	nHour = ((nHour & 0x30) >> 4) * 10 + (nHour & 0x0f);
	nnTmp = (pata[0] & 0x03) << 5; 
	nMinute = nnTmp | ((pata[1] & 0xf8) >> 3);
	nMinute = ((nMinute & 0x70) >> 4) * 10 + (nMinute & 0x0f);
	nnTmp = (pata[1] & 0x07) << 4; 
	nSec = nnTmp | ((pata[2] & 0xf0) >> 4);
	nSec = ((nSec & 0x70) >> 4) * 10 + (nSec & 0x0f);
	nMinSec = (pata[2] & 0x0f) * 100; 
	nMinSec += ((pata[3] & 0xf0) >> 4) * 10 + (pata[3] & 0x0f);


	nTmp = pata[4]; nTmp <<= 8; 
	nTmp |= pata[5]; nTmp <<= 8; 
	nTmp |= pata[6]; 
	UINT m_Direct = nTmp;
	nTmp = pata[7]; nTmp  <<= 8; 
	nTmp |= pata[8]; nTmp <<= 8; 
	nTmp |= pata[9]; 
	UINT m_Azimuth = nTmp;    //俯仰角

	//tt=(m_Direct-nCompos-nMovepos);
	//if(old!=tt)
	//   DEBUG_INFO2("%d   %d",m_Direct-nCompos-nMovepos,nCompos);

	//old=tt;
	sprintf((char*)m_CurrtComdata, "时间:%2d:%2d:%2d:%3d, 方位码:%d, 俯仰码:%d %d", nHour, nMinute, nSec, nMinSec, m_Direct, m_Azimuth);


	return m_CurrtComdata;
}


LONGLONG CBaseCasheMap::GetMapTime(int nImagePos)
{
	if((!m_IsOpen)||m_IsWrite)
		return 0;

	if ((nImagePos>=m_pHead->nSaveCount)||(nImagePos<0))
		return 0;

	DWORD nRet = 0;
	long savesize=m_pHead->m_nSize;

	if((savesize%512)!=0)
		savesize=(savesize/512+1)*512;

	ULONGLONG hearlen=FRAMEHEADLEN;
	ULONGLONG nsize=(ULONGLONG)savesize+hearlen;
	ULONGLONG imagenum=nImagePos;
	ULONGLONG len=DATHDLEN;

	ULONGLONG npos=nsize*imagenum+len;

	SetMapFilePtrPos(npos);

	BYTE hear[FRAMEHEADLEN];
	memset(hear,0,FRAMEHEADLEN);
	nRet = ReadData(hear,FRAMEHEADLEN);//512

	ULONGLONG time=0;
	memcpy( &time, hear, 8);
	//	memcpy( &time, hear, 4);

	return time;
}


/*long GetShowData(BYTE *PImageData, PBYTE* PShowData)
{
	if(!PImageData) return 0;
	int nBuf = 0, nSize = 0, nFactWidth = 0;
	PBYTE pBuf = NULL;

	int NewWidth=m_pHead->nWidth;

	if (m_pHead->nFormat==3)
		NewWidth = CountWidth(m_pHead->nWidth);

	if(m_pHead->m_nPiexl<=24)
	{
		nFactWidth = WIDTHBYTES(NewWidth * 24);
		nSize = nFactWidth  * m_nHeight;
	}

	if(nSize!=m_ShowLen)
	{
		if(*PShowData)
		{
			delete *PShowData;
			*PShowData=NULL;
		}

		*PShowData=new BYTE[nSize];

		m_ShowLen=nSize;
	}

	pBuf=*PShowData;

	if(m_Isbayer==1)
	{
		m_pBayer.SetSize(m_nWidth, nFactWidth, m_nHeight);
		m_pBayer.bayer_to_rgb24(PImageData, pBuf);
		return nSize;
	}
	else if(m_IsColor==1)
	{
		for(int i = m_nHeight - 1; i >= 0; i--)
		{
			int j = 0;
			int gray=0;
			switch(m_nPiexl)
			{
			case 8:
				for(j=0;j<m_nWidth;j++)
				{
					gray=PImageData[i * m_nWidth+j];
					
					pBuf[nBuf++]=m_ColorTranTable[BI][gray]; 
					pBuf[nBuf++]=m_ColorTranTable[GI][gray]; 
					pBuf[nBuf++]=m_ColorTranTable[RI][gray]; 
				//	nBuf +=3; //m_nWidth;
				}
				break;
			case 10:
			case 12:
			case 14:
			case 16:
				for(j = 0; j < 2*m_nWidth; j += 2)
				{
					WORD nVal = 0;
					if(m_nAlign)
					{
						nVal = *(PImageData + i*2*m_nWidth + j);
						nVal <<= 8; nVal |= *(PImageData + i*2*m_nWidth + j + 1);
					}
					else
					{
						nVal = *(WORD*)(PImageData + i*2*m_nWidth + j);
					}


					nVal >>= m_nOff;

					gray=nVal & 0xff;
				
					pBuf[nBuf++]=m_ColorTranTable[BI][gray]; 
					pBuf[nBuf++]=m_ColorTranTable[GI][gray]; 
					pBuf[nBuf++]=m_ColorTranTable[RI][gray];

					//nBuf +=3; //m_nWidth;


					//pBuf[nBuf++] = nVal & 0xff;
				}
				break;
			default:
				return 0;
			}
		}

		return nBuf;
	}
	else if(m_nPiexl==24)
	{
		memcpy(pBuf, PImageData, nSize);

		return nSize;
	}

	if (m_pHead->nFormat==3)
	{
		NewWidth = CountWidth(m_pHead->nWidth);
		newByte=1;
		int demod = pfDoubleRate_DeModulateImage(m_MapData, m_TempData+FRAMEHEADLEN, NewWidth , m_pHead->nHeight, m_pHead->nWidth);
		*nLen = (int)(m_pHead->nHeight*NewWidth*m_pHead->nPixel);
	}
	else if(m_pHead->nFormat==0)
	{
		NewWidth = m_pHead->nWidth;
		newByte=1;
		*nLen = (int)(m_pHead->nWidth*m_pHead->nHeight);
		CYUV2RGB::convert_to_gray8(m_TempData+FRAMEHEADLEN,m_MapData,m_pHead->nWidth,m_pHead->nHeight,m_nPixelByte,0,m_nOff);
		//memcpy(m_pTempImage,m_pSingleImage,NewWidth*m_pHead->nHeight);
	}
	else if(m_pHead->nFormat==1){

		int nFactWidth =m_pHead->nWidth * 3;
		newByte=3;
		NewWidth = m_pHead->nWidth;
		m_pBayer.SetSize(m_pHead->nWidth, nFactWidth, m_pHead->nHeight);

		m_pBayer.bayer_to_rgb24(m_TempData+FRAMEHEADLEN,m_MapData);

		*nLen=m_pHead->nWidth*newByte*m_pHead->nHeight;
		//	memcpy(m_MapData,m_BayerData,m_pHead->nWidth*newByte*m_pHead->nHeight);
	}
	else if(m_pHead->nFormat==2){

		NewWidth = m_pHead->nWidth;
		newByte=3;
		*nLen = (int)nRet;
		CYUV2RGB::convert_yuv_to_rgb_buffer(m_TempData+FRAMEHEADLEN,m_MapData,m_pHead->nWidth,m_pHead->nHeight);
		//memcpy(m_pTempImage,m_pSingleImage,NewWidth*m_pHead->nHeight);
	}



	if ((m_nRotateType==0)&&(m_nShiningType==0))
		return m_MapData;
	else 
	{
		//轴映射
		if (m_nShiningType==1)
		{

			for (int i = 0; i<m_pHead->nHeight; i++)
			{
				memcpy(m_TempData+NewWidth*newByte*i,m_MapData+NewWidth*newByte*(m_pHead->nHeight-i-1),NewWidth*newByte);
			}

		}
		else if (m_nShiningType==2)
		{

			int Tcount = 0;
			for (int i = 0; i< m_pHead->nHeight; i++)
			{		
				for (int j = NewWidth-1; j >=0 ; j --)
				{
					for(int byte=0;byte<newByte;byte++)
					{
						m_TempData[i*newByte*NewWidth+newByte*(NewWidth-1-j)+byte] = m_MapData[i*NewWidth*newByte+newByte*j+byte];
						Tcount++;
					}
				}
			}
		}
		else if (m_nShiningType==3)
		{
			for (int i = 0; i<m_pHead->nHeight; i++)
			{
				memcpy(m_TempData+NewWidth*newByte*i,m_MapData+NewWidth*newByte*(m_pHead->nHeight-i-1),NewWidth*newByte);
			}

			memcpy(m_MapData,m_TempData,*nLen);

			int Tcount = 0;
			for (int i = 0; i< m_pHead->nHeight; i++)
			{		
				for (int j = NewWidth-1; j >=0 ; j --)
				{
					for(int byte=0;byte<newByte;byte++)
					{
						m_TempData[i*newByte*NewWidth+newByte*(NewWidth-1-j)+byte] = m_MapData[i*NewWidth*newByte+newByte*j+byte];
						Tcount++;
					}
				}
			}
		}
		else 
		{
			memcpy(m_TempData,m_MapData,NewWidth*newByte*m_pHead->nHeight);
		}

		if (m_nRotateType==1)//旋转
		{
			int Tcount = 0;

			int RWidth=(m_pHead->nHeight/4)*4;

			for (int j = 0; j < NewWidth ; j ++)
			{

				// for (int i = 0;i<m_pHead->nHeight; i++)
				for (int i = RWidth; i>=0; i--)
				{
					for(int byte=0;byte<newByte;byte++){

						m_MapData[j*RWidth*newByte+newByte*i+byte] = m_TempData[i*NewWidth*newByte+newByte*j+byte];

						//Tcount++;
					}
				}
			}
		}
		else if (m_nRotateType==2)
		{
			int Tcount = 0;
			for (int i = m_pHead->nHeight-1; i>=0; i--)
			{

				for (int j = NewWidth-1; j >=0  ; j --)
				{
					for(int byte=0;byte<newByte;byte++){ 

						m_MapData[Tcount] = m_TempData[i*NewWidth*newByte+newByte*j+byte];
						Tcount++;
					}
				}
			}
		}
		else if (m_nRotateType==3)
		{
			int Tcount = 0;

			int RWidth=(m_pHead->nHeight/4)*4;
			for (int j = NewWidth-1; j >=0 ; j --)
			{		
				for (int i = 0; i< RWidth; i++)
				{
					for(int byte=0;byte<newByte;byte++){ 

						m_MapData[(NewWidth-1-j)*RWidth*newByte+newByte*i+byte] = m_TempData[i*NewWidth*newByte+newByte*j+byte];

						//Tcount++;
					}
				}
			}
		}
		else 
		{
			memcpy(m_MapData,m_TempData,NewWidth*newByte*m_pHead->nHeight);	
		}
	}
	*/