#pragma once

#include "stdafx.h"

#if !defined(SPI_GETFOREGROUNDLOCKTIMEOUT)
#define SPI_GETFOREGROUNDLOCKTIMEOUT          0x2000
#define SPI_SETFOREGROUNDLOCKTIMEOUT          0x2001
#endif

struct FOCUS_INFO {
    HWND m_hActiveWnd;
    HWND m_hFocusWnd;
};

namespace Window {

    BOOL SetAbsoluteForegroundWindow(HWND hWnd);
    void GetFocusInfo(FOCUS_INFO* focusInfo, HWND hForeground = NULL);
    void SetFocusInfo(const FOCUS_INFO* focusInfo);
    void SetFocus(const HWND hActiveWnd, const HWND hFocusWnd);
    CString GetWindowTitle(HWND hWnd);
    void FlatSB_UpdateMetrics(HWND hWnd);
    void GetCaretPos(POINT* CaretPos, FOCUS_INFO* focusInfo = NULL);

} // namespace Window
