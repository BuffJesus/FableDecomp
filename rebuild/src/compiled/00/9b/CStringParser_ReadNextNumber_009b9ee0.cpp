#include "fable_string_parser.h"
#include <ctype.h>

bool CStringParser::ReadNextNumber(CParsedItem& item)
{
    const char* start = String + StringPos;
    const char* cursor = start;
    bool hasPoint = false;
    while (*cursor)
    {
        unsigned char character = *cursor;
        if (!isdigit(character) && !(character == '.' && !hasPoint) &&
            !(character == '-' && cursor == String + StringPos))
            break;
        if (*cursor == '.') hasPoint = true;
        ++cursor;
    }
    CCharString value(start, static_cast<long>(cursor - start));
    if (hasPoint) item.InitAsFloat(value, value.GetLength());
    else item.InitAsInteger(value, value.GetLength());
    long length = static_cast<long>(cursor - (String + StringPos));
    if (length)
    {
        unsigned long position = StringPos;
        start = String + position;
        bool state = CalcInQuotes(InQuotes, start, start + length);
        InQuotes = state;
        StringPos = position + length;
    }
    return true;
}
