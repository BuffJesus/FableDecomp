#include "fable_symbol_map.h"
#include "fable_string_parser.h"

// lib_symbol_map.cpp, retail 009E5AD0 (Ego_r ?ParseBuffer@CSymbolMap@@IAE_NPBD@Z;
// locals parser_item, parser): parse every "enum" in the text. The parser's
// const char* constructor is expanded here (its out-of-line copy is
// 0x00414E30), so it is defined inline in this unit.
inline CStringParser::CStringParser(const char* string, ErrorCallbackFunction errorCallback,
                                    const CCharString& filename, long baseLineOffset)
{
    Init(string, errorCallback, filename, baseLineOffset);
}

bool CSymbolMap::ParseBuffer(const char* text)
{
    CStringParser parser(text, 0, CCharString(""), 0);
    CParsedItem parser_item;
    while (parser.SkipPastString("enum"))
    {
        if (!ParseEnum(parser))
            return false;
    }
    return true;
}
