#include "fable_string_parser.h"

CCharString CStringParser::ReadAsStringUntilPastString(const CCharString& text)
{
    const char* start = String + StringPos;
    if (SkipPastString(text))
        return CCharString(start, static_cast<long>(String + StringPos - start));
    return "";
}
