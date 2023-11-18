/*----------------------------------------------------------
    ChildViewクラス
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"

#include "ChildView.h"
#include "Charu3.h"

//---------------------------------------------------
//関数名	CChildView
//機能		コンストラクタ
//---------------------------------------------------
CChildView::CChildView()
{
}

//---------------------------------------------------
//関数名	~CChildView
//機能		デストラクタ
//---------------------------------------------------
CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
    //{{AFX_MSG_MAP(CChildView)
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------
// CChildView メッセージ ハンドラ
//---------------------------------------------------

//---------------------------------------------------
//関数名	PreCreateWindow(CREATESTRUCT& cs)
//機能		ウィンドウ作成前処理
//---------------------------------------------------
BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CWnd::PreCreateWindow(cs))
        return FALSE;

    cs.dwExStyle |= WS_EX_CLIENTEDGE;
    cs.style &= ~WS_BORDER;
    cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
        ::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW + 1), NULL);

    return TRUE;
}

//---------------------------------------------------
//関数名	OnPaint()
//機能		描画処理
//---------------------------------------------------
void CChildView::OnPaint()
{
    CPaintDC dc(this); // 描画用のデバイス コンテキスト

    // TODO: メッセージ ハンドラのコードをここに追加してください。

    // 描画のために CWnd::OnPaint を呼び出してはいけません。
}
