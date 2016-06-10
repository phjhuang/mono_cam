
#pragma once



#include <iostream>
#include <string>
#include <list>
#include <stdlib.h>
#include <stdio.h>
using namespace std;



class Str_Tools
{
public:
	Str_Tools();
	~Str_Tools();


	static int GetLength(string str1);//得到字符串的长度

	static char* GetChar(string str1);//得到字符串数组

	//wchar* 与 char* 之间的转变
	static wchar_t* GetWChar(string s);//得到字符串的宽字符数组

	static void w2c(char *pcstr,const wchar_t *pwstr, int len);//wchar_t转化为char， len为pcstr的最大长度

	static void c2w(wchar_t *pwstr,const char *str,int len);//char 转化为wchar_t，len为pwstr的长度

	static string Assign(const TCHAR* wstr);//把宽字符节赋值给 string

    //符合VC6和VC2010
	static  TCHAR* GetStr(string);

	//字符串查找
	static int Find(string s,char p);//找到p的第一个位置
	static int Find(string s, const char*p);//找到字符串*p的第一个位置
	static int Find (string s, char p ,int start);
	static int Find(string s, const char *p ,int start);
	static string StrCat(string str1,string str2);//字符串合并到新的字符串当中
	static list<string> Split(string s,string separator);//字符串拆分
	static string MakeUpper(string s);//字符串大写
	static string MakeLower(string s);//字符串小写
	static string MakeReverse(string str);//字符串倒置
	static int Compare(string str1,string str2);//两个字符串的比较
	static int CompareNoCase(string s1,string s2);//两个字符串不区分大小写比较
	static int Remove(string &s,char p);//删除字符串s中的p;
	static int ReverseFind(string s, char p );//找出字符串中最后一个p
	static string Left(string s, int nCount);//返回字符串前nCount个字符
	static string Right(string s, int nCount);//返回字符最后nCount个字符
	static string Mid(string s,int nFirst);//返回从中间nFirst+1以后的字符
	static string Mid(string s, int nFirst,int nCount);//从左边第 nFirst+1 个字符开始,获取后面  nCount 个字符
	static void SetAt(string &s, int nIndex, char ch);//把字符串s把nIndex位置上的字符变成ch；

	static string Format(int index);



};