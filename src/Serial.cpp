#include <iostream>
#include "Serial.h"
using namespace std;

Serial::Serial(void)
{
    hComm = INVALID_HANDLE_VALUE;
    hThread = INVALID_HANDLE_VALUE;
    threadExit_flag = false;
}

Serial::~Serial(void)
{
    closePort();
    closeThread();
}

bool Serial::initPort(UINT port, UINT baudRate, UINT byteSize, BYTE stopBits, BYTE parity)
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
    Timeout.ReadTotalTimeoutConstant = 2000;
    Timeout.WriteTotalTimeoutMultiplier = 0;
    Timeout.WriteTotalTimeoutConstant = 2000;
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
    SetCommMask(hComm, EV_RXCHAR);
    SetupComm(hComm, 1024, 1024);

    PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
    return true;
}

bool Serial::openPort(TCHAR *portName)
{
    hComm = CreateFile(portName,
                       GENERIC_READ | GENERIC_WRITE,
                       0,
                       NULL,
                       OPEN_EXISTING,
                       0,
                       NULL);
    if (hComm == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    return true;
}

bool Serial::closePort()
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

bool Serial::openThread()
{
    hThread = (HANDLE)_beginthreadex(NULL, 0, serialThread, this, 0, NULL);
    if (!hThread)
    {
        return false;
    }
    return true;
}

bool Serial::closeThread()
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

UINT WINAPI Serial::serialThread(void *pParam)
{
    Serial *pSerial = (Serial *)pParam;

    char buf[50];
    DWORD readNum;
    while (!pSerial->threadExit_flag)
    {
        UINT dataInCOM = 0;
        DWORD error;
        COMSTAT comstat;
        memset(&comstat, 0, sizeof(COMSTAT));
        if (ClearCommError(pSerial->hComm, &error, &comstat))
            dataInCOM = comstat.cbInQue;

        //cout << "test:thread " << dataInCOM << endl;
        string receive = "";
        while (dataInCOM > 0)
        {
            char dataRead;
            if (pSerial->readOneData(dataRead) == true)
            {
                receive += dataRead;
                //cout << "One data: " << dataRead << endl;
            }
            //cout << "The number of data in COM: " << dataInCOM << endl;
            dataInCOM--;
            if (dataInCOM == 0)
            {
                cout << "Received data in COM: " << receive << endl;
            }
        }
    }
    return 0;
}

bool Serial::writeData(char *data, int len)
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

bool Serial::readOneData(char &data)
{
    bool readSuc = false;
    DWORD readNum = 0;
    if (hComm == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    readSuc = ReadFile(hComm, &data, 1, &readNum, NULL);
    if (!readSuc)
    {
        PurgeComm(hComm, PURGE_RXCLEAR | PURGE_RXABORT);
        cout << "Read one data error!" << endl;
        return false;
    }
    return true;
}