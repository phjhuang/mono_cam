// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "StdAfx.h"
#include "CalibrationCtrl.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	m_viewNum=0;
}

CChildFrame::~CChildFrame()
{

}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

void CChildFrame::OnSize(UINT nType, int cx, int cy) 
{
	/**** JCz ***/
	// we cannot allow to maximize MDI-way (client of the child covers client of the parent
	// this functionality would require buttons in non-client area of the main window
	// maybe in the future i will implement this


		
	if((nType==SIZE_RESTORED)||(SIZE_MAXIMIZED == nType))//
	{
		
		
		CRect rectParent;
		CWnd *pWnd = GetParent();

		pWnd->GetClientRect(rectParent);

	
		int w=rectParent.Width();
		int h=rectParent.Height();
		
		if(nType==SIZE_RESTORED)
		{
			switch(m_viewNum)
			{
			case 1: break;
			case 2:
				if(nType==SIZE_RESTORED)
				{
					switch(m_ViewPos)
					{
					case 1: rectParent.right=w/2; 
						//	rectParent.bottom=h/2;
						break;
					case 2: rectParent.left=w/2; 
						//	rectParent.bottom=h/2;
						break;
					}
				}
				break;
			case 3:
			case 4:
				if(nType==SIZE_RESTORED)
				{
					switch(m_ViewPos)
					{
					case 1: rectParent.right=w/2; 
						rectParent.bottom=h/2;
						break;
					case 2: rectParent.left=w/2; 
						rectParent.bottom=h/2;
						break;
					case 3: rectParent.right=w/2; 
						rectParent.top=h/2;
						break;
					case 4: rectParent.left=w/2; 
						rectParent.top=h/2;
						break;
					}
				}
				break;
			case 5:
			case 6:
				if(nType==SIZE_RESTORED)
				{
					switch(m_ViewPos)
					{
					case 1: rectParent.right=w/3; 
						rectParent.bottom=h/2;
						break;
					case 2: rectParent.left=w/3; 
						rectParent.right=w*2/3; 
						rectParent.bottom=h/2;
						break;
					case 3: rectParent.left=w*2/3; 
						rectParent.bottom=h/2;
						break;
					case 4: rectParent.right=w/3; 
						rectParent.top=h/2;
						break;
					case 5: rectParent.left=w/3; 
						rectParent.right=w*2/3; 
						rectParent.top=h/2;
						break;
					case 6: rectParent.left=w*2/3; 
						rectParent.top=h/2;
						break;
					}
				}
				break;

			case 7:
			case 8:
				if(nType==SIZE_RESTORED)
				{
					switch(m_ViewPos)
					{
					case 1: rectParent.right=w/4; 
						rectParent.bottom=h/2;
						break;
					case 2: rectParent.left=w/4; 
						rectParent.right=w*2/4; 
						rectParent.bottom=h/2;
						break;
					case 3:  rectParent.left=2*w/4; 
						rectParent.right=w*3/4; 
						rectParent.bottom=h/2;
						break;
					case 4: rectParent.left=w*3/4; 
						rectParent.bottom=h/2;
						break;
					case 5: rectParent.right=w/4; 
						rectParent.top=h/2;
						break;
					case 6: rectParent.left=w/4; 
						rectParent.right=w*2/4; 
						rectParent.top=h/2;
						break;
					case 7: rectParent.left=2*w/4; 
						rectParent.right=w*3/4; 
						rectParent.top=h/2;
						break;
					case 8: rectParent.left=w*3/4; 
						rectParent.top=h/2;
						break;
					}
				}
				break;


			}
			
		}

		SetWindowPos(NULL, rectParent.left, rectParent.top, rectParent.Width(), rectParent.Height(), SWP_FRAMECHANGED | SWP_NOZORDER);
		BringWindowToTop();

		RecalcLayout();
	
		return;
	}
	

	CMDIChildWnd::OnSize(nType, cx, cy);
}

void CChildFrame::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{

	HMENU   hMenu; 
	
	hMenu   =   ::GetSystemMenu(this-> m_hWnd,FALSE); 
    EnableMenuItem(hMenu,SC_CLOSE|MF_BYCOMMAND,   MF_GRAYED); 


	lpwndpos->flags |= SWP_FRAMECHANGED;
	CMDIChildWnd::OnWindowPosChanging(lpwndpos);
}

void CChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIChildWnd::OnSetFocus(pOldWnd);
	
	CWnd *pWnd = GetDlgItem(AFX_IDW_PANE_FIRST);
	ASSERT(pWnd);

	pWnd->SetFocus();
	
}

void CChildFrame::UpdateView(){



		
		CRect rectParent;
		CWnd *pWnd = GetParent();

		pWnd->GetClientRect(rectParent);

	
		int w=rectParent.Width();
		int h=rectParent.Height();
		

			switch(m_viewNum)
			{
			case 1: break;
			case 2:
			
					switch(m_ViewPos)
					{
					case 1: rectParent.right=w/2; 
						//	rectParent.bottom=h/2;
						break;
					case 2: rectParent.left=w/2; 
						//	rectParent.bottom=h/2;
						break;
					}
			
				break;
			case 3:
			case 4:
		
					switch(m_ViewPos)
					{
					case 1: rectParent.right=w/2; 
						rectParent.bottom=h/2;
						break;
					case 2: rectParent.left=w/2; 
						rectParent.bottom=h/2;
						break;
					case 3: rectParent.right=w/2; 
						rectParent.top=h/2;
						break;
					case 4: rectParent.left=w/2; 
						rectParent.top=h/2;
						break;
					}
			
				break;
			case 5:
			case 6:
			
					switch(m_ViewPos)
					{
					case 1: rectParent.right=w/3; 
						rectParent.bottom=h/2;
						break;
					case 2: rectParent.left=w/3; 
						rectParent.right=w*2/3; 
						rectParent.bottom=h/2;
						break;
					case 3: rectParent.left=w*2/3; 
						rectParent.bottom=h/2;
						break;
					case 4: rectParent.right=w/3; 
						rectParent.top=h/2;
						break;
					case 5: rectParent.left=w/3; 
						rectParent.right=w*2/3; 
						rectParent.top=h/2;
						break;
					case 6: rectParent.left=w*2/3; 
						rectParent.top=h/2;
						break;
					}
				
				break;
			}
			
	

		SetWindowPos(NULL, rectParent.left, rectParent.top, rectParent.Width(), rectParent.Height(), SWP_FRAMECHANGED | SWP_NOZORDER);
		BringWindowToTop();

		RecalcLayout();
	
		return;


}
//DEL CStatusBar& CChildFrame::GetStatusBar()
//DEL {
//DEL    return m_wndStatusBar;
//DEL }
