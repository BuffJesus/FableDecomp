#include "fable_string_parser.h"

bool CStringParser::NextItemExists()
{
    unsigned long* cursor = reinterpret_cast<unsigned long*>(&InQuotes);
    unsigned long quoteWord = cursor[0];
    unsigned long position = cursor[1];
    unsigned long line = cursor[2];
    SkipWhitespace();
    if (!String[StringPos]) return false;
    cursor[0] = quoteWord;
    cursor[1] = position;
    cursor[2] = line;
    return true;
}
