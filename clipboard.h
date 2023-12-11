/*----------------------------------------------------------
    ClipBoardクラスヘッダ
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_CLIPBOARD_H__582AEB30_7E55_42E1_8D77_A28FAD0F7294__INCLUDED_)
#define AFX_CLIPBOARD_H__582AEB30_7E55_42E1_8D77_A28FAD0F7294__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

//---------------------------------------------------
// クラス定義
//---------------------------------------------------
class CClipBoard
{
public:
    CClipBoard() : m_hParentWnd(nullptr), m_bListening(false) {}
    virtual ~CClipBoard() {}

    void SetParent(HWND hWnd) {
        m_hParentWnd = hWnd;
        StartListener();
    }

    void StartListener() {
        if (!m_bListening) {
            ::AddClipboardFormatListener(m_hParentWnd);
            m_bListening = true;
        }
    }

    void StopListener() {
        ::RemoveClipboardFormatListener(m_hParentWnd);
        m_bListening = false;
    }

    bool SetClipboardText(const CString sData, int retryCount, int retryInterval);
    bool GetClipboardText(CString& sData, int retryCount, int retryInterval);

private:
    HWND m_hParentWnd;
    bool m_bListening;
};

#endif // !defined(AFX_CLIPBOARD_H__582AEB30_7E55_42E1_8D77_A28FAD0F7294__INCLUDED_)
