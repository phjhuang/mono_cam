#if !defined(AFX_PANEFRAME_H__3910F256_B5BC_4A2E_90AE_A0A883B81631__INCLUDED_)
#define AFX_PANEFRAME_H__3910F256_B5BC_4A2E_90AE_A0A883B81631__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PaneFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPaneFrame frame

class CPaneFrame : public CMDIFrameWnd
{
	DECLARE_DYNCREATE(CPaneFrame)
protected:
	CPaneFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaneFrame)
	public:
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
	public:
//		HWND m_hWndMDIClient;
protected:
	virtual ~CPaneFrame();
	// Generated message map functions
	//{{AFX_MSG(CPaneFrame)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg void OnPaint();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANEFRAME_H__3910F256_B5BC_4A2E_90AE_A0A883B81631__INCLUDED_)
