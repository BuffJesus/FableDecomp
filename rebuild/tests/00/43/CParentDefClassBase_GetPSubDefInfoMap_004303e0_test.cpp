// Behaviour of CParentDefClassBase::GetPSubDefInfoMap 0x004303E0: the address
// of the sub-def map at +0x29.
#include "fable_def_class_base.h"
#include <stdio.h>

int main()
{
    static unsigned char store[sizeof(CParentDefClassBase)];
    CParentDefClassBase* def = reinterpret_cast<CParentDefClassBase*>(store);
    if (def->CParentDefClassBase::GetPSubDefInfoMap() != store + 0x29) { puts("address"); return 1; }
    printf("PASS_004303e0\n");
    return 0;
}