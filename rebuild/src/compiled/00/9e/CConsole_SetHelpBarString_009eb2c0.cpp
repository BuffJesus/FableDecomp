#include "fable_console.h"
#include "fable_string_parser.h"

// The "(" that CConsoleCommandBase::GetFormattedName appends (0x013CAA48).
extern const CCharString g_013caa48;

// lib_console.cpp, retail 009EB2C0 (Ego_r locals parser, parsed_item): show
// the named input's help while the front line holds just its name, or its
// name followed by the opening bracket.
void CConsole::SetHelpBarString()
{
    CStringParser parser(FrontCommandLine.GetString(), 0, CCharString(""), 0);
    CParsedItem parsed_item;
    if (parser.ReadNextItem(parsed_item) && parsed_item.Type == CParsedItem::STRING)
    {
        CConsoleInputBase* input = GetRegisteredConsoleInput(parsed_item.GetString());
        if ((!parser.ReadNextItem(parsed_item) || parsed_item.Type != CParsedItem::SYMBOL ||
             parsed_item.GetSymbol() == g_013caa48.GetConstCharPtr()[0]) && input)
        {
            HelpBarString = input->GetHelp();
            return;
        }
    }
    HelpBarString = "";
}
