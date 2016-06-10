// Dialog2.cpp : 实现文件
//

#include "stdafx.h"
#include "CalibrationCtrl.h"
#include "Dialog2.h"
#include "afxdialogex.h"


// Dialog2 对话框

IMPLEMENT_DYNAMIC(Dialog2, CDialogEx)

Dialog2::Dialog2(CWnd* pParent /*=NULL*/)
	: CDialogEx(Dialog2::IDD, pParent)
{

}

Dialog2::~Dialog2()
{
}

void Dialog2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dialog2, CDialogEx)
END_MESSAGE_MAP()


// Dialog2 消息处理程序


BOOL Dialog2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//ShowWindow(SW_SHOW); 

	AfxBeginThread(TimeClose,this );


	 
	  
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


UINT Dialog2::TimeClose(LPVOID lpParam)
{
	Dialog2 * pThis = (Dialog2 *)lpParam;
	 
	Sleep(900);

	pThis ->PostMessage(WM_CLOSE, NULL, NULL);
	return 0;
}
