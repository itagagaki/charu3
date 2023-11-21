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
    m_ctrlBorderPal.setColor(Color::Swap_RGB_BGR(theApp.m_ini.m_nBorderColor));
    m_strBackgroundColor.Format(_T("%.6x"), theApp.m_ini.m_nBackgroundColor);
    m_ctrlBackgroundPal.setColor(Color::Swap_RGB_BGR(theApp.m_ini.m_nBackgroundColor));
    m_strTextColor.Format(_T("%.6x"), theApp.m_ini.m_nTextColor);
    m_ctrlTextPal.setColor(Color::Swap_RGB_BGR(theApp.m_ini.m_nTextColor));
}

//---------------------------------------------------
//関数名	DoDataExchange(CDataExchange* pDX)
//機能		データエクスチェンジ
//---------------------------------------------------
void COptStyle::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    //{{AFX_DATA_MAP(COptStyle)
    if (GetDlgItem(IDC_OPT_FONT_NAME))
        DDX_Control(pDX, IDC_OPT_FONT_NAME, m_ctrlFontCombo);
    if (GetDlgItem(IDC_OPT_FONT_SIZE))
        DDX_Text(pDX, IDC_OPT_FONT_SIZE, theApp.m_ini.m_nFontSize);
    if (GetDlgItem(IDC_OPT_ICON_FILE_NAME))
        DDX_Text(pDX, IDC_OPT_ICON_FILE_NAME, theApp.m_ini.m_strResourceName);
    if (GetDlgItem(IDC_OPT_BORDER_COLOR))
        DDX_Text(pDX, IDC_OPT_BORDER_COLOR, m_strBorderColor);
    if (GetDlgItem(IDC_OPT_BORDER_COLOR_PAL))
        DDX_Control(pDX, IDC_OPT_BORDER_COLOR_PAL, m_ctrlBorderPal);
    if (GetDlgItem(IDC_OPT_BACKGROUND_COLOR))
        DDX_Text(pDX, IDC_OPT_BACKGROUND_COLOR, m_strBackgroundColor);
    if (GetDlgItem(IDC_OPT_BACKGROUND_COLOR_PAL))
        DDX_Control(pDX, IDC_OPT_BACKGROUND_COLOR_PAL, m_ctrlBackgroundPal);
    if (GetDlgItem(IDC_OPT_TEXT_COLOR))
        DDX_Text(pDX, IDC_OPT_TEXT_COLOR, m_strTextColor);
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
    edit = (CEdit*)GetDlgItem(IDC_OPT_BACKGROUND_COLOR);
    edit->GetWindowText(strBuff);
    _stscanf_s(strBuff, _T("%x"), &theApp.m_ini.m_nBackgroundColor);

    edit = (CEdit*)GetDlgItem(IDC_OPT_TEXT_COLOR);
    edit->GetWindowText(strBuff);
    _stscanf_s(strBuff, _T("%x"), &theApp.m_ini.m_nTextColor);

    edit = (CEdit*)GetDlgItem(IDC_OPT_BORDER_COLOR);
    edit->GetWindowText(strBuff);
    _stscanf_s(strBuff, _T("%x"), &theApp.m_ini.m_nBorderColor);

    int nCursel = m_ctrlFontCombo.GetCurSel();
    if (nCursel == CB_ERR) nCursel = 0;
    m_ctrlFontCombo.GetLBText(nCursel, strBuff);
    theApp.m_ini.m_strFontName = strBuff;

    return CDialog::DestroyWindow();
}

//---------------------------------------------------
//関数名	OnOptVsBrows()
//機能		参照ボタン
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

//---------------------------------------------------
//関数名	OnChangeOptBorderColor()
//機能		枠色変更時にパレットチェンジ
//---------------------------------------------------
void COptStyle::OnChangeOptBorderColor()
{
    setTextToPalet((CEdit*)GetDlgItem(IDC_OPT_BORDER_COLOR), &m_ctrlBorderPal);
}

//---------------------------------------------------
//関数名	OnChangeOptBackColor()
//機能		背景色変更時にパレットチェンジ
//---------------------------------------------------
void COptStyle::OnChangeOptBackColor()
{
    setTextToPalet((CEdit*)GetDlgItem(IDC_OPT_BACKGROUND_COLOR), &m_ctrlBackgroundPal);
}

//---------------------------------------------------
//関数名	OnChangeOptTextColor()
//機能		テキスト色変更時にパレットチェンジ
//---------------------------------------------------
void COptStyle::OnChangeOptTextColor()
{
    setTextToPalet((CEdit*)GetDlgItem(IDC_OPT_TEXT_COLOR), &m_ctrlTextPal);
}

//---------------------------------------------------
//関数名	OnOptBorderColorPal()
//機能		枠色パレット
//---------------------------------------------------
void COptStyle::OnOptBorderColorPal()
{
    setPalet((CEdit*)GetDlgItem(IDC_OPT_BORDER_COLOR), &m_ctrlBorderPal);
}

//---------------------------------------------------
//関数名	OnOptBackColorPal()
//機能		背景色パレット
//---------------------------------------------------
void COptStyle::OnOptBackgroundColorPal()
{
    setPalet((CEdit*)GetDlgItem(IDC_OPT_BACKGROUND_COLOR), &m_ctrlBackgroundPal);
}

//---------------------------------------------------
//関数名	OnOptTextColorPal()
//機能		テキスト色パレット
//---------------------------------------------------
void COptStyle::OnOptTextColorPal()
{
    setPalet((CEdit*)GetDlgItem(IDC_OPT_TEXT_COLOR), &m_ctrlTextPal);
}

void COptStyle::OnBnClickedOptLoadStyle()
{
    ReadStyleFile();
    OnInitDialog();
    InvalidateRect(NULL, false);
}

//---------------------------------------------------
//関数名	setTextToPalet(CEdit *edit,CPaletStatic *stPal)
//機能		テキストボックスの内容にしたがってパレット色を変更
//---------------------------------------------------
void COptStyle::setTextToPalet(CEdit* edit, CPaletStatic* stPal)
{
    CString strBuff;
    int nColor;
    if (!edit || !stPal) return;

    edit->GetWindowText(strBuff);
    _stscanf_s(strBuff, _T("%x"), &nColor);

    stPal->setColor(Color::Swap_RGB_BGR(nColor));
    stPal->Invalidate();
}

//---------------------------------------------------
//関数名	setPalet(CEdit *edit,CPaletStatic *stPal)
//機能		色セット
//---------------------------------------------------
void COptStyle::setPalet(CEdit* edit, CPaletStatic* stPal)
{
    CColorDialog dlgColor;

    if (dlgColor.DoModal() == IDOK) {
        CString strBuff;
        COLORREF col;
        int nColor;
        col = dlgColor.GetColor();
        stPal->setColor(col);//パレットに色を設定
        nColor = Color::Swap_RGB_BGR((int)col);//変換
        strBuff.Format(_T("%06x"), nColor);//テキストにする
        //エディットボックスに設定
        edit->SetWindowText(strBuff);
        stPal->Invalidate();
    }
}

//---------------------------------------------------
//関数名	ReadStyleFile()
//機能		スタイル設定を読み込み
//---------------------------------------------------
void COptStyle::ReadStyleFile()
{
    CString strFileName;
    CFileDialog* pFileDialog;
    CString strRes;
    (void)strRes.LoadString(APP_INF_FILE_FILTER_VISUAL_PREF);
    pFileDialog = new CFileDialog(TRUE, _T("json"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strRes, NULL);

    if (pFileDialog) {
        CString prevIconResourceName = theApp.m_ini.m_strResourceName;
        pFileDialog->m_ofn.lpstrInitialDir = theApp.m_ini.m_strAppPath;
        if (IDOK == pFileDialog->DoModal()) {
            if (theApp.m_ini.m_bDebug) {
                LOG(_T("ReadStyleFile \"%s\""), pFileDialog->GetPathName().GetString());
            }

            strFileName = pFileDialog->GetPathName();
            nlohmann::json j;
            try { j = nlohmann::json::parse(std::ifstream(strFileName)); }
            catch (...) {}
            theApp.m_ini.m_nBorderColor = Color::Parse(jsonHelper::GetStringProperty(j, "borderColor", Color::String(theApp.m_ini.m_nBorderColor)));
            theApp.m_ini.m_nBackgroundColor = Color::Parse(jsonHelper::GetStringProperty(j, "backgroundColor", Color::String(theApp.m_ini.m_nBackgroundColor)));
            theApp.m_ini.m_nTextColor = Color::Parse(jsonHelper::GetStringProperty(j, "textColor", Color::String(theApp.m_ini.m_nTextColor)));
            //theApp.m_ini.m_strFontName = jsonHelper::GetStringPropertyAsCString(j, "fontName", theApp.m_ini.m_strFontName);
            //theApp.m_ini.m_nFontSize = static_cast<int>(jsonHelper::GetNumberProperty(j, "fontSize", theApp.m_ini.m_nFontSize));
            //theApp.m_ini.m_strResourceName = jsonHelper::GetStringPropertyAsCString(j, "iconFile", theApp.m_ini.m_strResourceName);
        }
        delete pFileDialog;

        if (theApp.m_ini.m_strResourceName != prevIconResourceName) {
            theApp.resetTreeDialog();
        }
    }
}

void COptStyle::SetOpacityText(int value)
{
    CString strBuff;
    strBuff.Format(_T("%u%%"), value);
    GetDlgItem(IDC_OPACITY_TEXT)->SetWindowText(strBuff);
}
