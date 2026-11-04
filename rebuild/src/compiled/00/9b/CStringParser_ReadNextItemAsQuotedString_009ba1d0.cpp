// lib_string_parser.cpp, retail 0x009BA1D0 (Ego_r
// ?ReadNextItemAsQuotedString@CStringParser@@QAE_NAAVCCharString@@@Z): read a
// quoted string after any whitespace. The closing quote is the first ' or "
// not preceded by a backslash; in the text, "\\" becomes one backslash and any
// other backslash is dropped. The read position moves past the closing quote.
// CalcInQuotes, defined earlier in the same file, is expanded here.
#include "fable_string_parser.h"

inline bool CStringParser::CalcInQuotes(bool state, const char* start, const char* end) const
{
    while (start != end)
    {
        if (*start == 34 || *start == 39)
            state = !state;
        ++start;
    }
    return state;
}

bool CStringParser::ReadNextItemAsQuotedString(CCharString& string)
{
    SkipWhitespace();
    bool escaped = false;
    long end = StringPos + 1;
    for (; String[end] != 0; ++end)
    {
        if (escaped)
            escaped = false;
        else if (String[end] == '\\')
            escaped = true;
        else if (String[end] == '"' || String[end] == '\'')
        {
            CCharString result;
            for (long i = StringPos + 1; i < end; ++i)
            {
                if (String[i] == '\\')
                {
                    if (i + 1 < end && String[i + 1] == '\\')
                    {
                        result += '\\';
                        ++i;
                    }
                }
                else
                    result += String[i];
            }
            long count = end - StringPos + 1;
            if (count)
            {
                InQuotes = CalcInQuotes(InQuotes, String + StringPos, String + StringPos + count);
                StringPos += count;
            }
            string = result;
            return true;
        }
    }
    return false;
}
