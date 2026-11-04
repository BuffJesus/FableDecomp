#include "fable_definition_manager.h"
#include "fable_string_parser.h"
#include "fable_persist_context.h"

void CDefinitionManager::ParseEnumForSymbols(CStringParser& parser)
{
    long current = 0;
    CCharString group;
    if (parser.PeekNextItemType() == PARSED_STRING) group = parser.ReadAsString();
    else group = "NULL";
    parser.ReadAsSymbol();
    CParsedItem item;
    while (parser.ReadNextItem(item))
    {
        if (item.Type == PARSED_SYMBOL && item.GetSymbol() == "}")
        {
            if (!(parser.ReadAsSymbol() == ";")) Error(parser, "Unexpected symbol in enum.");
            return;
        }
        if (item.Type != PARSED_STRING)
        {
            Error(parser, "Expected string in enum");
            return;
        }
        CCharString name = item.GetString();
        CCharString separator = parser.ReadAsSymbol();
        long value;
        if (separator == ",") value = current;
        else if (separator == "=")
        {
            CPersistContext context(parser, ErrorCallback, true, this, PSymbolMap.Data, 0);
            CPersistTraitsSLONG::TransferIn(context, parser, value);
            current = value;
        }
        else if (separator == "}")
        {
            if (!(parser.ReadAsSymbol() == ";")) Error(parser, "Unexpected symbol in enum.");
            return;
        }
        else
        {
            Error(parser, CCharString("Unknown symbol after '") + name + "'.");
            return;
        }
        PSymbolMap.Data->AddSymbolDirty(group, name, value);
        if (parser.PeekNextItem(item) && item.Type == PARSED_SYMBOL && item.GetSymbol() == ",")
            parser.ReadNextItem(item);
        current = static_cast<long>(static_cast<unsigned long>(current) + 1u);
    }
}
