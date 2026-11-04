#include "fable_string_parser.h"

bool CStringParser::ReadNextSymbol(CParsedItem& item)
{
    const char* start = String + StringPos;
    if (!*start) return false;
    item.String.SetAsSubString(start, 1);
    item.Type = CParsedItem::SYMBOL;
    unsigned long position = StringPos;
    start = String + position;
    bool state = CalcInQuotes(InQuotes, start, start + 1);
    StringPos = position + 1;
    InQuotes = state;
    return true;
}
