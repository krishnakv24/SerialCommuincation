#include "pch.h"
#include "hwSerialComm.h"

ChwSerialComm::ChwSerialComm() : m_hComm(INVALID_HANDLE_VALUE), m_reading(false)
{
}

ChwSerialComm::~ChwSerialComm()
{
    StopReading();
    Close();
}

BOOL ChwSerialComm::Open(LPCTSTR lpszPortName, DWORD dwBaudRate, BYTE byteSize, BYTE parity, BYTE stopBits, DWORD dwCommEvents)
{
    m_hComm = CreateFile(lpszPortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (m_hComm == INVALID_HANDLE_VALUE)
        return FALSE;

    if (!ConfigurePort(dwBaudRate, byteSize, parity, stopBits))
    {
        Close();
        return FALSE;
    }

    DCB dcb;
    if (!GetCommState(m_hComm, &dcb))
    {
        Close();
        return FALSE;
    }

    dcb.fBinary = TRUE;
    dcb.fParity = TRUE;
    dcb.ByteSize = byteSize;
    dcb.Parity = parity;
    dcb.StopBits = stopBits;

    if (!SetCommState(m_hComm, &dcb))
    {
        Close();
        return FALSE;
    }

    if (!SetCommMask(m_hComm, dwCommEvents))
    {
        Close();
        return FALSE;
    }

    return TRUE;
}

void ChwSerialComm::Close()
{
    if (m_hComm != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hComm);
        m_hComm = INVALID_HANDLE_VALUE;
    }
}

BOOL ChwSerialComm::IsOpen() const
{
    return (m_hComm != INVALID_HANDLE_VALUE);
}

BOOL ChwSerialComm::Write(const CString& strData)
{
    if (!IsOpen())
        return FALSE;

    CStringA strDataA(strData);
    LPCSTR pszData = strDataA.GetString();

    DWORD dwDataSize = strlen(pszData);

    char* pBuffer = new char[dwDataSize + 2]; 
    if (!pBuffer)
        return FALSE;

    strcpy_s(pBuffer, dwDataSize + 2, pszData);
    pBuffer[dwDataSize] = '\n'; 
    pBuffer[dwDataSize + 1] = '\0'; 

    DWORD dwWritten;
    BOOL bResult = WriteFile(m_hComm, pBuffer, dwDataSize + 1, &dwWritten, NULL); // +1 to include the newline character

    delete[] pBuffer; 

    if (!bResult || dwWritten != dwDataSize + 1) {
        return FALSE;
    }

    return TRUE;
}

void ChwSerialComm::StartReading()
{
    if (!m_reading && IsOpen())
    {
        m_reading = true;
        m_readThread = std::thread(&ChwSerialComm::ReadThreadFunction, this);
    }
}

void ChwSerialComm::StopReading()
{
    if (m_reading)
    {
        m_reading = false;
        if (m_readThread.joinable())
        {
            m_cv.notify_all();
            m_readThread.join();
        }
    }
}

void ChwSerialComm::ReadThreadFunction()
{
    while (m_reading)
    {
        BYTE buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        DWORD bytesRead = 0;
        if (ReadFile(m_hComm, buffer, sizeof(buffer), &bytesRead, NULL))
        {
            if (m_hwdHanlde != NULL && buffer)
            {
                ::PostMessage(m_hwdHanlde, UI_CMD_RECEIVED_SERIAL_DATA, 0, reinterpret_cast<LPARAM>(buffer));
            }
        }
        else
        {
        }

        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait_for(lock, std::chrono::milliseconds(2000), [this]() { return !m_reading; });
    }
}

BOOL ChwSerialComm::ConfigurePort(DWORD dwBaudRate, BYTE byteSize, BYTE parity, BYTE stopBits)
{
    if (!IsOpen())
        return FALSE;

    DCB dcb;
    memset(&dcb, 0, sizeof(DCB));
    dcb.DCBlength = sizeof(DCB);

    if (!GetCommState(m_hComm, &dcb))
        return FALSE;

    dcb.BaudRate = dwBaudRate;
    dcb.ByteSize = byteSize;
    dcb.Parity = parity;
    dcb.StopBits = stopBits;

    if (!SetCommState(m_hComm, &dcb))
        return FALSE;

    COMMTIMEOUTS timeouts;
    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;

    if (!SetCommTimeouts(m_hComm, &timeouts))
        return FALSE;

    return TRUE;
}

