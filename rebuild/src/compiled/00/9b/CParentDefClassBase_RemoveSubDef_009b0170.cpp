// CParentDefClassBase::RemoveSubDef(const CCharString&) @ 0x009B0170 (FableWin
// ?RemoveSubDef@CParentDefClassBase@@UAEXABVCCharString@@@Z; vtable slot 13).
// FableWin: EraseFromKey on the name's CRC, then Update (here Sort, which
// carries the Dirty test). The tail copy _STL::__copy (0x009B1620) is emitted
// by this unit.
#include "fable_def_class_base.h"
#include "fable_stlport_algobase.h"

// Forget every sub-def registered under this name.
void CParentDefClassBase::RemoveSubDef(const CCharString& name)
{
    SubDefInfoMap.EraseFromKey(name.GetCRC());
    SubDefInfoMap.Update();
}
