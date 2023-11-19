#pragma once

#include "stdafx.h"

class Logger {
public:
    Logger();
    ~Logger();
    void SetLogFile(const CString& strPath);
    void WriteLog(const CString& strText, const CString& strSourceFile, int nSourceLine);

private:
    char* m_logFilePath;
};

extern Logger gLogger;

#define LOG(text) gLogger.WriteLog(text, THIS_FILE, __LINE__)
