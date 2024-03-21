
#include "pch.h"
#include "framework.h"
#include "SerialCommuincation.h"
#include "SerialCommuincationDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()



CSerialCommuincationDlg::CSerialCommuincationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERIALCOMMUINCATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialCommuincationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSerialCommuincationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSerialCommuincationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSerialCommuincationDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


BOOL CSerialCommuincationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText(L"Serial Communication");
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	IntiSerialCommunication();

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);	
	SetIcon(m_hIcon, FALSE);

	return TRUE; 
}

void CSerialCommuincationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CSerialCommuincationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


HCURSOR CSerialCommuincationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSerialCommuincationDlg::OnBnClickedOk()
{
}


void CSerialCommuincationDlg::OnBnClickedCancel()
{
}

void CSerialCommuincationDlg::IntiSerialCommunication()
{
	if (!m_bConnected)
	{
		CString strPortName = _T("COM1"); // Change this to the appropriate port name
		DWORD dwBaudRate = CBR_9600; // Change this to the appropriate baud rate
		BYTE byteSize = 8; // Change this to the appropriate data size
		BYTE parity = NOPARITY; // Change this to the appropriate parity
		BYTE stopBits = ONESTOPBIT; // Change this to the appropriate stop bits

		if (m_serialComm.Open(strPortName, dwBaudRate, byteSize, parity, stopBits))
		{
			m_serialComm.StartReading();
			m_bConnected = TRUE;
			// Connection successful, do additional setup if needed
		}
		else
		{
			// Connection failed, handle error
		}
	}
}
