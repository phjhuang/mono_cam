// AT_Tools.cpp: implementation of the CAT_Tools class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CAT_Tools.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAT_Tools::CAT_Tools()
{

}

CAT_Tools::~CAT_Tools()
{

}

long CAT_Tools::EnumPlug(list<string>* pArr,CATID cat)
{
	USES_CONVERSION;
	
	int nCount=0;
	
	try
	{
		ICatInformationPtr pICatInformation(CLSID_StdComponentCategoriesMgr);
		IEnumCLSID* pIEnumCLSID = NULL ;
		HRESULT hr = pICatInformation->EnumClassesOfCategories(1, &cat, 0, NULL, &pIEnumCLSID) ;
		//ASSERT(SUCCEEDED(hr)) ;
		
		CLSID clsid ;
		string  strProgID;
		LPOLESTR lpwProgID = NULL;
		
		while ((hr = pIEnumCLSID->Next(1, &clsid, NULL)) == S_OK)
		{
			nCount++;
			
			if (pArr)//arr
			{	
				hr = ::ProgIDFromCLSID( clsid, &lpwProgID );
				if ( SUCCEEDED(hr) )
				{
					//::MessageBoxW( NULL, lpwProgID, L"ProgID", MB_OK );
					
					IMalloc * pMalloc = NULL;
					hr = ::CoGetMalloc( 1, &pMalloc );  // 取得 IMalloc
					
					strProgID=OLE2CT(lpwProgID);
					
					if ( SUCCEEDED(hr) )
					{
						pMalloc->Free( lpwProgID );  // 释放ProgID内存
						pMalloc->Release();          // 释放IMalloc
					}
				}
				
				pArr->push_back(strProgID);
				
				//AfxMessageBox(strProgID);
				
			}
			
			
		}
	}
	catch(...){
		
		return 0L;
	}
	
	return nCount;
}

bool  CAT_Tools::Register(CATID cat,CLSID plug,BOOL bRegister)
{
	ICatRegister* pICatRegister = NULL ;
	HRESULT hr = ::CoCreateInstance(    CLSID_StdComponentCategoriesMgr,
		NULL, CLSCTX_ALL, IID_ICatRegister,
		(void**)&pICatRegister) ;
	if (FAILED(hr))
	{
		::MessageBoxW( NULL, (LPCWSTR)"error", L"ProgID", MB_OK );
		//ErrorMessage("Could not create the ComCat component.", hr);
		return false ;
	}
	
	CATEGORYINFO CatInfo ;
	CatInfo.catid               = cat ;
	CatInfo.lcid                = LOCALE_SYSTEM_DEFAULT ;
	wcscpy(CatInfo.szDescription, L"epSDK Category") ;

	if (bRegister)
	{
		hr = pICatRegister->RegisterCategories(1, &CatInfo) ;
		hr = pICatRegister->RegisterClassImplCategories(plug,1, &cat) ;
	}
	else
	{
		hr = pICatRegister->UnRegisterClassImplCategories(plug,1, &cat);

		ICatInformation* pICatInformation = NULL ;
		hr = pICatRegister->QueryInterface(IID_ICatInformation, (void**)&pICatInformation) ;
		IEnumCLSID* pIEnumCLSID = NULL ;
		hr = pICatInformation->EnumClassesOfCategories(1, &cat, 0, NULL, &pIEnumCLSID) ;
		CLSID clsid ;
		hr = pIEnumCLSID->Next(1, &clsid, NULL) ;
		if (hr == S_FALSE)
		{
			hr = pICatRegister->UnRegisterCategories(1, &cat) ;
		}
		pIEnumCLSID->Release() ;
		pICatInformation->Release() ;
	}
	if (pICatRegister)
	{
		pICatRegister->Release() ;
	}

	return true;
}
