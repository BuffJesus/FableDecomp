#include "fable_string_parser.h"
#include <ctype.h>

bool CStringParser::ReadNextString(CParsedItem& item)
{
    const char* cursor = String + StringPos;
    while (*cursor && (isdigit(static_cast<unsigned char>(*cursor)) ||
                       isalpha(static_cast<unsigned char>(*cursor)) || *cursor == '_'))
        ++cursor;
    CCharString value(String + StringPos, static_cast<long>(cursor - (String + StringPos)));
    item.String.SetAsSubString(value, value.GetLength());
    item.Type = CParsedItem::STRING;
    long length = static_cast<long>(cursor - (String + StringPos));
    if (length)
    {
        unsigned long position = StringPos;
        const char* start = String + position;
        bool state = CalcInQuotes(InQuotes, start, start + length);
        InQuotes = state;
        StringPos = position + length;
    }
    return true;
}
