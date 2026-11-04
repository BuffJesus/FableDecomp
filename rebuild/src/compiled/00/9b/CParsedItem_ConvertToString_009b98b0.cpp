// lib_string_parser.cpp, retail 0x009B98B0 and 0x009B9980.
#include "fable_string_parser.h"

// CalcInQuotes is defined here as well as in its own unit (0x009B9570): in
// retail's single lib_string_parser.cpp VC7.1 sees that it only reads memory,
// so callers keep StringPos in a register or reload it instead of spilling.
// Only the gated roots below are indexed from this unit.

// Retail 009B98B0: the item as text (numbers formatted, floats to six
// places); an uninitialised item reads "NULL".
CCharString CParsedItem::ConvertToString() const
{
    switch (Type)
    {
    case PARSED_INTEGER:
        return GFIntToCharString(Value.Integer);
    case PARSED_FLOAT:
        return GFFloatToCharString(Value.Float, 6);
    case PARSED_SYMBOL:
    case PARSED_STRING:
        return String;
    }
    return "NULL";
}

bool CStringParser::CalcInQuotes(bool state, const char* start, const char* end) const
{
    while (start != end)
    {
        if (*start == 34 || *start == 39)
            state = !state;
        ++start;
    }
    return state;
}

// Retail 009B9980: advance the read position, keeping the in-quotes state.
void CStringParser::MoveStringPos(unsigned long count)
{
    if (count > 0)
    {
        InQuotes = CalcInQuotes(InQuotes, String + StringPos, String + StringPos + count);
        StringPos += count;
    }
}
