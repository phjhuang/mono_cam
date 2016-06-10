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

// CalibrationCtrlView.h : CCalibrationCtrlView ��Ľӿ�
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
protected: // �������л�����
    CCalibrationCtrlView();
    DECLARE_DYNCREATE(CCalibrationCtrlView)

public:
    enum { IDD = IDD_CALIBRATIONCTRL_FORM };

// ����
public:
    CCalibrationCtrlDoc* GetDocument() const;
// ����
public:
    int m_OldTab;
    CMoreImageCtrl m_MoreImageCtrl;

    // CImgPreview m_SmallPreview;
// ��д
public:
//    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual void OnInitialUpdate(); // ������һ�ε���
// ʵ��
public:
    virtual ~CCalibrationCtrlView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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


    int m_CalibrationPointNum;// ��־������
   // MonoCamera m_Cam[2];   //h
    H_Pt3D  m_OffsetPt;     
    BOOL m_IsRightHanded;  
    BOOL m_ChangeZ;         //��z����

    CodedTarget m_Ct[2];
    CString m_CamFileName[2];  //�궨ͼ��·��

  //  BinoCamera m_BinoCam;     //cal xyz
    CString m_XYZFileName;   
    CString m_CalibrationResultFile;
    CString m_RecordFileName;  

    CString m_DataPath;  // ����·��
    ////////////CString m_DataFileName; //����������ļ�������·��

    bool m_displaymark  ;  //�Ƿ���ʾ
    bool m_UseDefaultSetting;  //automatically
    bool m_SaveIsOK;           //

    long m_beginpos;         
    long m_endpos;         
    long m_offset;          //ƫ��

    IplImage* m_img[2];

    struct CalcRecord
    {
        //CString time;                                            //��¼ ��һ֡ ʱ�䣬����
        std::vector<int>  Index;             
        std::vector<bool> PtValid;                               //��ʧ
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
    //////int m_XYZ;//int ordernum, int _XYZ�ڼ����㣬xyz�����ĸ�
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

    int GetCalibrationImageMark(void);// �궨ʱ��õ������
    bool ReadXYZFromFile();//  
    bool ReadCalibrationResult(void);    
    void ShowCalibrationResult(void);    
    bool GetBinoMark(long pos, long offset);     // ����ʼ֡Ϊ׼
    void OnStartMeasure(void);
    void CalcPerFrame();
    // ÿһ֡����ά���������
    //void ExcelRecordPerFrame(libxl::Sheet* sheet,int row,int framepos,int pointnum); ///excel��������֡�ţ��������Ŀ

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

#ifndef _DEBUG  // CalibrationCtrlView.cpp �еĵ��԰汾
inline CCalibrationCtrlDoc* CCalibrationCtrlView::GetDocument() const
{
    return reinterpret_cast<CCalibrationCtrlDoc*>(m_pDocument);
}
#endif

