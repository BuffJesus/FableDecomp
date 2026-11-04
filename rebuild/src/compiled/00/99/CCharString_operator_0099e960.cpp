// CCharString::operator!=(const char*) const @ 0x0099E960
// (FableWin ??9CCharString@@QBE_NPBD@Z).
//
// The negation of the inline operator== (an empty string equals "").

#include "fable_string.h"

bool CCharString::operator!=(const char* other) const
{
    return !(*this == other);
}