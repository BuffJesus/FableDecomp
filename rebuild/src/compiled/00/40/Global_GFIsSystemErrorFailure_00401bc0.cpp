#include "fable_system.h"

bool FABLE_FASTCALL GFIsSystemErrorFailure(ESystemCode code)
{
    switch (code)
    {
    case 2:
    case 4:
    case 5:
    case 10:
    case 12:
    case 13:
    case 14:
    case 15:
        return true;
    default:
        return false;
    }
}
