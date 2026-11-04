#include "fable_string_parser.h"

CCharString CStringParser::ReadAsString()
{
    CParsedItem item;
    if (ReadNextItem(item) && item.Type == CParsedItem::STRING)
        return item.String;
    Error("Error parsing string");
    return "";
}
