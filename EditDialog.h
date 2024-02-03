/*----------------------------------------------------------
    EditDialogクラスヘッダ
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_EDITDIALOG_H__B4B68363_82C1_4D62_AC66_00D698DF05A4__INCLUDED_)
#define AFX_EDITDIALOG_H__B4B68363_82C1_4D62_AC66_00D698DF05A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

#include <vector>

#include "MyEditCtrl.h"
#include "Charu3Tree.h"
#include "Init.h"
#include "resource.h"

#define EDIT_WINDOW_WIDTH_MIN 368
#define EDIT_WINDOW_HEIGHT_MIN 460

//---------------------------------------------------
// CEditDialog ダイアログ
//---------------------------------------------------
class CEditDialog : public CDialog
{
public:
    CEditDialog(CWnd* pParent, STRING_DATA* pData, bool newData);

    // ダイアログ データ
private:
    //{{AFX_DATA(CEditDialog)
    enum { IDD = IDD_EDIT };
    CMyEditCtrl m_ctrlDataEdit;
    CMyEditCtrl m_ctrlNameEdit;
    CMyEditCtrl m_ctrlMacroEdit;
    CComboBox m_ctrlKindCombo;
    CComboBox m_ctrlIconCombo;
    CComboBox m_ctrlMacro;
    CComboBox m_ctrlDataMacro;
    CButton m_ctrlPasteFile;
    //}}AFX_DATA

    // オーバーライド
    // ClassWizard は仮想関数のオーバーライドを生成します。
public:
    //{{AFX_VIRTUAL(CEditDialog)
    virtual BOOL DestroyWindow();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
    //}}AFX_VIRTUAL

    // インプリメンテーション
private:
    void InsertText(CEdit& ctrl, CString strString);
    void ResetExtendedSettingItems();

    STRING_DATA* m_pData;
    std::vector<MACRO_STRUCT>* m_vctMacro;
    std::vector<MACRO_STRUCT>* m_vctDataMacro;
    int m_nKind;
    bool m_bNewData;
    bool m_bInitialized;

    // 生成されたメッセージ マップ関数
protected:
    //{{AFX_MSG(CEditDialog)
    afx_msg void OnEditPasteFile();
    virtual BOOL OnInitDialog();
    virtual void OnSize(UINT nType, int cx, int cy);
    virtual void OnOK();
    afx_msg void OnSelchangeEditMacroCombo();
    afx_msg void OnSelchangeEditDataMacroCombo();
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    afx_msg void OnKindChanged();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDITDIALOG_H__B4B68363_82C1_4D62_AC66_00D698DF05A4__INCLUDED_)
