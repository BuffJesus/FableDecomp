#pragma once

#include "fable_string.h"

// TLC's stripped 0x00415530 leaf was misidentified from a donor virtual
// GetActionName. Its retail ABI is a no-argument fastcall value factory: the
// hidden CCharString return slot arrives in ECX and the leaf fills it with the
// default language name.
CCharString FABLE_FASTCALL FableGetDefaultLanguageName_00415530();
