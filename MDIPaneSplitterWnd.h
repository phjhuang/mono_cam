#if !defined(AFX_MDIPANESPLITTERWND_H__0093AF2E_E2EC_4CAF_94AA_29DCB1EAF035__INCLUDED_)
#define AFX_MDIPANESPLITTERWND_H__0093AF2E_E2EC_4CAF_94AA_29DCB1EAF035__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MDIPaneSplitterWnd.h : header file
//

#include "Dialog2.h"


/////////////////////////////////////////////////////////////////////////////
// CMDIPaneSplitterWnd window

class CMDIPaneSplitterWnd : public CSplitterWnd
{
// Construction
public:
	CMDIPaneSplitterWnd();

// Attributes
public:
protected:
	CSize m_MultiPaneInfo;
// Operations
public:

	Dialog2 m_d2;



private:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDIPaneSplitterWnd)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	CView* CreateChildren(CDocument *pDoc,int viewnum,int pos,CString framtext="");//,int viewtype=0, int showchart=0
	CView* GetActiveImgView();
	CView* CreateMDIPaneChild(CCreateContext *pContext, BOOL bSizeParent = FALSE,int viewnum=0,int pos=0,CString framtext="");
	BOOL CreateMDIPane(int iRow, int iCol, CSize sizeInit, CCreateContext *pContext);
	virtual ~CMDIPaneSplitterWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMDIPaneSplitterWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDIPANESPLITTERWND_H__0093AF2E_E2EC_4CAF_94AA_29DCB1EAF035__INCLUDED_)
