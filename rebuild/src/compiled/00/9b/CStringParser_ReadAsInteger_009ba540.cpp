#include "fable_string_parser.h"

long CStringParser::ReadAsInteger()
{
    CParsedItem item;
    if (ReadNextItem(item) && item.Type == CParsedItem::INTEGER)
        return item.Value.Integer;
    Error("Error parsing integer");
    return 0;
}
