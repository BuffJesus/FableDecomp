// Behaviour of CParentDefClassBase::IsParentDef 0x004303D0: always true, reads nothing.
#include "fable_def_class_base.h"
#include <stdio.h>

int main()
{
    const CParentDefClassBase* def = reinterpret_cast<const CParentDefClassBase*>(0);
    if (!def->CParentDefClassBase::IsParentDef()) { puts("false"); return 1; }
    printf("PASS_004303d0\n");
    return 0;
}