#pragma once


// Dialog2 �Ի���

class Dialog2 : public CDialogEx
{
	DECLARE_DYNAMIC(Dialog2)

public:
	Dialog2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Dialog2();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


    static UINT  TimeClose(LPVOID lpParam);
	
};

