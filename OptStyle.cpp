/*----------------------------------------------------------
    ビジュアル設定クラス
                                    2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <fstream>
#include "nlomann/json.hpp"
#include "jsonHelper.h"

#include "OptStyle.h"
#include "Charu3.h"
#include "color.h"
#include "log.h"
#include "resource.h"

//---------------------------------------------------
//関数名	COptStyle
//機能		コンストラクタ
//---------------------------------------------------
COptStyle::COptStyle(CWnd* pParent /*=NULL*/)
    : CDialog(COptStyle::IDD, pParent)
{
    //{{AFX_DATA_INIT(COptStyle)
    //}}AFX_DATA_INIT
    m_strBorderColor.Format(_T("%.6x"), theApp.m_ini.m_nBorderColor);
    m_strBackgroundColor.Format(_T("%.6x"), theApp.m_ini.m_nBackgroundColor);
    m_strTextColor.Format(_T("%.6x"), theApp.m_ini.m_nTextColor);
    m_ctrlBorderPal.SetColor(Color::Swap_RGB_BGR(theApp.m_ini.m_nBorderColor));
    m_ctrlBackgroundPal.SetColor(Color::Swap_RGB_BGR(theApp.m_ini.m_nBackgroundColor));
    m_ctrlTextPal.SetColor(Color::Swap_RGB_BGR(theApp.m_ini.m_nTextColor));
}

//---------------------------------------------------
//関数名	DoDataExchange(CDataExchange* pDX)
//機能		データエクスチェンジ
//---------------------------------------------------
void COptStyle::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    //{{AFX_DATA_MAP(COptStyle)
    if (GetDlgItem(IDC_OPT_BORDER_COLOR))
        DDX_Text(pDX, IDC_OPT_BORDER_COLOR, m_strBorderColor);
    if (GetDlgItem(IDC_OPT_BACKGROUND_COLOR))
        DDX_Text(pDX, IDC_OPT_BACKGROUND_COLOR, m_strBackgroundColor);
    if (GetDlgItem(IDC_OPT_TEXT_COLOR))
        DDX_Text(pDX, IDC_OPT_TEXT_COLOR, m_strTextColor);
    if (GetDlgItem(IDC_OPT_FONT_NAME))
        DDX_Control(pDX, IDC_OPT_FONT_NAME, m_ctrlFontCombo);
    if (GetDlgItem(IDC_OPT_FONT_SIZE))
        DDX_Text(pDX, IDC_OPT_FONT_SIZE, theApp.m_ini.m_nFontSize);
    if (GetDlgItem(IDC_OPT_ICON_FILE_NAME))
        DDX_Text(pDX, IDC_OPT_ICON_FILE_NAME, theApp.m_ini.m_strResourceName);

    if (GetDlgItem(IDC_OPT_BORDER_COLOR_PAL))
        DDX_Control(pDX, IDC_OPT_BORDER_COLOR_PAL, m_ctrlBorderPal);
    if (GetDlgItem(IDC_OPT_BACKGROUND_COLOR_PAL))
        DDX_Control(pDX, IDC_OPT_BACKGROUND_COLOR_PAL, m_ctrlBackgroundPal);
    if (GetDlgItem(IDC_OPT_TEXT_COLOR_PAL))
        DDX_Control(pDX, IDC_OPT_TEXT_COLOR_PAL, m_ctrlTextPal);

    if (GetDlgItem(IDC_OPT_TOUMEI_SLIDER))
        DDX_Slider(pDX, IDC_OPT_TOUMEI_SLIDER, theApp.m_ini.m_nOpacity);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptStyle, CDialog)
    //{{AFX_MSG_MAP(COptStyle)
    ON_BN_CLICKED(IDC_OPT_VS_BROWS, OnOptVsBrows)
    ON_EN_CHANGE(IDC_OPT_BORDER_COLOR, OnChangeOptBorderColor)
    ON_EN_CHANGE(IDC_OPT_BACKGROUND_COLOR, OnChangeOptBackColor)
    ON_EN_CHANGE(IDC_OPT_TEXT_COLOR, OnChangeOptTextColor)
    ON_BN_CLICKED(IDC_OPT_BORDER_COLOR_PAL, OnOptBorderColorPal)
    ON_BN_CLICKED(IDC_OPT_BACKGROUND_COLOR_PAL, OnOptBackgroundColorPal)
    ON_BN_CLICKED(IDC_OPT_TEXT_COLOR_PAL, OnOptTextColorPal)
    ON_BN_CLICKED(IDC_OPT_LOAD_STYLE, OnBnClickedOptLoadStyle)
    ON_WM_SHOWWINDOW()
    ON_WM_HSCROLL()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------
// フォント列挙コールバック関数
//---------------------------------------------------
int CALLBACK EnumFontProc(ENUMLOGFONT* lpelf, NEWTEXTMETRIC* lpntm, int FontType, LPARAM lparam)
{
    (void)lpntm;
    (void)FontType;
    COptStyle* ThisClass = (COptStyle*)lparam;

    ThisClass->m_ctrlFontCombo.AddString(lpelf->elfLogFont.lfFaceName);
    if (_tcscmp(lpelf->elfLogFont.lfFaceName, LPCTSTR(theApp.m_ini.m_strFontName)) == 0)
        ThisClass->m_ctrlFontCombo.SetCurSel(ThisClass->m_ctrlFontCombo.GetCount() - 1);

    return TRUE; // 列挙継続
}

//---------------------------------------------------
// COptStyle メッセージ ハンドラ
//---------------------------------------------------

//---------------------------------------------------
//関数名	PreTranslateMessage(MSG* pMsg)
//機能		メッセージ前処理
//---------------------------------------------------
BOOL COptStyle::PreTranslateMessage(MSG* pMsg)
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
BOOL COptStyle::OnInitDialog()
{
    CDialog::OnInitDialog();

    //フォント名を列挙
    m_ctrlFontCombo.ResetContent();
    HDC hDC;
    hDC = ::GetDC(NULL);
    EnumFontFamilies(hDC, NULL, (FONTENUMPROC)EnumFontProc, (LPARAM)this);
    ::ReleaseDC(NULL, hDC);

    SetOpacityText(theApp.m_ini.m_nOpacity);
    return TRUE;
}

void COptStyle::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialog::OnShowWindow(bShow, nStatus);
    if (bShow) {
        CWnd* w = GetDlgItem(IDC_OPT_FONT_NAME);
        if (w) w->SetFocus();
    }
}

void COptStyle::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    void* pOpacitySlider = GetDlgItem(IDC_OPT_TOUMEI_SLIDER);
    if (pScrollBar == pOpacitySlider) {
        SetOpacityText(static_cast<CSliderCtrl*>(pOpacitySlider)->GetPos());
    }
}

//---------------------------------------------------
//関数名	DestroyWindow()
//機能		終了処理
//---------------------------------------------------
BOOL COptStyle::DestroyWindow()
{
    CString strBuff;
    CEdit* edit;
    edit = (CEdit*)GetDlgItem(IDC_OPT_BORDER_COLOR);
    edit->GetWindowText(strBuff);
    _stscanf_s(strBuff, _T("%x"), &theApp.m_ini.m_nBorderColor);

    edit = (CEdit*)GetDlgItem(IDC_OPT_BACKGROUND_COLOR);
    edit->GetWindowText(strBuff);
    _stscanf_s(strBuff, _T("%x"), &theApp.m_ini.m_nBackgroundColor);

    edit = (CEdit*)GetDlgItem(IDC_OPT_TEXT_COLOR);
    edit->GetWindowText(strBuff);
    _stscanf_s(strBuff, _T("%x"), &theApp.m_ini.m_nTextColor);

    int nCursel = m_ctrlFontCombo.GetCurSel();
    if (nCursel == CB_ERR) nCursel = 0;
    m_ctrlFontCombo.GetLBText(nCursel, strBuff);
    theApp.m_ini.m_strFontName = strBuff;

    return CDialog::DestroyWindow();
}

void COptStyle::OnChangeOptBorderColor()
{
    UpdateColor((CEdit*)GetDlgItem(IDC_OPT_BORDER_COLOR), &m_ctrlBorderPal);
}

void COptStyle::OnChangeOptBackColor()
{
    UpdateColor((CEdit*)GetDlgItem(IDC_OPT_BACKGROUND_COLOR), &m_ctrlBackgroundPal);
}

void COptStyle::OnChangeOptTextColor()
{
    UpdateColor((CEdit*)GetDlgItem(IDC_OPT_TEXT_COLOR), &m_ctrlTextPal);
}

void COptStyle::UpdateColor(CEdit* edit, CPaletStatic* stPal)
{
    CString cstr;
    edit->GetWindowText(cstr);
    int rgb;
    _stscanf_s(cstr, _T("%x"), &rgb);
    stPal->SetColor(Color::Swap_RGB_BGR(rgb));
    stPal->Invalidate();
}

void COptStyle::OnOptBorderColorPal()
{
    PickColor((CEdit*)GetDlgItem(IDC_OPT_BORDER_COLOR), &m_ctrlBorderPal);
}

void COptStyle::OnOptBackgroundColorPal()
{
    PickColor((CEdit*)GetDlgItem(IDC_OPT_BACKGROUND_COLOR), &m_ctrlBackgroundPal);
}

void COptStyle::OnOptTextColorPal()
{
    PickColor((CEdit*)GetDlgItem(IDC_OPT_TEXT_COLOR), &m_ctrlTextPal);
}

void COptStyle::PickColor(CEdit* edit, CPaletStatic* stPal)
{
    CColorDialog dlgColor;
    if (dlgColor.DoModal() == IDOK) {
        COLORREF col = dlgColor.GetColor();
        stPal->SetColor(col);
        stPal->Invalidate();
        CString strBuff;
        strBuff.Format(_T("%.6x"), Color::Swap_RGB_BGR((int)col));
        edit->SetWindowText(strBuff);
    }
}

//---------------------------------------------------
//関数名	OnOptVsBrows()
//---------------------------------------------------
void COptStyle::OnOptVsBrows()
{
    CString strRes;
    (void)strRes.LoadString(APP_INF_FILE_FILTER5);
    CFileDialog openFileDialog(TRUE, _T("bmp"), NULL, OFN_FILEMUSTEXIST, strRes);

    openFileDialog.m_ofn.lpstrInitialDir = theApp.m_ini.m_strAppPath;
    if (openFileDialog.DoModal() == IDOK)
        SetDlgItemText(IDC_OPT_ICON_FILE_NAME, openFileDialog.GetPathName());
}

void COptStyle::OnBnClickedOptLoadStyle()
{
    CString strFileName;
    CString strRes;
    (void)strRes.LoadString(APP_INF_FILE_FILTER_VISUAL_PREF);
    CFileDialog fileDialog(TRUE, _T("json"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strRes, NULL);
    fileDialog.m_ofn.lpstrInitialDir = theApp.m_ini.m_strAppPath;
    if (IDOK == fileDialog.DoModal()) {
        if (theApp.m_ini.m_bDebug) {
            LOG(_T("ReadStyleFile \"%s\""), fileDialog.GetPathName().GetString());
        }
        strFileName = fileDialog.GetPathName();
        nlohmann::json style;
        try { style = nlohmann::json::parse(std::ifstream(strFileName)); }
        catch (...) { style = nlohmann::json(); }
        std::string str;
        str = jsonHelper::GetStringProperty(style, "borderColor", "");
        if (str != "") {
            m_strBorderColor.Format(_T("%.6x"), Color::Parse(str));
            ((CEdit*)GetDlgItem(IDC_OPT_BORDER_COLOR))->SetWindowText(m_strBorderColor);
        }
        str = jsonHelper::GetStringProperty(style, "backgroundColor", "");
        if (str != "") {
            m_strBackgroundColor.Format(_T("%.6x"), Color::Parse(str));
            ((CEdit*)GetDlgItem(IDC_OPT_BACKGROUND_COLOR))->SetWindowText(m_strBackgroundColor);
        }
        str = jsonHelper::GetStringProperty(style, "textColor", "");
        if (str != "") {
            m_strTextColor.Format(_T("%.6x"), Color::Parse(str));
            ((CEdit*)GetDlgItem(IDC_OPT_TEXT_COLOR))->SetWindowText(m_strTextColor);
        }
        CString cstr = jsonHelper::GetStringPropertyAsCString(style, "fontName", "");
        int number= m_ctrlFontCombo.FindStringExact(0, cstr);
        if (CB_ERR != number) {
            m_ctrlFontCombo.SetCurSel(number);
        }
        number = static_cast<int>(jsonHelper::GetNumberProperty(style, "fontSize", -1));
        CWnd* ctrlWin;
        if (number >= 0) {
            cstr.Format(_T("%d"), number);
            ctrlWin = GetDlgItem(IDC_OPT_FONT_SIZE);
            ctrlWin->SetWindowTextW(cstr);
        }
        cstr = jsonHelper::GetStringPropertyAsCString(style, "iconFile", "");
        if (cstr != "") {
            ctrlWin = GetDlgItem(IDC_OPT_ICON_FILE_NAME);
            ctrlWin->SetWindowTextW(cstr);
        }
    }
}

void COptStyle::SetOpacityText(int value)
{
    CString strBuff;
    strBuff.Format(_T("%u%%"), value);
    GetDlgItem(IDC_OPACITY_TEXT)->SetWindowText(strBuff);
}
