#include <stddef.h>

const wchar_t* __fastcall WordWrap_FindNonWhiteSpaceForward(const wchar_t* text)
{
    while (*text == L'\t' || *text == L'\r' || *text == L' ' || *text == 0x3000)
        ++text;
    if (*text == L'\n')
        ++text;
    return *text != 0 ? text : 0;
}