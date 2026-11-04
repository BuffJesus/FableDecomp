// CBasicString<char>::CBasicString(char const*) @ 0x009A0590.
#include "fable_basic_string.h"

// An empty, fast-extending string set to a copy of the C string.
template <class T>
CBasicString<T>::CBasicString(const T* text)
    : PData(0), StringLength(0), DataLength(0), UseFastExtend(true)
{
    unsigned long length = 0;
    while (text[length] != 0)
        ++length;
    SetString(text, length);
}

template CBasicString<char>::CBasicString(const char* text);
