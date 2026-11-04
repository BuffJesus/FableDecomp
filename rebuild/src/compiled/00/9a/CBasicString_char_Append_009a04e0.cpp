// CBasicString<char>::Append(char const*, unsigned long) @ 0x009A04E0 (FableWin
// ?Append@?$CBasicString@D@@QAEXPBDK@Z; CCharString::operator+= 0x0099F0A0).
#include "fable_basic_string.h"

#include <string.h>

// Grow the buffer, copy the text after the current end and terminate it,
// then extend the length and zero the buffer up to the next 4-byte boundary.
template <class T>
void CBasicString<T>::Append(const T* text, unsigned long length)
{
    ExtendData(StringLength + length);
    T* end = PData + StringLength;
    memcpy(end, text, length);
    end[length] = 0;

    unsigned long newLength = StringLength + length;
    StringLength = newLength;
    PData[newLength] = 0;
    unsigned long clearFrom = StringLength + 1;
    memset(PData + clearFrom, 0, ((clearFrom + 3) & ~3) - clearFrom);
}

template void CBasicString<char>::Append(const char* text, unsigned long length);
