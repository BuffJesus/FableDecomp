#ifndef FABLETLC_LANDSCAPE_PATCH_RENDER_FOREGROUND_COMPILED_H
#define FABLETLC_LANDSCAPE_PATCH_RENDER_FOREGROUND_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

class CGraphicInfoFrame {};
class CEngineLandscapePatch;

struct CForegroundRenderEntry
{
    fable_u8 Unknown00[4];
    fable_i32 DiffuseLodIndex;
    fable_u8 Unknown08[4];
    fable_i32 ProjectedLodIndex;
    float GlobalLightScale;
    CGraphicInfoFrame* DiffuseFrame;
    fable_u8 Unknown18[4];
    CGraphicInfoFrame* ProjectedFrame;
    fable_u8 Unknown20[8];
    fable_i32 ShaderVariantIndex;
    fable_u8 Unknown2C[8];
    void* StreamBinding;
    void* SamplerBinding;
    CForegroundRenderEntry* Next;
    fable_u8 Unknown40[4];
    fable_u16 BaseVertexIndex;
    fable_u16 PrimitiveCount;
};

void BeginLandscapeForegroundRenderState();
bool ClipLandscapeForegroundPatch(const CEngineLandscapePatch* patch);
void SetupLandscapeForegroundLighting(
    const CEngineLandscapePatch* patch,
    int* diffusePass,
    int* projectedPass);
void SetLandscapeForegroundWorldAndConstants(const CEngineLandscapePatch* patch);
void UpdateLandscapeForegroundGlobalLighting(float scale);
void ApplyLandscapeForegroundShaderPass(int pass);
void ApplyLandscapeForegroundShaderVariant(int variant, const CEngineLandscapePatch* patch);
bool PrepareLandscapeForegroundTexture(CGraphicInfoFrame* frame, int lodIndex, bool projected);
void BindLandscapeForegroundEntry(const CForegroundRenderEntry* entry, bool bindProjected);
void DrawLandscapeForegroundEntry(unsigned long baseVertex, unsigned long primitiveCount);
void DisableLandscapeForegroundPointLights();
void UnbindLandscapeForegroundResources();
void RenderLandscapeForegroundWater(void* waterPatch);
void RestoreLandscapeForegroundRenderState();

class CEngineLandscapePatch
{
public:
    fable_u8 Unknown00[4];
    void* LandscapeMap;
    CForegroundRenderEntry* Foreground;
    fable_u8 Unknown0C[8];
    void* WaterPatch;
    fable_u8 Unknown18[4];
    fable_i32 LayerMeshCount;
    float BoundsMinX;
    float BoundsMinY;
    float BoundsMinZ;
    float BoundsMaxX;
    float BoundsMaxY;
    float BoundsMaxZ;
    fable_u16 PatchX;
    fable_u16 PatchY;
    fable_u8 Flags;
    fable_u8 Unknown3D[3];
    void* AsyncObject;
    void* AsyncReference;

    void RenderForeground();
};

FABLE_STATIC_ASSERT(sizeof(CForegroundRenderEntry) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CForegroundRenderEntry, DiffuseLodIndex) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CForegroundRenderEntry, ProjectedLodIndex) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CForegroundRenderEntry, GlobalLightScale) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CForegroundRenderEntry, DiffuseFrame) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CForegroundRenderEntry, ProjectedFrame) == 0x1C);
FABLE_STATIC_ASSERT(offsetof(CForegroundRenderEntry, ShaderVariantIndex) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CForegroundRenderEntry, StreamBinding) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CForegroundRenderEntry, SamplerBinding) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CForegroundRenderEntry, Next) == 0x3C);
FABLE_STATIC_ASSERT(offsetof(CForegroundRenderEntry, BaseVertexIndex) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CForegroundRenderEntry, PrimitiveCount) == 0x46);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, Foreground) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, WaterPatch) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, BoundsMinX) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, Flags) == 0x3C);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, AsyncObject) == 0x40);
FABLE_STATIC_ASSERT(sizeof(CEngineLandscapePatch) == 0x48);

#endif
