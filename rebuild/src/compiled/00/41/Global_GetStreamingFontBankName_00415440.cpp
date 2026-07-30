#include "fable_language.h"

CCharString FABLE_FASTCALL
FableGetStreamingFontBankName_00415440()
{
    switch (FableGetCurrentLanguage_00415070())
    {
    case 16:
        return CCharString("STREAMING_FONT_FRENCH_PC", -1);
    case 17:
        return CCharString("STREAMING_FONT_JAPANESE_PC", -1);
    case 7:
        return CCharString("STREAMING_FONT_GERMAN_PC", -1);
    case 18:
        return CCharString("STREAMING_FONT_KOREAN_PC", -1);
    case 4:
        return CCharString("STREAMING_FONT_CHINESE_PC", -1);
    case 10:
        return CCharString("STREAMING_FONT_SPANISH_PC", -1);
    case 12:
        return CCharString("STREAMING_FONT_ITALIAN_PC", -1);
    default:
        return CCharString("STREAMING_FONT_ENGLISH_PC", -1);
    }
}
