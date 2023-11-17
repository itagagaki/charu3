/*----------------------------------------------------------
    Generalクラスヘッダ
                                    2002/12/07 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_GENERAL_H__65853018_33C2_4D05_9492_06FAD9469F9B__INCLUDED_)
#define AFX_GENERAL_H__65853018_33C2_4D05_9492_06FAD9469F9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "nlomann/json.hpp"

#if !defined(SPI_GETFOREGROUNDLOCKTIMEOUT)
#define SPI_GETFOREGROUNDLOCKTIMEOUT          0x2000
#define SPI_SETFOREGROUNDLOCKTIMEOUT          0x2001
#endif
//フォーカス情報
struct FOCUS_INFO {
    HWND m_hActiveWnd;
    HWND m_hFocusWnd;
};

//---------------------------------------------------
// クラス定義
//---------------------------------------------------
class CGeneral
{
public:
    CGeneral();
    virtual ~CGeneral();

    static inline int BoolToInt(bool value) {
        return value ? 1 : 0;
    }

    static WORD hotkey2MOD(WORD wModS);
    static UINT mod2Hotkey(UINT uMod);
    static UINT mod2VK(UINT uMod);
    static COLORREF RGB2BGR(COLORREF col);
    static COLORREF BGR2RGB(COLORREF col);
    static COLORREF upDownLight(COLORREF col, double fLight);
    static void getCaretPos(POINT* CaretPos, FOCUS_INFO* focusInfo = NULL);
    static BOOL setAbsoluteForegroundWindow(HWND hWnd);
    static void getFocusInfo(FOCUS_INFO* focusInfo, HWND hForeground = NULL);
    static void setFocusInfo(const FOCUS_INFO* focusInfo);
    static void setFocus(const HWND hActiveWnd, const HWND hFocusWnd);
    static void flatSB_UpdateMetrics(HWND hWnd);
    static void removeScrollBar(HWND hWnd);

    static CString getKeyName(UINT uVkCode, HKL keyLayout);
    static bool loadBitmapFile(CString strFileName, CBitmap* BitMap);
    static CString getLastErrorMessage();
    static CString wideCharToCString(wchar_t* szUnicodeBuff);
    static bool getSettingBool(nlohmann::json& j, const char* key, bool defaultValue);
    static double getSettingNumber(nlohmann::json& j, const char* key, double defaultValue);
    static std::string getSettingString(nlohmann::json& j, const char* key, std::string defaultValue);
    static CString getSettingCString(nlohmann::json& j, const char* key, CString defaultValue);
    static void appendCStringArray(nlohmann::json& j, const char* key, std::vector<CString>& list);
    static bool getCStringFromJson(nlohmann::json j, CString& cs);
    static CStringA ConvertUnicodeToUTF8(const CStringW& uni);
    static CString getWindowTitle(HWND hWnd);

    static void writeLog(CString strFileName, CString strLogText, CString strSourceFile, int nLine);
    static CString convertWareki(long lDate);
    static CString getDateString();
    static CString getDateTimeString();
    static long countDays(long lYear, long lMonth, long lDay);
    static bool setCTime(long lDate, long* lYear, long* lMonth, long* lDay);
};

#endif // !defined(AFX_GENERAL_H__65853018_33C2_4D05_9492_06FAD9469F9B__INCLUDED_)
