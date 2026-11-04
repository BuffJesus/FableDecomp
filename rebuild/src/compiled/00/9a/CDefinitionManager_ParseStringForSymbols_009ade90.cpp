#include "fable_definition_manager.h"
#include "fable_string_parser.h"

// Retail scans each directive class separately, resetting between passes.
void CDefinitionManager::ParseStringForSymbols(const char* source, bool flag)
{
    CStringParser parser;
    parser.Init(source, ErrorCallback, CurrentFilename, 0);
    if (flag)
    {
        while (parser.SkipPastWholeString("#definition_template"))
        {
            CCharString className = parser.ReadAsString();
            CCharString instanceName = parser.ReadAsString();
            if (!InstantiateDef(className, instanceName, true))
                Error(parser, "Unable to find def class called " + className);
        }
        parser.SetStringPos(0);
        while (parser.SkipPastWholeString("#definition"))
        {
            CCharString className = parser.ReadAsString();
            CCharString instanceName = parser.ReadAsString();
            if (!InstantiateDef(className, instanceName, false))
                Error(parser, "Unable to find def class called " + className);
        }
        parser.SetStringPos(0);
    }
    while (parser.SkipPastWholeString("enum"))
        ParseEnumForSymbols(parser);
}
