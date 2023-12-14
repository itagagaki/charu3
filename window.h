#pragma once

#include "stdafx.h"

struct FOCUS_INFO {
    HWND m_hActiveWnd;
    HWND m_hFocusWnd;
};

namespace Window {

    BOOL SetAbsoluteForegroundWindow(HWND hWnd);
    void GetFocusInfo(FOCUS_INFO* focusInfo);
    void SetFocusInfo(const FOCUS_INFO* focusInfo);
    void SetFocus(const HWND hActiveWnd, const HWND hFocusWnd);
    CString GetWindowTitle(HWND hWnd);
    void FlatSB_UpdateMetrics(HWND hWnd);
    void GetCaretPos(POINT* CaretPos, FOCUS_INFO* focusInfo = NULL);

} // namespace Window
