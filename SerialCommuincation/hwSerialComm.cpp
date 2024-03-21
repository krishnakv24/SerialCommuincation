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

BOOL ChwSerialComm::Write(const BYTE* pData, DWORD dwDataSize)
{
    if (!IsOpen())
        return FALSE;

    DWORD dwWritten;
    BOOL bResult = WriteFile(m_hComm, pData, dwDataSize, &dwWritten, NULL);

    return (bResult && dwWritten == dwDataSize);
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
        DWORD bytesRead = 0;
        if (ReadFile(m_hComm, buffer, sizeof(buffer), &bytesRead, NULL))
        {
            // Process read data here
        }
        else
        {
            // Handle read error
        }

        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait_for(lock, std::chrono::milliseconds(10), [this]() { return !m_reading; });
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

