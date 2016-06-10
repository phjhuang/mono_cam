// AT_Tools.h: interface for the CAT_Tools class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AT_TOOLS_H__57D7C62A_8373_48C9_8C25_5B8F0B937EB7__INCLUDED_)
#define AFX_AT_TOOLS_H__57D7C62A_8373_48C9_8C25_5B8F0B937EB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable:4786)

#include <atlconv.h>
#include <comcat.h>
#include <comdef.h>
#include "toolsCLSID.h"
#include <list>

using namespace std;

class CAT_Tools  
{
public:
	CAT_Tools();
	virtual ~CAT_Tools();

	static long EnumPlug(list<string>* pArr,CATID cat);
	static bool Register(CATID cat,CLSID plug,BOOL bRegister);
};

#endif // !defined(AFX_AT_TOOLS_H__57D7C62A_8373_48C9_8C25_5B8F0B937EB7__INCLUDED_)
