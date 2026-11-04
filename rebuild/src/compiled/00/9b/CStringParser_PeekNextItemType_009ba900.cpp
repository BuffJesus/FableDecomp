#include "fable_string_parser.h"

EParsedItemType CStringParser::PeekNextItemType()
{
    CParsedItem item;
    unsigned long* cursor = reinterpret_cast<unsigned long*>(&InQuotes);
    unsigned long quoteWord = cursor[0];
    unsigned long position = cursor[1];
    unsigned long line = cursor[2];
    if (!ReadNextItem(item)) return static_cast<EParsedItemType>(0);
    cursor[0] = quoteWord;
    cursor[1] = position;
    cursor[2] = line;
    return item.Type;
}
