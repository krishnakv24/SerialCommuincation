

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

	void OnClose();

	void IntiSerialCommunication();
	void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
private:
	ChwSerialComm m_serialComm;
	BOOL m_bConnected;
public:
	CStatic m_stComnPrtTitle;
	CStatic m_stBaudRateTitle;
	CEdit m_edtCommonPort;
	CComboBox m_comboBaudrate;
	CEdit m_edtSendText;
	CButton m_btnSend;
	CStatic m_stReceivedText;
	CEdit m_edtReceivedText;
	afx_msg void OnBnClickedBtnStartComm();
	CButton m_btnInit;
	afx_msg void OnBnClickedBtnSend();
};
