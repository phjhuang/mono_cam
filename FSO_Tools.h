// FSO_Tools.h: interface for the FSO_Tools class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FSO_TOOLS_H__7D9EB20A_61AF_40EB_92B8_3ED826297140__INCLUDED_)
#define AFX_FSO_TOOLS_H__7D9EB20A_61AF_40EB_92B8_3ED826297140__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "StdAfx.h"
#include <string>
#include <vector>
#include <fstream>
#include <windows.h>
#include <list>

using namespace std;

class FSO_Tools  
{
public:
	static string GetPathFromFileName(string strFileName);//得到文件的路径

	static string GetShortFileName(string strLongFileName);//得到文件的名字

	FSO_Tools();
	virtual ~FSO_Tools();

	static bool isFsoExists(string strFileName,bool* pbDir=NULL,long* pnSize=NULL);//文件或者文件夹是否存在
	
	static bool CreateFile2(string strFileName);//创建文件

	static bool DeleteFile2(string strFileName);//删除文件


	static bool CreateDirectory2(string strPath);//建立文件夹

	static bool DeleteDirectory2(string strPath,bool isKeepMe=false);//删除文件夹

	static long ReadString(string strFileName,string& strText);// 读取文件里面的内容

	static bool WriteString(string strFileName,string strText,bool bCreate=true);//向文件里面写内容 
	
	static bool SaveStream2File(IStream* pStm,string strFileName);//把Stream留保存到strFileName中

	static long LoadStreamFromFile(IStream* pStm,string strFileName);//从file读取Stream流
	
	static long EnumFiles(string strPath,list<string>* pFiles,string strNotFile);//陈列Path下面的file文件
};

#endif 
