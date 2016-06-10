#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CTipDlg 对话框

class CTipDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTipDlg)

public:
	CTipDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTipDlg();
    bool m_flag;


// 对话框数据
	enum { IDD = IDD_TipDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTip();
	// 测量进度
	CProgressCtrl MeasureProcess;
	virtual BOOL OnInitDialog();
 
	// measure pos
	// measure pos
	afx_msg void OnEnChangeEdit1();
	CStatic m_pos;
};
