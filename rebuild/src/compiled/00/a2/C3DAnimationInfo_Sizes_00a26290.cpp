// C3DAnimationInfo::CAnimationData::UpdateSize, retail 0x00A26290 (no retail references; a
// non-template out-of-line member), and C3DAnimationInfo::Sizeof, retail 0x00A265B0 (vtable
// 0x0129CD64 slot 2; Ego_r ?Sizeof@C3DAnimationInfo@@UBEKXZ at 0x00B62E80 has the same body).
// FableWin's debug build shows Sizeof as GFPSizeof(Data) + 0x14 (FableWin 0x03140450 calls
// ??$GFPSizeof@V?$CIVCountedPointer@VCAnimationData@C3DAnimationInfo@@@@@@YIKAAV...@Z), which
// /O2 expands to `Data ? Data->Sizeof() + 4 : 4` (CAnimationData::Sizeof 0x00A25B20).
#include "engine/C3DAnimationInfo.h"

void C3DAnimationInfo::CAnimationData::UpdateSize()
{
    SetResourceSize(Sizeof());
}

unsigned long C3DAnimationInfo::Sizeof() const
{
    return GFPSizeof(Data) + 0x14;
}
