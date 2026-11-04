#include "fable_string_parser.h"
#include <ctype.h>

long CStringParser::SkipWhitespace()
{
    const char* cursor = String + StringPos;
    if (!*cursor) return 0;
    while (isspace(static_cast<unsigned char>(*cursor)) || *cursor == '\n')
    {
        if (!*cursor) break;
        ++cursor;
    }
    long skipped = static_cast<long>(cursor - (String + StringPos));
    if (skipped)
    {
        bool state = InQuotes;
        for (const char* p = String + StringPos; p != cursor; ++p)
            if (*p == 34 || *p == 39) state = !state;
        InQuotes = state;
        StringPos += skipped;
    }
    return skipped;
}
