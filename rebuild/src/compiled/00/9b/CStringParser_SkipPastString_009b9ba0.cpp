#include "fable_string_parser.h"

bool CStringParser::SkipPastString(const CCharString& text)
{
    if (!SkipUntilString(text)) return false;
    long length = text.GetLength();
    if (length > 0)
    {
        const char* start = String + StringPos;
        InQuotes = CalcInQuotes(InQuotes, start, start + length);
        StringPos += length;
    }
    return true;
}
