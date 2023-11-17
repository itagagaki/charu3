/*----------------------------------------------------------
    文字列処理クラス
                                    2002/11/20 (c)keizi
----------------------------------------------------------*/

#include "StringWork.h"

//---------------------------------------------------
//関数名	awk(char *strSource,int nAwk,char cSplit)
//機能		指定文字区切りで文字列を切り出す
//---------------------------------------------------
TCHAR* UStringWork::awk(TCHAR* strSource, TCHAR* strRet, int nSize, int nAwk, TCHAR cSplit /* = 0x20*/)
{
    bool isSpace = false;
    TCHAR* szStart = NULL;

    if (nSize < 1 || nAwk < 1 || *strSource == NULL) return strSource;
    while (*strSource != NULL) {

        if (!isSJIS(strSource)) {
            if (!isSpace) {
                if (*strSource == cSplit) {//スプリッタが見つかった
                    isSpace = true;
                }
            }
            else if (isSpace) {
                if (*strSource != cSplit) {//スプリッタじゃ無くなった
                    nAwk--;//カウンタを減らす
                    isSpace = false;
                }
            }
        }
        else if (isSpace) {
            nAwk--;//カウンタを減らす
            isSpace = false;
        }
        if (nAwk == 1) {
            if (!szStart)	szStart = strSource - 1;

            if (*strSource == 0x0a || *strSource == 0x0d || *strSource == cSplit ||
                *strSource == 0x1a || *strSource == NULL) {
                break;
            }
            if (nSize <= 1) break;
            *strRet = *strSource;
            if (isSJIS(strSource)) {
                strSource++;//ポインタを進める
                strRet++;
                *strRet = *strSource;
            }
            nSize--;
            strRet++;
        }
        else if (isSJIS(strSource))	strSource++;//ポインタを進める
        strSource++;//ポインタを進める
    }
    *strRet = (TCHAR)NULL;

    return szStart;
}

//---------------------------------------------------
//関数名	isSJIS(char *szSource)
//機能		S-JISかどうかを判別
//---------------------------------------------------
bool UStringWork::isSJIS(TCHAR* szSource)
{
    bool isRet = false;
#ifdef _UNICODE
    return false;
#endif
    if (*szSource == NULL) return isRet;
    if (((unsigned char)*szSource >= 0x81 && (unsigned char)*szSource <= 0x9f) ||
        ((unsigned char)*szSource >= 0xe0 && (unsigned char)*szSource <= 0xef))
        if (((unsigned char)*(szSource + 1) >= 0x40 && (unsigned char)*(szSource + 1) <= 0x7e) ||
            ((unsigned char)*szSource >= 0x80 && (unsigned char)*szSource <= 0xfc))
            isRet = true;

    return isRet;
}

//---------------------------------------------------
//関数名	compressionSpace(TCHAR *szSource)
//機能		複数のスペースを一つに纏める
//---------------------------------------------------
int UStringWork::compressionSpace(TCHAR* szSource)
{
    int i;
    for (i = 0; *szSource; i++, szSource++) {
        if (*szSource == _T(' ')) {
            TCHAR* szSpaceEnd;
            szSpaceEnd = szSource;
            int j;
            for (j = 0; *szSpaceEnd && *szSpaceEnd == _T(' '); j++, szSpaceEnd++);
            UStringWork::moveForward(szSpaceEnd, j - 1);
            szSource++;
        }
    }
    return i;
}

//---------------------------------------------------
//関数名	moveForward(TCHAR *szMovePos,int nMove)
//機能		文字列を前に移動(メモリ範囲に注意)
//---------------------------------------------------
int UStringWork::moveForward(TCHAR* szMovePos, int nMove)
{
    //文字列移動
    int i;
    for (i = 0; *szMovePos; i++, szMovePos++) {
        *(szMovePos - nMove) = *szMovePos;
    }
    *(szMovePos - nMove) = NULL;
    return i;
}
