
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
	DDX_Control(pDX, IDC_ST_COMPORT, m_stComnPrtTitle);
	DDX_Control(pDX, IDC_ST_BD_RATE, m_stBaudRateTitle);
	DDX_Control(pDX, IDC_EDIT_COM_PORT, m_edtCommonPort);
	DDX_Control(pDX, IDC_COMBO1, m_comboBaudrate);
	DDX_Control(pDX, IDC_EDIT2, m_edtSendText);
	DDX_Control(pDX, IDC_BTN_START_COMM2, m_btnSend);
	DDX_Control(pDX, IDC_ST_RECEIVED, m_stReceivedText);
	DDX_Control(pDX, IDC_EDIT_RECEIVED, m_edtReceivedText);
	DDX_Control(pDX, IDC_BTN_START_COMM, m_btnInit);
}

BEGIN_MESSAGE_MAP(CSerialCommuincationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_GETMINMAXINFO()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &CSerialCommuincationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSerialCommuincationDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_START_COMM, &CSerialCommuincationDlg::OnBnClickedBtnStartComm)
	ON_BN_CLICKED(IDC_BTN_SEND, &CSerialCommuincationDlg::OnBnClickedBtnSend)
END_MESSAGE_MAP()


BOOL CSerialCommuincationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText(L"Serial Communication");
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	

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

void CSerialCommuincationDlg::OnClose()
{
	int result = AfxMessageBox(_T("Are you want to Close the Application?"), MB_OKCANCEL | MB_ICONQUESTION);
	if (result == IDOK)
	{
		CDialogEx::OnCancel();
	}
	else if (result == IDCANCEL)
	{
	}
}


void CSerialCommuincationDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CDialogEx::OnGetMinMaxInfo(lpMMI);

	lpMMI->ptMinTrackSize.x = lpMMI->ptMaxTrackSize.x = 502;
	lpMMI->ptMinTrackSize.y = lpMMI->ptMaxTrackSize.y = 275;
}

void CSerialCommuincationDlg::OnBnClickedBtnStartComm()
{
	IntiSerialCommunication();
}

void CSerialCommuincationDlg::IntiSerialCommunication()
{
	if (!m_bConnected)
	{
		CString strPortName = _T("COM4"); 
		DWORD dwBaudRate = CBR_9600; 
		BYTE byteSize = 8; 
		BYTE parity = NOPARITY;
		BYTE stopBits = ONESTOPBIT; 

		if (m_serialComm.Open(strPortName, dwBaudRate, byteSize, parity, stopBits))
		{
			m_serialComm.StartReading();
			m_bConnected = TRUE;
		}
		else
		{
		}
	}
}

void CSerialCommuincationDlg::OnBnClickedBtnSend()
{
}
