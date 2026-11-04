#include "fable_console.h"

CCharString CConsoleCommandParameters::GetParameterTypeAsString(long index) const
{
    CCharString result;
    switch (ParamTypes[index])
    {
    case GENERIC_VAR_BOOL: result = "bool"; break;
    case GENERIC_VAR_SLONG: result = "slong"; break;
    case GENERIC_VAR_ULONG: result = "ulong"; break;
    case GENERIC_VAR_FLOAT: result = "float"; break;
    case GENERIC_VAR_STRING: result = "string"; break;
    default: result = "null"; break;
    }
    return result;
}
