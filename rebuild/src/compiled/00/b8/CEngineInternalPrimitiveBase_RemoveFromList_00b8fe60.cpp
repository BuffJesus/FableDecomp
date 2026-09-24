#include "fable_engine_primitive_list.h"

// The grid header's quarantine is an old padding-vs-member conflict at +04,
// not conflicting evidence for LocalCount. The only grid field used here is
// LocalCount +34, independently confirmed by retail 00B8FE71 and Ego_r PDB.
#include "engine/_quarantine/CEngineSceneGridCell.h"

void __fastcall FablePrimitiveRemoveFromList(
    CEngineInternalPrimitiveBase* primitive, int)
{
    // The generated bitfield storage is named after its last donor member.
    // Bit 1 at +34 is NonPersistent, not PostRenderEnable (which is bit 3).
    const unsigned char nonPersistent = 0x02;
    if (!(primitive->PostRenderEnable[0] & nonPersistent) && primitive->GridCell)
    {
        --primitive->GridCell->LocalCount;
        primitive->GridCell = 0;
    }

    if (primitive->RefPrimitive)
        *static_cast<CEngineInternalPrimitiveBase**>(primitive->RefPrimitive) = primitive->NextPrimitive;
    if (primitive->NextPrimitive)
        primitive->NextPrimitive->RefPrimitive = primitive->RefPrimitive;

    if (primitive->RefLayerMask)
    {
        *static_cast<CEngineInternalPrimitiveBase**>(primitive->RefLayerMask) = primitive->NextPrimitive;
        if (primitive->NextPrimitive)
        {
            primitive->NextPrimitive->RefLayerMask = primitive->RefLayerMask;
            if (primitive->NextLayerMask && primitive->NextLayerMask != primitive->NextPrimitive)
            {
                primitive->NextPrimitive->NextLayerMask = primitive->NextLayerMask;
                primitive->NextLayerMask->RefLayerMask = &primitive->NextPrimitive->NextLayerMask;
            }
        }
    }

    primitive->NextPrimitive = 0;
    primitive->RefPrimitive = 0;
    primitive->NextLayerMask = 0;
    primitive->RefLayerMask = 0;
}
