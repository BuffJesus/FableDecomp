// CEngineInternalPrimitiveOcclusionGroup::GetBoundingBoxWorldSpace @ 00bacba0
#include "engine/CEngineInternalPrimitiveOcclusionGroup.h"  // retyped onto the PDB layout; byte parity re-verified
struct C3DBoundingBox { float mn[3]; float mx[3]; };

// helper @ 0xb92d10 : native thiscall member
// bool CEngineSceneGrid::occ(C3DBoundingBox& bbox, int f58, int f5c)
struct CEngineSceneGrid {
    bool occ(C3DBoundingBox& bbox, int f58, int f5c);
};


bool __fastcall CEngineInternalPrimitiveOcclusionGroup_GetBoundingBoxWorldSpace(
        CEngineInternalPrimitiveOcclusionGroup* self, int /*edx*/, C3DBoundingBox& bbox)
{
    return self->Grid->occ(bbox, self->GridX, self->GridY);
}