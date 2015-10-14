#pragma once


// window_supdate_dlg dialog

class window_supdate_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(window_supdate_dlg)

public:
	window_supdate_dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~window_supdate_dlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
