#if !defined(AFX_MYEDITCTRL_H__A57B3A10_CCBB_48D2_9A46_A4D36FE50D26__INCLUDED_)
#define AFX_MYEDITCTRL_H__A57B3A10_CCBB_48D2_9A46_A4D36FE50D26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////
// CMyEditCtrl ウィンドウ

class CMyEditCtrl : public CEdit
{
    // コンストラクション
public:
    CMyEditCtrl();

    // アトリビュート
public:

    // オペレーション
public:

    // オーバーライド
    // ClassWizard は仮想関数のオーバーライドを生成します。
public:
    //{{AFX_VIRTUAL(CMyEditCtrl)
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_VIRTUAL

    // インプリメンテーション
public:
    virtual ~CMyEditCtrl();

    // 生成されたメッセージ マップ関数
protected:
    //{{AFX_MSG(CMyEditCtrl)
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MYEDITCTRL_H__A57B3A10_CCBB_48D2_9A46_A4D36FE50D26__INCLUDED_)
