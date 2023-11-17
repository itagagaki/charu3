/*----------------------------------------------------------
    Generalクラス
                                    2002/12/07 (c)Keizi
----------------------------------------------------------*/

#include "General.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//---------------------------------------------------
//関数名	CGeneral
//機能		コンストラクタ
//---------------------------------------------------
CGeneral::CGeneral()
{

}

//---------------------------------------------------
//関数名	~CGeneral
//機能		デストラクタ
//---------------------------------------------------
CGeneral::~CGeneral()
{

}

//---------------------------------------------------
//関数名	loadBitmapFile
//機能		BMPファイルを読み込む
//---------------------------------------------------
bool CGeneral::loadBitmapFile(CString strFileName, CBitmap* BitMap)
{
    HBITMAP hBitmap;
    BitMap->Detach();
    hBitmap = (HBITMAP)::LoadImage(NULL, strFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (hBitmap == NULL) 	return false;
    else {
        BitMap->Attach(hBitmap);
    }
    return true;
}

//---------------------------------------------------
//関数名	wideCharToCString(wchar_t *szUnicodeBuff)
//機能		ワイド文字をMBCSにしてCStringに入れる
//---------------------------------------------------
CString CGeneral::wideCharToCString(wchar_t* szUnicodeBuff)
{
    char* szMbcsBuff;
    CString strRet;
    int nDataSize = ::WideCharToMultiByte(CP_ACP, 0, szUnicodeBuff, -1, NULL, 0, NULL, NULL);
    szMbcsBuff = new char[nDataSize];
    if (szMbcsBuff) {
        ::WideCharToMultiByte(CP_ACP, 0, szUnicodeBuff, -1, szMbcsBuff, nDataSize, "", NULL);
        strRet = szMbcsBuff;
        delete[] szMbcsBuff;
    }

    return strRet;
}

bool CGeneral::getSettingBool(nlohmann::json& j, const char* key, bool defaultValue)
{
    nlohmann::json::iterator it = j.find(key);
    return (it != j.end() && it.value().is_boolean()) ? it.value().get<bool>() : defaultValue;
}

double CGeneral::getSettingNumber(nlohmann::json& j, const char* key, double defaultValue)
{
    nlohmann::json::iterator it = j.find(key);
    return (it != j.end() && it.value().is_number()) ? it.value().get<double>() : defaultValue;
}

std::string CGeneral::getSettingString(nlohmann::json& j, const char* key, std::string defaultValue)
{
    nlohmann::json::iterator it = j.find(key);
    return (it != j.end() && it.value().is_string()) ? it.value().get<std::string>() : defaultValue;
}

CString CGeneral::getSettingCString(nlohmann::json& j, const char* key, CString defaultValue)
{
    nlohmann::json::iterator it = j.find(key);
    if (it == j.end()) {
        return defaultValue;
    }
    CString cs;
    if (!getCStringFromJson(it.value(), cs)) {
        return defaultValue;
    }
    return cs;
}

void CGeneral::appendCStringArray(nlohmann::json& j, const char* key, std::vector<CString>& list)
{
    nlohmann::json::iterator it = j.find(key);
    if (it != j.end() && it.value().is_array()) {
        nlohmann::json array = it.value();
        for (nlohmann::json& item : array) {
            CString cs;
            if (getCStringFromJson(item, cs)) {
                list.push_back(cs);
            }
        }
    }
}

bool CGeneral::getCStringFromJson(nlohmann::json j, CString& cs)
{
    if (!j.is_string()) {
        return false;
    }
    std::string s = j.get<std::string>();
    const char* cptr = s.c_str();
    int size = MultiByteToWideChar(CP_UTF8, 0, cptr, -1, nullptr, 0);
    wchar_t* wbuf = new wchar_t[size];
    if (!wbuf) {
        return false;
    }
    MultiByteToWideChar(CP_UTF8, 0, cptr, -1, wbuf, size);
    cs = CString(wbuf);
    delete[] wbuf;
    return true;
}

CStringA CGeneral::ConvertUnicodeToUTF8(const CStringW& uni)
{
    if (uni.IsEmpty()) return "";
    CStringA utf8;
    int cc = 0;
    // get length (cc) of the new multibyte string excluding the \0 terminator first
    if ((cc = WideCharToMultiByte(CP_UTF8, 0, uni, -1, NULL, 0, 0, 0) - 1) > 0) {
        char* buf = utf8.GetBuffer(cc);
        if (buf) WideCharToMultiByte(CP_UTF8, 0, uni, -1, buf, cc, 0, 0);
        utf8.ReleaseBuffer();
    }
    return utf8;
}

void CGeneral::flatSB_UpdateMetrics(HWND hWnd)
{
    FlatSB_SetScrollProp(hWnd, WSB_PROP_CXVSCROLL, GetSystemMetrics(SM_CXVSCROLL), TRUE);
    FlatSB_SetScrollProp(hWnd, WSB_PROP_CXHSCROLL, GetSystemMetrics(SM_CXHSCROLL), TRUE);
    FlatSB_SetScrollProp(hWnd, WSB_PROP_CYVSCROLL, GetSystemMetrics(SM_CYVSCROLL), TRUE);
    FlatSB_SetScrollProp(hWnd, WSB_PROP_CYHSCROLL, GetSystemMetrics(SM_CYHSCROLL), TRUE);
    FlatSB_SetScrollProp(hWnd, WSB_PROP_CXHTHUMB, GetSystemMetrics(SM_CXHTHUMB), TRUE);
    FlatSB_SetScrollProp(hWnd, WSB_PROP_CYVTHUMB, GetSystemMetrics(SM_CYVTHUMB), TRUE);
}

//---------------------------------------------------
//関数名	mod2VK(UINT uMod)
//機能		キーコード変換
//---------------------------------------------------
UINT CGeneral::mod2VK(UINT uMod)
{
    UINT uVK = 0;
    if (uMod & MOD_ALT)		uVK = VK_MENU;
    if (uMod & MOD_CONTROL)	uVK = VK_CONTROL;
    if (uMod & MOD_SHIFT)	uVK = VK_SHIFT;
    return uVK;
}

//---------------------------------------------------
//関数名	mod2Hotkey(UINT uMod)
//機能		キーコード変換
//---------------------------------------------------
UINT CGeneral::mod2Hotkey(UINT uMod)
{
    WORD wMod = 0;
    if (uMod & MOD_ALT)		wMod |= HOTKEYF_ALT;
    if (uMod & MOD_CONTROL)	wMod |= HOTKEYF_CONTROL;
    if (uMod & MOD_SHIFT)	wMod |= HOTKEYF_SHIFT;
    return (UINT)wMod;
}

//---------------------------------------------------
//関数名	hotkey2MOD(WORD wModS)
//機能		キーコード変換
//---------------------------------------------------
WORD CGeneral::hotkey2MOD(WORD wModS)
{
    WORD wMod = 0;
    if (wModS & HOTKEYF_ALT)		wMod |= MOD_ALT;
    if (wModS & HOTKEYF_CONTROL)	wMod |= MOD_CONTROL;
    if (wModS & HOTKEYF_SHIFT)	wMod |= MOD_SHIFT;
    return wMod;
}

//---------------------------------------------------
//関数名	RGB2BGR(COLORREF col)
//機能		RGBをBGRに変換
//---------------------------------------------------
COLORREF CGeneral::RGB2BGR(COLORREF col)
{
    COLORREF ret;

    int R, G, B;

    R = 0x00ff0000 & col;
    R = R >> 16;
    G = 0x0000ff00 & col;
    B = 0x000000ff & col;
    B = B << 16;

    ret = (R + G + B);
    return ret;
}

//---------------------------------------------------
//関数名	BGR2RGB(COLORREF col)
//機能		BGRをRGBに変換
//---------------------------------------------------
COLORREF CGeneral::BGR2RGB(COLORREF col)
{
    COLORREF ret;

    int R, G, B;

    B = 0x00ff0000 & col;
    B = B >> 16;
    G = 0x0000ff00 & col;
    R = 0x000000ff & col;
    R = R << 16;

    ret = (R + G + B);
    return ret;
}

//---------------------------------------------------
//関数名	upDownLight(COLORREF col,float fLight)
//機能		RGBの明るさを変更
//---------------------------------------------------
COLORREF CGeneral::upDownLight(COLORREF col, double fLight)
{
    COLORREF ret;

    int R, G, B;

    R = 0x00ff0000 & col;
    R = R >> 16;
    R = (int)((double)R * fLight);
    if (R > 0xff) R = 0xff;

    G = 0x0000ff00 & col;
    G = G >> 8;
    G = (int)((double)G * fLight);
    if (G > 0xff) G = 0xff;

    B = 0x000000ff & col;
    B = (int)((double)B * fLight);
    if (B > 0xff) B = 0xff;

    ret = ((R << 16) + (G << 8) + B);
    return ret;
}

//---------------------------------------------------
//関数名	GetCaretPos(POINT *CaretPos)
//機能		アクティブなアプリのキャレット位置を取得
//---------------------------------------------------
void CGeneral::getCaretPos(POINT* CaretPos, FOCUS_INFO* focusInfo)
{
    //キャレット位置
    DWORD dwFrom, dwTo;
    HWND hActiveWnd, hForground;

    if (focusInfo) {
        focusInfo->m_hActiveWnd = NULL;
        focusInfo->m_hFocusWnd = NULL;
    }

    dwFrom = ::GetCurrentThreadId();//アクティブなスレッドのIDを取得
    hForground = ::GetForegroundWindow();
    if (focusInfo) focusInfo->m_hActiveWnd = hForground;
    if (hForground != NULL)
        dwTo = ::GetWindowThreadProcessId(hForground, NULL);//プロセスIDを取得
    else
        dwTo = NULL;


    if (dwFrom && dwTo) {
        BOOL isGetGuiInfo = false;
        GUITHREADINFO gui;
        ZeroMemory(&gui, sizeof(gui));
        gui.cbSize = sizeof(gui);
        isGetGuiInfo = GetGUIThreadInfo(dwTo, &gui);
        if (!isGetGuiInfo) {
            if (::AttachThreadInput(dwFrom, dwTo, TRUE)) {//アタッチ
                ::GetCaretPos(CaretPos);//キャレット座標を取得
                hActiveWnd = ::GetFocus();//フォーカスウィンドウハンドルを取得
                if (focusInfo) focusInfo->m_hFocusWnd = hActiveWnd;
                ClientToScreen(hActiveWnd, CaretPos);
                ::AttachThreadInput(dwFrom, dwTo, FALSE);//デタッチ
            }
        }
        else {
            hActiveWnd = gui.hwndFocus;//フォーカスウィンドウハンドルを取得
            if (focusInfo) focusInfo->m_hFocusWnd = hActiveWnd;
            CaretPos->x = gui.rcCaret.left;
            CaretPos->y = gui.rcCaret.top;

            ClientToScreen(hActiveWnd, CaretPos);
            CaretPos->y += gui.rcCaret.bottom - gui.rcCaret.top;
        }
    }
    //	if(CaretPos->x < 0)	CaretPos->x = 0;
    //	if(CaretPos->y < 0)	CaretPos->y = 0;
    if (focusInfo && focusInfo->m_hFocusWnd == NULL) focusInfo->m_hFocusWnd = focusInfo->m_hActiveWnd;
}

//---------------------------------------------------
//関数名	SetAbsoluteForegroundWindow(HWND hWnd)
//機能		ウィンドウを強制アクティブ
//---------------------------------------------------
BOOL CGeneral::setAbsoluteForegroundWindow(HWND hWnd)
{
    int nTargetID = NULL, nForegroundID = NULL;
    UINT nTimeout = 0;
    BOOL isRet = false;

    if (GetForegroundWindow() && hWnd) {
        nForegroundID = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
        nTargetID = GetWindowThreadProcessId(hWnd, NULL);
        AttachThreadInput(nTargetID, nForegroundID, TRUE);

        SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &nTimeout, 0);
        SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (PVOID)0, 0);

        isRet = SetForegroundWindow(hWnd);

        SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (PVOID)nTimeout, 0);

        AttachThreadInput(nTargetID, nForegroundID, FALSE);
        isRet = true;
    }
    return isRet;
}

//---------------------------------------------------
//関数名	setFocus(const HWND hActiveWnd, const HWND hFocusWnd)
//機能		フォーカスセット
//---------------------------------------------------
void CGeneral::setFocus(const HWND hActiveWnd, const HWND hFocusWnd)
{
    if (hFocusWnd && hActiveWnd) {
        DWORD dwID = GetWindowThreadProcessId(hActiveWnd, NULL);
        if (dwID) {
            AttachThreadInput(dwID, GetCurrentThreadId(), TRUE);
            if (GetFocus() != hFocusWnd) {
                HWND nRet = ::SetFocus(hFocusWnd);
            }
            dwID = GetWindowThreadProcessId(hActiveWnd, NULL);
            if (dwID) AttachThreadInput(dwID, GetCurrentThreadId(), FALSE);
        }
    }
}

//---------------------------------------------------
//関数名	setFocusInfo(const FOCUS_INFO *focusInfo)
//機能		フォーカスセット
//---------------------------------------------------
void CGeneral::setFocusInfo(const FOCUS_INFO* focusInfo)
{
    //アクティブウィンドウの設定
    if (focusInfo->m_hActiveWnd) {
        setAbsoluteForegroundWindow(focusInfo->m_hActiveWnd);
        if (focusInfo->m_hActiveWnd) ::SendMessage(focusInfo->m_hActiveWnd, WM_NCACTIVATE, (WPARAM)TRUE, 0);
        //フォーカスの設定
        setFocus(focusInfo->m_hActiveWnd, focusInfo->m_hFocusWnd);
    }
}

//---------------------------------------------------
//関数名	getFocusInfo(FOCUS_INFO *focusInfo)
//機能		フォーカス情報取得
//---------------------------------------------------
void CGeneral::getFocusInfo(FOCUS_INFO* focusInfo, HWND hForeground)
{
    //フォーカスを持つウィンドウの取得
    if (!hForeground)
        focusInfo->m_hActiveWnd = GetForegroundWindow();
    else
        focusInfo->m_hActiveWnd = hForeground;

    DWORD dwID = GetWindowThreadProcessId(focusInfo->m_hActiveWnd, NULL);
    if (dwID) {
        AttachThreadInput(dwID, GetCurrentThreadId(), TRUE);
        focusInfo->m_hFocusWnd = GetFocus();
        dwID = GetWindowThreadProcessId(focusInfo->m_hActiveWnd, NULL);
        if (dwID)	AttachThreadInput(dwID, GetCurrentThreadId(), FALSE);
    }
    if (focusInfo->m_hFocusWnd == NULL) focusInfo->m_hFocusWnd = focusInfo->m_hActiveWnd;
}

//---------------------------------------------------
//関数名	getKeyName(UINT uVkCode,HKL keyLayout)
//機能		仮想キーの名前を得る
//---------------------------------------------------
CString CGeneral::getKeyName(UINT uVkCode, HKL keyLayout)
{
    CString strRet;
    strRet = "";
    int nExtFlag = 0;

    TCHAR strKeyName[256];
    int nScanCode = MapVirtualKeyEx(uVkCode, 0, keyLayout) << 16;
    if (nScanCode) {
        if (uVkCode == VK_APPS || uVkCode == VK_PRIOR || uVkCode == VK_NEXT || uVkCode == VK_END ||
            uVkCode == VK_HOME || uVkCode == VK_LEFT || uVkCode == VK_UP || uVkCode == VK_RIGHT ||
            uVkCode == VK_DOWN || uVkCode == VK_INSERT || uVkCode == VK_DELETE || uVkCode == VK_NUMLOCK)
            nExtFlag = 1 << 24;

        GetKeyNameText(nScanCode | nExtFlag, strKeyName, _countof(strKeyName));
        strRet = strKeyName;
    }
    return strRet;
}

//---------------------------------------------------
//関数名	getWindowTitle(HWND hWnd)
//機能		ウィンドウタイトルを取得
//---------------------------------------------------
CString CGeneral::getWindowTitle(HWND hWnd)
{
    TCHAR strWindowName[1024] = {};
    *strWindowName = (TCHAR)NULL;
    CString strWinName;
    bool isRet = false;

    strWinName = "";
    if (!GetWindowText(hWnd, strWindowName, _countof(strWindowName))) return strWinName;//キャプションを取得

    strWinName = strWindowName;
    return 	strWinName;
}


//---------------------------------------------------
//関数名	getLastErrorMessage()
//機能		lasterrorをメッセージに変換
//---------------------------------------------------
CString CGeneral::getLastErrorMessage()
{
    LPTSTR lpMsgBuf = {};
    CString strRet;
    strRet = _T("");
    if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
        GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL)) {
        strRet = lpMsgBuf;
        LocalFree((void*)lpMsgBuf);
    }
    return strRet;
}

//---------------------------------------------------
//関数名	writeLog(CString strFileName,strLogText)
//機能		ログをファイルに書き出す
//---------------------------------------------------
void CGeneral::writeLog(CString strFileName, CString strLogText, CString strSourceFile, int nLine)
{
    FILE* outPut = nullptr;
    CString strWrite, strTest;

#ifdef _UNICODE
    char* szMbcsBuff = NULL;
    int nDataSize;
    errno_t err = -1;

    nDataSize = ::WideCharToMultiByte(CP_ACP, 0, strFileName, -1, NULL, 0, NULL, NULL);
    szMbcsBuff = new char[nDataSize + 1];
    if (szMbcsBuff) {
        ::WideCharToMultiByte(CP_ACP, 0, strFileName, -1, szMbcsBuff, nDataSize, "", NULL);
        szMbcsBuff[nDataSize] = NULL;
        err = fopen_s(&outPut, szMbcsBuff, "a");
        delete[] szMbcsBuff;
    }
    if (0 == err) {
        strWrite.Format(_T("%s %s:%d %s"), getDateTimeString().GetString(), strSourceFile.GetString(), nLine, strLogText.GetString());
        nDataSize = ::WideCharToMultiByte(CP_ACP, 0, strWrite, -1, NULL, 0, NULL, NULL);
        szMbcsBuff = new char[nDataSize + 1];
        if (szMbcsBuff) {
            ::WideCharToMultiByte(CP_ACP, 0, strWrite, -1, szMbcsBuff, nDataSize, "", NULL);
            szMbcsBuff[nDataSize] = NULL;
            fputs(szMbcsBuff, outPut);
            delete[] szMbcsBuff;
        }
        fclose(outPut);

        OutputDebugString(strWrite);
    }
#else
    if ((outPut = fopen(strFileName, "a")) != NULL) {
        strWrite.Format("%s %s:%d %s", getDateTimeString().GetString(), strSourceFile.GetString(), nLine, strLogText.GetString());
        fputs(strWrite, outPut);
        fflush(outPut);
        OutputDebugString(strWrite);
        fclose(outPut);
    }
#endif
}

//---------------------------------------------------
//関数名	convertWareki(long lDate)
//機能		西暦を和暦に変換
//---------------------------------------------------
CString CGeneral::convertWareki(long lDate)
{
    CString strRet;
    strRet = "";

    const TCHAR* szGengou[] = { _T("文久"),_T("元治"),_T("慶応"),_T("明治"),_T("大正"),_T("昭和"),_T("平成") };
    const long lDateTbl[] = { 18640220,18650407,18680908,19120730,19261225,19890108,999991231 };

    for (int i = 0; i <= 6; i++) {
        long lYear, lMonth, lDay, lGannen;
        DWORD dwComp, dwNow;
        //元号の区切り
        setCTime(lDateTbl[i], &lYear, &lMonth, &lDay);
        dwComp = countDays(lYear, lMonth, lDay);
        //今
        setCTime(lDate, &lYear, &lMonth, &lDay);
        dwNow = countDays(lYear, lMonth, lDay);

        if (dwComp > dwNow) {
            if (i > 0)	setCTime(lDateTbl[i - 1], &lGannen, &lMonth, &lDay);
            else lGannen = 1861;
            lYear = lYear - lGannen + 1;
            if (lYear == 1)	strRet.Format(_T("%s元"), szGengou[i]);
            else			strRet.Format(_T("%s%2d"), szGengou[i], lYear);
            break;
        }
    }
    return strRet;
}

//---------------------------------------------------
//関数名	countDays(long lYear,long lMonth,long lDay)
//機能		日数のカウント
//---------------------------------------------------
long CGeneral::countDays(long lYear, long lMonth, long lDay)
{
    long lRet = 0;
    //   1, 2, 3, 4, 5, 6  7, 8, 9,10,11,12
    int nMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

    //365掛けて、4年に一度はうるう年で、でも100年に一度はうるう年でない。ところが400年に一度はうるう年
    lRet += ((lYear - 1) * 365) + (int)((lYear - 1) / 4) - (int)((lYear - 1) / 100) + (int)((lYear - 1) / 400);

    //月の日数を足す
    for (int i = 0; i < lMonth - 1; i++) {
        lRet += nMonth[i];
    }
    if ((lYear % 4 == 0 && lYear % 100 != 0 || lYear % 400 == 0) && lMonth >= 3)	lRet++;

    //日を足す
    lRet += lDay;

    return lRet;
}

//---------------------------------------------------
//関数名	getDateString()
//機能		日付文字列を作る
//---------------------------------------------------
CString CGeneral::getDateString()
{
    CString strDate;

    struct tm newtime;
    time_t long_time;

    time(&long_time);
    localtime_s(&newtime, &long_time);

    strDate.Format(_T("%04d/%02d/%02d"), newtime.tm_year + 1900, newtime.tm_mon + 1, newtime.tm_mday);
    return strDate;
}

//---------------------------------------------------
//関数名	getDateTimeString()
//機能		日付時間文字列を作る
//---------------------------------------------------
CString CGeneral::getDateTimeString()
{
    CString strDate;

    struct tm newtime;
    time_t long_time;

    time(&long_time);
    localtime_s(&newtime, &long_time);

    strDate.Format(_T("%04d/%02d/%02d-%02d:%02d:%02d"), newtime.tm_year + 1900, newtime.tm_mon + 1, newtime.tm_mday,
        newtime.tm_hour, newtime.tm_min, newtime.tm_sec);
    return strDate;
}

//---------------------------------------------------
//関数名	setCTime(long lDate,long *lYear,long *lMonth,long *lDay)
//機能		日付データの変換
//入力		20021003　日付数列
//---------------------------------------------------
bool CGeneral::setCTime(long lDate, long* lYear, long* lMonth, long* lDay)
{
    bool isRet = false;

    //年を割り出す
    *lYear = lDate / 10000;
    if (*lYear <= 0) return isRet;

    //月を割り出す
    *lMonth = (lDate - (*lYear) * 10000) / 100;
    if (*lMonth <= 0) return isRet;

    //日を割り出す
    *lDay = (lDate - ((*lYear) * 10000 + (*lMonth) * 100));
    if (*lDay <= 0) return isRet;

    isRet = true;
    return isRet;
}
