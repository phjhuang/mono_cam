// ShowPicZoom.cpp : 实现文件
//

#include "stdafx.h"
#include "CalibrationCtrl.h"
#include "ShowPicZoom.h"
#include "afxdialogex.h"


// ShowPicZoom 对话框

IMPLEMENT_DYNAMIC(ShowPicZoom, CDialogEx)

ShowPicZoom::ShowPicZoom(CWnd* pParent /*=NULL*/)
	: CDialogEx(ShowPicZoom::IDD, pParent)
{

}

ShowPicZoom::~ShowPicZoom()
{
}

void ShowPicZoom::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ShowPicZoom, CDialogEx)
END_MESSAGE_MAP()


// ShowPicZoom 消息处理程序


//void ShowPicZoom::OnCancel()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//	DestroyWindow();
//	//huangjin65
//	//CDialogEx::OnCancel();
//}


//void ShowPicZoom::PostNcDestroy()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	/////huangjin65
//	CDialogEx::PostNcDestroy();
//	
//	delete this;
//}
