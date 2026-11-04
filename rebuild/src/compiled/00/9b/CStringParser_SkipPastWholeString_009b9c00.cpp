#include "fable_string_parser.h"

bool CStringParser::SkipPastWholeString(const CCharString& text)
{
    if (!SkipUntilWholeString(text)) return false;
    unsigned long length = text.GetLength();
    if (length)
    {
        const char* cursor = String + StringPos;
        const char* end = cursor + length;
        bool state = InQuotes;
        while (cursor != end)
        {
            if (*cursor == 34 || *cursor == 39) state = !state;
            ++cursor;
        }
        StringPos += length;
        InQuotes = state;
    }
    return true;
}
