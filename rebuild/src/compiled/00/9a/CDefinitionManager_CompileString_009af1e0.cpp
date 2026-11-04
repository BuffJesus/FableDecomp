#include "fable_definition_manager.h"
#include "fable_string_parser.h"
#include <string.h>

void CDefinitionManager::CompileString(const char* source, const CCharString& typeFilter)
{
    CStringParser parser;
    parser.Init(source, ErrorCallback, CurrentFilename, 0);
    while (parser.SkipPastWholeString("#definition_template"))
    {
        CCharString className = parser.ReadAsString();
        CCharString instanceName = parser.ReadAsString();
        if (typeFilter == "" || !strcmp(typeFilter.GetConstCharPtr(), className.GetConstCharPtr()))
            CompileDefinition(parser, className, instanceName);
    }
    parser.SetStringPos(0);
    while (parser.SkipPastWholeString("#definition"))
    {
        CCharString className = parser.ReadAsString();
        CCharString instanceName = parser.ReadAsString();
        if (typeFilter == "" || !strcmp(typeFilter.GetConstCharPtr(), className.GetConstCharPtr()))
            CompileDefinition(parser, className, instanceName);
    }
}
