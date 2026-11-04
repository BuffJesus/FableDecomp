#include "fable_string_parser.h"

void CStringParser::Error(const CCharString& message)
{
    CCharString formatted = Filename + "(" + GFIntToCharString(Line + 1) + ")" + " : error : " + message;
    if (ErrorCallback) ErrorCallback(formatted);
}
