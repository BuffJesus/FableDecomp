#include "fable_string_parser.h"
#include <ctype.h>
#include <string.h>

bool CStringParser::SkipUntilWholeString(const CCharString& text)
{
    const char* needle = text;
    const char* found = strstr(String + StringPos, needle);
    while (found)
    {
        unsigned long position = static_cast<unsigned long>(found - String);
        unsigned char before = position ? String[position - 1] : 0;
        unsigned char after = String[position + text.GetLength()];
        if ((!position || (!isalpha(before) && !isdigit(before) && before != '_')) &&
            !isalpha(after) && !isdigit(after) && after != '_')
        {
            bool state = InQuotes;
            for (const char* cursor = String + StringPos; cursor != found; ++cursor)
                if (*cursor == 34 || *cursor == 39) state = !state;
            InQuotes = state;
            StringPos = position;
            return true;
        }
        found = strstr(found + 1, needle);
    }
    return false;
}
