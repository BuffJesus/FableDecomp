// lib_char_string.cpp, retail 0x0099E900: the negation of the inline
// operator== (shared data, then null, then length and characters).
#include "fable_string.h"

bool CCharString::operator!=(const CCharString& other) const
{
    return !(*this == other);
}
