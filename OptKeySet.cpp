/*----------------------------------------------------------
    キー設定クラス
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <list>

#include "OptKeySet.h"
#include "OptKeySetEditDlg.h"
#include "Charu3.h" // for theApp.m_ini
#include "key.h"
#include "resource.h"

//---------------------------------------------------
//関数名	COptKeySet
//機能		コンストラクタ
//---------------------------------------------------
COptKeySet::COptKeySet(CWnd* pParent /*=NULL*/)
    : CDialog(COptKeySet::IDD, pParent)
    , m_dataPtrSelect(nullptr)
    , m_nSelItem(-1)
{
}

//---------------------------------------------------
//関数名	DoDataExchange(CDataExchange* pDX)
//機能		データエクスチェンジ
//---------------------------------------------------
void COptKeySet::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    //{{AFX_DATA_MAP(COptKeySet)

    // obsolete
    //if(GetDlgItem(IDC_OPT_KEY_ACTION01))
    //	DDX_Radio(pDX, IDC_OPT_KEY_ACTION01, theApp.m_ini.m_key.m_defKeySet.m_nMessage);

    if (GetDlgItem(IDC_OPT_COPYKEY)) {
        DDX_Control(pDX, IDC_OPT_COPYKEY, m_ctrlCopyKey);
    }
    if (GetDlgItem(IDC_OPT_PASTKEY)) {
        DDX_Control(pDX, IDC_OPT_PASTKEY, m_ctrlPasteKey);
    }
    if (GetDlgItem(IDC_OPT_COPY_WAIT)) {
        DDX_Text(pDX, IDC_OPT_COPY_WAIT, theApp.m_ini.m_key.m_defKeySet.m_nCopyWait);
        // Not worked correctly
        //DDV_MinMaxInt(pDX, theApp.m_ini.m_key.m_defKeySet.m_nCopyWait, 0, 1000);
    }
    if (GetDlgItem(IDC_OPT_PASTE_WAIT)) {
        DDX_Text(pDX, IDC_OPT_PASTE_WAIT, theApp.m_ini.m_key.m_defKeySet.m_nPasteWait);
        // Not worked correctly
        //DDV_MinMaxInt(pDX, theApp.m_ini.m_key.m_defKeySet.m_nPasteWait,0, 1000);
    }
    if (GetDlgItem(IDC_OPT_HISTORY_SIZE_LIMIT)) {
        DDX_Text(pDX, IDC_OPT_HISTORY_SIZE_LIMIT, theApp.m_ini.m_key.m_nHistoryLimit);
    }
    if (GetDlgItem(IDC_OPT_KEY_INI_LIST)) {
        DDX_Control(pDX, IDC_OPT_KEY_INI_LIST, m_ctrlIniList);
    }
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptKeySet, CDialog)
    //{{AFX_MSG_MAP(COptKeySet)
    ON_BN_CLICKED(IDC_OPT_KEY_EDIT, OnOptKeyEdit)
    ON_BN_CLICKED(IDC_OPT_KEY_ADD, OnOptKeyAdd)
    ON_BN_CLICKED(IDC_OPT_KEY_DALETE, OnOptKeyDalete)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_OPT_KEY_INI_LIST, OnItemchangedOptKeyIniList)
    ON_NOTIFY(NM_DBLCLK, IDC_OPT_KEY_INI_LIST, OnDblclkOptKeyIniList)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------
//関数名	SetListData(CHANGE_KEY sKeyData)	[public]
//機能		リストビューに設定を追加
//---------------------------------------------------
void COptKeySet::SetListData(CHANGE_KEY sKeyData, CHANGE_KEY* dataPtr, bool isSet, int nLine)
{
    LVITEM AddItem = {};
    LPTSTR pString[4] = {};
    UINT uMod;
    int i;
    CString strPaste, strCopy, strTmp;

    CString strMatch[4];
    for (i = 0; i <= 3; i++) {
        (void)strMatch[i].LoadString(APP_INF_KEY_COMBO01 + i);
    }

    //ペーストキー設定名を作成
    uMod = sKeyData.m_sCopyPasteKey.m_uMod_Paste;
    strPaste = "";
    if (uMod & MOD_CONTROL) {
        strPaste = strPaste + _T("Ctrl+");
    }
    if (uMod & MOD_SHIFT) {
        strPaste = strPaste + _T("Shift+");
    }
    if (uMod & MOD_ALT) {
        strPaste = strPaste + _T("Alt+");
    }
    strPaste = strPaste + KeyHelper::GetKeyName(sKeyData.m_sCopyPasteKey.m_uVK_Paste, theApp.m_ini.m_keyLayout);

    //コピーキー設定名を作成
    uMod = sKeyData.m_sCopyPasteKey.m_uMod_Copy;
    strCopy = "";
    if (uMod & MOD_CONTROL) {
        strCopy = strCopy + _T("Ctrl+");
    }
    if (uMod & MOD_SHIFT) {
        strCopy = strCopy + _T("Shift+");
    }
    if (uMod & MOD_ALT) {
        strCopy = strCopy + _T("Alt+");
    }
    strCopy = strCopy + KeyHelper::GetKeyName(sKeyData.m_sCopyPasteKey.m_uVK_Copy, theApp.m_ini.m_keyLayout);

    pString[0] = (TCHAR*)LPCTSTR(sKeyData.m_strTitle);
    pString[1] = (TCHAR*)LPCTSTR(strMatch[sKeyData.m_nMatch]);
    pString[2] = (TCHAR*)LPCTSTR(strCopy);
    pString[3] = (TCHAR*)LPCTSTR(strPaste);
    AddItem.mask = LVIF_TEXT | LVIF_PARAM;
    for (i = 0; i <= 3; i++) {
        AddItem.iItem = nLine;
        AddItem.iSubItem = i;
        AddItem.pszText = pString[i];

        //新規
        if (i == 0 && !isSet) {
            AddItem.lParam = (LPARAM)dataPtr;//データのアドレスを設定
            m_ctrlIniList.InsertItem(&AddItem);
        }
        //変更
        else if (i == 0 && isSet) {
            AddItem.mask = LVIF_TEXT;
            AddItem.lParam = (LPARAM)dataPtr;//データのアドレスを設定
            m_ctrlIniList.SetItem(&AddItem); // modify existing item (the sub-item text)
        }
        else {
            m_ctrlIniList.SetItem(&AddItem); // modify existing item (the sub-item text)
        }
        AddItem.mask = LVIF_TEXT;
    }
}

//---------------------------------------------------
//関数名	findData(CHANGE_KEY *ptr)
//機能		キー設定データリストからptrが指している要素を探す
//---------------------------------------------------
std::list<CHANGE_KEY>::iterator COptKeySet::findData(CHANGE_KEY* dataPtr)
{
    std::list<CHANGE_KEY>::iterator it;
    for (it = m_MyKeyList.begin(); it != m_MyKeyList.end(); it++) {
        if (&*it == dataPtr) {
            break;
        }
    }
    return it;
}

//---------------------------------------------------
//関数名	windouToKeyOption()
//機能		キー設定を読む
//---------------------------------------------------
CHANGE_KEY COptKeySet::windouToKeyOption()
{
    CHANGE_KEY key;
    /*	WORD wMod,wVkCode;

        //ペーストキー変換
        m_ctrlPasteKey2.GetHotKey(wVkCode,wMod);
        key.m_sCopyPasteKey.m_uMod_Paste = KeyHelper::HotkeyToMod(wMod);
        key.m_sCopyPasteKey.m_uVK_Paste  = wVkCode;

        //コピーキー変換
        m_ctrlCopyKey2.GetHotKey(wVkCode,wMod);
        key.m_sCopyPasteKey.m_uMod_Copy = KeyHelper::HotkeyToMod(wMod);
        key.m_sCopyPasteKey.m_uVK_Copy  = wVkCode;

        key.m_nMatch = m_ctrlMatchCombo.GetCurSel();		//一致方法
        m_ctrlCaptionCombo.GetWindowText(key.m_strTitle);	//ウィンドウキャプション

        //待ち時間
        CString strBuff;
        m_ctrlCopyWaitEdit.GetWindowText(strBuff);
        key.m_sCopyPasteKey.m_nCopyWait = _ttoi(strBuff);
        if(key.m_sCopyPasteKey.m_nCopyWait < 1) key.m_sCopyPasteKey.m_nCopyWait = 100;

        m_ctrlPasteWaitEdit.GetWindowText(strBuff);
        key.m_sCopyPasteKey.m_nPasteWait = _ttoi(strBuff);
        if(key.m_sCopyPasteKey.m_nPasteWait < 1) key.m_sCopyPasteKey.m_nPasteWait = 100;

        key.m_sCopyPasteKey.m_nMessage = GetCheckedRadioButton(IDC_OPT_KEY_ACTION2_01,IDC_OPT_KEY_ACTION2_03) - IDC_OPT_KEY_ACTION2_01;
    */
    return key;
}

//---------------------------------------------------
// COptKeySet メッセージ ハンドラ
//---------------------------------------------------

//---------------------------------------------------
//関数名	PreTranslateMessage(MSG* pMsg)
//機能		メッセージ前処理
//---------------------------------------------------
BOOL COptKeySet::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB && ::GetKeyState(VK_CONTROL) < 0) {
        ::PostMessage(::GetParent(this->m_hWnd), pMsg->message, VK_PRIOR, pMsg->lParam);
    }

    return CDialog::PreTranslateMessage(pMsg);
}

//---------------------------------------------------
//関数名	OnInitDialog()
//機能		初期化
//---------------------------------------------------
BOOL COptKeySet::OnInitDialog()
{
    CDialog::OnInitDialog();

    UINT uKey, uMod, uCopyKey, uCopyMod;
    theApp.m_ini.getPasteHotKey(&uKey, &uMod, &uCopyKey, &uCopyMod);
    uMod = KeyHelper::ModToHotkey(uMod);
    uCopyMod = KeyHelper::ModToHotkey(uCopyMod);

    m_ctrlPasteKey.SetHotKey(uKey, uMod);
    m_ctrlCopyKey.SetHotKey(uCopyKey, uCopyMod);

    CString strRes;
    (void)strRes.LoadString(APP_INF_KEY_LIST_CAPTION01);
    m_ctrlIniList.InsertColumn(0, strRes, LVCFMT_LEFT, 172);
    (void)strRes.LoadString(APP_INF_KEY_LIST_CAPTION02);
    m_ctrlIniList.InsertColumn(1, strRes, LVCFMT_LEFT, 60);
    (void)strRes.LoadString(APP_INF_KEY_LIST_CAPTION03);
    m_ctrlIniList.InsertColumn(2, strRes, LVCFMT_LEFT, 78);
    (void)strRes.LoadString(APP_INF_KEY_LIST_CAPTION04);
    m_ctrlIniList.InsertColumn(3, strRes, LVCFMT_LEFT, 78);
    ListView_SetExtendedListViewStyle(m_ctrlIniList.GetSafeHwnd(), LVS_EX_FULLROWSELECT);

    std::list<CHANGE_KEY>::iterator it;
    CHANGE_KEY key;
    int i;

    for (it = theApp.m_ini.m_key.m_KeyList.begin(); it != theApp.m_ini.m_key.m_KeyList.end(); it++) {
        m_MyKeyList.insert(m_MyKeyList.end(), *it);
    }

    for (i = 0, it = m_MyKeyList.begin(); it != m_MyKeyList.end(); it++, i++) {
        SetListData(*it, &*it, false, i);
    }

    /*	m_ctrlCaptionCombo.ResetContent ();
        EnumWindows(&EnumWindowTitle,(LPARAM )this);
        m_ctrlCaptionCombo.SetCurSel(0);
        m_ctrlMatchCombo.SetCurSel(0);

        m_ctrlCopyWaitEdit.SetWindowText(_T("100"));
        m_ctrlPasteWaitEdit.SetWindowText(_T("100"));
        CheckRadioButton(IDC_OPT_KEY_ACTION2_01,IDC_OPT_KEY_ACTION2_03,IDC_OPT_KEY_ACTION2_01);
    */
    return TRUE;
}

//---------------------------------------------------
//関数名	DestroyWindow()
//機能		終了処理
//---------------------------------------------------
BOOL COptKeySet::DestroyWindow()
{
    WORD wVkCodeP, wModP;
    WORD wVkCodeC, wModC;
    //基本キー設定を取得、設定
    m_ctrlCopyKey.GetHotKey(wVkCodeC, wModC);
    wModC = KeyHelper::HotkeyToMod(wModC);

    m_ctrlPasteKey.GetHotKey(wVkCodeP, wModP);
    wModP = KeyHelper::HotkeyToMod(wModP);

    theApp.m_ini.setPasteHotkey(wVkCodeP, wModP, wVkCodeC, wModC);

    //拡張キー設定をコピーする
    CHANGE_KEY ckey;
    std::list<CHANGE_KEY>::iterator it;

    theApp.m_ini.m_key.m_KeyList.clear();

    for (it = m_MyKeyList.begin(); it != m_MyKeyList.end(); it++) {
        theApp.m_ini.m_key.m_KeyList.insert(theApp.m_ini.m_key.m_KeyList.end(), *it);
    }
    return CDialog::DestroyWindow();
}

//---------------------------------------------------
//関数名	OnOptKeyEdit()
//機能		キー設定変更
//---------------------------------------------------
void COptKeySet::OnOptKeyEdit()
{
    COptKeySetEditDlg keySetEditDlg(this);

    if (m_nSelItem > -1) {
        CString strBuff;
        keySetEditDlg.setKeyInfo(*m_dataPtrSelect);

        strBuff.Format(_T("%x,%x,%x"), m_dataPtrSelect->m_sCopyPasteKey.m_pasteMessage.Msg,
            m_dataPtrSelect->m_sCopyPasteKey.m_pasteMessage.wParam,
            m_dataPtrSelect->m_sCopyPasteKey.m_pasteMessage.lParam);
        keySetEditDlg.m_pasteMessage = strBuff;
        strBuff.Format(_T("%x,%x,%x"), m_dataPtrSelect->m_sCopyPasteKey.m_copyMessage.Msg,
            m_dataPtrSelect->m_sCopyPasteKey.m_copyMessage.wParam,
            m_dataPtrSelect->m_sCopyPasteKey.m_copyMessage.lParam);
        keySetEditDlg.m_copyMessage = strBuff;

        keySetEditDlg.m_keyAction = m_dataPtrSelect->m_sCopyPasteKey.m_nMessage;
        keySetEditDlg.m_matchCombo = m_dataPtrSelect->m_nMatch;
        keySetEditDlg.m_caption = m_dataPtrSelect->m_strTitle;
        keySetEditDlg.m_copyWait = m_dataPtrSelect->m_sCopyPasteKey.m_nCopyWait;
        keySetEditDlg.m_pasteWait = m_dataPtrSelect->m_sCopyPasteKey.m_nPasteWait;
        keySetEditDlg.m_nHistoryLimit = m_dataPtrSelect->m_nHistoryLimit;

        int nRet = keySetEditDlg.DoModal();
        if (nRet == IDOK) {
            *m_dataPtrSelect = keySetEditDlg.getKeyInfo();
            SetListData(*m_dataPtrSelect, m_dataPtrSelect, true, m_nSelItem);
        }
    }
}

//---------------------------------------------------
//関数名	OnOptKeyAdd()
//機能		設定追加
//---------------------------------------------------
void COptKeySet::OnOptKeyAdd()
{
    CHANGE_KEY deffault;
    COptKeySetEditDlg keySetEditDlg;

    deffault.m_sCopyPasteKey = theApp.m_ini.m_key.m_defKeySet;
    keySetEditDlg.setKeyInfo(deffault);

    keySetEditDlg.m_pasteMessage = _T("111,0,0");
    keySetEditDlg.m_copyMessage = _T("111,0,0");

    keySetEditDlg.m_keyAction = 0;
    keySetEditDlg.m_matchCombo = 0;
    keySetEditDlg.m_copyWait = 100;
    keySetEditDlg.m_pasteWait = 100;
    keySetEditDlg.m_nHistoryLimit = -1;

    int nRet = keySetEditDlg.DoModal();
    if (nRet == IDOK) {
        CHANGE_KEY key;
        key = keySetEditDlg.getKeyInfo();
        SetListData(key, &*m_MyKeyList.insert(m_MyKeyList.end(), key), false, m_ctrlIniList.GetItemCount());
    }
}

//---------------------------------------------------
//関数名	OnOptKeyDalete()
//機能		設定削除
//---------------------------------------------------
void COptKeySet::OnOptKeyDalete()
{
    if (m_nSelItem > -1) {
        CString strBuff, strCaption;
        CString strRes;
        (void)strRes.LoadString(APP_MES_DELETE_OK);
        strBuff.Format(strRes, m_dataPtrSelect->m_strTitle);
        int nRet = AfxMessageBox(strBuff, MB_YESNO | MB_ICONEXCLAMATION | MB_APPLMODAL);
        if (nRet == IDYES) {
            std::list<CHANGE_KEY>::iterator it = findData(m_dataPtrSelect);
            it = m_MyKeyList.erase(it);
            m_dataPtrSelect = it != m_MyKeyList.end() ? &*it : nullptr;
            m_ctrlIniList.DeleteItem(m_nSelItem);

            if (m_nSelItem == m_ctrlIniList.GetItemCount()) {
                m_nSelItem = m_ctrlIniList.GetItemCount() - 1;
                m_ctrlIniList.SetItemState(m_nSelItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
            }
        }
    }
}

//---------------------------------------------------
//関数名	OnItemchangedOptKeyIniList(NMHDR* pNMHDR, LRESULT* pResult)
//機能		リストの選択変更
//---------------------------------------------------
void COptKeySet::OnItemchangedOptKeyIniList(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if (pNMListView->uOldState != 0) return;
    DWORD_PTR lParam;

    lParam = m_ctrlIniList.GetItemData(pNMListView->iItem);//選択データのアドレスを取得
    m_dataPtrSelect = (CHANGE_KEY*)lParam;
    m_nSelItem = pNMListView->iItem;

    return;
    /*	UINT uMod = m_itSelect->m_sCopyPasteKey.m_uMod_Paste;
        if(m_itSelect->m_sCopyPasteKey.m_uVK_Paste >= 0x60)
            m_ctrlPasteKey2.SetHotKey(m_itSelect->m_sCopyPasteKey.m_uVK_Paste, KeyHelper::ModToHotkey(uMod));
        else
            m_ctrlPasteKey2.SetHotKey(m_itSelect->m_sCopyPasteKey.m_uVK_Paste, KeyHelper::ModToHotkey(uMod) | HOTKEYF_EXT);

        uMod = m_itSelect->m_sCopyPasteKey.m_uMod_Copy;
        if(m_itSelect->m_sCopyPasteKey.m_uVK_Copy >= 0x60)
            m_ctrlCopyKey2.SetHotKey(m_itSelect->m_sCopyPasteKey.m_uVK_Copy, KeyHelper::ModToHotkey(uMod));
        else
            m_ctrlCopyKey2.SetHotKey(m_itSelect->m_sCopyPasteKey.m_uVK_Copy, KeyHelper::ModToHotkey(uMod) | HOTKEYF_EXT);

        m_ctrlCaptionCombo.ResetContent ();
        m_ctrlCaptionCombo.SetWindowText(m_itSelect->m_strTitle);
        EnumWindows(&EnumWindowTitle,(LPARAM )this);

        m_ctrlMatchCombo.SetCurSel(m_itSelect->m_nMatch);
        CString strBuff;
        strBuff.Format(_T("%d"),m_itSelect->m_sCopyPasteKey.m_nCopyWait);
        m_ctrlCopyWaitEdit.SetWindowText(strBuff);

        strBuff.Format(_T("%d"),m_itSelect->m_sCopyPasteKey.m_nPasteWait);
        m_ctrlPasteWaitEdit.SetWindowText(strBuff);

        CheckRadioButton(IDC_OPT_KEY_ACTION2_01,IDC_OPT_KEY_ACTION2_03,
            IDC_OPT_KEY_ACTION2_01 + m_itSelect->m_sCopyPasteKey.m_nMessage);
        *pResult = 0;*/
}

void COptKeySet::OnDblclkOptKeyIniList(NMHDR* pNMHDR, LRESULT* pResult)
{
    OnOptKeyEdit();
    *pResult = 0;
}
