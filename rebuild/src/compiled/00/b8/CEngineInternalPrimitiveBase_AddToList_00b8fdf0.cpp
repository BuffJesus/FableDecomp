#include "fable_engine_primitive_list.h"

// Retail 00B8FDF0; donor member names verified against the native link stores.
// Each render-layer-mask group has one representative in NextLayerMask.
// NextPrimitive traverses every member, including non-representatives.
void __fastcall FablePrimitiveAddToList(
    CEngineInternalPrimitiveBase* primitive, int,
    CEngineInternalPrimitiveBase** firstPrimitive)
{
    const unsigned long mask = primitive->RenderLayerMask;
    CEngineInternalPrimitiveBase* previousHead = *firstPrimitive;
    CEngineInternalPrimitiveBase* group = previousHead;
    while (group && group->RenderLayerMask != mask)
        group = group->NextLayerMask;

    if (!group)
    {
        primitive->NextPrimitive = previousHead;
        primitive->NextLayerMask = previousHead;
        if (previousHead)
        {
            previousHead->RefPrimitive = &primitive->NextPrimitive;
            previousHead->RefLayerMask = &primitive->NextLayerMask;
        }
        primitive->RefPrimitive = firstPrimitive;
        primitive->RefLayerMask = firstPrimitive;
        *firstPrimitive = primitive;
        return;
    }

    primitive->NextPrimitive = group->NextPrimitive;
    primitive->RefPrimitive = &group->NextPrimitive;
    group->NextPrimitive = primitive;
    if (primitive->NextPrimitive)
        primitive->NextPrimitive->RefPrimitive = &primitive->NextPrimitive;
    primitive->NextLayerMask = 0;
    primitive->RefLayerMask = 0;
}
