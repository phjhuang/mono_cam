// Dialog2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CalibrationCtrl.h"
#include "Dialog2.h"
#include "afxdialogex.h"


// Dialog2 �Ի���

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


// Dialog2 ��Ϣ�������


BOOL Dialog2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//ShowWindow(SW_SHOW); 

	AfxBeginThread(TimeClose,this );


	 
	  
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


UINT Dialog2::TimeClose(LPVOID lpParam)
{
	Dialog2 * pThis = (Dialog2 *)lpParam;
	 
	Sleep(900);

	pThis ->PostMessage(WM_CLOSE, NULL, NULL);
	return 0;
}
