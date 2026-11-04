#include "fable_frontend_def.h"

void CFrontEndDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CFrontEndDef*>(other);
}
