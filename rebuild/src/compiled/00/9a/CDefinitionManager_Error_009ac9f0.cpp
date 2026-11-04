#include "fable_definition_manager.h"
#include "fable_string_parser.h"

void CDefinitionManager::Error(const CStringParser& parser, const CCharString& message)
{
    CCharString formatted = parser.Filename + "(" + GFIntToCharString(parser.Line + 1) + ")" + " : error : " + message;
    ErrorCallback(formatted);
}
