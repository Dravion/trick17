
// trick17Dlg.h : header file
//

#pragma once
#include "afxwin.h"


// Ctrick17Dlg dialog
class Ctrick17Dlg : public CDialogEx
{
// Construction
public:
	Ctrick17Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TRICK17_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support






// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck1();
	CButton ck_explorer_order;


	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

	CButton bt_dns_flush;
	afx_msg void OnBnClickedDnsflush();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButtonaddwinuser();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClicked_bt_dlexec();
	afx_msg void OnCbnSelchangeCombodl();
	CComboBox combobox_utility_download;
	CButton BUTTON_run_dl;
	afx_msg void OnCbnEditupdateCombodl();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButtonrundl();
};
