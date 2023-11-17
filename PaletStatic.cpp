/*----------------------------------------------------------
    PaletStaticクラス
                                    2002/12/07 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#include "PaletStatic.h"
#include "charu3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------
//関数名	CPaletStatic
//機能		コンストラクタ
//---------------------------------------------------
CPaletStatic::CPaletStatic()
{
    m_colMyColor = 0;
}

//---------------------------------------------------
//関数名	~CPaletStatic
//機能		デストラクタ
//---------------------------------------------------
CPaletStatic::~CPaletStatic()
{
}

BEGIN_MESSAGE_MAP(CPaletStatic, CStatic)
    //{{AFX_MSG_MAP(CPaletStatic)
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------
// CPaletStatic メッセージ ハンドラ
//---------------------------------------------------

//---------------------------------------------------
//関数名	OnPaint()
//機能		描画
//---------------------------------------------------
void CPaletStatic::OnPaint()
{
    CPaintDC dc(this); // 描画用のデバイス コンテキスト

    RECT rect;
    GetClientRect(&rect);
    dc.IntersectClipRect(&rect);
    dc.FillSolidRect(&rect, m_colMyColor);
}
