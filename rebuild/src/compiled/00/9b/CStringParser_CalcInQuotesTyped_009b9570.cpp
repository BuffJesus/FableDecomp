#include "fable_string_parser.h"
bool CStringParser::CalcInQuotes(bool state, const char* start, const char* end) const
{
    while (start != end)
    {
        if (*start == 34 || *start == 39) state = !state;
        ++start;
    }
    return state;
}
