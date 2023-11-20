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

namespace Text {

    TCHAR* Awk(TCHAR* strSource, TCHAR* strRet, int nSize, int nAwk, TCHAR cSplit = 0x20);
    bool IsSJIS(TCHAR* szSource);
    int CompressionSpace(TCHAR* szSource);
    int MoveForward(TCHAR* szMovePos, int nMove);
    CString ConvertWcharToCString(wchar_t* szUnicodeBuff);
    CStringA ConvertUnicodeToUTF8(const CStringW& uni);

} // namespace Text

#endif // !defined(AFX_STRINGWORK_H__E26F2A4C_01CC_4E2E_8A31_1E4C6D507C1E__INCLUDED_)
