

#pragma once
#include "hwSerialComm.h"

class CSerialCommuincationDlg : public CDialogEx
{

public:
	CSerialCommuincationDlg(CWnd* pParent = nullptr);	

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERIALCOMMUINCATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	
protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	void IntiSerialCommunication();

private:
	ChwSerialComm m_serialComm;
	BOOL m_bConnected;
};
