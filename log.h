#pragma once

#include "stdafx.h"

#include <stdarg.h>

class Logger {
public:
    Logger();
    ~Logger();
    void SetLogFile(const CString& strPath);
    void WriteLog(const CString& strSourceFile, int nSourceLine, const CString strFormat, ...);

private:
    char* m_logFilePath;
};

extern Logger gLogger;

#define LOG(...) gLogger.WriteLog(THIS_FILE, __LINE__, __VA_ARGS__)
