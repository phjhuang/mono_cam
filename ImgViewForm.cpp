// ImgViewForm.cpp : ʵ���ļ�
//

#include "StdAfx.h"
#include "CalibrationCtrl.h"
#include "ImgViewForm.h"


// CImgViewForm

IMPLEMENT_DYNCREATE(CImgViewForm, CFormView)

CImgViewForm::CImgViewForm()
    : CFormView(CImgViewForm::IDD)
{
    m_nIndex=0;
    m_nCampos=0;
    m_hWnd=NULL;

    m_IsCheckPoint=FALSE;

    m_PonitCount=0;

    m_hpointWnd=NULL;
    m_PointMsgID=0;
}

CImgViewForm::~CImgViewForm()
{
}

void CImgViewForm::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImgViewForm, CFormView)
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    ON_WM_PAINT()
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CImgViewForm ���

#ifdef _DEBUG
void CImgViewForm::AssertValid() const
{
    CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CImgViewForm::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CImgViewForm ��Ϣ�������
BOOL CImgViewForm::OnEraseBkgnd(CDC* pDC)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    CFormView::OnEraseBkgnd(pDC);
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

    //return CFormView::OnEraseBkgnd(pDC);
}

HDC CImgViewForm::GetShowWndInfo(CRect& Rect)
{


    //   SetWindowText("�ɼ�");
    //  DEBUG_INFO1("�ͻ���:%d",m_nIndex);
    GetClientRect(&Rect);
    return this->GetDC()->GetSafeHdc();

}

void CImgViewForm::OnSize(UINT nType, int cx, int cy)
{
    CFormView::OnSize(nType, cx, cy);
    //DEBUG_INFO1("ONSize:%d",2);
    //DEBUG_INFO1("index:%d",m_nIndex);
//	DEBUG_INFO1("ncampos:%d",m_nCampos);
//	UpdateShowInfo();
    ::PostMessage(m_hWnd,m_MsgID,m_nIndex,m_nCampos);
    // TODO: �ڴ˴������Ϣ����������
}

void CImgViewForm::SetHwnd(HWND hWnd,int nMsg)
{
    if(!m_hWnd)
        {
            m_hWnd = hWnd;
            m_MsgID=nMsg;
        }

}

void CImgViewForm::UpdateShowInfo()
{
    Invalidate();
    UpdateWindow();
}


int CImgViewForm::CloseCtrl()
{
    GetParent()->SendMessage(WM_CLOSE);
    return 0;
}

void CImgViewForm:: UpdatePaint()
{

    //UpdateWindow();
    OnPaint();
}




void CImgViewForm::OnDraw(CDC* /*pDC*/)
{
    /*if(&m_ImgPreview)
    {
      m_ImgPreview.UpdateDraw();
    }
    */
	 
}


void CImgViewForm::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: �ڴ˴������Ϣ����������
    // ��Ϊ��ͼ��Ϣ���� CFormView::OnPaint()

    if(&m_ImgPreview)
        {
            m_ImgPreview.UpdateDraw();
        }
}


void CImgViewForm::OnLButtonDown(UINT nFlags, CPoint point)
{

    //CPoint Svcpoint;

    /* if(m_ImgPreview.GetImagePos(point,m_Svcpoint))
     {

    	 CClientDC dc(this);
    	 CPen pen(PS_SOLID,1,RGB(255,0,0));
    	 dc.SelectObject(&pen);

    	 CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
    	 dc.SelectObject(pBrush);

    	 dc.Ellipse(point.x-5,point.y-5,point.x+5,point.y+5);

    	 m_PonitCount++;

    	 CString msg="";
    	 msg.Format("%d",m_PonitCount);
    	 dc.TextOut(point.x+6,point.y+6,msg,msg.GetLength());

    	 if(m_hpointWnd)
    	   ::PostMessage(m_hpointWnd,m_PointMsgID,m_nCampos*100+m_PonitCount,(LPARAM)&m_Svcpoint);
     }*/

	//huangjin64
	if(m_hpointWnd)
		::PostMessage(m_hpointWnd,m_PointMsgID,m_nCampos*10000+point.x,point.y);



    CFormView::OnLButtonDown(nFlags, point);
}

void CImgViewForm::SetPointHwnd(HWND hWnd,int nMsg)
{
    m_hpointWnd=hWnd;
    m_PointMsgID=nMsg;
}
