// MyEditCtrl.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MyEditCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CMyEditCtrl

CMyEditCtrl::CMyEditCtrl()
{
}

CMyEditCtrl::~CMyEditCtrl()
{
}

BEGIN_MESSAGE_MAP(CMyEditCtrl, CEdit)
    //{{AFX_MSG_MAP(CMyEditCtrl)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEditCtrl メッセージ ハンドラ

BOOL CMyEditCtrl::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == 0x41 && ::GetKeyState(VK_CONTROL) < 0) {
        this->SetSel(0, -1);
        return TRUE;
    }

    return CEdit::PreTranslateMessage(pMsg);
}
