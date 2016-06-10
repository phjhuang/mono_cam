#pragma once



// CImgViewForm 窗体视图
#include "resource.h"
#include"ImgPreview.h"

class CImgViewForm : public CFormView
{
	DECLARE_DYNCREATE(CImgViewForm)

protected:
	CImgViewForm();           // 动态创建所使用的受保护的构造函数
	virtual ~CImgViewForm();

public:
	enum { IDD = IDD_IMGVIEW_FORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	HWND m_hWnd;
	int m_MsgID;
	int m_MsgPaintID;

	HWND m_hpointWnd;
	int m_PointMsgID;

	CImgPreview m_ImgPreview;
    int m_nIndex;
	int m_nCampos;

	BOOL m_IsCheckPoint;
	int m_PonitCount;
	 CPoint m_Svcpoint;

	void SetHwnd(HWND hWnd,int nMsg);
	void SetPointHwnd(HWND hWnd,int nMsg);
	HDC GetShowWndInfo(CRect& Rect);
	
	void UpdateShowInfo();
	int  CloseCtrl();

void UpdatePaint();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//afx_msg void OnPaint();
	//virtual void OnDraw(CDC* /*pDC*/);
	virtual void OnDraw(CDC* /*pDC*/);

	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


