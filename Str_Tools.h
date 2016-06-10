
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


	static int GetLength(string str1);//�õ��ַ����ĳ���

	static char* GetChar(string str1);//�õ��ַ�������

	//wchar* �� char* ֮���ת��
	static wchar_t* GetWChar(string s);//�õ��ַ����Ŀ��ַ�����

	static void w2c(char *pcstr,const wchar_t *pwstr, int len);//wchar_tת��Ϊchar�� lenΪpcstr����󳤶�

	static void c2w(wchar_t *pwstr,const char *str,int len);//char ת��Ϊwchar_t��lenΪpwstr�ĳ���

	static string Assign(const TCHAR* wstr);//�ѿ��ַ��ڸ�ֵ�� string

    //����VC6��VC2010
	static  TCHAR* GetStr(string);

	//�ַ�������
	static int Find(string s,char p);//�ҵ�p�ĵ�һ��λ��
	static int Find(string s, const char*p);//�ҵ��ַ���*p�ĵ�һ��λ��
	static int Find (string s, char p ,int start);
	static int Find(string s, const char *p ,int start);
	static string StrCat(string str1,string str2);//�ַ����ϲ����µ��ַ�������
	static list<string> Split(string s,string separator);//�ַ������
	static string MakeUpper(string s);//�ַ�����д
	static string MakeLower(string s);//�ַ���Сд
	static string MakeReverse(string str);//�ַ�������
	static int Compare(string str1,string str2);//�����ַ����ıȽ�
	static int CompareNoCase(string s1,string s2);//�����ַ��������ִ�Сд�Ƚ�
	static int Remove(string &s,char p);//ɾ���ַ���s�е�p;
	static int ReverseFind(string s, char p );//�ҳ��ַ��������һ��p
	static string Left(string s, int nCount);//�����ַ���ǰnCount���ַ�
	static string Right(string s, int nCount);//�����ַ����nCount���ַ�
	static string Mid(string s,int nFirst);//���ش��м�nFirst+1�Ժ���ַ�
	static string Mid(string s, int nFirst,int nCount);//����ߵ� nFirst+1 ���ַ���ʼ,��ȡ����  nCount ���ַ�
	static void SetAt(string &s, int nIndex, char ch);//���ַ���s��nIndexλ���ϵ��ַ����ch��

	static string Format(int index);



};