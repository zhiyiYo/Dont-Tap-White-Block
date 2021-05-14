#include "Serial.h"

SerialCom::SerialCom(void)
{
    hComm = INVALID_HANDLE_VALUE;
    hThread = INVALID_HANDLE_VALUE;
    threadExit_flag = false;
}

SerialCom::~SerialCom(void)
{
    closePort();
    closeThread();
}

bool SerialCom::initPort(UINT port, UINT baudRate, UINT byteSize, BYTE stopBits, BYTE parity)
{
    TCHAR portName[10];
    wsprintf(portName, _T("COM%d"), port);
    if (!openPort(portName))
    {
        return false;
    }

    COMMTIMEOUTS Timeout;
    Timeout.ReadIntervalTimeout = 0;
    Timeout.ReadTotalTimeoutMultiplier = 0;
    Timeout.ReadTotalTimeoutConstant = 0;
    Timeout.WriteTotalTimeoutMultiplier = 0;
    Timeout.WriteTotalTimeoutConstant = 0;
    SetCommTimeouts(hComm, &Timeout);

    DCB dcb;
    if (!GetCommState(hComm, &dcb))
    {
        return false;
    }
    dcb.BaudRate = baudRate;
    dcb.ByteSize = byteSize;
    dcb.StopBits = stopBits;
    dcb.fBinary = true;
    if (parity == 0)
    {
        dcb.fParity = false;
        dcb.Parity = NOPARITY;
    }
    else if (parity == 1)
    {
        dcb.fParity = true;
        dcb.Parity = ODDPARITY;
    }
    else if (parity == 2)
    {
        dcb.fParity = true;
        dcb.Parity = EVENPARITY;
    }
    else
    {
        return false;
    }
    if (!SetCommState(hComm, &dcb))
    {
        return false;
    }

    PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
    return true;
}

bool SerialCom::openPort(TCHAR *portName)
{
    hComm = CreateFile(portName,
                       GENERIC_READ | GENERIC_WRITE,
                       0,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_OVERLAPPED,
                       NULL);
    if (hComm == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    return true;
}

bool SerialCom::closePort()
{
    if (hComm != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hComm);
        hComm = INVALID_HANDLE_VALUE;
        return true;
    }
    else
        return false;
}

bool SerialCom::openThread()
{
    hThread = (HANDLE)_beginthreadex(NULL, 0, serialThread, this, 0, NULL);
    if (!hThread)
    {
        return false;
    }
    return true;
}

bool SerialCom::closeThread()
{
    if (hThread != INVALID_HANDLE_VALUE)
    {
        threadExit_flag = true;
        Sleep(10);
        CloseHandle(hThread);
        hThread = INVALID_HANDLE_VALUE;
        return true;
    }
    return false;
}

UINT WINAPI SerialCom::serialThread(void *pParam)
{
    SerialCom *pSerialCom = (SerialCom *)pParam;
    char buf[512];
    DWORD readNum;
    while (!pSerialCom->threadExit_flag)
    {
        bool readSuc = ReadFile(pSerialCom->hComm, buf, 512, &readNum, NULL);
        if (readSuc && readNum > 0)
        {
            buf[readNum] = '\0';
        }
    }
    return 0;
}

bool SerialCom::writeData(char *data, int len)
{
    if (hComm == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);
    DWORD writeNum;
    bool writeSuc = WriteFile(hComm, data, len, &writeNum, NULL);
    if (!writeSuc)
    {
        PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT);
        return false;
    }
    return true;
}