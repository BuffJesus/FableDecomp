// lib_basic_string, retail <char> 0x009A0040-0x009A05E0: CBasicString's
// copy, truncate and erase members (FableWin names). Truncate reallocates to
// the aligned size of the shorter string, sets the new length and zeroes the
// buffer's tail; Erase closes the gap and truncates. The two filling
// constructors are emitted out of line here (0x009A0550, 0x009A05E0).
#include <new>
#include <string.h>
#include "fable_basic_string.h"

template <class T>
unsigned long __fastcall CBasicString<T>::Align(unsigned long length)
{
    return (length + 3) & ~3;
}

template <class T>
void __fastcall CBasicString<T>::CopyAndTerminate(T* dest, const T* src, unsigned long length)
{
    memcpy(dest, src, length * sizeof(T));
    dest[length] = 0;
}

template <class T>
void CBasicString<T>::AllocateDataFromDataLength(unsigned long dataLength, T** data)
{
    *data = new T[dataLength];
}

template <class T>
unsigned long CBasicString<T>::AllocateDataFromStringLength(unsigned long length, T** data)
{
    unsigned long dataLength = Align(length + 1);
    AllocateDataFromDataLength(dataLength, data);
    return dataLength;
}

template <class T>
void CBasicString<T>::ClearStringEnd(T* data, unsigned long length)
{
    unsigned long start = length + 1;
    memset(data + start, 0, (Align(start) - start) * sizeof(T));
}

template <class T>
void CBasicString<T>::SetStringLength(long length)
{
    StringLength = length;
    PData[length] = 0;
    ClearStringEnd(PData, StringLength);
}

template <class T>
void CBasicString<T>::Truncate(unsigned long length)
{
    if (length < StringLength)
    {
        T* data;
        unsigned long dataLength = AllocateDataFromStringLength(length, &data);
        CopyAndTerminate(data, PData, length);
        if (PData != 0)
        {
            delete[] PData;
            PData = 0;
        }
        DataLength = dataLength;
        PData = data;
        SetStringLength(length);
    }
}

template <class T>
void CBasicString<T>::Erase(unsigned long start, unsigned long count)
{
    unsigned long end = start + count;
    memmove(PData + start, PData + end, (StringLength - end) * sizeof(T));
    Truncate(StringLength - count);
}

template void CBasicString<char>::Truncate(unsigned long);
template void CBasicString<char>::Erase(unsigned long, unsigned long);
template void __fastcall CBasicString<char>::CopyAndTerminate(char*, const char*, unsigned long);

template CBasicString<char>::CBasicString(unsigned long, char);
template CBasicString<char>::CBasicString(const char*, unsigned long);
