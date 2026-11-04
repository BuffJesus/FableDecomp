// lib_string_parser.cpp, retail 0x009BA5B0: an unsigned 64-bit decimal after
// any whitespace (digits only; the position moves past them). MoveStringPos,
// defined earlier in the same file, is expanded; CalcInQuotes stays a call.
// CalcInQuotes is defined here as well as in its own unit (0x009B9570): in
// retail's single lib_string_parser.cpp VC7.1 sees that it only reads memory,
// so callers keep StringPos in a register or reload it instead of spilling.
// Only the gated roots below are indexed from this unit.
#include <ctype.h>
#include "fable_string_parser.h"

bool CStringParser::CalcInQuotes(bool state, const char* start, const char* end) const
{
    while (start != end)
    {
        if (*start == 34 || *start == 39)
            state = !state;
        ++start;
    }
    return state;
}

inline void CStringParser::MoveStringPos(unsigned long count)
{
    if (count > 0)
    {
        InQuotes = CalcInQuotes(InQuotes, String + StringPos, String + StringPos + count);
        StringPos += count;
    }
}

unsigned __int64 CStringParser::ReadAsUHUGE()
{
    SkipWhitespace();
    const char* p = String + StringPos;
    unsigned __int64 value = 0;
    while (*p != 0 && isdigit(static_cast<unsigned char>(*p)))
    {
        value = value * 10 + (*p - '0');
        ++p;
    }
    MoveStringPos(p - (String + StringPos));
    return value;
}
