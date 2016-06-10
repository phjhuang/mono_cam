// PaneFrame.cpp : implementation file
//

#include "StdAfx.h"
#include "CalibrationCtrl.h"
#include "PaneFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaneFrame

IMPLEMENT_DYNCREATE(CPaneFrame, CMDIFrameWnd)

CPaneFrame::CPaneFrame()
{
}

CPaneFrame::~CPaneFrame()
{
}


BEGIN_MESSAGE_MAP(CPaneFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CPaneFrame)
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaneFrame message handlers

BOOL CPaneFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	ASSERT(m_hWnd != NULL);
	ASSERT(m_hWndMDIClient == NULL);
	
	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_BORDER |
		WS_CLIPCHILDREN | WS_CLIPSIBLINGS |	WS_HSCROLL | WS_VSCROLL |
		MDIS_ALLCHILDSTYLES;    // allow children to be created invisible
	DWORD dwExStyle = 0;
	
	CLIENTCREATESTRUCT ccs;
	ccs.hWindowMenu = LoadMenu(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_CalibrationCtrlTYPE));
	ccs.idFirstChild = AFX_IDM_FIRST_MDICHILD;
	
	// Create MDICLIENT control with special IDC
	if ((m_hWndMDIClient = ::CreateWindowEx(dwExStyle, _T("mdiclient"), NULL,
		dwStyle, 0, 0, 0, 0, m_hWnd, (HMENU)AFX_IDW_PANE_FIRST,
		AfxGetInstanceHandle(), (LPVOID)&ccs)) == NULL)
	{
		TRACE("Warning: CPaneFrame::OnCreateClient: �������ʧ��");
		return FALSE;
	}
	// Move it to the top of z-order
	::BringWindowToTop(m_hWndMDIClient);
	OnPaint();
	
	return TRUE;
}



//DEL CStatusBar& CPaneFrame::GetStatusBar()
//DEL {
//DEL    return m_wndStatusBar;
//DEL }

/*
BOOL CPaneFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ


	//	CMDIFrameWnd::OnEraseBkgnd(pDC);
	  
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP_BG);

		//��������DC
		CDC dcCompatible;
		dcCompatible.CreateCompatibleDC(pDC);

		//λͼѡ�����DC
		dcCompatible.SelectObject(&bitmap);

		//��ȡλͼ��Ϣ
		BITMAP bmp;
		bitmap.GetBitmap(&bmp);

		//��ʾλͼ
		CRect rect;
		GetClientRect(&rect);
		pDC->SetStretchBltMode(COLORONCOLOR);
		pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &dcCompatible, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		OnPaint();
		return TRUE;
		//return CMDIFrameWnd::OnEraseBkgnd(pDC);

		//return TRUE;
	
		//return TRUE;
	//OnPaint();
	//return CMDIFrameWnd::OnEraseBkgnd(pDC);
}

*/


//void CPaneFrame::OnPaint()
//{
	//CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CMDIFrameWnd::OnPaint()
	/****
	   CDC cdc;

		 if(!m_hWndMDIClient){
		    return ;
		 }
		
		 cdc.m_hDC = ::GetDC( this->m_hWndMDIClient );
		 
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP_BG);

		//��������DC
		CDC dcCompatible;
		dcCompatible.CreateCompatibleDC(&cdc);

		//λͼѡ�����DC
		dcCompatible.SelectObject(&bitmap);

		//��ȡλͼ��Ϣ
		BITMAP bmp;
		bitmap.GetBitmap(&bmp);

		//��ʾλͼ
		CRect rect;
		GetClientRect(&rect);
		cdc.SetStretchBltMode(COLORONCOLOR);
		cdc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcCompatible, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
	*/
//}
