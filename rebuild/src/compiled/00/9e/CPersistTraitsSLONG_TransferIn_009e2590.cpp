#include "fable_persist_context.h"
#include "fable_string_parser.h"
#include "fable_symbol_map.h"

void FABLE_FASTCALL CPersistTraitsSLONG::TransferIn(CPersistContext& context, CStringParser& parser, long& value)
{
    if (parser.PeekNextItemType() == PARSED_STRING)
    {
        context.PSymbolMap->UpdateDirtyQuick();
        CCharString name = parser.ReadAsString();
        long found = 0;
        const CSymbolMap* symbols = context.PSymbolMap;
        if (!symbols->FindSymbol(name, found))
            context.PersistError(parser, "Unable to find symbol " + name);
        value = found;
    }
    else
        value = parser.ReadAsInteger();
    if (parser.SkipPastNextItemIfIs("|"))
    {
        long rhs;
        TransferIn(context, parser, rhs);
        value |= rhs;
    }
    else if (parser.SkipPastNextItemIfIs("+"))
    {
        long rhs;
        TransferIn(context, parser, rhs);
        value += rhs;
    }
    else if (parser.SkipPastNextItemIfIs("-"))
    {
        long rhs;
        TransferIn(context, parser, rhs);
        value -= rhs;
    }
    else if (parser.SkipPastNextItemIfIs("<<"))
    {
        long rhs;
        TransferIn(context, parser, rhs);
        value <<= rhs;
    }
    else if (parser.SkipPastNextItemIfIs(">>"))
    {
        long rhs;
        TransferIn(context, parser, rhs);
        value >>= rhs;
    }
}
