#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "window.h"

#if !defined(SPI_GETFOREGROUNDLOCKTIMEOUT)
#define SPI_GETFOREGROUNDLOCKTIMEOUT          0x2000
#define SPI_SETFOREGROUNDLOCKTIMEOUT          0x2001
#endif

//---------------------------------------------------
//関数名	SetAbsoluteForegroundWindow(HWND hWnd)
//機能		ウィンドウを強制アクティブ
//---------------------------------------------------
BOOL Window::SetAbsoluteForegroundWindow(HWND hWnd)
{
    int nTargetID = NULL, nForegroundID = NULL;
    UINT nTimeout = 0;
    BOOL isRet = false;

    if (GetForegroundWindow() && hWnd) {
        nForegroundID = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
        nTargetID = GetWindowThreadProcessId(hWnd, NULL);
        AttachThreadInput(nTargetID, nForegroundID, TRUE);

        SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &nTimeout, 0);
        SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (PVOID)0, 0);

        isRet = SetForegroundWindow(hWnd);

        SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (PVOID)nTimeout, 0);

        AttachThreadInput(nTargetID, nForegroundID, FALSE);
        isRet = true;
    }
    return isRet;
}

//---------------------------------------------------
//関数名	setFocus(const HWND hActiveWnd, const HWND hFocusWnd)
//機能		フォーカスセット
//---------------------------------------------------
void Window::SetFocus(const HWND hActiveWnd, const HWND hFocusWnd)
{
    if (hFocusWnd && hActiveWnd) {
        DWORD dwID = GetWindowThreadProcessId(hActiveWnd, NULL);
        if (dwID) {
            AttachThreadInput(dwID, GetCurrentThreadId(), TRUE);
            if (GetFocus() != hFocusWnd) {
                HWND nRet = ::SetFocus(hFocusWnd);
            }
            dwID = GetWindowThreadProcessId(hActiveWnd, NULL);
            if (dwID) AttachThreadInput(dwID, GetCurrentThreadId(), FALSE);
        }
    }
}

//---------------------------------------------------
//関数名	setFocusInfo(const FOCUS_INFO *focusInfo)
//機能		フォーカスセット
//---------------------------------------------------
void Window::SetFocusInfo(const FOCUS_INFO* focusInfo)
{
    //アクティブウィンドウの設定
    if (focusInfo->m_hActiveWnd) {
        SetAbsoluteForegroundWindow(focusInfo->m_hActiveWnd);
        if (focusInfo->m_hActiveWnd) ::SendMessage(focusInfo->m_hActiveWnd, WM_NCACTIVATE, (WPARAM)TRUE, 0);
        //フォーカスの設定
        SetFocus(focusInfo->m_hActiveWnd, focusInfo->m_hFocusWnd);
    }
}

//---------------------------------------------------
//関数名	getFocusInfo(FOCUS_INFO *focusInfo)
//機能		フォーカス情報取得
//---------------------------------------------------
void Window::GetFocusInfo(FOCUS_INFO* focusInfo, HWND hForeground)
{
    focusInfo->m_hActiveWnd = hForeground ? hForeground : focusInfo->m_hActiveWnd = GetForegroundWindow();
    DWORD dwID = GetWindowThreadProcessId(focusInfo->m_hActiveWnd, NULL);
    if (dwID) {
        AttachThreadInput(dwID, GetCurrentThreadId(), TRUE);
        focusInfo->m_hFocusWnd = GetFocus();
        dwID = GetWindowThreadProcessId(focusInfo->m_hActiveWnd, NULL);
        if (dwID) {
            AttachThreadInput(dwID, GetCurrentThreadId(), FALSE);
        }
    }
    if (nullptr == focusInfo->m_hFocusWnd) {
        focusInfo->m_hFocusWnd = focusInfo->m_hActiveWnd;
    }
}

//---------------------------------------------------
//関数名	getWindowTitle(HWND hWnd)
//機能		ウィンドウタイトルを取得
//---------------------------------------------------
CString Window::GetWindowTitle(HWND hWnd)
{
    TCHAR strWindowName[1024] = {};
    *strWindowName = (TCHAR)NULL;
    CString strWinName;
    bool isRet = false;

    strWinName = "";
    if (!GetWindowText(hWnd, strWindowName, _countof(strWindowName))) return strWinName;//キャプションを取得

    strWinName = strWindowName;
    return 	strWinName;
}

void Window::FlatSB_UpdateMetrics(HWND hWnd)
{
    FlatSB_SetScrollProp(hWnd, WSB_PROP_CXVSCROLL, GetSystemMetrics(SM_CXVSCROLL), TRUE);
    FlatSB_SetScrollProp(hWnd, WSB_PROP_CXHSCROLL, GetSystemMetrics(SM_CXHSCROLL), TRUE);
    FlatSB_SetScrollProp(hWnd, WSB_PROP_CYVSCROLL, GetSystemMetrics(SM_CYVSCROLL), TRUE);
    FlatSB_SetScrollProp(hWnd, WSB_PROP_CYHSCROLL, GetSystemMetrics(SM_CYHSCROLL), TRUE);
    FlatSB_SetScrollProp(hWnd, WSB_PROP_CXHTHUMB, GetSystemMetrics(SM_CXHTHUMB), TRUE);
    FlatSB_SetScrollProp(hWnd, WSB_PROP_CYVTHUMB, GetSystemMetrics(SM_CYVTHUMB), TRUE);
}

//---------------------------------------------------
//関数名	GetCaretPos(POINT *CaretPos)
//機能		アクティブなアプリのキャレット位置を取得
//---------------------------------------------------
void Window::GetCaretPos(POINT* CaretPos, FOCUS_INFO* focusInfo)
{
    //キャレット位置
    DWORD dwFrom, dwTo;
    HWND hActiveWnd, hForground;

    if (focusInfo) {
        focusInfo->m_hActiveWnd = NULL;
        focusInfo->m_hFocusWnd = NULL;
    }

    dwFrom = ::GetCurrentThreadId();//アクティブなスレッドのIDを取得
    hForground = ::GetForegroundWindow();
    if (focusInfo) focusInfo->m_hActiveWnd = hForground;
    if (hForground != NULL)
        dwTo = ::GetWindowThreadProcessId(hForground, NULL);//プロセスIDを取得
    else
        dwTo = NULL;


    if (dwFrom && dwTo) {
        BOOL isGetGuiInfo = false;
        GUITHREADINFO gui;
        ZeroMemory(&gui, sizeof(gui));
        gui.cbSize = sizeof(gui);
        isGetGuiInfo = GetGUIThreadInfo(dwTo, &gui);
        if (!isGetGuiInfo) {
            if (::AttachThreadInput(dwFrom, dwTo, TRUE)) {//アタッチ
                ::GetCaretPos(CaretPos);//キャレット座標を取得
                hActiveWnd = ::GetFocus();//フォーカスウィンドウハンドルを取得
                if (focusInfo) focusInfo->m_hFocusWnd = hActiveWnd;
                ClientToScreen(hActiveWnd, CaretPos);
                ::AttachThreadInput(dwFrom, dwTo, FALSE);//デタッチ
            }
        }
        else {
            hActiveWnd = gui.hwndFocus;//フォーカスウィンドウハンドルを取得
            if (focusInfo) focusInfo->m_hFocusWnd = hActiveWnd;
            CaretPos->x = gui.rcCaret.left;
            CaretPos->y = gui.rcCaret.top;

            ClientToScreen(hActiveWnd, CaretPos);
            CaretPos->y += gui.rcCaret.bottom - gui.rcCaret.top;
        }
    }
    //	if(CaretPos->x < 0)	CaretPos->x = 0;
    //	if(CaretPos->y < 0)	CaretPos->y = 0;
    if (focusInfo && focusInfo->m_hFocusWnd == NULL) focusInfo->m_hFocusWnd = focusInfo->m_hActiveWnd;
}
