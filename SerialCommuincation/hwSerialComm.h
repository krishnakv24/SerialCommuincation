#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>

class ChwSerialComm
{
public:
    ChwSerialComm();
    virtual ~ChwSerialComm();

    BOOL Open(LPCTSTR lpszPortName, DWORD dwBaudRate, BYTE byteSize, BYTE parity, BYTE stopBits, DWORD dwCommEvents = EV_RXCHAR | EV_CTS | EV_DSR);
    void Close();
    BOOL IsOpen() const;

    BOOL Write(const BYTE* pData, DWORD dwDataSize);
    void StartReading();
    void StopReading();

protected:
    HANDLE m_hComm;
    std::thread m_readThread;
    bool m_reading;
    std::mutex m_mutex;
    std::condition_variable m_cv;

    BOOL ConfigurePort(DWORD dwBaudRate, BYTE byteSize, BYTE parity, BYTE stopBits);
    void ReadThreadFunction();
};

