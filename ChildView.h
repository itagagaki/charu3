/*----------------------------------------------------------
    ChildViewクラスヘッダ
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_CHILDVIEW_H__F8B13439_ACED_4C46_87CF_498ACB16F2B8__INCLUDED_)
#define AFX_CHILDVIEW_H__F8B13439_ACED_4C46_87CF_498ACB16F2B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//---------------------------------------------------
// CChildView ウィンドウ
//---------------------------------------------------
class CChildView : public CWnd
{
    // コンストラクタ
public:
    CChildView();

    // アトリビュート
public:

    // オペレーション
public:

    // オーバーライド
        // ClassWizard は仮想関数のオーバーライドを生成します。
        //{{AFX_VIRTUAL(CChildView)
protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    //}}AFX_VIRTUAL

// インプリメンテーション
public:
    virtual ~CChildView();

    // 生成されたメッセージ マップ関数
protected:
    //{{AFX_MSG(CChildView)
    afx_msg void OnPaint();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ が新しい定義を加える場合には、この行よりも前に追加します。

#endif // !defined(AFX_CHILDVIEW_H__F8B13439_ACED_4C46_87CF_498ACB16F2B8__INCLUDED_)
