#include "fable_string.h"

// lib_string.cpp, retail 0099E520 (Ego_r out-of-line operator!=(char) over
// the inline operator==(char)).
bool CCharString::operator!=(char c) const
{
    return !(*this == c);
}
