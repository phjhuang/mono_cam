// ShowPicZoom.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CalibrationCtrl.h"
#include "ShowPicZoom.h"
#include "afxdialogex.h"


// ShowPicZoom �Ի���

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


// ShowPicZoom ��Ϣ�������


//void ShowPicZoom::OnCancel()
//{
//	// TODO: �ڴ����ר�ô����/����û���
//
//	DestroyWindow();
//	//huangjin65
//	//CDialogEx::OnCancel();
//}


//void ShowPicZoom::PostNcDestroy()
//{
//	// TODO: �ڴ����ר�ô����/����û���
//	/////huangjin65
//	CDialogEx::PostNcDestroy();
//	
//	delete this;
//}
