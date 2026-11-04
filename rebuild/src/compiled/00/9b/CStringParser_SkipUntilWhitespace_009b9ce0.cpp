#include "fable_string_parser.h"
#include <ctype.h>

long CStringParser::SkipUntilWhitespace()
{
    const char* start = String + StringPos;
    const char* end = start;
    while (!isspace(static_cast<unsigned char>(*end)) && *end) ++end;
    long length = static_cast<long>(end - start);
    if (length)
    {
        InQuotes = CalcInQuotes(InQuotes, start, end);
        StringPos += length;
    }
    return length;
}
