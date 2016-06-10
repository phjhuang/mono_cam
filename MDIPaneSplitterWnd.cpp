// MDIPaneSplitterWnd.cpp : implementation file
//

#include "StdAfx.h"
#include "CalibrationCtrl.h"
#include "MDIPaneSplitterWnd.h"
#include "PaneFrame.h"

#include ".\ChildFrm.h"

#include "ImgViewForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDIPaneSplitterWnd

CMDIPaneSplitterWnd::CMDIPaneSplitterWnd() : m_MultiPaneInfo(0, 0)
{
}

CMDIPaneSplitterWnd::~CMDIPaneSplitterWnd()
{
}


BEGIN_MESSAGE_MAP(CMDIPaneSplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CMDIPaneSplitterWnd)
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMDIPaneSplitterWnd message handlers

BOOL CMDIPaneSplitterWnd::CreateMDIPane(int iRow, int iCol, CSize sizeInit, CCreateContext *pContext)
{
	if(m_MultiPaneInfo != CSize(0, 0))
	{
		TRACE("Pane�Ѿ�������!\n"); return FALSE;
	}

	m_MultiPaneInfo = CSize(iRow, iCol);

	return CreateView(0, 1, RUNTIME_CLASS(CPaneFrame), sizeInit, pContext);

}

/**** JCz ***/
// declared as void; it can be changed to BOOL if check is important 
CView* CMDIPaneSplitterWnd::CreateChildren(CDocument *pDoc,int viewnum,int pos,CString framtext  )//,int viewtype ,int showchart 
{
	CCreateContext cntxt;

// 	/huangjin
// 			
// 				if ( showchart ==1)
// 				{
// 					cntxt.m_pNewViewClass = RUNTIME_CLASS(Dialog2);
// 				} 
// 				else
// 				{
		cntxt.m_pNewViewClass = RUNTIME_CLASS(CImgViewForm);
/////	}
	

	cntxt.m_pCurrentDoc = pDoc;

	return CreateMDIPaneChild(&cntxt, TRUE, viewnum, pos,framtext);
}

CView* CMDIPaneSplitterWnd::CreateMDIPaneChild(CCreateContext *pContext, BOOL bSizeParent /*= FALSE*/,int viewnum,int pos,CString framtext)
{
	CMDIFrameWnd *pFrame = static_cast<CMDIFrameWnd *>(GetPane(m_MultiPaneInfo.cx, m_MultiPaneInfo.cy));
	ASSERT(pFrame);

	CRuntimeClass *pClass = RUNTIME_CLASS(CChildFrame);
	CChildFrame *pChildFrame = static_cast<CChildFrame *>(pClass->CreateObject());
	pChildFrame->m_viewNum=viewnum;
	pChildFrame->m_ViewPos=pos;
	ASSERT(pChildFrame);
	//mono
	BOOL bRes = pChildFrame->LoadFrame(IDR_CalibrationCtrlTYPE, WS_BORDER, pFrame, pContext);  //WS_OVERLAPPEDWINDOW
	pChildFrame->InitialUpdateFrame(NULL, TRUE);

	CString msg;
	if(framtext=="")
		msg.Format("����%d",pos);
	else 
	{
		msg.Format("����%d-",pos);   
		msg+=framtext;
	}

	pChildFrame->SetWindowText(msg);

	return pChildFrame->GetActiveView();
}

CView* CMDIPaneSplitterWnd::GetActiveImgView()
{
	CMDIFrameWnd *pFrame = static_cast<CMDIFrameWnd *>(GetPane(m_MultiPaneInfo.cx, m_MultiPaneInfo.cy));
	ASSERT(pFrame);

	CChildFrame *pChild = (CChildFrame*) pFrame->GetActiveFrame();

	CView *pView = pChild->GetActiveView();

	return pView;
}

BOOL CMDIPaneSplitterWnd::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	CMDIFrameWnd *pFrame = static_cast<CMDIFrameWnd *>(GetPane(m_MultiPaneInfo.cx, m_MultiPaneInfo.cy));
	ASSERT(pFrame);

	if(pFrame->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	{
		return TRUE;
	}

	return CSplitterWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


BOOL CMDIPaneSplitterWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return   CSplitterWnd::OnEraseBkgnd(pDC);

	/*
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

	return TRUE;
	*/
	//return 
}


void CMDIPaneSplitterWnd::OnSize(UINT nType, int cx, int cy)
{
	CSplitterWnd::OnSize(nType, cx, cy);



	// TODO: �ڴ˴������Ϣ����������
}
