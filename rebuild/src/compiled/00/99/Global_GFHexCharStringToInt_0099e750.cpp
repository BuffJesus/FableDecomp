// lib_char_string.cpp, retail 0x0099E750: a hexadecimal value after leading
// spaces and tabs; digits in either case, up to the first other character.
#include "fable_string.h"

unsigned long FABLE_FASTCALL GFHexCharStringToInt(const CCharString& text)
{
    unsigned long value = 0;
    long i = 0;
    while (i < text.GetLength())
    {
        char c = text.GetConstCharPtr()[i];
        if (c != ' ' && c != '\t')
            break;
        ++i;
    }
    while (i < text.GetLength())
    {
        char c = text.GetConstCharPtr()[i];
        long digit;
        if (c >= '0' && c <= '9')
            digit = c - '0';
        else if (c >= 'a' && c <= 'f')
            digit = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F')
            digit = c - 'A' + 10;
        else
            break;
        value = value * 16 + digit;
        ++i;
    }
    return value;
}
