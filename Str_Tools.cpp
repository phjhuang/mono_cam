//////////////////////////////////////////////////////////////////////
//字符串的操作
//@author 戴兰平
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Str_Tools.h"

//进行初始化
Str_Tools::Str_Tools(){


}
Str_Tools::~Str_Tools(){


}
//得到长度
int Str_Tools::GetLength(string str1){

	int length=str1.length();
	return length;

}
char* Str_Tools::GetChar(string str){
	int length=str.length();
	char* p=new char[length+1];
	for(int i=0;i<length;i++)
		p[i]=str[i];
	p[length]='\0';

	return p;

}

//得到宽字符节

wchar_t* Str_Tools::GetWChar(string s){
	//wchar_t wstr[256];

	int len=s.length();
	wchar_t *pwstr=new wchar_t[len];
	const char* str=s.c_str();
	int read= mbstowcs(pwstr,str,len);
	pwstr[read]=0;

	return pwstr;
}




//字符串合并
string Str_Tools::StrCat(string s1,string s2){
	string s3=s1+s2;
	return s3;

}

//将char* 转成wchar_t*的实现函数如下：

//这是把asii字符转换为unicode字符，和上面相同的原理
/**
pwstr:返回的wchar_t字符
len:pwstr的长度
str：要转化的char型字符
*/
void Str_Tools::c2w(wchar_t *pwstr,const char *str,int len)
{
/*
	if(str)
	{
		int nu = strlen(str);
		int n =(size_t)MultiByteToWideChar(CP_ACP,0,str,nu,NULL,0);
		if(n>=len)n=len-1;
		MultiByteToWideChar(CP_ACP,0,str,nu,pwstr,n);
		pwstr[n]=0;
	}
	*/

	if(str){
	 int nu=strlen(str)+1;

	int read= mbstowcs(pwstr,str,len);
	pwstr[read]=0;
	}

} 
/*
pcstr:返回的char型字符
pwstr:输入wchar_t型字符
*len:pcstr 返回的最大长度 2 * wcslen(pwstr)+1
*

*/
void Str_Tools::w2c(char *pcstr,const wchar_t *pwstr, int len)
{
/*
	int nlength=wcslen(pwstr);

	//获取转换后的长度
	int nbytes = WideCharToMultiByte( 0,0, pwstr,nlength,NULL,0,NULL,NULL ); // we don't want to know if a character didn't make it through the translation

	// make sure the buffer is big enough for this, making it larger if necessary
	if(nbytes>len)   nbytes=len;

	// 通过以上得到的结果，转换unicode 字符为ascii 字符
	WideCharToMultiByte( 0,0, pwstr,nlength,pcstr,nbytes, NULL,NULL );

	//return pcstr ;
*/


   int read=wcstombs(pcstr,pwstr, len);
   pcstr[read]=0;



}




/*
字符串查找
*/
int Str_Tools::Find(string s, char p){
	int pos=s.find(p,0);
	return pos;

}

int Str_Tools:: Find(string s, const char*p){
	int pos=s.find(p,0);
	return pos;


}

int Str_Tools::Find(string s,char p ,int start){

	int pos=s.find(p,start);
	return pos;
}
int Str_Tools::Find(string s,const char *p ,int start){

	int pos=s.find(p,start);
	return pos;
}


string Str_Tools::Assign(const TCHAR *pwstr){

	//把wchar_t 转化为char 
#ifdef UNICODE
	int nlength=wcslen(pwstr);
	int len=2*nlength+1;
    char* pcstr=new char[len+1];//转化后的字节数
	int read=wcstombs(pcstr,pwstr, len);
	pcstr[read]=0;
/*
	int nbytes = WideCharToMultiByte( 0,0, pwstr,nlength,NULL,0,NULL,NULL ); 
	if(nbytes>len)   nbytes=len;
	char* pcstr=new char[nbytes+1];//转化后的字节数
	memset(pcstr,0,nbytes);
	WideCharToMultiByte( 0,0, pwstr,nlength,pcstr,nbytes, NULL,NULL );
	pcstr[nbytes]=0;
	*/
	string s=std::string(pcstr);
	delete[] pcstr;
	pcstr=NULL;
	return s;

#else
	string s=std::string(pwstr);
	return s;

#endif
}

TCHAR* Str_Tools::GetStr(string str){
#ifdef UNICODE	
  	return GetWChar(str);
#else
	 return	GetChar(str);
#endif


}
//大小写转化
string Str_Tools::MakeLower(string str){
	string s=std::string(str.c_str());
	int i;

	for(i=0;i<str.length();i++){


		if(str[i]>='A'&&str[i]<='Z')
		{
			s[i]=str[i]+32;

			continue;
		}

	}
	return s;

}


string Str_Tools::MakeUpper(string str){
	string s=std::string(str.c_str());
	int i;

	for(i=0;i<str.length();i++){


		if(str[i]>='a'&&str[i]<='z')
		{
			s[i]=str[i]-32;

			continue;
		}

	}
	return s;


}
//
list<string> Str_Tools::Split(string str,string separator){
	list<string> result;  
	int cutAt;  
	while( (cutAt = str.find_first_of(separator)) != str.npos )  
	{  
		if(cutAt > 0)  
		{  
			result.push_back(str.substr(0, cutAt));  
		}  
		str = str.substr(cutAt + 1);  
	}  
	if(str.length() > 0)  
	{  
		result.push_back(str);  
	}  
	return result; 
}
//字符串倒置

string Str_Tools::MakeReverse(string str){

	string s=std::string(str.c_str());
	int i;
	int nlen=str.length();

	for(i=0;i<nlen;i++){
		s[i]=str[nlen-1-i];
	}
	return s;
}
//字符串比较
int Str_Tools::Compare(string s1,string s2){

	return s1.compare(s2);

}
//没有大小写比较
int Str_Tools::CompareNoCase(string s1,string s2){
	s1=MakeLower(s1);
	s2=MakeLower(s2);

	return s1.compare(s2);


}

//把字符串从p中剔除
int Str_Tools::Remove(string &str, char c){
   
	const char *cpc=str.c_str(); 
	char* s = new char[256];//足够长
     strcpy(s,cpc);
	int i,j; 
	int nCount=0;
    for (i = 0, j = 0; s[i] != '\0'; i++) 
    { 
        if (s[i] != c) 
        { 
            s[j++] = s[i]; 
        } 
		else
			nCount++;
    } 
    s[j] = '\0';    //这一条语句千万不能忘记，字符串的结束标记 

	str=std::string(s);
		return nCount;
	}
//获得前nCount个字符
string Str_Tools::Left(string s, int nCount){

	int nlen=s.length();
	if(nCount<nlen)
		return s.substr(0,nCount);
	else
		return s.substr(0,nlen);


}
//获得最后nCount个字符
string Str_Tools::Right(string s, int nCount){
	int nlen=s.length();
	if(nCount<nlen)
		return s.substr(nlen-nCount,nlen-1);
	else
		return s.substr(0,nlen);


}
//中间开始
string Str_Tools::Mid(string s, int nFirst){
	int nlen=s.length();
	if(nFirst<0){
	return NULL;
	
	}
	else
	{
	  if(nFirst>nlen)
		  return NULL;
	  else
		 return s.substr(nFirst,nlen-nFirst);
	
	
	}


}
string Str_Tools::Mid(string s, int nFirst, int nCount){
	int nlen=s.length();
	if(nFirst<0){
	return NULL;
	
	}
	else
	{
	  if(nFirst>nlen)
		  return NULL;
	  else
	  {
		  int len=(nlen-nFirst)<=nCount?(nlen-nFirst):nCount;
		  return s.substr(nFirst,len);
	  }
	
	
	}


}

//字符串替换
void Str_Tools::SetAt(string &s, int nIndex,char ch){
	 int nlen=s.length();
	 if(nIndex<=nlen-1){
	 s[nIndex]=ch;
	 
	 }
  

}

//string中 format
string Str_Tools::Format(int index){
	char c[256];
   sprintf(c, "%d", index);
   return (std::string(c));


}