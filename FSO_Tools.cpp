// FSO_Tools.cpp: implementation of the FSO_Tools class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "FSO_Tools.h"
#include "Str_Tools.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FSO_Tools::FSO_Tools()
{

}

FSO_Tools::~FSO_Tools()
{

}
//文件是否存在
bool FSO_Tools::isFsoExists(string strFileName,bool* pbDir,long* pnSize)
{
	bool isFound=false;

	WIN32_FIND_DATA fd; 
	//char 转变为wchar_t
	
	/*wchar_t wstr[256];
	char * str=Str_Tools::GetChar(strFileName);
	Str_Tools::c2w(wstr,str,1024);*/

	HANDLE hFind = FindFirstFile((LPCTSTR)strFileName.c_str(), &fd); 

	//find path or file
	if ((hFind != INVALID_HANDLE_VALUE))
	{

		if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY)
		{
			if (pbDir)
				*pbDir=true;
		}
		else //is file
		{
			if (pbDir)
				*pbDir=false;

			if (pnSize)	*pnSize=fd.nFileSizeLow;

		}

		isFound=true;
	}

	FindClose(hFind); 

	return isFound;	
}
//创建文件
//在假设文件的路径已经存在了
bool FSO_Tools::CreateFile2(string strFileName){
	HANDLE hFile;
	hFile = CreateFile(Str_Tools::GetStr(strFileName),GENERIC_WRITE&&GENERIC_READ , FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);                          // no template 
	if (hFile == INVALID_HANDLE_VALUE) 
	{ 
		return false;
	} 

	else	
	{
		return true;
	}


}


//删除文件
bool FSO_Tools::DeleteFile2(string strFileName){
	return DeleteFile(Str_Tools::GetStr(strFileName));


}



//创建文件夹
bool FSO_Tools::CreateDirectory2(string strPath)
{
	vector<string>	m_arr;		// string Array to hold Directory Structures
	BOOL tt;			// BOOL used to test if Create Directory was successful
	int x1 = 0;			// Counter
	string tem = "";		// Temporary string Object

	m_arr.clear();


	for(x1 = 0; x1 < strPath.size(); x1++ )		// Parse the supplied string Directory String
	{
		if(strPath[x1] != '\\')		// if the Charachter is not a 
			tem += strPath[x1];		// add the character to the Temp String
		else
		{
			m_arr.push_back(tem);			// if the Character is a
			tem += '\\';			// Now add the \ to the temp string
		}
		if(x1 == strPath.size()-1)		// If we reached the end of the String
			m_arr.push_back(tem);
	}


	// Now lets cycle through the String Array and create each directory in turn
	for(x1 = 1; x1 < m_arr.size(); x1++)
	{
		tem = m_arr[x1];
		tt = CreateDirectory(Str_Tools::GetStr(tem),NULL);

		// If the Directory exists it will return a false
		if(tt)
			SetFileAttributes(Str_Tools::GetStr(tem),FILE_ATTRIBUTE_NORMAL);
		// If we were successful we set the attributes to normal
	}

	m_arr.clear();

	return isFsoExists((string)strPath);
}

//删除文件夹
bool FSO_Tools::DeleteDirectory2(string strPath,bool isKeepMe)
{
	if (strPath.size()==0) return false;

	string  tempFsoName=strPath+"\\*.*";//[200];

	WIN32_FIND_DATA FindFileData ={0};
	BOOL bNext = TRUE;
	HANDLE hFind =FindFirstFile(Str_Tools::GetStr(tempFsoName), &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{ 
		while(bNext)
		{
			string str1="..";
			string str2=".";
			if (_tcsicmp(FindFileData.cFileName, Str_Tools::GetStr(str1)) && _tcsicmp(FindFileData.cFileName , Str_Tools::GetStr(str2)) )
			{
				tempFsoName=Str_Tools::Assign(FindFileData.cFileName);

				if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)== FILE_ATTRIBUTE_DIRECTORY)
				{
					DeleteDirectory2(strPath+"\\"+tempFsoName,false);
				}
				else
				{

				DeleteFile(Str_Tools::GetStr(string(strPath+"\\"+tempFsoName).c_str()));
				}
			}
			bNext=FindNextFile(hFind, &FindFileData);
		}
		FindClose(hFind);
	} 


	if(!isKeepMe && !RemoveDirectory( Str_Tools::GetStr(strPath)))
	{
		return false;
	}

	return true;
}

//读文件里面的读内容
long FSO_Tools::ReadString(string strFileName,string& strText)
{
	long iFileSize=0;

	if (isFsoExists(strFileName,NULL,&iFileSize))
	{
		char* buf = new char[iFileSize+1];
		buf[iFileSize]=0;

		ifstream ifs(strFileName.c_str());//ios::binary | ios::in /out
		ifs.seekg (0, ios::beg);



		ifs.read(buf, iFileSize);

		strText=buf;

		delete [] buf;
		ifs.close();

		return iFileSize;
	}
	else
		return 0;
}
//向文件里面写内容
bool FSO_Tools::WriteString(string strFileName,string strText,bool bCreate)
{
	ofstream ofs(strFileName.c_str()) ;////ios::binary | ios::in /out  ios_base::out);
	ofs.write(strText.c_str(), strText.size());
	ofs.close();

	return true;

}
//把Stream保存到 strFileName中
bool FSO_Tools::SaveStream2File(IStream* pStm,string strFileName)
{
	if (!pStm) return false;


	ULONG x;

	STATSTG st;
	pStm->Stat(&st,STATFLAG_NONAME);

	LARGE_INTEGER liBeggining = { 0 }; 
	pStm->Seek(liBeggining,STREAM_SEEK_SET,NULL);

	ULONG size2= st.cbSize.QuadPart;

	char * buf = new char[size2];

	pStm->Read(buf,size2,&x);

	if (x>0)//save file
	{
		ofstream ofs(strFileName.c_str(),ios::binary|ios::out);//TEST_FILE, ios_base::out);
		ofs.write(buf, x);
		ofs.close();

	}

	delete[] buf;

	return true;	
}


//Load Stream from file
long FSO_Tools::LoadStreamFromFile(IStream* pStm,string strFileName)
{
	if (!pStm) return 0L;

	long iFileSize=0;

	if (!FSO_Tools::isFsoExists(strFileName,NULL,&iFileSize))
		return false;

	if (iFileSize<=0) return false;

	ULONG x;
	_ULARGE_INTEGER temp;
	temp.LowPart=iFileSize;
	pStm->SetSize(temp);
	STATSTG st;
	pStm->Stat(&st,STATFLAG_NONAME);

	LARGE_INTEGER liBeggining = { 0 }; 
	pStm->Seek(liBeggining,STREAM_SEEK_SET,NULL);

	int size2=st.cbSize.QuadPart;

	if (size2 <iFileSize) //????
	{
		//f.Close();
		return false;
	}

	ifstream ifs(strFileName.c_str() , ios::binary);
	ifs.seekg (0, ios::beg);

	char * buf = new char[iFileSize];
	ifs.read(buf, iFileSize);

	ifs.close();

	pStm->Write(buf,iFileSize,&x);
	delete [] buf;

	return iFileSize;
}

long FSO_Tools::EnumFiles(string strPath,list<string>* pFiles,string strNotFile)
{
	if (!isFsoExists(strPath)) return false;

	pFiles->clear();
	int nCount=0;

	string  tempFsoName=strPath+"\\*.*";

	WIN32_FIND_DATA FindFileData ={0};
	BOOL bNext = TRUE;
	HANDLE hFind =FindFirstFile(Str_Tools::GetStr(tempFsoName), &FindFileData);

	string name=strNotFile+".txt";

	if (hFind != INVALID_HANDLE_VALUE)
	{ 
		while(bNext)
		{
			
			string str1="..";
			string str2=".";
			if (_tcsicmp(FindFileData.cFileName, Str_Tools::GetStr(str1)) && _tcsicmp(FindFileData.cFileName , Str_Tools::GetStr(str2)) )
			{
				tempFsoName=Str_Tools::Assign(FindFileData.cFileName);

				if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!= FILE_ATTRIBUTE_DIRECTORY)
				{
					if( strNotFile.size()>0 && name!= tempFsoName )
					{
						if (pFiles)
							pFiles->push_back(tempFsoName);

						nCount++;
					}
					else 
					{
						if (pFiles)
							pFiles->push_back(tempFsoName);

						nCount++;
					}
				}

			}
			bNext=FindNextFile(hFind, &FindFileData);
		}

		FindClose(hFind);
	} 

	return nCount;
}
//得到文件的名字
string FSO_Tools::GetShortFileName(string strLongFileName)
{
	string strShortName="";

	int iPos=strLongFileName.find_last_of('\\');//strLongFileName.ReverseFind('\\');

	if (iPos>0 && iPos<strLongFileName.size())
		strShortName=strLongFileName.substr(iPos+1);

	return strShortName;
}
//得到文件的路径
string FSO_Tools::GetPathFromFileName(string strFileName)
{
	string strPath="";

	int iPos=strFileName.find_last_of('\\');//strFileName.ReverseFind('\\');

	if (iPos>0)
		strPath=strFileName.substr(0,iPos);//-1

	return strPath;
}
