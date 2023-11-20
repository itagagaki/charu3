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

    static bool loadBitmapFile(CString strFileName, CBitmap* BitMap);

    static CString getLastErrorMessage();

    static CString ConvertWcharToCString(wchar_t* szUnicodeBuff);
    static CStringA ConvertUnicodeToUTF8(const CStringW& uni);

    static CString convertWareki(long lDate);
    static CString getDateString();
    static CString getDateTimeString();
    static long countDays(long lYear, long lMonth, long lDay);
    static bool setCTime(long lDate, long* lYear, long* lMonth, long* lDay);
};

#endif // !defined(AFX_GENERAL_H__65853018_33C2_4D05_9492_06FAD9469F9B__INCLUDED_)
