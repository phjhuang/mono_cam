#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CTipDlg �Ի���

class CTipDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTipDlg)

public:
	CTipDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTipDlg();
    bool m_flag;


// �Ի�������
	enum { IDD = IDD_TipDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTip();
	// ��������
	CProgressCtrl MeasureProcess;
	virtual BOOL OnInitDialog();
 
	// measure pos
	// measure pos
	afx_msg void OnEnChangeEdit1();
	CStatic m_pos;
};
