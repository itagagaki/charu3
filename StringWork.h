/*----------------------------------------------------------
    文字列処理クラスヘッダ
                                    2002/11/20 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_STRINGWORK_H__E26F2A4C_01CC_4E2E_8A31_1E4C6D507C1E__INCLUDED_)
#define AFX_STRINGWORK_H__E26F2A4C_01CC_4E2E_8A31_1E4C6D507C1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

#include <tchar.h>

//---------------------------------------------------
//　クラス宣言
//---------------------------------------------------
class UStringWork
{
public:
    static TCHAR* awk(TCHAR* strSource, TCHAR* strRet, int nSize, int nAwk, TCHAR cSplit = 0x20);
    static bool isSJIS(TCHAR* szSource);
    static int compressionSpace(TCHAR* szSource);
    static int moveForward(TCHAR* szMovePos, int nMove);
    static CString ConvertWcharToCString(wchar_t* szUnicodeBuff);
    static CStringA ConvertUnicodeToUTF8(const CStringW& uni);
};

#endif // !defined(AFX_STRINGWORK_H__E26F2A4C_01CC_4E2E_8A31_1E4C6D507C1E__INCLUDED_)
