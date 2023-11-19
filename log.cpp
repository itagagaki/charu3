#include "stdafx.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "log.h"

namespace{

    CString getDateTimeString()
    {
        time_t long_time;
        time(&long_time);

        struct tm newtime;
        localtime_s(&newtime, &long_time);

        CString strDate;
        strDate.Format(_T("%.4d/%.2d/%.2d %.2d:%.2d:%.2d"),
            newtime.tm_year + 1900,
            newtime.tm_mon + 1,
            newtime.tm_mday,
            newtime.tm_hour,
            newtime.tm_min,
            newtime.tm_sec);

        return strDate;
    }

} // namespace {anonymous}

Logger::Logger() : m_logFilePath(nullptr)
{
}

Logger::~Logger()
{
    if (m_logFilePath) {
        delete[] m_logFilePath;
        m_logFilePath = nullptr;
    }
}

void Logger::SetLogFile(const CString& strPath)
{
    if (m_logFilePath) {
        delete[] m_logFilePath;
        m_logFilePath = nullptr;
    }
    int pathSize = ::WideCharToMultiByte(CP_ACP, 0, strPath, -1, NULL, 0, NULL, NULL);
    m_logFilePath = new char[pathSize + 1];
    if (m_logFilePath) {
        ::WideCharToMultiByte(CP_ACP, 0, strPath, -1, m_logFilePath, pathSize, "", NULL);
        m_logFilePath[pathSize] = NULL;
    }
}

void Logger::WriteLog(const CString& strText, const CString& strSourceFile, int nSourceLine)
{
    CString strWrite;
    strWrite.Format(_T("%s %s [%s:%d]\n"), getDateTimeString().GetString(), strText.GetString(), strSourceFile.GetString(), nSourceLine);
    OutputDebugString(strWrite);

    if (m_logFilePath) {
        FILE* outPut = nullptr;
        if (fopen_s(&outPut, m_logFilePath, "a") == 0) {
            int nDataSize = ::WideCharToMultiByte(CP_ACP, 0, strWrite, -1, NULL, 0, NULL, NULL);
            char* szMbcsBuff = new char[nDataSize + 1];
            if (szMbcsBuff) {
                ::WideCharToMultiByte(CP_ACP, 0, strWrite, -1, szMbcsBuff, nDataSize, "", NULL);
                szMbcsBuff[nDataSize] = NULL;
                fputs(szMbcsBuff, outPut);
                delete[] szMbcsBuff;
            }
            fclose(outPut);
        }
    }
}

Logger gLogger;
