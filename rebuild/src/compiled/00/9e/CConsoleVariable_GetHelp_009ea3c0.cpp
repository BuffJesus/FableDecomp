#include "fable_console.h"

CCharString CConsoleVariable::GetHelp()
{
    CCharString help = GetTypeAsString() + " of type ";
    if (Var.Type == GENERIC_VAR_BOOL)
        help += "bool";
    else if (Var.Type == GENERIC_VAR_FLOAT)
        help += "float";
    else if (Var.Type == GENERIC_VAR_SLONG)
        help += "slong";
    else if (Var.Type == GENERIC_VAR_STRING)
        help += "string";
    CCharString value;
    Var.PrintToString(value);
    help += "; value is currently " + value;
    return help;
}
