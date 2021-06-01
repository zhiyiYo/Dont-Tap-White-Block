#pragma once
#include <Windows.h>
#include <process.h>
#include <tchar.h>
#include <string>

class Serial
{
public:
    Serial();
    ~Serial();

    bool initPort(UINT port, UINT baudRate, UINT byteSize, BYTE stopBits, BYTE parity);
    bool writeData(char *data, int len);
    bool readOneData(char &data);
    bool openThread();
    bool closeThread();
    std::string readData(bool clr = false, int len = 1);

private:
    bool openPort(TCHAR *portName);
    bool closePort();
    static UINT WINAPI serialThread(void *pParam);

private:
    HANDLE hComm;
    volatile HANDLE hThread;
    bool threadExit_flag;
};