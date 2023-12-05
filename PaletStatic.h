/*----------------------------------------------------------
    PaletStaticクラスヘッダ
                                    2002/12/07 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_PALETSTATIC_H__52F92273_F258_4DFC_B200_6D3A5F68B707__INCLUDED_)
#define AFX_PALETSTATIC_H__52F92273_F258_4DFC_B200_6D3A5F68B707__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

//---------------------------------------------------
// CPaletStatic ウィンドウ
//---------------------------------------------------
class CPaletStatic : public CStatic
{
    // コンストラクション
public:
    CPaletStatic();

    // アトリビュート
public:

    // オペレーション
public:
    void SetColor(COLORREF color) {
        m_color = color;
    }

    // オーバーライド
    // ClassWizard は仮想関数のオーバーライドを生成します。
    //{{AFX_VIRTUAL(CPaletStatic)
    //}}AFX_VIRTUAL

    // インプリメンテーション
public:
    virtual ~CPaletStatic();

private:
    COLORREF m_color;

    // 生成されたメッセージ マップ関数
protected:
    //{{AFX_MSG(CPaletStatic)
    afx_msg void OnPaint();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_PALETSTATIC_H__52F92273_F258_4DFC_B200_6D3A5F68B707__INCLUDED_)
