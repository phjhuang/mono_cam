// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once

#include "MFCRibbonSliderUG.h"
#include "MDIPaneSplitterWnd.h"
#include "ImgViewForm.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:
	//CSplitterWnd m_wndSplitter1;

	CMFCRibbonEdit* m_EditCtrlPos1;
	CMFCRibbonEdit* m_EditCtrlPos2;

	CMFCRibbonSlider* m_SliderCtrlPos1;
	CMFCRibbonSlider* m_SliderCtrlPos2;

	//CMFCSpinButtonCtrl* m_SpinCalibNum;//


	BOOL  m_ClibIsOpen;
	BOOL  m_MsmtIsOpen;
 
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//protected:  // �ؼ���Ƕ���Ա

public:
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg LRESULT OnChangeRibbonCategory(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
//	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

public:
	void OnSeginalwindows();
	void OnFourwindows(int num);
	void OnSixwindows(int num);
	void OnTwowindows();
	void SetActive(int index);
	void OnEightwindows(int num);
    CView* GetRunningView();
	CView* ViewAdd(int viewtype,CString str);
	void SetViewNum(int num);
	void UpdateShowView();
	void SetViewCtrlMsg(HWND hWnd,int nMsg);

	void SetMsgHwnd(HWND phwnd, int nmsg,int ctrlid);
//private:    //huangjin
	CMDIPaneSplitterWnd m_wndSplitter;
public:
	int m_ViewNum;
	CImgViewForm *m_ShowView[DEVICEMAX];
	HWND m_hViewWnd;
	int m_ViewMsgID;
	CString m_Title[DEVICEMAX];

	HWND m_MsgHWND;
	int m_MsgID;
	int m_CtrlID;//�������Ҽ�

	CImgViewForm *p_MyGraphForm;    //////huangjin

	CImgViewForm *  MyGraphForm(void);


	afx_msg void OnOpenimage();
	afx_msg void OnCloseclib();
	afx_msg void OnOpendatfile();
	afx_msg void OnClosedatfile();

	afx_msg void OnCalibration();    //h
	afx_msg void OnSaveCalibrationReult();    //h
	afx_msg void OnStartMeasure();            //huangjin



	afx_msg void OnPos1();
	afx_msg void OnPos2();
	afx_msg void OnSliderpos1();
	afx_msg void OnSliderpos2();
	afx_msg void OnFristframe1();
	afx_msg void OnUp1();
	afx_msg void OnDown1();
	afx_msg void OnEndfram1();
	afx_msg void OnFristframe2();
	afx_msg void OnUp2();
	afx_msg void OnDown2();
	afx_msg void OnEndfram2();
	afx_msg void OnUpdateOpendatfile(CCmdUI *pCmdUI);
	afx_msg void OnUpdateClosedatfile(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFristframe1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateUp1(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePos1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSliderpos1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDown1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEndfram1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFristframe2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateUp2(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePos2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSliderpos2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDown2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEndfram2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOpenimage(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCloseclib(CCmdUI *pCmdUI);





	afx_msg void OnDataProcessing();
	afx_msg void OnShowGraph();
	afx_msg void OnXYZTransform();
	afx_msg void OnSaveGraph();
	afx_msg void OnShowMark();
	// set the left dialog width
	void SetFramSzie(int width);
 
};


