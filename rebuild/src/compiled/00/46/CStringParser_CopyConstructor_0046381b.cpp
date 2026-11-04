#include "fable_string_parser.h"

CStringParser::CStringParser(const CStringParser& other)
    : CBaseClass(other), String(other.String), BaseLineOffset(other.BaseLineOffset),
      Filename(other.Filename), TempCharString(other.TempCharString),
      Comments(other.Comments), ErrorCallback(other.ErrorCallback)
{
    // Retail copies CCursorPos as three words, including InQuotes padding.
    unsigned long* cursor = reinterpret_cast<unsigned long*>(&InQuotes);
    const unsigned long* source = reinterpret_cast<const unsigned long*>(&other.InQuotes);
    cursor[0] = source[0];
    cursor[1] = source[1];
    cursor[2] = source[2];
}
