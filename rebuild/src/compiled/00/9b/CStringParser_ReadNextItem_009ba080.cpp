#include "fable_string_parser.h"
#include <ctype.h>

bool CStringParser::ReadNextItem(CParsedItem& item)
{
    SkipWhitespace();
    unsigned char character = String[StringPos];
    if (isdigit(character) || (character == '-' && isdigit(static_cast<unsigned char>(String[StringPos + 1]))))
        return ReadNextNumber(item);
    if (isalpha(character) || character == '_')
        return ReadNextString(item);
    return ReadNextSymbol(item);
}
