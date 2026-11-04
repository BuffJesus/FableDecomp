#include "fable_string_parser.h"

bool CStringParser::SkipPastNextItemIfIs(const CCharString& text)
{
    unsigned long* cursor = reinterpret_cast<unsigned long*>(&InQuotes);
    unsigned long quoteWord = cursor[0];
    unsigned long position = cursor[1];
    unsigned long line = cursor[2];
    SkipWhitespace();
    long matched = 0;
    const char* expected = text.GetConstCharPtr();
    while (matched < text.GetLength() && expected[matched] == String[StringPos + matched])
        ++matched;
    if (matched == text.GetLength())
    {
        if (matched > 0)
        {
            InQuotes = CalcInQuotes(InQuotes, String + StringPos, String + StringPos + matched);
            StringPos += matched;
        }
        return true;
    }
    cursor[0] = quoteWord;
    cursor[1] = position;
    cursor[2] = line;
    return false;
}
