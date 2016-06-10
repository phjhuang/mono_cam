// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MainFrm.h : CMainFrame 类的接口
//

#pragma once

#include "MFCRibbonSliderUG.h"
#include "MDIPaneSplitterWnd.h"
#include "ImgViewForm.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
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

//protected:  // 控件条嵌入成员

public:
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;

// 生成的消息映射函数
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
	int m_CtrlID;//键盘左右键

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


