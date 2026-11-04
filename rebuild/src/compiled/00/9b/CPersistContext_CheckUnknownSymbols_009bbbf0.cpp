#include "fable_persist_context.h"
#include "fable_string_parser.h"

void CPersistContext::CheckUnknownSymbols()
{
    CStringParser parser(*PParser);
    parser.SkipWhitespace();
    while (parser.NextItemExists())
    {
        PersistError(parser, "Unknown entry \"" + parser.ReadAsStringUntilWhitespace() + "\"");
        parser.SkipWhitespace();
    }
}
