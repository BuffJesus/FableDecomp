#ifndef FABLE_ENGINE_CENGINESCENEGRIDCELL_H
#define FABLE_ENGINE_CENGINESCENEGRIDCELL_H
// Retail bitfield layout confirmed at B91CA0; donor names/order preserved.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineSceneGridCell = 0x48.
// B91CA0 clears BoundingBoxValid bit0 at44; adjacent donor-named flags retain their bits.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CEngineInternalPrimitiveBase;

#pragma pack(push, 1)
class CEngineSceneGridCell {
public:
    void InvalidateAll();
    CEngineSceneGridCell*         Parent;                    // +0x00
    CEngineInternalPrimitiveBase* FirstPrimitive;            // +0x04
    unsigned char                 BoundingBox[0x18];         // +0x08 C3DBoundingBox
    float                         DrawDistance;              // +0x20
    unsigned char                 GroupPrimitiveHandle[0x10]; // +0x24 CEnginePrimitiveHandle[2]
    long                          LocalCount;                // +0x34
    unsigned long                 TotalCount;                // +0x38
    unsigned long                 RenderMask;                // +0x3c
    unsigned long                 OccludedLayers;            // +0x40
    union {
      unsigned char Flags;
      struct {
    unsigned char BoundingBoxValid:1;
    unsigned char TotalCountValid:1;
    unsigned char RenderMaskValid:1;
    unsigned char WasRendered:1;
    unsigned char HasOcclusionResults:1;
    unsigned char UnusedFlags:3;
      };
    };
    unsigned char                 OccludedFrames;            // +0x45
    unsigned char                 PreloadFrames;             // +0x46
    unsigned char                 _pad_0x47[0x1];            // +0x47
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CEngineSceneGridCell) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CEngineSceneGridCell, Parent) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CEngineSceneGridCell, FirstPrimitive) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CEngineSceneGridCell, BoundingBox) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CEngineSceneGridCell, DrawDistance) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CEngineSceneGridCell, GroupPrimitiveHandle) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CEngineSceneGridCell, LocalCount) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CEngineSceneGridCell, TotalCount) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CEngineSceneGridCell, RenderMask) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CEngineSceneGridCell, OccludedLayers) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CEngineSceneGridCell, Flags) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CEngineSceneGridCell, OccludedFrames) == 0x45);
FABLE_STATIC_ASSERT(offsetof(CEngineSceneGridCell, PreloadFrames) == 0x46);

#endif // FABLE_ENGINE_CENGINESCENEGRIDCELL_H
