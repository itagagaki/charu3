/*----------------------------------------------------------
    Generalクラス
                                    2002/12/07 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "General.h"

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
//関数名	ConvertWcharToCString(wchar_t *szUnicodeBuff)
//機能		ワイド文字をMBCSにしてCStringに入れる
//---------------------------------------------------
CString CGeneral::ConvertWcharToCString(wchar_t* szUnicodeBuff)
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
