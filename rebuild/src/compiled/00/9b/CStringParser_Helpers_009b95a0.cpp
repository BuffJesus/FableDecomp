// lib_string_parser.cpp, retail 0x009B95A0-0x009B9850: small parser and
// parsed-item helpers (Ego_r names).
#include <ctype.h>
#define FABLE_PARSED_ITEM_OUT_OF_LINE
#include "fable_string_parser.h"

// Retail 009B95A0: line breaks before the read position.
long CStringParser::CountLinesParsed() const
{
    long lines = 0;
    for (unsigned long i = 0; i < StringPos; ++i)
    {
        if (String[i] == '\n')
            ++lines;
    }
    return lines;
}

// Retail 009B95C0.
CCharString CStringParser::ReadStringBetween(const char* start, const char* end)
{
    return CCharString(start, static_cast<long>(end - start));
}

// Retail 009B96A0: anything but a letter, digit or underscore.
bool CStringParser::CharSeperatesString(char c)
{
    if (isalpha(static_cast<unsigned char>(c)) || isdigit(static_cast<unsigned char>(c)) || c == '_')
        return false;
    return true;
}

// Retail 009B96E0.
void CParsedItem::InitAsSymbol(const char* text, unsigned long length)
{
    String.SetAsSubString(text, length);
    Type = PARSED_SYMBOL;
}

// Retail 009B9830.
void CParsedItem::InitAsString(const char* text, unsigned long length)
{
    String.SetAsSubString(text, length);
    Type = PARSED_STRING;
}

// Retail 009B9850.
void CParsedItem::InitAsString(const CCharString& text)
{
    String = text;
    Type = PARSED_STRING;
}
