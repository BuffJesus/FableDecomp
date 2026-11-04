#include "fable_persist_context.h"
#include "fable_string_parser.h"

void CPersistContext::PersistError(const CStringParser& parser, const CCharString& message) const
{
    CCharString formatted = parser.Filename + "(" + GFIntToCharString(parser.Line + 1) + ")" + " : error : " + message;
    ErrorCallback(formatted);
}
