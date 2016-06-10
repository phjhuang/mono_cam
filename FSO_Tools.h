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
	static string GetPathFromFileName(string strFileName);//�õ��ļ���·��

	static string GetShortFileName(string strLongFileName);//�õ��ļ�������

	FSO_Tools();
	virtual ~FSO_Tools();

	static bool isFsoExists(string strFileName,bool* pbDir=NULL,long* pnSize=NULL);//�ļ������ļ����Ƿ����
	
	static bool CreateFile2(string strFileName);//�����ļ�

	static bool DeleteFile2(string strFileName);//ɾ���ļ�


	static bool CreateDirectory2(string strPath);//�����ļ���

	static bool DeleteDirectory2(string strPath,bool isKeepMe=false);//ɾ���ļ���

	static long ReadString(string strFileName,string& strText);// ��ȡ�ļ����������

	static bool WriteString(string strFileName,string strText,bool bCreate=true);//���ļ�����д���� 
	
	static bool SaveStream2File(IStream* pStm,string strFileName);//��Stream�����浽strFileName��

	static long LoadStreamFromFile(IStream* pStm,string strFileName);//��file��ȡStream��
	
	static long EnumFiles(string strPath,list<string>* pFiles,string strNotFile);//����Path�����file�ļ�
};

#endif 
