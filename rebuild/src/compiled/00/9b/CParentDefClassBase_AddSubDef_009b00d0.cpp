// CParentDefClassBase::AddSubDef(const CCharString&, long, long) @ 0x009B00D0
// (FableWin ?AddSubDef@CParentDefClassBase@@UAEXABVCCharString@@JJ@Z; vtable
// slot 12). push_back's growth path (0x009B60D0) and STLport's sort
// (0x009B84C0) are out of line.
#include <new>
#include "fable_def_class_base.h"
#include "fable_stlport_vector_impl.h"
// Register a sub-def under the CRC of its name and keep the map sorted.
void CParentDefClassBase::AddSubDef(const CCharString& name, long defIndex, long originalParentDefIndex)
{
    const unsigned long key = name.GetCRC();
    CSubDefInfo info;
    info.DefIndex = defIndex;
    info.OriginalParentDefIndex = originalParentDefIndex;
    SubDefInfoMap.Insert(key, info);
    SubDefInfoMap.Update();
}
