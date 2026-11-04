// lib_file_abstract.cpp, retail 0x00998170-0x0099824A: CAFile's private path
// parser look-aheads (Ego_r names). The Is...Next forms save StringPos, try
// the reader and put the position back.
#define FABLE_CHAR_STRING_EQUALS_FORCEINLINE
#include "fable_filesystem.h"
#include "fable_string.h"
#include "fable_string_parser.h"

// Retail 00997DA0: the next item is the symbol "\" or "/".
bool FABLE_FASTCALL CAFile::ParserIsSeparatorNext(CStringParser& parser)
{
    bool result = false;
    CParsedItem item;
    if (parser.PeekNextItem(item) && item.Type == CParsedItem::SYMBOL &&
        (item.GetSymbol() == '\\' || item.GetSymbol() == '/'))
        result = true;
    return result;
}

// Retail 00997E90: as ParserIsSeparatorNext, consuming the item.
bool FABLE_FASTCALL CAFile::ParserReadPastSeparator(CStringParser& parser)
{
    bool result = false;
    CParsedItem item;
    if (parser.ReadNextItem(item) && item.Type == CParsedItem::SYMBOL &&
        (item.GetSymbol() == '\\' || item.GetSymbol() == '/'))
        result = true;
    return result;
}

// Retail 00998170: a string item followed by the symbol ":".
bool FABLE_FASTCALL CAFile::ParserReadPastDriveLabel(CStringParser& parser, CParsedItem& item)
{
    if (!parser.ReadNextItem(item) || item.Type != CParsedItem::STRING)
        return false;
    if (!parser.ReadNextItem(item) || item.Type != CParsedItem::SYMBOL || !(item.GetSymbol() == ":"))
        return false;
    return true;
}

