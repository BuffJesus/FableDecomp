#include "fable_console.h"

extern const CCharString g_013caa48;

CCharString CConsoleCommandBase::GetFormattedName() const
{
    return Name + g_013caa48;
}
