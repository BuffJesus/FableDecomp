#include "fable_string_parser.h"

void CStringParser::SetStringPos(unsigned long position)
{
    const char* cursor = String;
    const char* end = String + position;
    bool inQuotes = false;
    StringPos = position;
    while (cursor != end)
    {
        if (*cursor == '\"' || *cursor == '\'')
            inQuotes = !inQuotes;
        ++cursor;
    }
    InQuotes = inQuotes;
}
