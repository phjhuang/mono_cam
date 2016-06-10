#pragma once
#include "afxribbonslider.h"

class CMFCRibbonSliderUG : public CMFCRibbonSlider{

	DECLARE_DYNCREATE(CMFCRibbonSliderUG)

public:
	
	CMFCRibbonSliderUG();

	CMFCRibbonSliderUG(UINT nID, int nWidth);

	virtual ~CMFCRibbonSliderUG();


protected:
	 virtual void CopyFrom(const CMFCRibbonBaseElement& s);  
    virtual void OnLButtonDown(CPoint point);
	virtual void OnLButtonUp(CPoint point);
	virtual void OnMouseMove(CPoint point);

public:
   CMFCRibbonSlider *m_pSrcSlider;
   BOOL m_bIsClicked;
};

