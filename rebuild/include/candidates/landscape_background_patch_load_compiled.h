#ifndef FABLETLC_LANDSCAPE_BACKGROUND_PATCH_LOAD_COMPILED_H
#define FABLETLC_LANDSCAPE_BACKGROUND_PATCH_LOAD_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

class CDataInputStream
{
public:
    const fable_u8* Data;
    unsigned long Size;
    unsigned long Position;

    CDataInputStream(const void* data, unsigned long size);
    bool Read8(fable_u8& value);
    bool Read16(fable_u16& value);
    bool Read32(fable_u32& value);
    bool ReadFloat(float& value);
    bool ReadBytes(void* destination, unsigned long size);
};

class CTexture {};
class CVertexBuffer {};
class CIndexBuffer {};
class CEngineWaterBackgroundSubPatch {};
class CLandscapeBackgroundPatch;

struct CBackgroundLandscapeVertex
{
    fable_u16 X;
    fable_u16 Y;
    fable_u32 Z;
    fable_u8 Unknown08[9];
    fable_u8 Byte11;
    fable_u8 Byte12;
    fable_u8 Unknown13[5];
};

class CPatchTesselationEdgeStrip
{
public:
    fable_u8 Storage[0x2C];
    bool Load(CDataInputStream& input, CLandscapeBackgroundPatch* owner);
};

class CEngineLandscapeRuntime
{
public:
    fable_u8 Unknown00[0x60];
    fable_u16 BaseMapX;
    fable_u16 BaseMapY;
};

class CEngineLandscapeMap
{
public:
    fable_u8 Unknown00[0x14];
    CEngineLandscapeRuntime* Runtime;
};

class CLandscapeBackgroundTreeNode
{
public:
    fable_u8 Unknown00[0xA4];
    CEngineLandscapeMap* LandscapeMap;
};

CTexture* AllocateLandscapeBackgroundTexture(
    CLandscapeBackgroundPatch* owner,
    unsigned long width,
    unsigned long height,
    bool dxt1);
bool LoadLandscapeBackgroundTexture(CTexture* texture, CDataInputStream& input);
CVertexBuffer* AllocateLandscapeBackgroundVertexBuffer(
    CLandscapeBackgroundPatch* owner,
    unsigned long vertexCount,
    unsigned long stride);
void* LockLandscapeBackgroundVertexBuffer(CVertexBuffer* buffer);
void UnlockLandscapeBackgroundVertexBuffer(CVertexBuffer* buffer);
void SetLandscapeBackgroundTargetNormal(CBackgroundLandscapeVertex* vertex, fable_u32 normal);
bool DecompressLandscapeRange(
    const fable_u8* source,
    unsigned long sourceSize,
    unsigned long elementCount,
    unsigned long elementSize,
    void* destination);
CIndexBuffer* GetLandscapeBackgroundSharedIndexBuffer(unsigned long width, unsigned long height);
CIndexBuffer* AllocateLandscapeBackgroundIndexBuffer(
    CLandscapeBackgroundPatch* owner,
    unsigned long byteCount,
    unsigned long format,
    unsigned long primitiveType);
void* LockLandscapeBackgroundIndexBuffer(CIndexBuffer* buffer);
void UnlockLandscapeBackgroundIndexBuffer(CIndexBuffer* buffer);
CEngineWaterBackgroundSubPatch* CreateLandscapeWaterBackgroundSubPatch(CEngineLandscapeMap* map);
bool LoadLandscapeWaterBackgroundSubPatch(
    CEngineWaterBackgroundSubPatch* water,
    CDataInputStream& input,
    unsigned long width,
    unsigned long height,
    unsigned long mapX,
    unsigned long mapY,
    float minZ,
    float maxZ);

class CLandscapeBackgroundPatch
{
public:
    fable_u8 Unknown00[4];
    CLandscapeBackgroundTreeNode* Parent;
    CPatchTesselationEdgeStrip EdgeStrips[4];
    CTexture* Texture;
    CIndexBuffer* IndexBuffer;
    CVertexBuffer* VertexBuffer;
    fable_u8 UnknownC4[0x30];
    CEngineWaterBackgroundSubPatch* WaterSubPatch;
    float MinZ;
    float MaxZ;
    fable_u8 Unknown100[4];
    fable_u16 VertexCount;
    fable_u16 PolyCountIncDegenerates;
    fable_u16 TextureWidthNeeded;
    fable_u16 TextureHeightNeeded;
    fable_u16 MapX;
    fable_u16 MapY;
    fable_u16 Width;
    fable_u16 Height;
    fable_u8 Unknown114[4];
    fable_u8 LODBand;
    bool SharedIndexBuffer;
    bool WaterOnlyPatch;
    bool IsDefaultSection;

    bool Load(CDataInputStream& input, float minZ, float maxZ);
};

FABLE_STATIC_ASSERT(sizeof(CBackgroundLandscapeVertex) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CBackgroundLandscapeVertex, Byte11) == 0x11);
FABLE_STATIC_ASSERT(sizeof(CPatchTesselationEdgeStrip) == 0x2C);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundTreeNode, LandscapeMap) == 0xA4);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, Runtime) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRuntime, BaseMapX) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, Parent) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, EdgeStrips) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, Texture) == 0xB8);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, IndexBuffer) == 0xBC);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, VertexBuffer) == 0xC0);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, WaterSubPatch) == 0xF4);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, MinZ) == 0xF8);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, VertexCount) == 0x104);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, TextureWidthNeeded) == 0x108);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, MapX) == 0x10C);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, Width) == 0x110);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, LODBand) == 0x118);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, SharedIndexBuffer) == 0x119);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, WaterOnlyPatch) == 0x11A);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundPatch, IsDefaultSection) == 0x11B);
FABLE_STATIC_ASSERT(sizeof(CLandscapeBackgroundPatch) == 0x11C);

#endif
