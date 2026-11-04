#include "fable_string_parser.h"

CCharString CStringParser::ReadAsStringUntilString(const CCharString& text)
{
    const char* start = String + StringPos;
    if (SkipUntilString(text))
        return CCharString(start, static_cast<long>(String + StringPos - start));
    return "";
}
