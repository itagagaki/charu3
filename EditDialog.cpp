/*----------------------------------------------------------
    EditDialogクラス
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <vector>

#include "EditDialog.h"
#include "Charu3.h"
#include "clipboard.h"
#include "resource.h"

//---------------------------------------------------
//関数名	CEditDialog
//機能		コンストラクタ
//---------------------------------------------------
CEditDialog::CEditDialog(CWnd* pParent, STRING_DATA* pData, bool newData)
    : CDialog(CEditDialog::IDD, pParent)
    , m_pData(pData)
    , m_bNewData(newData)
    , m_bInitialized(false)
{
    //{{AFX_DATA_INIT(CEditDialog)
    //}}AFX_DATA_INIT
    m_vctMacro = &theApp.m_ini.m_vctMacro;
    m_vctDataMacro = &theApp.m_ini.m_vctDataMacro;
    m_nKind = m_pData->m_cKind;
}

//---------------------------------------------------
//関数名	DoDataExchange(CDataExchange* pDX)
//機能		データエクスチェンジ
//---------------------------------------------------
void CEditDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CEditDialog)

    if (GetDlgItem(IDC_EDIT_DATA))
        DDX_Control(pDX, IDC_EDIT_DATA, m_ctrlDataEdit);
    if (GetDlgItem(IDC_NAME))
        DDX_Control(pDX, IDC_NAME, m_ctrlNameEdit);
    if (GetDlgItem(IDC_EDIT_MACRO))
        DDX_Control(pDX, IDC_EDIT_MACRO, m_ctrlMacroEdit);
    if (GetDlgItem(IDC_KIND_COMBO))
        DDX_Control(pDX, IDC_EDIT_PASTE_FILE, m_ctrlPasteFile);
    if (GetDlgItem(IDC_KIND_COMBO))
        DDX_Control(pDX, IDC_KIND_COMBO, m_ctrlKindCombo);
    if (GetDlgItem(IDC_ICON_COMBO))
        DDX_Control(pDX, IDC_ICON_COMBO, m_ctrlIconCombo);
    if (GetDlgItem(IDC_EDIT_MACRO_COMBO))
        DDX_Control(pDX, IDC_EDIT_MACRO_COMBO, m_ctrlMacro);
    if (GetDlgItem(IDC_EDIT_DATA_MACRO_COMBO))
        DDX_Control(pDX, IDC_EDIT_DATA_MACRO_COMBO, m_ctrlDataMacro);
    //}}AFX_DATA_MAP

}

BEGIN_MESSAGE_MAP(CEditDialog, CDialog)
    //{{AFX_MSG_MAP(CEditDialog)
    ON_BN_CLICKED(IDC_EDIT_PASTE_FILE, OnEditPasteFile)
    ON_CBN_SELCHANGE(IDC_EDIT_MACRO_COMBO, OnSelchangeEditMacroCombo)
    ON_CBN_SELCHANGE(IDC_EDIT_DATA_MACRO_COMBO, OnSelchangeEditDataMacroCombo)
    ON_CBN_SELCHANGE(IDC_KIND_COMBO, OnKindChanged)
    ON_WM_SIZE()
    ON_WM_GETMINMAXINFO()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------
// CEditDialog メッセージ ハンドラ
//---------------------------------------------------

void CEditDialog::OnEditPasteFile()
{
    CFileDialog* pFileDialog;
    CString strRes;
    (void)strRes.LoadString(APP_INF_FILE_FILTER6);


    pFileDialog = new CFileDialog(TRUE, _T("*.*"), NULL, NULL, strRes);

    if (pFileDialog) {
        int selStart, selEnd;
        m_ctrlDataEdit.GetSel(selStart, selEnd); // save caret position
        if (IDOK == pFileDialog->DoModal()) {
            m_ctrlDataEdit.SetSel(selStart, selEnd, FALSE); // resume caret position
            InsertText(m_ctrlDataEdit, pFileDialog->GetPathName());
        }
        delete pFileDialog;
    }
}

//---------------------------------------------------
//関数名	OnInitDialog()
//機能		ダイアログの初期化
//---------------------------------------------------
BOOL CEditDialog::OnInitDialog()
{
    CString strRes;

    CDialog::OnInitDialog();

    SetWindowPos(NULL, 0, 0, theApp.m_ini.m_editWindowSize.x, theApp.m_ini.m_editWindowSize.y, SWP_NOMOVE | SWP_NOZORDER);

    (void)strRes.LoadString(m_bNewData ? APP_INF_CAPTION_NEWITEM : APP_INF_CAPTION_EDITITEM);
    SetWindowText(strRes);

    m_ctrlDataEdit.SetWindowText(m_pData->m_strData);
    m_ctrlNameEdit.SetWindowText(m_pData->m_strTitle);
    m_ctrlMacroEdit.SetWindowText(m_pData->m_strMacro);

    if (m_nKind & KIND_FOLDER_ALL) {
        CString strRes;
        (void)strRes.LoadString(APP_INF_FOLDER_REGULAR);
        m_ctrlKindCombo.AddString(strRes);
        (void)strRes.LoadString(APP_INF_FOLDER_HISTORY);
        m_ctrlKindCombo.AddString(strRes);
        m_ctrlKindCombo.SetCurSel(m_nKind & KIND_RIREKI ? 1 : 0);
        GetDlgItem(IDC_STATIC_ICON)->ShowWindow(SW_HIDE);
        m_ctrlIconCombo.EnableWindow(FALSE);
        m_ctrlIconCombo.ShowWindow(SW_HIDE);
        m_ctrlDataEdit.ShowWindow(SW_HIDE);
        m_ctrlMacro.ShowWindow(SW_HIDE);
        m_ctrlPasteFile.ShowWindow(SW_HIDE);
        m_ctrlNameEdit.SetFocus();
    }
    else {
        (void)strRes.LoadString(APP_INF_DATA_PERMANENT);
        m_ctrlKindCombo.AddString(strRes);
        (void)strRes.LoadString(APP_INF_DATA_ONETIME);
        m_ctrlKindCombo.AddString(strRes);
        m_ctrlKindCombo.SetCurSel(m_pData->m_cKind & KIND_ONETIME ? 1 : 0);
        m_ctrlIconCombo.SetCurSel(m_bNewData ? 0 : (1 + m_pData->m_cIcon));

        std::vector<MACRO_STRUCT>::iterator it;
        int i;
        //int filter = m_bNewData ? KIND_DATA_ALL : m_nKind;
        int filter = KIND_DATA_ALL;
        if (m_vctMacro) {
            for (it = m_vctMacro->begin(), i = 0; it != m_vctMacro->end(); it++, i++) {
                if (it->m_cKind & filter) {
                    m_ctrlMacro.AddString(it->m_strName);
                    m_ctrlMacro.SetItemData(m_ctrlMacro.GetCount() - 1, i);
                }
            }
        }
        if (m_ctrlMacro.GetCount() == 0) m_ctrlMacro.EnableWindow(FALSE);
        m_ctrlDataEdit.SetFocus();
    }
    ResetExtendedSettingItems();
    m_bInitialized = true;
    return FALSE;
}

void CEditDialog::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    CDialog::OnGetMinMaxInfo(lpMMI);
    lpMMI->ptMinTrackSize.x = EDIT_WINDOW_WIDTH_MIN;
    lpMMI->ptMinTrackSize.y = EDIT_WINDOW_HEIGHT_MIN;
}

//---------------------------------------------------
//関数名	OnSize(UINT nType, int cx, int cy)
//機能		リサイズ
//---------------------------------------------------
void CEditDialog::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);
    if (m_bInitialized) {
        RECT rect;
        GetWindowRect(&rect);
        theApp.m_ini.m_editWindowSize.x = rect.right - rect.left;
        theApp.m_ini.m_editWindowSize.y = rect.bottom - rect.top;
    }
}

//---------------------------------------------------
//関数名	OnOK()
//機能		OK処理
//---------------------------------------------------
void CEditDialog::OnOK()
{
    CString strData;
    CString strName;
    CString strMacro;
    m_ctrlDataEdit.GetWindowText(strData);
    m_ctrlNameEdit.GetWindowText(strName);
    m_ctrlMacroEdit.GetWindowText(strMacro);

    if (strName == _T("")) {
        if (!m_bNewData) {
            CString strRes;
            (void)strRes.LoadString(APP_MES_NAME_EMPTY);
            AfxMessageBox(strRes);
            m_ctrlNameEdit.SetFocus();
            return;
        }
        if (strData == _T("")) {
            CString strRes;
            (void)strRes.LoadString(APP_MES_ALL_EMPTY);
            AfxMessageBox(strRes);
            m_ctrlDataEdit.SetFocus();
            return;
        }
        m_pData->m_strTitle = theApp.m_pTree->makeTitle(strData);
    }
    else {
        m_pData->m_strTitle = strName;
    }
    m_pData->m_strData = strData;
    m_pData->m_strMacro = strMacro;

    if (m_pData->m_cKind & KIND_FOLDER_ALL) {
        m_pData->m_cKind = m_ctrlKindCombo.GetCurSel() == 1 ? KIND_RIREKI : KIND_FOLDER;
        m_pData->m_cIcon = KIND_DEFAULT;
    }
    else {
        m_pData->m_cKind = m_ctrlKindCombo.GetCurSel() == 1 ? KIND_ONETIME : KIND_LOCK;

        int icon = m_ctrlIconCombo.GetCurSel();
        if (icon > 0) {
            m_pData->m_cIcon = icon - 1;
        }
        else {
            m_pData->m_cIcon = theApp.m_pTree->decideIcon(m_pData->m_strData);
        }
    }

    CDialog::OnOK();
}

//---------------------------------------------------
//関数名	OnSelchangeEditMacroCombo()
//機能		マクロコンボボックス変更
//---------------------------------------------------
void CEditDialog::OnSelchangeEditMacroCombo()
{
    if (m_vctMacro) {
        int nSelect = m_ctrlMacro.GetItemData(m_ctrlMacro.GetCurSel());
        if (nSelect >= 0) {
            MACRO_STRUCT macro;
            macro = m_vctMacro->at(nSelect);
            InsertText(m_ctrlDataEdit, macro.m_strMacro);
        }
    }
}

void CEditDialog::OnKindChanged()
{
    if (m_nKind & KIND_FOLDER_ALL) {
        m_nKind = m_ctrlKindCombo.GetCurSel() == 1 ? KIND_RIREKI : KIND_FOLDER;
        ResetExtendedSettingItems();
    }
    else if (m_nKind & KIND_DATA_ALL) {
        m_nKind = m_ctrlKindCombo.GetCurSel() == 1 ? KIND_ONETIME : KIND_LOCK;
    }
}

//---------------------------------------------------
//関数名	OnSelchangeEditDataMacroCombo()
//機能		マクロコンボボックス変更
//---------------------------------------------------
void CEditDialog::OnSelchangeEditDataMacroCombo()
{
    if (m_vctDataMacro) {
        int nSelect = m_ctrlDataMacro.GetItemData(m_ctrlDataMacro.GetCurSel());
        if (nSelect >= 0) {
            MACRO_STRUCT macro;
            macro = m_vctDataMacro->at(nSelect);
            InsertText(m_ctrlMacroEdit, macro.m_strMacro);
        }
    }
}

//---------------------------------------------------
//関数名	InsertText(CMyEditCtrl& ctrl, CString strString)
//---------------------------------------------------
void CEditDialog::InsertText(CEdit& ctrl, CString strString)
{
    ctrl.ReplaceSel(strString.GetString(), TRUE);
    ctrl.SetFocus();
}

BOOL CEditDialog::DestroyWindow()
{
    theApp.RedrawDataTreeView();

    return CDialog::DestroyWindow();
}

void CEditDialog::ResetExtendedSettingItems()
{
    for (int i = m_ctrlDataMacro.GetCount() - 1; i >= 0; i--) {
        m_ctrlDataMacro.DeleteString(i);
    }
    std::vector<MACRO_STRUCT>::iterator it;
    int i;
    int filter = m_bNewData ? KIND_DATA_ALL : m_nKind;
    if (m_vctDataMacro) {
        for (it = m_vctDataMacro->begin(), i = 0; it != m_vctDataMacro->end(); it++, i++) {
            if (it->m_cKind & filter) {
                m_ctrlDataMacro.AddString(it->m_strName);
                m_ctrlDataMacro.SetItemData(m_ctrlDataMacro.GetCount() - 1, i);
            }
        }
    }
    m_ctrlDataMacro.EnableWindow(m_ctrlDataMacro.GetCount() > 0);
}
