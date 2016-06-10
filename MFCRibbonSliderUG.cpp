#include "StdAfx.h"
#include "MFCRibbonSliderUG.h"

IMPLEMENT_DYNCREATE(CMFCRibbonSliderUG, CMFCRibbonSlider)
	
static const int cySliderHeight = 18;
static const int cxThumbWidth = 10;

static const int nThumbIndex = 0;
static const int nSliderIndex = 1;
static const int nZoomInIndex = 2;
static const int nZoomOutIndex = 3;

CMFCRibbonSliderUG::CMFCRibbonSliderUG():CMFCRibbonSlider(){

	m_pSrcSlider=NULL;
	m_bIsClicked=FALSE;

}

CMFCRibbonSliderUG::CMFCRibbonSliderUG(UINT nID, int nWidth):CMFCRibbonSlider(nID,nWidth){

	 m_pSrcSlider=NULL;
	m_bIsClicked=FALSE;
}


CMFCRibbonSliderUG::~CMFCRibbonSliderUG(){
	
}



void CMFCRibbonSliderUG::CopyFrom(const CMFCRibbonBaseElement& s)  
{  
    ASSERT_VALID(this);  
    CMFCRibbonSlider::CopyFrom(s);  
    CMFCRibbonSlider& src = (CMFCRibbonSlider&) s;  
    m_nZoomIncrement = src.GetZoomIncrement();//复制遗漏的那个成员  
    if(NULL == m_pSrcSlider)  
        m_pSrcSlider=&src;//记录原始数据地地址  
}  



void CMFCRibbonSliderUG::OnLButtonDown(CPoint point)
{
	ASSERT_VALID(this);

	CMFCRibbonBaseElement::OnLButtonDown(point);
	
	if (!IsDisabled())
	{   
		
		m_nPressed = GetHitTest(point);
		Redraw();
	}
}

void CMFCRibbonSliderUG::OnLButtonUp(CPoint point)  
{  
    ASSERT_VALID(this);  
    //printf("LL");
   // if (!m_bIsClicked || NULL == m_pSrcSlider)  
	//if ( NULL == m_pSrcSlider)  
    {  
        CMFCRibbonSlider::OnLButtonUp(point);  
        if (NULL != m_pSrcSlider)  
        {  
            m_pSrcSlider->SetPos(m_nPos);//更新原始数据  

            NotifyCommand();//通知已经更新  
        }  
      //  m_bIsClicked=true;  
    }  
      
}  


void CMFCRibbonSliderUG::OnMouseMove(CPoint point)
{
	ASSERT_VALID(this);

	CMFCRibbonBaseElement::OnMouseMove(point);

	if (IsDisabled())
	{
		return;
	}

	int nHighlightedOld = m_nHighlighted;

	m_nHighlighted = GetHitTest(point);

	if (nHighlightedOld != m_nHighlighted)
	{
		Redraw();
	}

	if (m_bIsPressed)
	{
		if (m_nPressed == nThumbIndex)
		{
			int nPos = GetPosFromPoint(point.x);
			if (nPos != m_nPos)
			{   
				
				SetPos(nPos);
				if(m_pSrcSlider){
				m_pSrcSlider->SetPos(nPos);
				}
				NotifyCommand();
			}
		}
	}
}
