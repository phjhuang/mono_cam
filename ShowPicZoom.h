#pragma once


// ShowPicZoom �Ի���

class ShowPicZoom : public CDialogEx
{
	DECLARE_DYNAMIC(ShowPicZoom)

public:
	ShowPicZoom(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ShowPicZoom();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
//	virtual void OnCancel();
//	virtual void PostNcDestroy();
};
