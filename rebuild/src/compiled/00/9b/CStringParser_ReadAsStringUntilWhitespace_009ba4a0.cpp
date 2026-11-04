#include "fable_string_parser.h"

CCharString CStringParser::ReadAsStringUntilWhitespace()
{
    const char* start = String + StringPos;
    if (*start)
    {
        SkipUntilWhitespace();
        return CCharString(start, static_cast<long>(String + StringPos - start));
    }
    return "";
}
