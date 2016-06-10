#pragma once


// ShowPicZoom 对话框

class ShowPicZoom : public CDialogEx
{
	DECLARE_DYNAMIC(ShowPicZoom)

public:
	ShowPicZoom(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ShowPicZoom();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
//	virtual void OnCancel();
//	virtual void PostNcDestroy();
};
