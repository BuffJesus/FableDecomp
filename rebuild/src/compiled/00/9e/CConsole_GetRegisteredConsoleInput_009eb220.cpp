#include "fable_console.h"
#include "fable_string_parser.h"

// lib_console.cpp, retail 009EB220: inputs are registered under lower-case
// names (Ego_r parameter `key`). The lower-cased temporary reuses the
// parameter's stack slot.
CConsoleInputBase* CConsole::GetRegisteredConsoleInput(const CCharString& key)
{
    _STL::map<CCharString, CCountedPointer<CConsoleInputBase> >::iterator it =
        RegisteredConsoleInputs.find(key.GetAsLowerCase());
    if (it != RegisteredConsoleInputs.end())
        return it->second.Get();
    return 0;
}

// Retail 009EB430 (Ego_r locals parser, parsed_item, input_name): the front
// line's first item names a registered input, which then parses the whole
// line. GetRegisteredConsoleInput is expanded here.
void CConsole::RunCommandLine()
{
    CStringParser parser(FrontCommandLine.GetString(), 0, CCharString(""), 0);
    CParsedItem parsed_item;
    if (parser.ReadNextItem(parsed_item))
    {
        if (parsed_item.Type == CParsedItem::STRING)
        {
            CCharString input_name = parsed_item.GetString();
            CConsoleInputBase* input = GetRegisteredConsoleInput(input_name);
            if (!input)
                SendMessageToUser("unknown input - " + input_name);
            else
                input->ProcessCommandLine(FrontCommandLine.GetString(), *this);
        }
        else
        {
            SendMessageToUser("command line was not a string");
        }
    }
}
