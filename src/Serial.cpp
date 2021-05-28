#include <iostream>
#include "Serial.h"

/** @brief 构造函数，初始化句柄
 */
Serial::Serial()
{
    hComm = INVALID_HANDLE_VALUE;
    hThread = INVALID_HANDLE_VALUE;
    threadExit_flag = false;
}

/** @brief 析构函数，释放串口/线程句柄
 */
Serial::~Serial()
{
    closePort();
    closeThread();
}

/** @brief 初始化串口，返回是否成功
 * @param port 串口号
 * @param baudRate 波特率
 * @param byteSize 数据位数
 * @param stopBits 停止位数
 * @param parity 校验位，0-无校验；1-奇校验；2-偶校验
 * @return 初始化串口是否成功
 */
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

    switch (parity)
    {
    case 0:
        dcb.fParity = false;
        dcb.Parity = NOPARITY;
        break;
    case 1:
        dcb.fParity = true;
        dcb.Parity = ODDPARITY;
        break;
    case 2:
        dcb.fParity = true;
        dcb.Parity = EVENPARITY;
        break;
    default:
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

/** @brief 打开串口，成功返回true，否则false
 * @param portName 串口名称，例："COM1"
 * @return 打开串口是否成功
 */
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

/** @brief 关闭串口
 * @return 关闭串口是否成功
 */
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

/** @brief 打开线程，开始监听串口是否接受到数据，并打印到终端
 * @return 打开线程是否成功
 */
bool Serial::openThread()
{
    hThread = (HANDLE)_beginthreadex(NULL, 0, serialThread, this, 0, NULL);
    if (!hThread)
    {
        return false;
    }
    return true;
}

/** @brief 关闭线程
 * @return 关闭线程是否成功
 */
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

/** @brief 线程函数，监听串口信息并打印出
 * @param pParam
 */
UINT WINAPI Serial::serialThread(void *pParam)
{
    Serial *pSerial = (Serial *)pParam;

    while (!pSerial->threadExit_flag)
    {
        UINT dataInCOM = 0;
        DWORD error;
        COMSTAT comstat;
        memset(&comstat, 0, sizeof(COMSTAT));
        if (ClearCommError(pSerial->hComm, &error, &comstat))
            dataInCOM = comstat.cbInQue;

        // std::cout << "test:thread " << dataInCOM << std::endl;
        std::string receive = "";
        while (dataInCOM > 0)
        {
            char dataRead;
            if (pSerial->readOneData(dataRead))
            {
                receive += dataRead;
                dataInCOM--;
            }
            if (dataInCOM == 0)
            {
                std::cout << "Received data in COM: " << receive << std::endl;
            }
        }
    }
    return 0;
}

/** @brief 向串口发送数据
 * @param data 需要发送数据-字符数组
 * @param len 字符数组长度
 * @return 发送成功返回true
 */
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

/** @brief 接受一个串口数据
 * @param data 接收的数据
 * @return 接受成功返回true
 */
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
        std::cout << "Read one data error!" << std::endl;
        return false;
    }
    return true;
}