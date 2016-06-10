//Cache Map保存

#include "StdAfx.h"
#include "MapFileCtrl.h"
//#include "..\..\include\Str_Tools.h"
//Map file Name
CMapFileCtrl::CMapFileCtrl()
{

	m_pHead = NULL;
	m_pSaver = NULL;	
	m_IsMapInitOK=FALSE;
	m_IsOpenOK=FALSE;
	m_SavePath="";
	m_IsCreateOK=FALSE;
	m_SaveCount=0;


	m_pSaver = new IBaseCasheMap;
}


CMapFileCtrl::~CMapFileCtrl(){
	m_pSaver=NULL;
	m_pHead=NULL;
	m_SavePath="";
	m_SaveCount=0;
	/*** 20140211 dlp */
	//if(m_pHead)
		//delete m_pHead;
	//m_pHead=NULL;


	if (m_pSaver!=NULL)
	{
		delete m_pSaver;
		m_pSaver = NULL;

	}

}
//初始化组件
BOOL CMapFileCtrl::CacheMapCom_Init(string sProgID,IUnknown* pOwner )
{
	//ASSERT( !m_IsMapInitOK );
	////	radar_uninit();	
	//HRESULT   hr;
	//IUnknown* pCard = NULL;
	//GUID      clsid  = GUID_NULL;

	//USES_CONVERSION; 
	//hr = CLSIDFromProgID( A2W(sProgID.c_str()) , &clsid );
	//if( FAILED(hr) ) //没有这个协议
	//	return FALSE;
	//hr = ::CoCreateInstance( clsid  ,pOwner,
	//	CLSCTX_ALL,IID_IUnknown , (LPVOID*)&pCard );//__uuidof(RadarComm)
	//if( NULL == pCard )
	//	return FALSE;

	// hr = pCard->QueryInterface(__uuidof(IBaseCasheMap),(void**)&m_pSaver);
	//SAFE_RELEASE( pCard );		
	//if( NULL == m_pSaver ) 
	//	return FALSE;



	m_IsMapInitOK= TRUE;	
	return 	m_IsMapInitOK;			

}
//释放组件

BOOL CMapFileCtrl::CacheCom_uninit(){

	if( !m_IsMapInitOK)
		return FALSE;

	//SAFE_RELEASE(m_pSaver);	



	m_IsMapInitOK =FALSE;	
	return TRUE;

}


long CMapFileCtrl::MAP_EnumPlugINS(CStringArray* pArr)
{
	pArr->RemoveAll();

	CATID cat=CATID_CACHEMAP;
	list<string> plugList;

	long nCount=CAT_Tools::EnumPlug(&plugList,cat);

	CString sPlugName;

	for (list<string>::iterator i2= plugList.begin();i2!=plugList.end();i2++)
	{
		sPlugName=(*i2).c_str();
		pArr->Add(sPlugName);
	}

	plugList.clear();

	return nCount;
}

BOOL CMapFileCtrl::CreatMapFile(string path,string pDevcename,string pPortname)
{

	if(m_IsMapInitOK)
	{
		m_SavePath=path;

		HRESULT RES = m_pSaver->CreatMapFile((PBYTE)(m_SavePath.c_str()),(PBYTE)(pDevcename.c_str()),(PBYTE)(pPortname.c_str()));
		if (RES == S_FALSE)
			return FALSE;

		m_IsCreateOK=TRUE;
		m_SaveCount=0;
		return TRUE;
	}
	else
		return FALSE;
}

string CMapFileCtrl::GetSaveFilePath()
{
	if(m_IsMapInitOK)
	{
		return (char*)m_pSaver->GetSaveFilePath();
	}

	return "";
}

BOOL CMapFileCtrl::SetHeaderInfo(CCASHEMAPLib::MAPFILEHEAD* pHead)
{
	if(m_IsMapInitOK)//m_IsCreateOK
	{
		m_pSaver->SetMapFileHead(pHead);

		return TRUE;
	}
	return FALSE;
}

CCASHEMAPLib::MAPFILEHEAD* CMapFileCtrl::GetHeaderInfo()
{
	if(m_IsMapInitOK)
	{
		m_pHead=m_pSaver->GetMapFileHead();
		return m_pHead;
	}
	else return NULL;
}

BOOL CMapFileCtrl::WriteData(BYTE* data,int nSize,BYTE* pframhear,int nnhear)
{
	if(m_IsMapInitOK&&m_IsCreateOK)
	{
		long nlen=m_pSaver->WriteData(data,nSize,pframhear,nnhear);

		if(nlen>0)
		{
			m_SaveCount++;

			if(m_pHead)
				m_pHead->nSaveCount=m_SaveCount;
			return TRUE;
		}
	}

	return FALSE;
}

void CMapFileCtrl::CloseMapFile()
{
	if(m_IsMapInitOK&&(m_IsCreateOK||m_IsOpenOK))
	{
		m_pSaver->CloseMapFile();
		m_IsCreateOK=FALSE;
		m_IsOpenOK=FALSE;
	}
}

BOOL CMapFileCtrl::SetLoopEnable(BOOL IsLoop,long nLoopCount)
{
	if((!m_IsMapInitOK) || (!m_IsCreateOK))
		return FALSE;

	return m_pSaver->SetLoopEnable(IsLoop,nLoopCount);
}

long CMapFileCtrl::GetFreeDisk(string pPath)  //剩余多少兆
{
	if(!m_IsMapInitOK)
		return 0;

	ULONGLONG mFreeDiskSize=0;

	mFreeDiskSize=m_pSaver->GetFreeDisk((BYTE*)pPath.c_str());

	long feszie= mFreeDiskSize/((ULONGLONG)(1024*1024));
	return feszie;
}

BOOL CMapFileCtrl::SaveToMapFile(string pFileName)  //另存
{
	if((!m_IsMapInitOK) || (!m_IsCreateOK))
		return FALSE;

	return m_pSaver->SaveToMapFile((BYTE*)pFileName.c_str());
}

BOOL CMapFileCtrl::SetMinDiskSize(long nSize) //不小于多少兆
{
	if(!m_IsMapInitOK)
		return FALSE;
	

	ULONGLONG lsize=(ULONGLONG)nSize;

	ULONGLONG minSize=nSize*(ULONGLONG)1024*(ULONGLONG)1024;
	
	return m_pSaver->SetMinDiskSize(minSize);

}

BOOL CMapFileCtrl::SetCalbackInfo(void* pMsgCallBack,void* pHwn)//消息传递
{
	if(!m_IsMapInitOK)
		return FALSE;

	return m_pSaver->SetCalbackInfo(pMsgCallBack,pHwn);
}

BOOL CMapFileCtrl::OpenMapFile(string pFileName)
{
	if(m_IsMapInitOK)
	{
		m_SavePath=pFileName;

		HRESULT RES = m_pSaver->OpenMapFile((PBYTE)(m_SavePath.c_str()));
		if (RES == S_FALSE)
			return FALSE;

		m_IsOpenOK=TRUE;
		m_SaveCount=0;
		m_pHead=m_pSaver->GetMapFileHead();
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CMapFileCtrl::SetDataType(int nRotateType,int nShiningType)
{
	if(m_IsMapInitOK)
	{
		HRESULT RES = m_pSaver->SetDataType(nRotateType,nShiningType);
		if (RES == S_FALSE)
			return FALSE;

		return TRUE;
	}
	else
		return FALSE;
}

BOOL CMapFileCtrl::GetDataType(long& nRotateType,long& nShiningType)
{
	if(m_IsMapInitOK)
	{
		HRESULT RES = m_pSaver->GetDataType(&nRotateType,&nShiningType);
		if (RES == S_FALSE)
			return FALSE;

		return TRUE;
	}
	else
		return FALSE;
}


PBYTE CMapFileCtrl::GetMapData(int nImagePos,int nDataFormat,int& nLen)
{
	if(m_IsMapInitOK&&m_IsOpenOK)
        return m_pSaver->GetMapData(nImagePos,nDataFormat,&nLen);

	return NULL;
}

PBYTE CMapFileCtrl::GetComData(int nCompos,int nMovepos,int nDataType)
{
	if(m_IsMapInitOK&&m_IsOpenOK)
        return m_pSaver->GetComData( nCompos,nMovepos,nDataType);

	return NULL;
}

	
BOOL CMapFileCtrl::SaveToData(int nBeginPos,int nEndPos,int nFrame,int nType,string pSavePath,int nfps)
{
	if(m_IsMapInitOK&&m_IsOpenOK)
      {
		  HRESULT RES = m_pSaver->SaveToData(nBeginPos,nEndPos,nFrame,nType,(PBYTE)(pSavePath.c_str()),nfps);
		if (RES == S_FALSE)
			return FALSE;

		return TRUE;
	}

	return NULL;
}

void  CMapFileCtrl::SetSaveCtrl(BOOL enable){

	if(m_IsMapInitOK&&m_IsOpenOK){
	   m_pSaver->SetSaveCtrl(enable);
	
	}
}