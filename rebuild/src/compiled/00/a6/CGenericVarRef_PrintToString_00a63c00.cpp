#include "fable_console.h"

void CGenericVarRef::PrintToString(CCharString& output)
{
    switch (Type)
    {
    case GENERIC_VAR_FLOAT:
        output.Sprintf("%f", *PFloat);
        break;
    case GENERIC_VAR_STRING:
        {
            CCharString value(*PString);
            output = value;
        }
        break;
    case GENERIC_VAR_BOOL:
        if (*PBool)
            output = "TRUE";
        else
            output = "FALSE";
        break;
    case GENERIC_VAR_SLONG:
        output.Sprintf("%d", *PSLong);
        break;
    default:
        break;
    }
}
