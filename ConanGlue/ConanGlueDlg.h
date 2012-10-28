
// ConanGlueDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CConanGlueDlg dialog
class CConanGlueDlg : public CDialogEx
{
// Construction
public:
	CConanGlueDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CONANGLUE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CEdit SettingsFile;
	CEdit Log;
	afx_msg void OnBnClickedButton2();
	CEdit filedir;
	afx_msg void OnBnClickedButton3();
	CEdit OutputFile;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
  CProgressCtrl ProgressBar;
};
