/*----------------------------------------------------------
    ClipBordクラス
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#include "ClipBord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//---------------------------------------------------
//関数名	setClipboardText(const TCHAR* sData)
//機能		クリップボードにテキストを設定
//---------------------------------------------------
bool CClipBoard::setClipboardText(const CString sData)
{
    return setClipboardText(sData, m_nRetryCount, m_nRetryInterval);
}

bool CClipBoard::setClipboardText(const CString sData, int retryCount, int retryInterval)
{
    size_t len = sData.GetLength() + 1;
    TCHAR* newText = new TCHAR[len];
    _tcscpy_s(newText, len, sData);

    bool isRet = false;
    UINT uFormat;
#ifdef _UNICODE
    uFormat = CF_UNICODETEXT;
#else
    uFormat = CF_TEXT;
#endif
    do {
        if (::OpenClipboard(m_hParentWnd)) {
            StopListener();
            HGLOBAL hMem = ::GlobalAlloc(GHND, len * sizeof(TCHAR));
            if (hMem) {
                LPVOID pPtr = ::GlobalLock(hMem);
                if (pPtr) {
                    _tcscpy_s(static_cast<TCHAR*>(pPtr), len, newText);
                    ::GlobalUnlock(hMem);
                    ::EmptyClipboard();
                    HANDLE hRet = ::SetClipboardData(uFormat, hMem);
                    if (hRet) {
                        isRet = true;
                    }
                }
                else {
                    ::GlobalFree(hMem);
                }
            }
            ::CloseClipboard();
            StartListener();
            isRet = true;
            break;
        }
        if (retryCount < 1) {
            break;
        }
        retryCount--;
        Sleep(retryInterval);
    } while (true);

    delete[] newText;
    return isRet;
}

//---------------------------------------------------
//関数名	getClipboardText(CString& sData)
//機能		クリップボードのテキストを取得
//---------------------------------------------------
bool CClipBoard::getClipboardText(CString& sData)
{
    return getClipboardText(sData, m_nRetryCount, m_nRetryInterval);
}

bool CClipBoard::getClipboardText(CString& sData, int retryCount, int retryInterval)
{
    bool isRet = false;

    UINT uFormat;
#ifdef _UNICODE
    uFormat = CF_UNICODETEXT;
#else
    uFormat = CF_TEXT;
#endif
    if (::IsClipboardFormatAvailable(uFormat)) {
        do {
            if (::OpenClipboard(m_hParentWnd)) {
                HGLOBAL hGet = ::GetClipboardData(uFormat);
                if (hGet) {
                    LPVOID pPtr = ::GlobalLock(hGet);
                    if (pPtr) {
                        sData = CString(static_cast<TCHAR*>(pPtr));
                        isRet = true;
                        ::GlobalUnlock(hGet);
                    }
                }
                ::CloseClipboard();
                break;
            }
            if (retryCount < 1) {
                break;
            }
            retryCount--;
            Sleep(retryInterval);
        } while (true);
    }

    return isRet;
}
