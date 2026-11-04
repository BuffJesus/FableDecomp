#pragma once

#include "fable_string.h"

namespace NLocalisation
{
    // Retail 0x00415070: detect once from registered text-bank handles.
    unsigned long FABLE_FASTCALL GetLanguage();
    // Retail 0x00415530: hidden value-return slot in ECX.
    CCharString FABLE_FASTCALL GetLanguageFolder();
    // Retail 0x00415260: TEXT_*_MAIN handle for the detected language.
    CCharString FABLE_FASTCALL GetTextBankName();
}

CCharString FABLE_FASTCALL FableGetStreamingFontBankName_00415440();
