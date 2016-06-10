// TipDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CalibrationCtrl.h"
#include "TipDlg.h"
#include "afxdialogex.h"


// CTipDlg �Ի���

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


// CTipDlg ��Ϣ�������


void CTipDlg::OnBnClickedButtonTip()
{
	int tmp= MessageBox("ȷ��Ҫ�˳��������̣�","��" ,MB_OKCANCEL ) ;

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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_flag=false;
	
	MeasureProcess.SetRange(0,64);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



// void CTipDlg::OnCancel()
// {
// 	// TODO: �ڴ����ר�ô����/����û���
// 	OnBnClickedButtonTip();
// 	//CDialogEx::OnCancel();
// }


