#ifndef FABLETLC_LANDSCAPE_BACKGROUND_PATCH_RENDER_COMPILED_H
#define FABLETLC_LANDSCAPE_BACKGROUND_PATCH_RENDER_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

class CTexture {};
class CIndexBuffer {};
class CVertexBuffer {};

struct CBackgroundForegroundPatchProbe
{
    fable_u8 Unknown00[0x3C];
    fable_u8 Flags;
    fable_u8 Unknown3D[0x0B];
};

class CBackgroundRenderLandscapeMap
{
public:
    fable_u8 Unknown00[8];
    CBackgroundForegroundPatchProbe* ForegroundPatches;
    int PatchColumns;
};

class CBackgroundRenderTreeNode
{
public:
    fable_u8 Unknown00[0xA4];
    CBackgroundRenderLandscapeMap* LandscapeMap;
};

void BeginLandscapeBackgroundRenderState();
void ApplyLandscapeBackgroundPixelShader();
void PrepareLandscapeBackgroundTexture(CTexture* texture);
void ApplyLandscapeBackgroundVertexShader(bool specialPath);
void SetLandscapeBackgroundRenderConstants();
void BindLandscapeBackgroundBuffers(CVertexBuffer* vertices, CIndexBuffer* indices);
fable_u8 GetLandscapeBackgroundVertexStride(CVertexBuffer* vertices);
void DrawLandscapeBackgroundBase(unsigned long count104, unsigned long count106);
void DrawLandscapeBackgroundLayer(CVertexBuffer* vertices, CIndexBuffer* indices, unsigned long indexCount);
void RestoreLandscapeBackgroundRenderState();

class CLandscapeBackgroundPatch
{
public:
    fable_u8 Unknown00[4];
    CBackgroundRenderTreeNode* Parent;
    fable_u8 Unknown08[0xB0];
    CTexture* Texture;
    CIndexBuffer* IndexBuffer;
    CVertexBuffer* VertexBuffer;
    CIndexBuffer* LayerIndexBuffers[4];
    CVertexBuffer* LayerVertexBuffers[4];
    fable_u16 LayerIndexCounts[4];
    fable_u8 UnknownEC[0x18];
    fable_u16 Count104;
    fable_u16 Count106;
    fable_u8 Unknown108[4];
    fable_u16 SectionX;
    fable_u16 SectionY;
    fable_u8 Unknown110[8];
    fable_u8 ShaderVariant;
    fable_u8 Unknown119[2];
    fable_u8 DisableSpecialShaderPath;

    bool Render(bool unused);
};

FABLE_STATIC_ASSERT(sizeof(CBackgroundForegroundPatchProbe) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CBackgroundForegroundPatchProbe, Flags) == 0x3C);
FABLE_STATIC_ASSERT(offsetof(CBackgroundRenderTreeNode, LandscapeMap) == 0xA4);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, Parent) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, Texture) == 0xB8);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, IndexBuffer) == 0xBC);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, VertexBuffer) == 0xC0);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, LayerIndexBuffers) == 0xC4);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, LayerVertexBuffers) == 0xD4);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, LayerIndexCounts) == 0xE4);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, Count104) == 0x104);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, Count106) == 0x106);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, SectionX) == 0x10C);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, ShaderVariant) == 0x118);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, DisableSpecialShaderPath) == 0x11B);
FABLE_STATIC_ASSERT(sizeof(CLandscapeBackgroundPatch) == 0x11C);

#endif
