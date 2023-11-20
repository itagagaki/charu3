/*----------------------------------------------------------
    一般設定クラス
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "OptEtc.h"
#include "Charu3.h" // for theApp.m_ini
#include "key.h"
#include "General.h"
#include "resource.h"

//---------------------------------------------------
//関数名	COptEtc
//機能		コンストラクタ
//---------------------------------------------------
COptEtc::COptEtc(CWnd* pParent /*=NULL*/) : CDialog(COptEtc::IDD, pParent)
{
    //{{AFX_DATA_INIT(COptEtc)
    //}}AFX_DATA_INIT
    m_nPutBackClipboard = CGeneral::BoolToInt(theApp.m_ini.m_etc.m_bPutBackClipboard);
    m_nShowClipboardInNotifyIconTooltip = CGeneral::BoolToInt(theApp.m_ini.m_etc.m_bShowClipboardInTooltipOfNofifyIcon);
}

//---------------------------------------------------
//関数名	DoDataExchange(CDataExchange* pDX)
//機能		データエクスチェンジ
//---------------------------------------------------
void COptEtc::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    //{{AFX_DATA_MAP(COptEtc)

    if (GetDlgItem(IDC_OPT_KEYRADIO_POP))
        DDX_Radio(pDX, IDC_OPT_KEYRADIO_POP, theApp.m_ini.m_pop.m_nDoubleKeyPOP);
    if (GetDlgItem(IDC_OPT_POPUPKEY))
        DDX_Control(pDX, IDC_OPT_POPUPKEY, m_ctrlPopupKey);

    if (GetDlgItem(IDC_OPT_KEYRADIO_FIFO))
        DDX_Radio(pDX, IDC_OPT_KEYRADIO_FIFO, theApp.m_ini.m_pop.m_nDoubleKeyFIFO);
    if (GetDlgItem(IDC_OPT_FIFO_KEY))
        DDX_Control(pDX, IDC_OPT_FIFO_KEY, m_ctrlFifoKey);

    if (GetDlgItem(IDC_OPT_ICON_POP))
        DDX_Radio(pDX, IDC_OPT_ICON_POP, theApp.m_ini.m_etc.m_nIconClick);
    if (GetDlgItem(IDC_OPT_TOCLIP))
        DDX_Check(pDX, IDC_OPT_TOCLIP, m_nPutBackClipboard);
    if (GetDlgItem(IDC_OPT_TOOLTIP_TITLE))
        DDX_Check(pDX, IDC_OPT_TOOLTIP_TITLE, m_nShowClipboardInNotifyIconTooltip);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptEtc, CDialog)
    //{{AFX_MSG_MAP(COptEtc)
    ON_BN_CLICKED(IDC_OPT_KEYRADIO_POP, hotkeyEnablePOP)
    ON_BN_CLICKED(IDC_OPT_KEYRADIO_POP2, hotkeyDisablePOP)
    ON_BN_CLICKED(IDC_OPT_KEYRADIO_POP3, hotkeyDisablePOP)
    ON_BN_CLICKED(IDC_OPT_KEYRADIO_POP4, hotkeyDisablePOP)
    ON_BN_CLICKED(IDC_OPT_KEYRADIO_FIFO, hotkeyEnableFIFO)
    ON_BN_CLICKED(IDC_OPT_KEYRADIO_FIFO2, hotkeyDisableFIFO)
    ON_BN_CLICKED(IDC_OPT_KEYRADIO_FIFO3, hotkeyDisableFIFO)
    ON_BN_CLICKED(IDC_OPT_KEYRADIO_FIFO4, hotkeyDisableFIFO)
    ON_WM_SHOWWINDOW()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------
// COptEtc メッセージ ハンドラ
//---------------------------------------------------

//---------------------------------------------------
//関数名	PreTranslateMessage(MSG* pMsg)
//機能		メッセージ前処理
//---------------------------------------------------
BOOL COptEtc::PreTranslateMessage(MSG* pMsg)
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
BOOL COptEtc::OnInitDialog()
{
    CDialog::OnInitDialog();

    //ホットキー
    if (theApp.m_ini.m_pop.m_nDoubleKeyPOP != 0)	m_ctrlPopupKey.EnableWindow(false);
    if (theApp.m_ini.m_pop.m_nDoubleKeyFIFO != 0)	m_ctrlFifoKey.EnableWindow(false);

    UINT uPopKey, uPopMod, uRirekiKey, uRirekiMod;
    theApp.m_ini.getHotKey(&uPopKey, &uPopMod, &uRirekiKey, &uRirekiMod);

    //コントロールにセット
    uPopMod = KeyHelper::ModToHotkey(uPopMod);
    m_ctrlPopupKey.SetHotKey(uPopKey, uPopMod);
    uRirekiMod = KeyHelper::ModToHotkey(uRirekiMod);
    m_ctrlFifoKey.SetHotKey(uRirekiKey, uRirekiMod);

    return TRUE;
}

void COptEtc::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialog::OnShowWindow(bShow, nStatus);
    if (bShow) {
        CWnd* w;
        switch (theApp.m_ini.m_pop.m_nDoubleKeyPOP) {
        default:
        case 0:
            w = GetDlgItem(IDC_OPT_KEYRADIO_POP);
            break;
        case 1:
            w = GetDlgItem(IDC_OPT_KEYRADIO_POP2);
            break;
        case 2:
            w = GetDlgItem(IDC_OPT_KEYRADIO_POP3);
            break;
        case 3:
            w = GetDlgItem(IDC_OPT_KEYRADIO_POP4);
            break;
        }
        if (w) w->SetFocus();
    }
}

//---------------------------------------------------
//関数名	DestroyWindow()
//機能		設定を反映
//---------------------------------------------------
BOOL COptEtc::DestroyWindow()
{
    WORD wVkCodeP, wModP;
    WORD wVkCodeF, wModF;

    m_ctrlPopupKey.GetHotKey(wVkCodeP, wModP);
    wModP = KeyHelper::HotkeyToMod(wModP);
    m_ctrlFifoKey.GetHotKey(wVkCodeF, wModF);
    wModF = KeyHelper::HotkeyToMod(wModF);
    theApp.m_ini.setHotkey(wVkCodeP, wModP, wVkCodeF, wModF);
    theApp.m_ini.m_etc.m_bPutBackClipboard = ((CButton*)GetDlgItem(IDC_OPT_TOCLIP))->GetCheck() != 0;
    theApp.m_ini.m_etc.m_bShowClipboardInTooltipOfNofifyIcon = ((CButton*)GetDlgItem(IDC_OPT_TOOLTIP_TITLE))->GetCheck() != 0;

    return CDialog::DestroyWindow();
}

void COptEtc::hotkeyEnablePOP()
{
    m_ctrlPopupKey.EnableWindow(true);
    m_ctrlPopupKey.SetFocus();
}

void COptEtc::hotkeyDisablePOP()
{
    m_ctrlPopupKey.EnableWindow(false);
}

void COptEtc::hotkeyEnableFIFO()
{
    m_ctrlFifoKey.EnableWindow(true);
    m_ctrlFifoKey.SetFocus();
}

void COptEtc::hotkeyDisableFIFO()
{
    m_ctrlFifoKey.EnableWindow(false);
}
