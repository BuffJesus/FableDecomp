#include <ctype.h>
#include "fable_string.h"

// lib_string.cpp number helpers (Ego_r locals found_dp, is_negative).
// Retail 0099E620, 0099E7F0 and 0099E870; isdigit is the CRT function (0x00BFEDA6).

// Retail 0099E620: digits with at most one '.', and '-' only first.
bool FABLE_FASTCALL GFIsCharStringNumber(const CCharString& cstr)
{
    bool found_dp = false;
    long i = 0;
    char c = cstr.GetConstCharPtr()[i];
    while (i < cstr.GetLength())
    {
        if (c == '.')
        {
            if (found_dp)
                return false;
            found_dp = true;
        }
        else if (c == '-')
        {
            if (i != 0)
                return false;
        }
        else if (!isdigit((unsigned char)c))
        {
            return false;
        }
        c = cstr.GetConstCharPtr()[++i];
    }
    return true;
}

// Retail 0099E7F0: the integer part; a '-' anywhere negates, '.' stops.
long FABLE_FASTCALL GFCharStringToInt(const CCharString& cstr)
{
    long value = 0;
    bool is_negative = false;
    unsigned long i = 0;
    char c = cstr.GetConstCharPtr()[i];
    while (i < (unsigned long)cstr.GetLength())
    {
        if (c == '-')
            is_negative = true;
        else if (c == '.')
            break;
        else if (isdigit((unsigned char)c))
            value = value * 10 + c - '0';
        c = cstr.GetConstCharPtr()[++i];
    }
    if (is_negative)
        value = -value;
    return value;
}

// Retail 0099E870: as GFCharStringToInt, unsigned, so a '-' is skipped.
unsigned __int64 FABLE_FASTCALL GFCharStringToUHUGE(const CCharString& cstr)
{
    unsigned __int64 value = 0;
    unsigned long i = 0;
    char c = cstr.GetConstCharPtr()[i];
    while (i < (unsigned long)cstr.GetLength())
    {
        if (c == '-')
        {
        }
        else if (c == '.')
            break;
        else if (isdigit((unsigned char)c))
            value = value * 10 + (c - '0');
        c = cstr.GetConstCharPtr()[++i];
    }
    return value;
}
