#include "fable_console.h"

CCharString CConsoleCommandBase::GetHelp()
{
    CCharString help = GetTypeAsString() + " with ";
    if (ParamTypes.ParamTypes.size() == 0)
    {
        help += "no parameters";
    }
    else
    {
        help += GFIntToCharString(static_cast<long>(ParamTypes.ParamTypes.size())) + " parameter";
        if (ParamTypes.ParamTypes.size() > 1)
            help += "s";
        help += ": ";
        for (long i = 0; i < static_cast<long>(ParamTypes.ParamTypes.size()); ++i)
            help += ParamTypes.GetParameterTypeAsString(i) + " ";
    }
    return help;
}
