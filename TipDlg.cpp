// TipDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CalibrationCtrl.h"
#include "TipDlg.h"
#include "afxdialogex.h"


// CTipDlg 对话框

IMPLEMENT_DYNAMIC(CTipDlg, CDialogEx)

CTipDlg::CTipDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTipDlg::IDD, pParent)
{

}

CTipDlg::~CTipDlg()
{
}

void CTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, MeasureProcess);

	DDX_Control(pDX, IDC_POS, m_pos);
}


BEGIN_MESSAGE_MAP(CTipDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_TIP, &CTipDlg::OnBnClickedButtonTip)
	//ON_EN_CHANGE(IDC_EDIT1, &CTipDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CTipDlg 消息处理程序


void CTipDlg::OnBnClickedButtonTip()
{
	int tmp= MessageBox("确定要退出测量过程？","！" ,MB_OKCANCEL ) ;

    if (IDOK  == tmp ) 
        {
			m_flag=true;

			Sleep(256);
			// OnCancel();
			// ::EndDialog(m_hWnd,IDCANCEL);
			SendMessage(WM_CLOSE,0,0);
        }
}


BOOL CTipDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_flag=false;
	
	MeasureProcess.SetRange(0,64);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



// void CTipDlg::OnCancel()
// {
// 	// TODO: 在此添加专用代码和/或调用基类
// 	OnBnClickedButtonTip();
// 	//CDialogEx::OnCancel();
// }


