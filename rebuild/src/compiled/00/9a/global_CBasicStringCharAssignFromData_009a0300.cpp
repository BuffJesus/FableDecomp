// CBasicString<char>::SetString(char const*, unsigned long) @ 0x009A0300.
#include "fable_basic_string.h"

#include <new>
#include <string.h>

// Replace the buffer with a fresh one sized for the text plus its terminator,
// rounded up to four bytes; the terminator and the rounding slack are zeroed
// before the text is copied in.
template <class T>
void CBasicString<T>::SetString(const T* text, unsigned long length)
{
    if (PData != 0)
    {
        delete[] PData;
        PData = 0;
    }

    unsigned long dataLength = (length + 4) & ~3;
    PData = new T[dataLength];
    DataLength = dataLength;
    StringLength = length;
    PData[length] = 0;

    unsigned long end = StringLength + 1;
    memset(PData + end, 0, ((end + 3) & ~3) - end);
    memcpy(PData, text, length);
}

template void CBasicString<char>::SetString(const char* text, unsigned long length);
