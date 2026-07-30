#pragma once

#include "fable_string.h"

// TLC's stripped 0x00415530 leaf was misidentified from a donor virtual
// GetActionName. Its retail ABI is a no-argument fastcall value factory: the
// hidden CCharString return slot arrives in ECX and the leaf fills it with the
// default language name.
CCharString FABLE_FASTCALL FableGetDefaultLanguageName_00415530();

// Retail language selector used by the startup font-bank lookup.
fable_i32 FableGetCurrentLanguage_00415070();
CCharString FABLE_FASTCALL FableGetFontBankName_00415260();
CCharString FABLE_FASTCALL FableGetStreamingFontBankName_00415440();
