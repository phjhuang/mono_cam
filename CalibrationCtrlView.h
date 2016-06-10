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

// CalibrationCtrlView.h : CCalibrationCtrlView 类的接口
//

#pragma once


#include "resource.h"
#include "MoreImageCtrl.h"
#include "afxwin.h"

#include "TipDlg.h"
#include "Dialog2.h"
#include "ShowPicZoom.h"
//#include "..\\CalibrationCtrl\\excel_libxl\\libxl.h"
//#pragma comment(lib,"..\\CalibrationCtrl\\excel_libxl\\libxl.lib")  ////excel save

#include <opencv\cv.h>
//#include <omp.h>
#include <algorithm>

#include "..\\CalibrationCtrl\\codedmark\\_codedTarget.h"
#include "..\\CalibrationCtrl\\jianginclude\\CalculationProcedure.h"
#include "..\\CalibrationCtrl\\MiniCSV\\minicsv.h"
#include "..\\CalibrationCtrl\\MiniCSV\\CSVOperator.h"


#include "..\\CalibrationCtrl\\ChartCtrl\\ChartLineSerie.h"
#include "..\\CalibrationCtrl\\ChartCtrl\\ChartCtrl.h"
#include "..\\CalibrationCtrl\\ChartCtrl\\ChartTitle.h"
#include "..\\CalibrationCtrl\\ChartCtrl\\ChartLabel.h"
#include "..\\CalibrationCtrl\\ChartCtrl\\ChartAxis.h"
#include "..\\CalibrationCtrl\\ChartCtrl\\ChartString.h"
#include "..\\CalibrationCtrl\\ChartCtrl\\ChartAxisLabel.h"

//#include "..\\CalibrationCtrl\\jianginclude\\CommonProc.h"


//#include "..\\CalibrationCtrl\\graph\\Pegrpapi.h"
//#pragma comment(lib, "..\\CalibrationCtrl\\graph\\PEGRP64E.lib")


#define WM_TAB_MSG 2
#define WM_CTRLKEYDOWN_MSG WM_USER + 448
#define WM_IMAGEPOINT_MSG WM_USER + 449

class CCalibrationCtrlView : public CFormView
{
protected: // 仅从序列化创建
    CCalibrationCtrlView();
    DECLARE_DYNCREATE(CCalibrationCtrlView)

public:
    enum { IDD = IDD_CALIBRATIONCTRL_FORM };

// 特性
public:
    CCalibrationCtrlDoc* GetDocument() const;
// 操作
public:
    int m_OldTab;
    CMoreImageCtrl m_MoreImageCtrl;

    // CImgPreview m_SmallPreview;
// 重写
public:
//    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual void OnInitialUpdate(); // 构造后第一次调用
// 实现
public:
    virtual ~CCalibrationCtrlView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
    afx_msg void OnFilePrintPreview();
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg LRESULT OnTabMsg(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()

    void TabView(int ntab,BOOL Change);

    void OnOpenimage();
    void OnCloseclib();

    void OnCalibration();  //h
    void OnSaveCalibrationResult();   //h

    void OnOpendatfile();
    void OnClosedatfile();
    void ShowPos1();
    void ShowPos2();
    void SetPos1();
    void SetPos2();
    void SetSliderPos1();
    void SetSliderPos2();

    void OnFristframe1();
    void OnUp1();
    void OnDown1();
    void OnEndfram1();

    void OnFristframe2();
    void OnUp2();
    void OnDown2();
    void OnEndfram2();
public:
    afx_msg void OnBnClickedGetonecampath();
    afx_msg void OnBnClickedGettwocampath();
    afx_msg void OnBnClickedGetsddatapath();
    afx_msg LRESULT OnKeyDownMsg(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnImagePointMsg(WPARAM wParam,LPARAM lParam);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

    afx_msg void OnEdit_beginpos();
    afx_msg void OnEdit_endpos();
    afx_msg void OnEdit_offset();
    afx_msg void OnDefaultSetting();

    afx_msg void OnUpdateDefaultsetting(CCmdUI *pCmdUI);
    afx_msg void OnUpdateSaveclbt(CCmdUI *pCmdUI);
    afx_msg void OnUpdateEdit5(CCmdUI *pCmdUI);
    afx_msg void OnUpdateEdit6(CCmdUI *pCmdUI);
    afx_msg void OnUpdateEdit7(CCmdUI *pCmdUI);

    //afx_msg void OnEdit2();
    //afx_msg void OnUpdateEdit2(CCmdUI *pCmdUI);
    afx_msg void OnComboPt();
    afx_msg void OnComboDisplaywhat();
    afx_msg void OnEditWindow();

    afx_msg void OnDataProcessing();
    afx_msg void OnXYZTransform();
    afx_msg void OnSaveGraph();

    afx_msg void OnEditCalibNum();
	afx_msg void OnEditDia();  ////mono
public:
	//IplImage *m_grayimg[2];
	//	int m_colortype;

    int m_tab;
    CEdit m_Edit5;    
	//mono
	CFont m_Font;


    int m_CalibrationPointNum;// 标志点数量
   // MonoCamera m_Cam[2];   //h
    H_Pt3D  m_OffsetPt;     
    BOOL m_IsRightHanded;  
    BOOL m_ChangeZ;         //左z负号

    CodedTarget m_Ct[2];
    CString m_CamFileName[2];  //标定图像路径

  //  BinoCamera m_BinoCam;     //cal xyz
    CString m_XYZFileName;   
    CString m_CalibrationResultFile;
    CString m_RecordFileName;  

    CString m_DataPath;  // 数据路径
    ////////////CString m_DataFileName; //上面的数据文件名不含路径

    bool m_displaymark  ;  //是否显示
    bool m_UseDefaultSetting;  //automatically
    bool m_SaveIsOK;           //

    long m_beginpos;         
    long m_endpos;         
    long m_offset;          //偏移

    IplImage* m_img[2];

    struct CalcRecord
    {
        //CString time;                                            //记录 此一帧 时间，备用
        std::vector<int>  Index;             
        std::vector<bool> PtValid;                               //丢失
        std::vector < H_Pt2D > ImgPt0;                        
        std::vector < H_Pt2D > ImgPt1;                        
        std::vector < H_Pt3D > ResultPt;                       
    } m_Record;



    std::vector <std::vector <H_Pt3D> > m_AllRecord;

    CChartCtrl *m_ChartCtrl;
    DataProcessing m_DataProcessing;    
    CString m_table_title;
    CString m_Y_title;
    int m_ordernum;
    //////int m_XYZ;//int ordernum, int _XYZ第几个点，xyz坐标哪个
    int m_ShowItem;    //choose  speed a ?
    int m_window; 
    std::vector<int> pt_index;    //order num
    std::vector<double>  m_graphdata;
    std::vector<double> vpos;
    CCSVOperator m_csv;
    int m_begin,m_end ;

    CTipDlg m_tipdlg;
    CWinThread* m_pMyThread1;   //for show measure progress
    CWinThread* m_pMyThread2;   //for show measure progress
    bool m_abortflag ;  //measure abort flag


    Dialog2  m_dia2;
    bool m_GraphFromIsShow;
    CImgViewForm *p_MyGraphView;
	
    //////////////
    ShowPicZoom   *p_ShowPicZoomDlg;
    CMoreImageCtrl m_MorePicZoom;

    /////////////
    struct
    {
        CString Cam1Path;
        CString Cam2Path;
		CString _XYZ;  
        CString CalibResultPath;

        CString Dat1Path;
        CString Dat2Path;

        CString ResultFilesPath;
		CString BackupRecordPath;

    } m_hpath;
	
///////////////////

    int GetCalibrationImageMark(void);// 标定时获得单个相机
    bool ReadXYZFromFile();//  
    bool ReadCalibrationResult(void);    
    void ShowCalibrationResult(void);    
    bool GetBinoMark(long pos, long offset);     // 以起始帧为准
    void OnStartMeasure(void);
    void CalcPerFrame();
    // 每一帧的三维坐标计算结果
    //void ExcelRecordPerFrame(libxl::Sheet* sheet,int row,int framepos,int pointnum); ///excel的行数，帧号，结果点数目

    void OnOpenXYZData(void);
    void SaveMeasureRecord(void);
    void DisplayGraph(int length, double* XValues, double* YValues,CString table_title, CString Y_title);
    void CalcData();
	
    void OnShowMark(void);
    bool m_MarkOfViewForm[2];
    void MarkInvisable(int i );    
    bool UnicodeFile2Ansi(std::string file, std::string savefile);
    //afx_msg void OnButton4();     
    BOOL CheckRightHanded(void);    
    void GetH_Path(void);


	///////////huj fff
	int hujfff1[2048];

	/////mono
	//mono
	std::vector < H_Pt2D >  h_FirstImgPt;
	std::vector<double> h_scale;
	float h_Diameter;
  
	int h_end; //last
};

volatile static long  _showpos;
UINT MfcThreadProc1   (LPVOID lpParam);
UINT MfcThreadProc2   (LPVOID lpParam);

#ifndef _DEBUG  // CalibrationCtrlView.cpp 中的调试版本
inline CCalibrationCtrlDoc* CCalibrationCtrlView::GetDocument() const
{
    return reinterpret_cast<CCalibrationCtrlDoc*>(m_pDocument);
}
#endif

