// lib_string_parser.cpp, retail 0x009BA2D0-0x009BA650: text readers that
// return everything up to a boundary (whitespace, end of line, a whole word),
// and the float reader (an integer item converts).
#include "fable_string_parser.h"

// Defined earlier in the same file (out of line at 0x009B9570); expanded below.
__forceinline bool CStringParser::CalcInQuotes(bool state, const char* start, const char* end) const
{
    while (start != end)
    {
        if (*start == 34 || *start == 39)
            state = !state;
        ++start;
    }
    return state;
}

// Retail 009B9D70 (near match: retail pushes ebx/edi only around the quote
// scan and reloads String + StringPos there): to the end of the line and past its line break(s); the
// number of characters moved. ReadLineAsString below relies on it leaving
// ecx intact, which VC7.1 only knows within one unit.
long CStringParser::SkipPastNewline()
{
    const char* start = String + StringPos;
    const char* p = start;
    while (*p != 0)
    {
        if (*p == '\n' || *p == '\r')
        {
            while (*p == '\n' || *p == '\r')
                ++p;
            break;
        }
        ++p;
    }
    long count = static_cast<long>(p - start);
    if (count != 0)
    {
        InQuotes = CalcInQuotes(InQuotes, String + StringPos, String + StringPos + count);
        StringPos += count;
    }
    return count;
}

// Retail 009BA2D0 (near match: retail tests *start straight from memory).
bool CStringParser::ReadAsStringUntilWhitespace(CParsedItem& item)
{
    const char* start = String + StringPos;
    if (*start != 0)
    {
        SkipUntilWhitespace();
        item.InitAsString(CCharString(start, static_cast<long>(String + StringPos - start)));
        return true;
    }
    return false;
}

// Retail 009BA390.
CCharString CStringParser::ReadLineAsString()
{
    const char* start = String + StringPos;
    if (SkipPastNewline())
        return CCharString(start, static_cast<long>(String + StringPos - start));
    return "";
}

// Retail 009BA440.
CCharString CStringParser::ReadAsStringUntilWholeString(const CCharString& text)
{
    const char* start = String + StringPos;
    if (SkipUntilWholeString(text))
        return CCharString(start, static_cast<long>(String + StringPos - start));
    return "";
}

// Retail 009BA650 (near match: retail keeps the FLOAT case out of line and
// converts the integer after the item is destroyed).
float CStringParser::ReadAsFloat()
{
    CParsedItem item;
    if (ReadNextItem(item))
    {
        if (item.Type == CParsedItem::FLOAT)
            return item.Value.Float;
        if (item.Type == CParsedItem::INTEGER)
            return static_cast<float>(item.Value.Integer);
    }
    Error("Error parsing float");
    return 0.0f;
}
