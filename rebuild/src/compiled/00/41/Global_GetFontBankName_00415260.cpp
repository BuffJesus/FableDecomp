#include "fable_language.h"

CCharString FABLE_FASTCALL
FableGetFontBankName_00415260()
{
    switch (FableGetCurrentLanguage_00415070())
    {
    case 16:
        return CCharString("TEXT_FRENCH_MAIN", -1);
    case 17:
        return CCharString("TEXT_JAPANESE_MAIN", -1);
    case 7:
        return CCharString("TEXT_GERMAN_MAIN", -1);
    case 18:
        return CCharString("TEXT_KOREAN_MAIN", -1);
    case 4:
        return CCharString("TEXT_CHINESE_MAIN", -1);
    case 10:
        return CCharString("TEXT_SPANISH_MAIN", -1);
    case 12:
        return CCharString("TEXT_ITALIAN_MAIN", -1);
    default:
        return CCharString("TEXT_ENGLISH_MAIN", -1);
    }
}
