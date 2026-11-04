#include "fable_string_parser.h"
#include "fable_persist_context.h"

void CPersistTraits<bool>::TransferIn(CPersistContext& context, CStringParser& parser, bool& value)
{
    CCharString token = parser.ReadAsString();
    if (token.EqualsCaseInsensitive("TRUE") || token.EqualsCaseInsensitive("BTRUE"))
        value = true;
    else if (token.EqualsCaseInsensitive("FALSE") || token.EqualsCaseInsensitive("BFALSE"))
        value = false;
    else
        context.PersistError(parser, "Unknown boolean value entry " + token);
}
