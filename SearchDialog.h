/*----------------------------------------------------------
    Charu3
    検索ダイアログヘッダ
----------------------------------------------------------*/
#if !defined(AFX_SEARCHDIALOG_H__F054EA83_2A07_11D4_8AC7_0050DAC3512B__INCLUDED_)
#define AFX_SEARCHDIALOG_H__F054EA83_2A07_11D4_8AC7_0050DAC3512B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

#include "resource.h"

//---------------------------------------------------
// CSearchDialog ダイアログ
//---------------------------------------------------
class CSearchDialog : public CDialog
{
    // コンストラクション
public:
    CSearchDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ

    // ダイアログ データ
    //{{AFX_DATA(CSearchDialog)
    enum { IDD = IDD_SEARCH };
    CEdit	m_ctrlSearchText;
    CButton m_ctrlSearchByName;
    CButton m_ctrlSearchByData;
    CButton m_ctrlSearchAnd;
    CButton m_ctrlSearchOr;
    CButton m_ctrlCaseInsensitive;
    //}}AFX_DATA

    int GetTarget();
    int GetSearchLogic();
    bool GetCaseInsensitive();
    CString GetSearchText();

    // オーバーライド
    // ClassWizard は仮想関数のオーバーライドを生成します。
    //{{AFX_VIRTUAL(CSearchDialog)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
    //}}AFX_VIRTUAL

// インプリメンテーション
protected:
    // 生成されたメッセージ マップ関数
    //{{AFX_MSG(CSearchDialog)
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    virtual void OnOK();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    void OnFindPrev();
    void OnFindNext();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SEARCHDIALOG_H__F054EA83_2A07_11D4_8AC7_0050DAC3512B__INCLUDED_)
