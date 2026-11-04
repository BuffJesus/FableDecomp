// CMeshDataBank::PrepareDataForInclusion, retail 0x00A24410 (vtable 0x0129CE94 slot 16; its only
// reference is that slot, 0x0129CED4). The retail build keeps none of the bank-building body
// (Ego_r's PrepareRender/Physics/AnimationMeshDataForInclusion are editor-only): only the
// by-value CCountedPointer<CBankStateBlock> argument's destruction survives.
#include "engine/CMeshDataBank.h"

void CMeshDataBank::PrepareDataForInclusion(unsigned long, const CArray<CCharString>&,
    unsigned long, CCountedPointer<CBankStateBlock>, CDataOutputStream&, CDataOutputStream&)
{
}
