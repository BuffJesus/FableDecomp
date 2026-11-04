#include "fable_console.h"
#include "fable_string_parser.h"

// lib_console.cpp, retail 009EC430 (Ego_r locals parsed_item, parser,
// var_val_as_string, help_message). The first item is the variable's own
// name; with nothing after it the value is printed, otherwise it is read,
// and a failed read reports the variable's type.
bool CConsoleVariable::ProcessCommandLine(const CCharString& cmd_string, CConsole& user)
{
    bool result;
    CParsedItem parsed_item;
    CStringParser parser(cmd_string, 0, CCharString(""), 0);
    parser.ReadNextItem(parsed_item);
    if (!parser.NextItemExists())
    {
        CCharString var_val_as_string;
        Var.PrintToString(var_val_as_string);
        user.SendMessageToUser(Name + " = " + var_val_as_string);
        result = true;
    }
    else
    {
        result = Var.ReadFromParser(parser);
        if (!result)
        {
            user.SendMessageToUser("error - could not read target for variable");
            CCharString help_message = "type of " + Name + ": ";
            if (Var.Type == GENERIC_VAR_BOOL)
                help_message += "bool";
            else if (Var.Type == GENERIC_VAR_FLOAT)
                help_message += "float";
            else if (Var.Type == GENERIC_VAR_SLONG)
                help_message += "slong";
            else if (Var.Type == GENERIC_VAR_STRING)
                help_message += "string";
            user.SendMessageToUser(help_message);
        }
    }
    return result;
}
