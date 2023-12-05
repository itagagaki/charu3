/*----------------------------------------------------------
    MyTreeDialogクラス
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <list>
#include <vector>

#include "MyTreeDialog.h"
#include "EditDialog.h"
#include "SearchDialog.h"
#include "Charu3.h"
#include "color.h"
#include "window.h"
#include "log.h"

namespace {
    //---------------------------------------------------
    //関数名	DrawLline(CDC* pDC, CPoint* point, COLORREF color)
    //機能		線を描く
    //---------------------------------------------------
    void DrawLline(CDC* pDC, CPoint* point, COLORREF color)
    {
        CPen pen(PS_SOLID, 1, color);
        CPen* old_pen = pDC->SelectObject(&pen);
        pDC->MoveTo(point[0]);
        pDC->LineTo(point[1]);
        pDC->LineTo(point[2]);
        pDC->SelectObject(old_pen);
    }

    //---------------------------------------------------
    //関数名	DrawFrame(CDC* pDC, CRect& rect, COLORREF color)
    //機能		枠を描く
    //---------------------------------------------------
    void DrawFrame(CDC* pDC, CRect& rect, COLORREF color)
    {
        CPoint point[3];
        COLORREF colRD[3] = {};
        COLORREF colLU[3] = {};

        colRD[1] = colLU[0] = Color::MultiplyBrightness(color, 1.2);
        colRD[2] = colLU[1] = color;
        colRD[0] = colLU[2] = Color::MultiplyBrightness(color, 0.8);

        for (int i = 1; i <= 3; i++) {
            point[0].x = rect.left + i;
            point[0].y = rect.bottom - i;
            point[1].x = rect.right - i;
            point[1].y = rect.bottom - i;
            point[2].x = rect.right - i;
            point[2].y = rect.top + i - 1;
            DrawLline(pDC, point, colRD[i - 1]);
        }

        for (int i = 0; i <= 2; i++) {
            point[0].x = rect.left + i;
            point[0].y = rect.bottom - i - 1;
            point[1].x = rect.left + i;
            point[1].y = rect.top + i;
            point[2].x = rect.right - i;
            point[2].y = rect.top + i;
            DrawLline(pDC, point, colLU[i]);
        }
    }
} // anonymous namespace

//---------------------------------------------------
//関数名	CMyTreeDialog
//機能		コンストラクタ
//---------------------------------------------------
CMyTreeDialog::CMyTreeDialog(CWnd* pParent /*=NULL*/)
    : CDialog(CMyTreeDialog::IDD, pParent)
    , m_pTreeCtrl(nullptr)
    , m_selectDataPtr(nullptr)
    , m_isInitOK(false)
    , m_isModal(true)
    , m_isAltDown(false)
    , m_dataPtrDbClick(nullptr)
    , m_hCopyData(nullptr)
    , m_dwStartTime(0)
    , m_hDLL(NULL)
    , m_pExStyle(NULL)
    , m_cFont(nullptr)
    , m_cOlgFont(nullptr)
    , m_colFrame(0xff9900)
    , m_strQuickKey("")
    , m_hQuickItem(nullptr)
    , m_bCheckbox(false)
    , m_bFind(false)
    , m_findDialog(nullptr)
{
    //{{AFX_DATA_INIT(CMyTreeDialog)
    //}}AFX_DATA_INIT
    m_brBack.m_hObject = NULL;
}

//---------------------------------------------------
//関数名	DoDataExchange(CDataExchange* pDX)
//機能		データエクスチェンジ
//---------------------------------------------------
void CMyTreeDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMyTreeDialog)
    if (GetDlgItem(IDC_MY_TREE))
        DDX_Control(pDX, IDC_MY_TREE, *m_pTreeCtrl);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyTreeDialog, CDialog)
    //{{AFX_MSG_MAP(CMyTreeDialog)
    ON_WM_SIZE()
    ON_COMMAND(IDML_EDIT, OnEdit)
    ON_COMMAND(IDML_DELETE, OnDelete)
    ON_COMMAND(IDML_RENAME, OnRename)
    ON_COMMAND(IDML_ICON_KEY, OnIconKey)
    ON_COMMAND(IDML_ICON_DATE, OnIconDate)
    ON_COMMAND(IDML_ICON_EXE, OnIconExe)
    ON_COMMAND(IDML_ICON_RELATE, OnIconRelate)
    ON_COMMAND(IDML_ICON_SELECT, OnIconSelect)
    ON_COMMAND(IDML_ICON_CLIP, OnIconClip)
    ON_COMMAND(IDML_ICON_PLUGIN, OnIconPlugin)
    ON_COMMAND(IDML_ICON_KEYMACRO, OnIconKeymacro)
    ON_COMMAND(IDML_MAKE_ONETIME, OnMakeOnetime)
    ON_COMMAND(IDML_MAKE_PERMANENT, OnMakePermanent)
    ON_COMMAND(IDML_FOLDER_CLEAR, OnFolderClear)
    ON_COMMAND(IDML_NEW_DATA, OnNewData)
    ON_COMMAND(IDML_NEW_FOLDER, OnNewFolder)
    ON_COMMAND(IDML_RESELECT_ICONS, OnReselectIcons)
    ON_COMMAND(IDML_CLEANUP_ONETIME, OnCleanupAllOnetime)
    ON_COMMAND(IDML_MAKE_PERMANENT_ALL, OnMakeAllOnetimePermanent)
    ON_COMMAND(IDML_CLOSE_ALL, OnCloseAll)
    ON_COMMAND(IDML_LIST_SERCH, OnListSearch)
    ON_COMMAND(IDML_CHECK_ITEM, OnCheckItem)
    ON_COMMAND(IDML_COPY_DATA, OnCopyData)
    ON_COMMAND(IDML_DATA_PASTE, OnDataPaste)
    ON_COMMAND(IDML_IMPORT, OnImport)
    ON_COMMAND(IDML_EXPORT, OnExport)
    ON_COMMAND(IDML_OPTION, OnOption)
    ON_NOTIFY(NM_RCLICK, IDC_MY_TREE, OnRclickMyTree)
    ON_NOTIFY(NM_CLICK, IDC_MY_TREE, OnClickMyTree)
    ON_NOTIFY(NM_KILLFOCUS, IDC_MY_TREE, OnKillFocusMyTree)
    ON_NOTIFY(NM_SETFOCUS, IDC_MY_TREE, OnSetFocusMyTree)
    ON_NOTIFY(TVN_KEYDOWN, IDC_MY_TREE, OnKeydownMyTree)
    ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_MY_TREE, OnBeginlabeleditMyTree)
    ON_NOTIFY(TVN_ENDLABELEDIT, IDC_MY_TREE, OnEndlabeleditMyTree)
    ON_WM_SHOWWINDOW()
    ON_WM_NCPAINT()
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------
//関数名	OnInitDialog()
//機能		ダイアログの初期化
//---------------------------------------------------
BOOL CMyTreeDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_pTreeCtrl->ModifyStyle(NULL, TVS_SHOWSELALWAYS);

    m_toolTip.Create(this, TTS_ALWAYSTIP);
    m_toolTip.SetMaxTipWidth(400);

    m_toolTip.Activate(TRUE);

    m_toolTip.AddTool(m_pTreeCtrl, _T(""));

    m_hDLL = ::LoadLibrary(_T("user32"));
    m_pExStyle = m_hDLL != 0 ? (PFUNC)::GetProcAddress(m_hDLL, "SetLayeredWindowAttributes") : NULL;
    m_PopupMenu.LoadMenu(MAKEINTRESOURCE(IDR_LISTMENU));//メニュークラスにメニューを読む
    if (!m_findDialog) {
        m_findDialog = new CSearchDialog(this);
    }
    return TRUE;
}

//---------------------------------------------------
//関数名	PopupMenu(CPoint point)
//機能		ポップアップメニューを出す
//---------------------------------------------------
void CMyTreeDialog::PopupMenu(CPoint point)
{
    if (m_isModal || !m_isInitOK) return;

    CPoint HitPoint = point;
    ScreenToClient(&HitPoint);
    HTREEITEM hTreeItem = m_pTreeCtrl->HitTest(HitPoint);
    m_PopupMenu.EnableMenuItem(IDML_EDIT, hTreeItem ? MF_ENABLED : MF_GRAYED);
    m_PopupMenu.EnableMenuItem(IDML_DELETE, hTreeItem ? MF_ENABLED : MF_GRAYED);
    m_PopupMenu.EnableMenuItem(IDML_RENAME, hTreeItem ? MF_ENABLED : MF_GRAYED);
    m_PopupMenu.EnableMenuItem(IDML_CHECK_ITEM, m_bCheckbox ? MF_GRAYED : MF_ENABLED);
    m_PopupMenu.EnableMenuItem(IDML_COPY_DATA, hTreeItem ? MF_ENABLED : MF_GRAYED);
    m_PopupMenu.EnableMenuItem(IDML_DATA_PASTE, m_hCopyData ? MF_ENABLED : MF_GRAYED);
    bool isData = false;
    bool isFolder = false;
    if (hTreeItem) {
        m_pTreeCtrl->SelectItem(hTreeItem);
        STRING_DATA* dataPtr = m_pTreeCtrl->getDataPtr(hTreeItem);
        isData = (dataPtr->m_cKind & KIND_DATA_ALL) != 0;
        isFolder = (dataPtr->m_cKind & KIND_FOLDER_ALL) != 0;
        m_PopupMenu.EnableMenuItem(IDML_MAKE_PERMANENT, !isData || (dataPtr->m_cKind & KIND_LOCK) ? MF_GRAYED : MF_ENABLED);
        m_PopupMenu.EnableMenuItem(IDML_MAKE_ONETIME, !isData || (dataPtr->m_cKind & KIND_ONETIME) ? MF_GRAYED : MF_ENABLED);
    }
    else {
        m_PopupMenu.EnableMenuItem(IDML_MAKE_PERMANENT, MF_GRAYED);
        m_PopupMenu.EnableMenuItem(IDML_MAKE_ONETIME, MF_GRAYED);
    }
    m_PopupMenu.EnableMenuItem(IDML_ICON_KEY, isData ? MF_ENABLED : MF_GRAYED);
    m_PopupMenu.EnableMenuItem(IDML_ICON_DATE, isData ? MF_ENABLED : MF_GRAYED);
    m_PopupMenu.EnableMenuItem(IDML_ICON_EXE, isData ? MF_ENABLED : MF_GRAYED);
    m_PopupMenu.EnableMenuItem(IDML_ICON_RELATE, isData ? MF_ENABLED : MF_GRAYED);
    m_PopupMenu.EnableMenuItem(IDML_ICON_SELECT, isData ? MF_ENABLED : MF_GRAYED);
    m_PopupMenu.EnableMenuItem(IDML_ICON_CLIP, isData ? MF_ENABLED : MF_GRAYED);
    m_PopupMenu.EnableMenuItem(IDML_ICON_PLUGIN, isData ? MF_ENABLED : MF_GRAYED);
    m_PopupMenu.EnableMenuItem(IDML_ICON_KEYMACRO, isData ? MF_ENABLED : MF_GRAYED);
    m_PopupMenu.EnableMenuItem(IDML_FOLDER_CLEAR, isFolder ? MF_ENABLED : MF_GRAYED);
    m_PopupMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
}

//---------------------------------------------------
//関数名	showWindowPos(POINT pos,int nCmdShow)
//機能		ウィンドウを移動して表示
//---------------------------------------------------
BOOL CMyTreeDialog::ShowWindowPos(POINT pos, POINT size, int nCmdShow, bool keepSelection, HTREEITEM hOpenItem)
{
    m_pTreeCtrl->OnWindowPosChanging(NULL);
    m_toolTip.SetDelayTime(theApp.m_ini.m_nToolTipDelay);
    m_toolTip.SetDelayTime(TTDT_AUTOPOP, theApp.m_ini.m_nToolTipTime);

    m_pTreeCtrl->SetBkColor(COLORREF(Color::Swap_RGB_BGR(theApp.m_ini.m_nBackgroundColor)));
    m_pTreeCtrl->SetTextColor(COLORREF(Color::Swap_RGB_BGR(theApp.m_ini.m_nTextColor)));
    m_pTreeCtrl->SetInsertMarkColor(COLORREF(Color::Swap_RGB_BGR(theApp.m_ini.m_nBorderColor)));
    m_pTreeCtrl->ModifyStyle(NULL, TVS_TRACKSELECT, NULL);
    if (theApp.m_ini.m_bSingleExpand) {
        m_pTreeCtrl->ModifyStyle(NULL, TVS_SINGLEEXPAND, NULL);
    }
    else {
        m_pTreeCtrl->ModifyStyle(TVS_SINGLEEXPAND, NULL, NULL);
    }

#if false
    // I thought about displaying the pathname of the data above the data tree.But decided not to.
    CWnd* cpDataName = GetDlgItem(IDC_DATA_NAME);
    if (cpDataName) {
        TCHAR buf[MAX_PATH];
        TCHAR* pFileName;
        GetFullPathName(theApp.m_ini.m_strDataPath.GetString(), MAX_PATH, buf, &pFileName);
        cpDataName->SetWindowText(pFileName);
    }
#endif

    HTREEITEM hSelectItem = m_pTreeCtrl->GetSelectedItem();
    if (!keepSelection) {
        m_pTreeCtrl->SelectItem(m_pTreeCtrl->GetRootItem());
    }
    if (!theApp.m_ini.m_bKeepFolders) {
        m_pTreeCtrl->closeFolder(m_pTreeCtrl->GetRootItem());
        if (keepSelection) {
            m_pTreeCtrl->SelectItem(hSelectItem);
        }
    }
    else if (hSelectItem && !hOpenItem) {
        RECT rSelItem;
        m_pTreeCtrl->GetItemRect(hSelectItem, &rSelItem, NULL);
        if (size.y < rSelItem.top || rSelItem.bottom < 0)
            m_pTreeCtrl->EnsureVisible(hSelectItem); //EnsureVisibleするとフォルダは開かれる
    }
    if (hOpenItem) m_pTreeCtrl->Expand(hOpenItem, TVE_EXPAND);

    m_colFrame = Color::Swap_RGB_BGR(theApp.m_ini.m_nBorderColor);

    if (m_brBack.GetSafeHandle()) {
        m_brBack.DeleteObject();
        m_brBack.m_hObject = NULL;
    }
    if (m_brBack.m_hObject == NULL)
        m_brBack.CreateSolidBrush(COLORREF(Color::Swap_RGB_BGR(theApp.m_ini.m_nBackgroundColor)));

    Window::SetAbsoluteForegroundWindow(theApp.m_pMainFrame->m_hWnd);
    m_pTreeCtrl->SetFocus();

    m_selectDataPtr = nullptr;
    m_isInitOK = true;
    m_isModal = false;
    m_isAltDown = false;
    m_dataPtrDbClick = nullptr;

    m_pTreeCtrl->m_ltCheckItems.clear();
    if (theApp.m_ini.m_nToolTip == 2)	m_toolTip.Activate(FALSE);
    else	m_toolTip.Activate(TRUE);
    SetWindowPos(&wndTopMost, pos.x, pos.y, size.x, size.y, NULL);

    return ShowWindow(nCmdShow);
}

//---------------------------------------------------
// CMyTreeDialog メッセージ ハンドラ
//---------------------------------------------------
void CMyTreeDialog::OnNcPaint()
{
    CDialog::OnNcPaint();
    // Set scrollbars
    m_pTreeCtrl->setScrollBar();
    // Draw border
    {
        CRect rect;
        GetWindowRect(rect);
        CRect localRect = rect - rect.TopLeft();
        CDC* pDC = GetWindowDC();
        DrawFrame(pDC, localRect, m_colFrame);
        ReleaseDC(pDC);
    }
    // Workaround for the problem where the border is overwritten
    {
        DWMNCRENDERINGPOLICY policy = DWMNCRP_DISABLED;
        DwmSetWindowAttribute(m_hWnd, DWMWA_NCRENDERING_POLICY, &policy, sizeof policy);
        BOOL allow = FALSE;
        DwmSetWindowAttribute(m_hWnd, DWMWA_ALLOW_NCPAINT, &allow, sizeof allow);
        // Even with this countermeasure, there are still occasional cases where the
        // border is hidden by overwriting. In such cases, I used the workaround of
        // calling RedrawWindow there.
    }
}

//---------------------------------------------------
//関数名	OnSize(UINT nType, int cx, int cy)
//機能		リサイズ
//---------------------------------------------------
void CMyTreeDialog::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);
    if (m_pTreeCtrl->m_hWnd) {
        m_pTreeCtrl->SetWindowPos(&wndTop, 0, 0, cx, cy, SWP_NOMOVE);
        m_pTreeCtrl->setScrollBar();
    }
}

//---------------------------------------------------
//関数名	DestroyWindow()
//機能		ウィンド破棄時処理
//---------------------------------------------------
BOOL CMyTreeDialog::DestroyWindow()
{
    m_toolTip.DelTool(m_pTreeCtrl);
    if (m_findDialog) {
        delete m_findDialog;
        m_findDialog = nullptr;
    }
    return CDialog::DestroyWindow();
}

//---------------------------------------------------
//関数名	OnKeydownMyTree(NMHDR* pNMHDR, LRESULT* pResult)
//機能		キー押下処理
//---------------------------------------------------
void CMyTreeDialog::OnKeydownMyTree(NMHDR* pNMHDR, LRESULT* pResult)
{
    TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;

    *pResult = 0;
}

//---------------------------------------------------
//関数名	OnRclickMyTree(NMHDR* pNMHDR, LRESULT* pResult)
//機能		右ボタン押下処理
//---------------------------------------------------
void CMyTreeDialog::OnRclickMyTree(NMHDR* pNMHDR, LRESULT* pResult)
{
    if (m_isModal || m_pTreeCtrl->IsDragging()) return;
    POINT point;
    ::GetCursorPos(&point);
    PopupMenu(point);
    *pResult = 0;
}

//---------------------------------------------------
//関数名	OnClickMyTree(NMHDR* pNMHDR, LRESULT* pResult)
//機能		左ボタン押下処理
//---------------------------------------------------
void CMyTreeDialog::OnClickMyTree(NMHDR* pNMHDR, LRESULT* pResult)
{
    RECT TreeRect, ItemRect;
    POINT pCursolPos;
    HTREEITEM hClickItem;
    UINT Flags = 0;

    m_pTreeCtrl->GetWindowRect(&TreeRect);
    GetCursorPos(&pCursolPos);

    pCursolPos.x -= TreeRect.left;
    pCursolPos.y -= TreeRect.top;

    hClickItem = m_pTreeCtrl->HitTest(pCursolPos, &Flags);
    if (hClickItem) {
        if (m_pTreeCtrl->GetItemRect(hClickItem, &ItemRect, true)) {
            if (TVHT_ONITEMICON & Flags) {
                // Clicking on the icon of the one-time data item in the data tree view changes it to permanent data.
                STRING_DATA* pData = m_pTreeCtrl->getDataPtr(hClickItem);
                if (pData->m_cKind & KIND_ONETIME) {
                    pData->m_cKind = KIND_LOCK;
                    m_pTreeCtrl->editData2(hClickItem);
                }
            }
            if (TVHT_ONITEMSTATEICON & Flags) {
                // Clicked on checkbox
                m_pTreeCtrl->ToggleItemCheck(hClickItem);
                m_pTreeCtrl->SetCheck(hClickItem, !m_pTreeCtrl->GetCheck(hClickItem));
            }
        }
        m_pTreeCtrl->SelectItem(hClickItem);

        // Enter if m_bSingleEnter
        if (theApp.m_ini.m_bSingleEnter && !m_pTreeCtrl->IsDragging() && (TVHT_ONITEMSTATEICON & Flags) == 0) {
            if (theApp.m_ini.m_bSelectByTypingAutoPaste) KillTimer(CHARU_QUICK_TIMER);
            EnterData(GetClickedItem());
        }
    }
    *pResult = 0;
}

//---------------------------------------------------
//関数名	OnKillFocusMyTree(NMHDR* pNMHDR, LRESULT* pResult)
//機能		ツリーのフォーカスが外れたら隠す
//---------------------------------------------------
void CMyTreeDialog::OnKillFocusMyTree(NMHDR* pNMHDR, LRESULT* pResult)
{
    RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);

    if (m_isInitOK && !m_isModal && !m_bFind) {
        ::PostMessage(theApp.GetAppWnd(), WM_TREE_CLOSE, IDCANCEL, NULL);
        KillTimer(CHARU_QUICK_TIMER);
        m_isInitOK = false;
    }
    *pResult = 0;
}

void CMyTreeDialog::OnSetFocusMyTree(NMHDR* pNMHDR, LRESULT* pResult)
{
    RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
}

//---------------------------------------------------
//関数名	OnShowWindow(BOOL bShow, UINT nStatus)
//機能		ウィンドウ表示時処理
//---------------------------------------------------
void CMyTreeDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
    RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
    if (bShow) {
        //半透明処理
        LONG lExStyle = ::GetWindowLong(this->m_hWnd, GWL_EXSTYLE);
        if (m_pExStyle && theApp.m_ini.m_nOpacity < 100) {
            SetWindowLong(this->m_hWnd, GWL_EXSTYLE, lExStyle | 0x80000);
            int nTansparent = 255 * theApp.m_ini.m_nOpacity / 100;
            m_pExStyle(this->m_hWnd, 0, nTansparent, 2);
        }
        else {
            SetWindowLong(this->m_hWnd, GWL_EXSTYLE, lExStyle & 0xfff7ffff);
        }
        m_cOlgFont = m_pTreeCtrl->GetFont();
        m_cFont = new CFont;
        if (m_cFont) {
            m_cFont->CreatePointFont(theApp.m_ini.m_nFontSize, theApp.m_ini.m_strFontName);
            m_pTreeCtrl->SetFont(m_cFont, TRUE);
        }
    }
    else if (m_cFont) {
        RECT rect;
        GetWindowRect(&rect);
        theApp.m_ini.m_DialogSize.x = rect.right - rect.left;
        theApp.m_ini.m_DialogSize.y = rect.bottom - rect.top;
        //		if(m_cOlgFont) m_pTreeCtrl->SetFont(m_cOlgFont,FALSE);
        delete m_cFont;
    }
    CDialog::OnShowWindow(bShow, nStatus);
}

//---------------------------------------------------
//関数名	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//機能		背景色を変える
//---------------------------------------------------
HBRUSH CMyTreeDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    if (m_brBack.GetSafeHandle()) hbr = m_brBack;
    return hbr;
}

//---------------------------------------------------
//関数名	OnBeginlabeleditMyTree(NMHDR* pNMHDR, LRESULT* pResult)
//機能		ラベル編集開始
//---------------------------------------------------
void CMyTreeDialog::OnBeginlabeleditMyTree(NMHDR* pNMHDR, LRESULT* pResult)
{
    TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
    m_isModal = true;

    *pResult = 0;
}

//---------------------------------------------------
//関数名	OnEndlabeleditMyTree(NMHDR* pNMHDR, LRESULT* pResult)
//機能		ラベル編集終了
//---------------------------------------------------
void CMyTreeDialog::OnEndlabeleditMyTree(NMHDR* pNMHDR, LRESULT* pResult)
{
    TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

    //変更をツリーに反映
    m_pTreeCtrl->SetFocus();
    TV_ITEM* item = &(pTVDispInfo->item);
    if (item->pszText && *(item->pszText)) {
        HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem();
        STRING_DATA* pData = m_pTreeCtrl->getDataPtr(hTreeItem);
        pData->m_strTitle = item->pszText;
        m_pTreeCtrl->editData2(hTreeItem);
    }
    m_isModal = false;

    *pResult = 0;
}

STRING_DATA* CMyTreeDialog::GetClickedItem()
{
    HTREEITEM hTreeItem;
    hTreeItem = m_pTreeCtrl->GetSelectedItem();
    if (hTreeItem) {
        STRING_DATA* dataPtr = m_pTreeCtrl->getDataPtr(hTreeItem);
        if (!(dataPtr->m_cKind & KIND_FOLDER_ALL)) {
            return dataPtr;
        }
    }
    return nullptr;
}

//---------------------------------------------------
//関数名	PreTranslateMessage(MSG* pMsg)
//機能		メッセージ前処理
//---------------------------------------------------
BOOL CMyTreeDialog::PreTranslateMessage(MSG* pMsg)
{
    m_toolTip.RelayEvent(pMsg);

    switch (pMsg->message) {
    case WM_TREE_CLOSE:
        ClosePopup();
        return TRUE;

    case WM_LBUTTONDBLCLK:
        if (!m_pTreeCtrl->IsDragging()) {
            if (theApp.m_ini.m_bSelectByTypingAutoPaste) KillTimer(CHARU_QUICK_TIMER);
            m_dataPtrDbClick = GetClickedItem();
        }
        break;

    case WM_LBUTTONUP:
        if (!m_pTreeCtrl->IsDragging()) {
            if (m_dataPtrDbClick != nullptr) {
                EnterData(m_dataPtrDbClick);
                return TRUE;
            }
        }
        break;

    case WM_FIND_CLOSE:
        if (m_bFind) {
            GetFindParam();
            m_findDialog->DestroyWindow();
            m_bFind = false;
            RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
        }
        break;

    case WM_FIND_ONCE:
        if (m_bFind) {
            GetFindParam();
            FindNext(false);
            m_findDialog->DestroyWindow();
            m_bFind = false;
            RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
        }
        break;

    case WM_FIND_NEXT:
        if (m_bFind) {
            GetFindParam();
            FindNext(false);
        }
        break;

    case WM_FIND_PREV:
        if (m_bFind) {
            GetFindParam();
            FindNext(true);
        }
        break;

    case WM_TIPS_CHANGE:
    {
        HTREEITEM hTarget = (HTREEITEM)pMsg->wParam;
        if (hTarget) {
            ChangeTipString(m_pTreeCtrl->getDataPtr(hTarget));
        }
        else {
            m_toolTip.Activate(FALSE);
        }
    }
    break;
    }

    if (WM_SYSKEYDOWN == pMsg->message) m_isAltDown = true;
    else if (((WM_SYSKEYUP == pMsg->message && m_isAltDown) || (WM_KEYDOWN == pMsg->message && pMsg->wParam == VK_APPS)) && !theApp.isCloseKey() && !m_pTreeCtrl->IsDragging()) {
        if (theApp.m_ini.m_bSelectByTypingAutoPaste) KillTimer(CHARU_QUICK_TIMER);
        m_isAltDown = false;
        CPoint point;
        ZeroMemory(&point, sizeof(point));
        RECT rSelItem;
        HTREEITEM hSelectItem = m_pTreeCtrl->GetSelectedItem();//選択位置を取得
        if (hSelectItem) {
            m_pTreeCtrl->GetItemRect(hSelectItem, &rSelItem, NULL);
            point.x = 0;
            point.y = rSelItem.bottom - 5;
        }
        ClientToScreen(&point);
        PopupMenu(point);
        //		return true;
    }

    if (WM_KEYDOWN == pMsg->message) {
        if (!m_pTreeCtrl->IsDragging()) {
            MSG msg;
            ZeroMemory(&msg, sizeof(msg));
            msg.message = WM_LBUTTONDOWN;
            msg.hwnd = this->m_hWnd;
            m_toolTip.RelayEvent(&msg);

            ::PostMessage(m_pTreeCtrl->m_hWnd, WM_MOUSEMOVE, 0, 0);
            ::SetCursor(NULL);
        }
        //ESCで閉じる
        if (VK_ESCAPE == pMsg->wParam && !m_isModal && !m_pTreeCtrl->IsDragging()) {
            ClosePopup();
            return TRUE;
        }
        //ラベル編集中なら編集終了
        else if (VK_RETURN == pMsg->wParam && m_isModal) {
            m_pTreeCtrl->SetFocus();
            return TRUE;
        }
        //リターンキーを押したら決定
        else if (VK_RETURN == pMsg->wParam && !m_pTreeCtrl->IsDragging()) {
            if (theApp.m_ini.m_bSelectByTypingAutoPaste) KillTimer(CHARU_QUICK_TIMER);
            //データ取得
            HTREEITEM hTreeItem;
            hTreeItem = m_pTreeCtrl->GetSelectedItem();
            if (hTreeItem) {
                STRING_DATA* dataPtr = m_pTreeCtrl->getDataPtr(hTreeItem);

                //フォルダか確認
                if (!(dataPtr->m_cKind & KIND_FOLDER_ALL) || (m_pTreeCtrl->GetStyle() & TVS_CHECKBOXES && m_pTreeCtrl->GetCheck(hTreeItem))) {
                    EnterData(dataPtr);//データを決定
                }
                else
                    m_pTreeCtrl->Expand(hTreeItem, TVE_TOGGLE);
                return TRUE;
            }
        }
        //スペースキー
        else if (VK_SPACE == pMsg->wParam && !m_isModal) {
            if (theApp.m_ini.m_bSelectByTypingAutoPaste) KillTimer(CHARU_QUICK_TIMER);
            HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem();
            if (hTreeItem) {
                bool goBackwards = ::GetKeyState(VK_SHIFT) < 0;
                if (::GetKeyState(VK_CONTROL) < 0) {
                    m_pTreeCtrl->ToggleItemCheck(m_pTreeCtrl->GetSelectedItem());
                    hTreeItem = goBackwards ? m_pTreeCtrl->GetPrevSiblingItem(hTreeItem) : m_pTreeCtrl->GetNextSiblingItem(hTreeItem);
                }
                else {
                    if (goBackwards) {
                        hTreeItem = m_pTreeCtrl->GetPrevVisibleItem(hTreeItem);
                        if (!hTreeItem) {
                            hTreeItem = m_pTreeCtrl->getLastVisibleItem();
                        }
                    }
                    else {
                        hTreeItem = m_pTreeCtrl->GetNextVisibleItem(hTreeItem);
                        if (!hTreeItem) {
                            hTreeItem = m_pTreeCtrl->GetRootItem();
                        }
                    }
                }
                if (hTreeItem) {
                    m_pTreeCtrl->SelectItem(hTreeItem);
                }
            }
            else {//選択されてなかったら0番目を選択
                m_pTreeCtrl->Select(m_pTreeCtrl->GetRootItem(), TVGN_FIRSTVISIBLE);
            }
            return true;
        }
        //デリートキー(データ削除)
        else if (VK_DELETE == pMsg->wParam && !m_pTreeCtrl->IsDragging() && !m_isModal) {
            if (theApp.m_ini.m_bSelectByTypingAutoPaste) KillTimer(CHARU_QUICK_TIMER);
            OnDelete();
        }
        //F1キー 内容表示
        else if (VK_F1 == pMsg->wParam && !m_pTreeCtrl->IsDragging()) {
            CPoint point;
            ZeroMemory(&point, sizeof(point));
            RECT rSelItem;
            HTREEITEM hSelectItem = m_pTreeCtrl->GetSelectedItem();//選択位置を取得
            if (hSelectItem) {
                m_pTreeCtrl->GetItemRect(hSelectItem, &rSelItem, true);
                point.x = rSelItem.left + ((rSelItem.right - rSelItem.left) / 2);
                point.y = rSelItem.top + ((rSelItem.bottom - rSelItem.top) / 2);

                MSG msg;
                ZeroMemory(&msg, sizeof(msg));
                msg.message = WM_MOUSEMOVE;
                msg.wParam = NULL;
                msg.lParam = point.y;
                msg.lParam = (msg.lParam << 16) + point.x;
                msg.hwnd = m_pTreeCtrl->m_hWnd;

                ::PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
                ClientToScreen(&point);
                ::SetCursorPos(point.x, point.y);
            }
        }
        // F2 : Rename
        else if (VK_F2 == pMsg->wParam && !m_pTreeCtrl->IsDragging()) {
            if (theApp.m_ini.m_bSelectByTypingAutoPaste) KillTimer(CHARU_QUICK_TIMER);
            if (m_pTreeCtrl->GetSelectedItem())
                m_pTreeCtrl->EditLabel(m_pTreeCtrl->GetSelectedItem());
        }
        // F3 : Find Next
        else if (VK_F3 == pMsg->wParam && !m_pTreeCtrl->IsDragging() && !m_isModal) {
            if (m_bFind) {
                GetFindParam();
            }
            FindNext(::GetKeyState(VK_SHIFT) < 0);
        }
        // Ctrl+F : Open Find dialog
        else if (::GetKeyState(VK_CONTROL) < 0 && pMsg->wParam == 'F' && !m_pTreeCtrl->IsDragging() && !m_isModal) {
            OnListSearch();
            return true;
        }
        //TABキーでチェック
        else if (VK_TAB == pMsg->wParam && !m_pTreeCtrl->IsDragging() && !m_isModal) {
            m_pTreeCtrl->ToggleItemCheck(m_pTreeCtrl->GetSelectedItem());
            return true;
        }
        //上下
        else if (VK_DOWN == pMsg->wParam || VK_UP == pMsg->wParam && !m_isModal) {
            if (theApp.m_ini.m_bSelectByTypingAutoPaste) KillTimer(CHARU_QUICK_TIMER);

            HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem(), hTreeItemTmp;
            if (hTreeItem != NULL && ::GetKeyState(VK_SHIFT) < 0) {//選択されていて、SHIFTを押してる
                do {
                    hTreeItemTmp = hTreeItem;
                    if (VK_DOWN == pMsg->wParam)	hTreeItem = m_pTreeCtrl->GetNextVisibleItem(hTreeItem);
                    else							hTreeItem = m_pTreeCtrl->GetPrevVisibleItem(hTreeItem);
                    if (m_pTreeCtrl->GetChildItem(hTreeItem)) break;
                } while (hTreeItem);
                if (hTreeItem) {
                    m_pTreeCtrl->SelectItem(hTreeItem);
                    return true;
                }
            }
        }
        //左
        else if (VK_LEFT == pMsg->wParam && !m_isModal) {
            if (theApp.m_ini.m_bSelectByTypingAutoPaste) KillTimer(CHARU_QUICK_TIMER);
            if (::GetKeyState(VK_SHIFT) < 0) {
                if (::GetKeyState(VK_CONTROL) < 0) {
                    m_pTreeCtrl->closeFolder(m_pTreeCtrl->GetRootItem()); // Ctrl+Shift+Left: collapse all
                }
                else {
                    HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem();
                    if (hTreeItem) {
                        hTreeItem = m_pTreeCtrl->searchMyRoots(hTreeItem);
                        if (hTreeItem) {
                            m_pTreeCtrl->SelectItem(hTreeItem); // Shift+Left: collapse parent
                        }
                    }
                }
            }
        }
        // "select by typing"
        else if (!m_isModal && !m_pTreeCtrl->IsDragging()) {
            if (SelectByTyping((UINT)pMsg->wParam)) return TRUE;
        }
    }
    // "select by typing" 確定処理
    else if (WM_TIMER == pMsg->message && CHARU_QUICK_TIMER == pMsg->wParam && !m_pTreeCtrl->IsDragging()) {
        BOOL retval = FALSE;
        if (m_hQuickItem) {
            STRING_DATA* dataPtr = m_pTreeCtrl->getDataPtr(m_hQuickItem);
            if (dataPtr->m_cKind & KIND_DATA_ALL) {
                if (theApp.m_ini.m_bSelectByTypingAutoPaste) {
                    EnterData(dataPtr);
                }
            }
            else if (dataPtr->m_cKind & KIND_FOLDER_ALL) {
                if (theApp.m_ini.m_bSelectByTypingAutoExpand) {
                    m_pTreeCtrl->Expand(m_hQuickItem, TVE_EXPAND);
                }
            }
            retval = TRUE;
        }
        this->KillTimer(CHARU_QUICK_TIMER);
        return retval;
    }

    return CDialog::PreTranslateMessage(pMsg);
}

void CMyTreeDialog::ClosePopup()
{
    if (m_bFind) {
        m_findDialog->DestroyWindow();
        m_bFind = false;
    }
    ::PostMessage(theApp.GetAppWnd(), WM_TREE_CLOSE, IDCANCEL, NULL);
    m_isInitOK = false;
    KillTimer(CHARU_QUICK_TIMER);
}

//---------------------------------------------------
//関数名	enterData(list<STRING_DATA>::iterator it)
//機能		データを決定してダイアログを隠蔽
//---------------------------------------------------
void CMyTreeDialog::EnterData(STRING_DATA* dataPtr)
{
    if (dataPtr) {
        if (m_bFind) {
            m_findDialog->DestroyWindow();
            m_bFind = false;
        }
        m_selectDataPtr = dataPtr;
        ::PostMessage(theApp.GetAppWnd(), WM_TREE_CLOSE, IDOK, NULL);
        m_isInitOK = false;
        this->KillTimer(CHARU_QUICK_TIMER);
    }
}

//---------------------------------------------------
//関数名	OnEdit()
//機能		選択項目を編集
//---------------------------------------------------
void CMyTreeDialog::OnEdit()
{
    if (m_isModal) return;
    m_isModal = true;
    HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem();
    CEditDialog dlgEdit(this, m_pTreeCtrl->getDataPtr(hTreeItem), false);
    if (dlgEdit.DoModal() == IDOK) {
        m_pTreeCtrl->editData2(hTreeItem);
    }
    m_isModal = false;
}

//---------------------------------------------------
//関数名	OnDelete()
//機能		選択項目を削除
//---------------------------------------------------
void CMyTreeDialog::OnDelete()
{
    HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem();
    if (!hTreeItem) {
        return;
    }
    if (m_isModal) {
        return;
    }
    m_isModal = true;
    STRING_DATA* data = m_pTreeCtrl->getDataPtr(hTreeItem);
    CString strMessage;
    strMessage.Format(APP_MES_DELETE_OK, data->m_strTitle);
    int nRet = AfxMessageBox(strMessage, MB_YESNO | MB_ICONEXCLAMATION | MB_APPLMODAL);
    RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
    if (nRet == IDYES) {
        if ((data->m_cKind & KIND_FOLDER_ALL) && m_pTreeCtrl->ItemHasChildren(hTreeItem)) {
            (void)strMessage.LoadString(APP_MES_DELETE_FOLDER);
            nRet = AfxMessageBox(strMessage, MB_YESNO | MB_ICONEXCLAMATION | MB_APPLMODAL);
            RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
        }
    }
    if (nRet == IDYES) {
        m_pTreeCtrl->deleteData(hTreeItem);
    }
    m_pTreeCtrl->SetFocus();
    m_isModal = false;
}

//---------------------------------------------------
//関数名	OnRename()
//機能		選択項目の名前の変更
//---------------------------------------------------
void CMyTreeDialog::OnRename()
{
    HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem();
    if (!hTreeItem) return;

    m_pTreeCtrl->EditLabel(hTreeItem);
}

//---------------------------------------------------
//関数名	OnIcon～
//機能		データアイコンを変更
//---------------------------------------------------

//クリップボードマクロ
void CMyTreeDialog::OnIconClip()
{
    HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem();
    if (hTreeItem)	m_pTreeCtrl->changeIcon(hTreeItem, KIND_CLIP);
}
//日付マクロ
void CMyTreeDialog::OnIconDate()
{
    HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem();
    if (hTreeItem)	m_pTreeCtrl->changeIcon(hTreeItem, KIND_DATE);
}
//シェルマクロ
void CMyTreeDialog::OnIconExe()
{
    HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem();
    if (hTreeItem)	m_pTreeCtrl->changeIcon(hTreeItem, KIND_EXE);
}
//普通項目
void CMyTreeDialog::OnIconKey()
{
    HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem();
    if (hTreeItem)	m_pTreeCtrl->changeIcon(hTreeItem, 0);
}
//キーボードマクロ
void CMyTreeDialog::OnIconKeymacro()
{
    HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem();
    if (hTreeItem)	m_pTreeCtrl->changeIcon(hTreeItem, KIND_KEY);
}
//プラグイン
void CMyTreeDialog::OnIconPlugin()
{
    HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem();
    if (hTreeItem)	m_pTreeCtrl->changeIcon(hTreeItem, KIND_PLUG);
}
//関連付け実行マクロ
void CMyTreeDialog::OnIconRelate()
{
    HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem();
    if (hTreeItem)	m_pTreeCtrl->changeIcon(hTreeItem, KIND_RELATE);
}
//選択テキストマクロ
void CMyTreeDialog::OnIconSelect()
{
    HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem();
    if (hTreeItem)	m_pTreeCtrl->changeIcon(hTreeItem, KIND_SELECT);
}

//---------------------------------------------------
//関数名	OnMakeOnetime()
//機能		ワンタイムデータに変更
//---------------------------------------------------
void CMyTreeDialog::OnMakeOnetime()
{
    HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem();
    if (hTreeItem) {
        STRING_DATA* pData = m_pTreeCtrl->getDataPtr(hTreeItem);
        pData->m_cKind = KIND_ONETIME;
        m_pTreeCtrl->editData2(hTreeItem);
    }
}

//---------------------------------------------------
//関数名	OnMakePermanent()
//機能		恒久データに変更
//---------------------------------------------------
void CMyTreeDialog::OnMakePermanent()
{
    HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem();
    if (hTreeItem) {
        STRING_DATA* pData = m_pTreeCtrl->getDataPtr(hTreeItem);
        pData->m_cKind = KIND_LOCK;
        m_pTreeCtrl->editData2(hTreeItem);
    }
}

//---------------------------------------------------
//関数名	OnFolderClear()
//機能		選択フォルダをクリア
//---------------------------------------------------
void CMyTreeDialog::OnFolderClear()
{
    if (m_isModal) {
        return;
    }
    m_isModal = true;
    HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem();
    if (hTreeItem) {
        //フォルダを再帰で削除
        CString strRes;
        (void)strRes.LoadString(APP_MES_FOLDER_CLEAR);
        int nRet = AfxMessageBox(strRes, MB_YESNO | MB_ICONEXCLAMATION | MB_APPLMODAL);
        RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
        if (nRet == IDYES) {
            HTREEITEM hChildItem = m_pTreeCtrl->GetChildItem(hTreeItem);
            m_pTreeCtrl->clearFolder(hChildItem);
        }
    }
    m_pTreeCtrl->SetFocus();
    m_isModal = false;
}

//---------------------------------------------------
//関数名	OnAdd()
//機能		新規データを追加
//---------------------------------------------------
void CMyTreeDialog::OnNewData()
{
    if (m_isModal) {
        return;
    }
    m_isModal = true;
    STRING_DATA data;
    CEditDialog editDialog(this, &data, true);
    if (editDialog.DoModal() == IDOK) {
        HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem();
        m_pTreeCtrl->SelectItem(theApp.m_pTree->addData(hTreeItem, data));
    }
    m_isModal = false;
}

//---------------------------------------------------
//関数名	OnNewFolder()
//機能		新規フォルダを追加
//---------------------------------------------------
void CMyTreeDialog::OnNewFolder()
{
    HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem();
    CString strRes;
    (void)strRes.LoadString(APP_INF_NEW_FOLDER);
    m_pTreeCtrl->EditLabel(m_pTreeCtrl->addNewFolder(hTreeItem, strRes));
}

void CMyTreeDialog::OnReselectIcons()
{
    if (m_isModal) {
        return;
    }
    m_isModal = true;
    CString strRes;
    (void)strRes.LoadString(APP_MES_DECIDE_ICONS);
    int nRet = AfxMessageBox(strRes, MB_YESNO | MB_ICONEXCLAMATION | MB_APPLMODAL);
    RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
    if (nRet == IDYES) {
        m_pTreeCtrl->allIconCheck();
    }
    m_isModal = false;
}

void CMyTreeDialog::OnCleanupAllOnetime()
{
    m_pTreeCtrl->cleanupOneTimeItems(m_pTreeCtrl->GetRootItem());
}

void CMyTreeDialog::OnMakeAllOnetimePermanent()
{
    m_pTreeCtrl->cleanupOneTimeItems(m_pTreeCtrl->GetRootItem(), KIND_LOCK);
}

void CMyTreeDialog::OnCloseAll()
{
    m_pTreeCtrl->closeFolder(m_pTreeCtrl->GetRootItem());
}

//---------------------------------------------------
//関数名	OnListSearch()
//機能		検索処理
//---------------------------------------------------
void CMyTreeDialog::OnListSearch()
{
    if (m_isModal || m_bFind || !m_findDialog) {
        return;
    }
    m_bFind = true;
    m_findDialog->Create(IDD_SEARCH, this);
    m_findDialog->ShowWindow(SW_SHOW);
}

void CMyTreeDialog::OnCheckItem()
{
    m_pTreeCtrl->ToggleItemCheck(m_pTreeCtrl->GetSelectedItem());
    m_bCheckbox = true;
}

//---------------------------------------------------
//関数名	OnCopyData()
//機能		選択データをコピー
//---------------------------------------------------
void CMyTreeDialog::OnCopyData()
{
    HTREEITEM hTreeItem;
    hTreeItem = m_pTreeCtrl->GetSelectedItem();
    if (hTreeItem) {
        m_hCopyData = hTreeItem;
    }
}

//---------------------------------------------------
//関数名	OnDataPaste()
//機能		コピーデータを貼り付け
//---------------------------------------------------
void CMyTreeDialog::OnDataPaste()
{
    if (m_hCopyData && !m_isModal) {
        HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem();
        STRING_DATA data = m_pTreeCtrl->getData(m_hCopyData);
        CString strRes;
        (void)strRes.LoadString(APP_INF_COPY);
        data.m_strTitle = data.m_strTitle + strRes;
        if (m_pTreeCtrl->ItemHasChildren(m_hCopyData)) {
            if (!m_pTreeCtrl->checkMyChild(m_hCopyData, hTreeItem)) {
                hTreeItem = m_pTreeCtrl->addData(hTreeItem, data);
                m_pTreeCtrl->copyChildren(m_hCopyData, hTreeItem);
            }
            else {
                m_isModal = true;
                CString strRes;
                (void)strRes.LoadString(APP_MES_CANT_COPY);
                AfxMessageBox(strRes, MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
                m_pTreeCtrl->SetFocus();
                m_isModal = false;
            }
        }
        else {
            m_pTreeCtrl->addData(hTreeItem, data);
        }
    }
}

//---------------------------------------------------
//関数名	OnImport()
//機能		インポート処理
//---------------------------------------------------
void CMyTreeDialog::OnImport()
{
    if (m_isModal) {
        return;
    }
    m_isModal = true;

    CString strDisplay, strPattern;
    (void)strDisplay.LoadString(APP_INF_FILE_FILTER_C3D_DISPLAY);
    (void)strPattern.LoadString(APP_INF_FILE_FILTER_C3D_PATTERN);
    CString strFilter = strDisplay + _T('\0') + strPattern + _T('\0');
    for (std::vector<RW_PLUGIN>::iterator it = m_pTreeCtrl->m_rwPlugin.begin(); it != m_pTreeCtrl->m_rwPlugin.end(); it++) {
        CString strFormat, strDisplay, strPattern;
        (void)strFormat.LoadString(APP_INF_FILE_FILTER_FMT_DISPLAY);
        strDisplay.Format(strFormat, it->m_strFormatName, it->m_strExtension);
        (void)strFormat.LoadString(APP_INF_FILE_FILTER_FMT_PATTERN);
        strPattern.Format(strFormat, it->m_strExtension);
        strFilter = strFilter + strDisplay + _T('\0') + strPattern + _T('\0'); // NOTE: Don't use operator +=
    }
    strFilter = strFilter + _T('\0') + _T('\0'); // NOTE: Don't use operator +=

    OPENFILENAME param;
    ZeroMemory(&param, sizeof param);
    TCHAR tcPath[MAX_PATH] = _T("");
    param.lStructSize = sizeof param;
    param.hwndOwner = theApp.GetAppWnd();
    param.lpstrFilter = strFilter.GetBuffer();
    param.lpstrCustomFilter = NULL;
    param.nFilterIndex = 1;
    param.lpstrFile = tcPath;
    param.nMaxFile = MAX_PATH;
    param.lpstrFileTitle = NULL;
    param.lpstrInitialDir = NULL;
    param.lpstrTitle = NULL;
    param.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    param.nFileOffset = 0;
    param.nFileExtension = 0;
    param.lpstrDefExt = NULL;
    if (GetOpenFileName(&param)) {
        if (theApp.m_ini.m_bDebug) {
            LOG(_T("OnImport \"%s\""), tcPath);
        }
        CString format = param.nFilterIndex < 2 ? DAT_FORMAT : m_pTreeCtrl->m_rwPlugin[param.nFilterIndex - 2].m_strFormatName;
        std::list<STRING_DATA> tmplist;
        m_pTreeCtrl->loadDataFile(CString(tcPath), format, &tmplist);
        HTREEITEM hTreeItem = m_pTreeCtrl->mergeTreeData(m_pTreeCtrl->GetSelectedItem(), &tmplist, false);

        CString strRes;
        CString strMessage;
        (void)strRes.LoadString(APP_MES_IMPORT_OK);
        strMessage.Format(strRes, tmplist.size());
        AfxMessageBox(strMessage, MB_OK | MB_APPLMODAL);

        if (hTreeItem) m_pTreeCtrl->SelectItem(hTreeItem);
    }
    m_pTreeCtrl->SetFocus();
    m_isModal = false;
}

//---------------------------------------------------
//関数名	OnExport()
//機能		エクスポート処理
//---------------------------------------------------
void CMyTreeDialog::OnExport()
{
    if (m_isModal) {
        return;
    }
    m_isModal = true;
    HTREEITEM hTreeItem = m_pTreeCtrl->GetSelectedItem();
    if (hTreeItem) {
        CString file = theApp.NewFile();
        if (file != _T("")) {
            if (!m_pTreeCtrl->saveDataToFile(file, DAT_FORMAT, hTreeItem)) {
                CString strRes;
                (void)strRes.LoadString(APP_MES_EXPORT_FAILURE);
                AfxMessageBox(strRes, MB_ICONEXCLAMATION, 0);
            }
        }
    }
    m_pTreeCtrl->SetFocus();
    m_isModal = false;
}

//---------------------------------------------------
//関数名	OnOption()
//機能		設定
//---------------------------------------------------
void CMyTreeDialog::OnOption()
{
    if (m_isModal) return;
    m_isModal = true;
    theApp.OnOption();
    m_isModal = false;
}

//---------------------------------------------------
//関数名	selectByTyping(UINT uKeyCode)
//機能		"select by typing" 処理
//---------------------------------------------------
bool CMyTreeDialog::SelectByTyping(UINT uKeyCode)
{
    bool isRet = false;
    BYTE byteKey[256];
    BOOL valid = GetKeyboardState(byteKey);
    char strbuff[16] = {};
    if (valid && ToAsciiEx(uKeyCode, 0, byteKey, (LPWORD)strbuff, 0, theApp.m_ini.m_keyLayout) == 1) {
        strbuff[1] = NULL;
        DWORD span = timeGetTime() - m_dwStartTime;
        m_dwStartTime = timeGetTime();
        m_hQuickItem = m_pTreeCtrl->GetSelectedItem();
        if (span >= static_cast<DWORD>(theApp.m_ini.m_nSelectByTypingFinalizePeriod)) {
            // Reset findkey, find from the next item onward, and reset timer
            m_strQuickKey = "";
            if (m_hQuickItem) {
                m_hQuickItem = m_pTreeCtrl->getTrueNextItem(m_hQuickItem);
            }
            if (theApp.m_ini.m_bSelectByTypingAutoPaste || theApp.m_ini.m_bSelectByTypingAutoExpand) {
                this->KillTimer(CHARU_QUICK_TIMER);
                this->SetTimer(CHARU_QUICK_TIMER, theApp.m_ini.m_nSelectByTypingFinalizePeriod, NULL);
            }
        }
        CString strKey = CString(strbuff);
        if (theApp.m_ini.m_bSelectByTypingCaseInsensitive) {
            strKey.MakeLower();
        }
        m_strQuickKey += strKey;
        if (!m_hQuickItem) {
            m_hQuickItem = m_pTreeCtrl->GetRootItem();
        }
        if (m_hQuickItem) {
            m_hQuickItem = m_pTreeCtrl->searchTitle(m_hQuickItem, m_strQuickKey, theApp.m_ini.m_bSelectByTypingCaseInsensitive);
            if (m_hQuickItem) {
                m_pTreeCtrl->SelectItem(m_hQuickItem);
                isRet = true;//標準のインクリメンタルサーチをキャンセル
            }
        }
    }
    return isRet;
}

//---------------------------------------------------
//関数名	changeTipString(CString strData)
//機能		引数のテキストをツールチップに設定
//---------------------------------------------------
void CMyTreeDialog::ChangeTipString(STRING_DATA* data)
{
    CString strTip = _T("");
    CString strRes;
    bool gap = false;

    m_toolTip.Activate(FALSE);
    if (theApp.m_ini.m_nToolTip == 0) {
        (void)strRes.LoadString(APP_INF_TIP_DATA01);
        CString s = data->m_strTitle;
        int max = 100;
        if (s.GetLength() > max) {
            s = s.Left(max - 3) + _T("...");
        }
        strTip += strRes + s;
        gap = true;
    }
    if (theApp.m_ini.m_nToolTip != 2) {
        if (data->m_strData != _T("")) {
            if (gap) strTip += _T("\n\n");
            CString s = data->m_strData;
            (void)s.Replace(_T('\t'), _T(' '));
            int max = 500;
            if (s.GetLength() > max) {
                s = s.Left(max - 3) + _T("...");
            }
            strTip += s;
            gap = true;
        }
        if (data->m_strMacro != _T("")) {
            (void)strRes.LoadString(APP_INF_TIP_DATA02);
            CString s = data->m_strMacro;
            (void)s.Replace(_T('\t'), _T(' '));
            (void)s.Replace(_T("\n"), _T("\n  "));
            int max = 300;
            if (s.GetLength() > max) {
                s = s.Left(max - 3) + _T("...");
            }
            if (gap) strTip += _T("\n");
            strTip += strRes + s;
            gap = true;
        }
    }
    if (theApp.m_ini.m_nToolTip == 0) {
        if (gap) strTip += _T("\n");
        (void)strRes.LoadString(APP_INF_TIP_DATA03);
        strTip += strRes + CTime(data->m_timeCreate).Format(_T("%x %X"));
        (void)strRes.LoadString(APP_INF_TIP_DATA04);
        strTip += strRes + CTime(data->m_timeEdit).Format(_T("%x %X"));
    }
    m_toolTip.UpdateTipText(strTip, m_pTreeCtrl); // NOTE: Experiments have shown that if strTip exceeds 1024 characters, the app will crash after MFC fails with Debug Assertion Failed.
    m_toolTip.Activate(TRUE);
}

void CMyTreeDialog::GetFindParam()
{
    if (m_findDialog) {
        theApp.m_ini.m_nSearchTarget = m_findDialog->GetTarget();
        theApp.m_ini.m_nSearchLogic = m_findDialog->GetSearchLogic();
        theApp.m_ini.m_bSearchCaseInsensitive = m_findDialog->GetCaseInsensitive();
        theApp.m_ini.m_strSearchKeywords = m_findDialog->GetSearchText();
        theApp.m_ini.writeEnvInitData();
    }
}

void CMyTreeDialog::FindNext(bool backward)
{
    HTREEITEM hSearchItem = m_pTreeCtrl->GetSelectedItem();
    hSearchItem = m_pTreeCtrl->searchItem(hSearchItem, backward);
    if (hSearchItem) {
        m_pTreeCtrl->SelectItem(hSearchItem);
    }
    else {
        PlaySound((LPCTSTR)SND_ALIAS_SYSTEMASTERISK, NULL, SND_ASYNC | SND_ALIAS_ID);
    }
}
