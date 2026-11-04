#include "fable_string_parser.h"
#include <string.h>

bool CStringParser::SkipUntilString(const CCharString& text)
{
    const char* start = String + StringPos;
    if (!*start) return false;
    const char* found = strstr(start, text);
    if (!found)
    {
        SetStringPos(static_cast<unsigned long>(strlen(String) + 1));
        return false;
    }
    unsigned long length = static_cast<unsigned long>(found - start);
    if (length)
    {
        InQuotes = CalcInQuotes(InQuotes, start, start + length);
        StringPos += length;
    }
    return true;
}
