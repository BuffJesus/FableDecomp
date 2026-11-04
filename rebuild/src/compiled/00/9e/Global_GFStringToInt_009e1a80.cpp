// lib_global_tools.cpp, retail 0x009E1A80 (Ego_r ?GFStringToInt@@YIJPB_W@Z):
// decimal digits to a long. Any '-' anywhere makes the result negative, and
// every other character is taken as a digit. The sign is applied through a
// separate result local; negating `value` in place lets VC7.1 thread the
// empty-string exit past the `negative` test, which retail does not do.
#include <wchar.h>
#include "rebuild_abi.h"

long FABLE_FASTCALL GFStringToInt(const wchar_t* string)
{
    long value = 0;
    bool negative = false;
    unsigned long length = wcslen(string);
    unsigned long i = 0;
    wchar_t c = string[i];
    while (i < length)
    {
        if (c == L'-')
            negative = true;
        else
            value = value * 10 + c - L'0';
        c = string[++i];
    }
    long result = value;
    if (negative == true)
        result = -value;
    return result;
}
