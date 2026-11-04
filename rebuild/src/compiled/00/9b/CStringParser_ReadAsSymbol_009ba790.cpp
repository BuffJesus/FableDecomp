#include "fable_string_parser.h"

CCharString CStringParser::ReadAsSymbol()
{
    CParsedItem item;
    if (ReadNextItem(item) && item.Type == CParsedItem::SYMBOL)
        return item.String;
    Error("Error parsing symbol");
    return "";
}
