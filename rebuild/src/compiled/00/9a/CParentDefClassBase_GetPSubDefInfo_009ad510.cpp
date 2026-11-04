// CParentDefClassBase::GetPSubDefInfo(const CCharString&) @ 0x009AD510
// (FableWin ?GetPSubDefInfo@CParentDefClassBase@@UAEPAVCSubDefInfo@CDefClassBase@@ABVCCharString@@@Z;
// vtable slot 15). The map's LowerBound is out of line (0x009B20D0).
#include "fable_def_class_base.h"
// The sub-def registered under this name (keyed by its CRC), or null.
CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString& name)
{
    CSubDefInfoMap::value_type* it = SubDefInfoMap.Find(name.GetCRC());
    if (it != SubDefInfoMap.end())
        return &it->second;
    return 0;
}
