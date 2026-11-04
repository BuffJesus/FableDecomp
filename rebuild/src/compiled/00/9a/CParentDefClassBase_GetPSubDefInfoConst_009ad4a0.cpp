// CParentDefClassBase::GetPSubDefInfo(const CCharString&) const @ 0x009AD4A0
// (FableWin ?GetPSubDefInfo@CParentDefClassBase@@UBEPBVCSubDefInfo@CDefClassBase@@ABVCCharString@@@Z;
// vtable slot 14). The map's const LowerBound is out of line (0x009B2130).
#include "fable_def_class_base.h"
// The sub-def registered under this name (keyed by its CRC), or null.
const CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString& name) const
{
    const CSubDefInfoMap::value_type* it = SubDefInfoMap.Find(name.GetCRC());
    if (it != SubDefInfoMap.end())
        return &it->second;
    return 0;
}
