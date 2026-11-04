// CParentDefClassBase::GetPSubDefInfoMap() @ 0x004303E0 (FableWin
// ?GetPSubDefInfoMap@CParentDefClassBase@@UAEPAV?$CVectorMap@KVCSubDefInfo@CDefClassBase@@...@@XZ;
// vtable slot 24; the base's slot is modelled as void*).
#include "fable_def_class_base.h"

// The def's sub-def map (+0x29).
void* CParentDefClassBase::GetPSubDefInfoMap()
{
    return &SubDefInfoMap;
}