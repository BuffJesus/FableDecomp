#ifndef FABLETLC_LANDSCAPE_LAYER_MESH_LOAD_FOREGROUND_COMPILED_H
#define FABLETLC_LANDSCAPE_LAYER_MESH_LOAD_FOREGROUND_COMPILED_H

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
    bool ReadBytes(void* destination, unsigned long size);
};

struct CEngineTexturePaletteEntry
{
    fable_u32 TextureId;
    fable_u32 Unknown04;
};

class CEngineTexturePalette
{
public:
    fable_u8 Unknown00[4];
    CEngineTexturePaletteEntry* Entries;
    fable_u8 Unknown08[0x0C];
    fable_u8 DirectMode;
};

struct CLandscapeForegroundVertex
{
    fable_u16 Value0;
    fable_u16 Value1;
    fable_u32 NormalBase;
    fable_u8 Unknown08[0x0C];
    fable_u8 Byte14;
    fable_u8 Byte15;
    fable_u8 Byte16;
    fable_u8 Unknown17;
};

class CGraphicInfoFrame {};
class CVertexBuffer {};
class CIndexBuffer {};
class CLandscapeLayerMesh;

CVertexBuffer* AllocateLandscapeForegroundVertexBuffer(
    CLandscapeLayerMesh* owner,
    unsigned long vertexCount,
    unsigned long stride);
void* LockLandscapeVertexBuffer(CVertexBuffer* buffer);
void UnlockLandscapeVertexBuffer(CVertexBuffer* buffer);
void SetLandscapeTargetVertexNormal(CLandscapeForegroundVertex* vertex, fable_u32 normal);
CGraphicInfoFrame* ResolveLandscapeTextureFrame(fable_i32 textureIndex);
CIndexBuffer* GetSharedLandscapeIndexBuffer();
CIndexBuffer* AllocateLandscapeForegroundIndexBuffer(
    CLandscapeLayerMesh* owner,
    unsigned long byteCount,
    unsigned long format,
    unsigned long primitiveType);
void* LockLandscapeIndexBuffer(CIndexBuffer* buffer);
void UnlockLandscapeIndexBuffer(CIndexBuffer* buffer);

class CLandscapeLayerMesh
{
public:
    fable_u8 Unknown00[4];
    fable_i32 ForegroundTextureIndex;
    fable_i32 BackgroundTextureIndex;
    fable_i32 BumpMapTextureIndex;
    float SelfIllumination;
    CGraphicInfoFrame* ForegroundTexture;
    CGraphicInfoFrame* BackgroundTexture;
    CGraphicInfoFrame* BumpMap;
    fable_i32 MinTextureMipMap;
    fable_i32 MinBumpMapMipMap;
    fable_u8 MappingDirection;
    fable_u8 Unknown29[0x0B];
    CIndexBuffer* IndexBuffer;
    CVertexBuffer* ForegroundVertexBuffer;
    CLandscapeLayerMesh* Next;
    fable_u8 Unknown40[4];
    fable_u16 VertexCount;
    fable_u16 PolyCountIncDegenerates;
    bool UsesSharedIndexBuffer;
    fable_u8 Unknown49[0x0B];

    bool LoadForeground(CDataInputStream& stream, CEngineTexturePalette& texturePalette);
};

FABLE_STATIC_ASSERT(sizeof(CEngineTexturePaletteEntry) == 8);
FABLE_STATIC_ASSERT(offsetof(CEngineTexturePalette, Entries) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CEngineTexturePalette, DirectMode) == 0x14);
FABLE_STATIC_ASSERT(sizeof(CLandscapeForegroundVertex) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CLandscapeForegroundVertex, Byte14) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CLandscapeLayerMesh, ForegroundTextureIndex) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CLandscapeLayerMesh, BackgroundTextureIndex) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CLandscapeLayerMesh, BumpMapTextureIndex) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CLandscapeLayerMesh, SelfIllumination) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CLandscapeLayerMesh, ForegroundTexture) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CLandscapeLayerMesh, MinTextureMipMap) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CLandscapeLayerMesh, MappingDirection) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CLandscapeLayerMesh, IndexBuffer) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CLandscapeLayerMesh, ForegroundVertexBuffer) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CLandscapeLayerMesh, Next) == 0x3C);
FABLE_STATIC_ASSERT(offsetof(CLandscapeLayerMesh, VertexCount) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CLandscapeLayerMesh, PolyCountIncDegenerates) == 0x46);
FABLE_STATIC_ASSERT(offsetof(CLandscapeLayerMesh, UsesSharedIndexBuffer) == 0x48);
FABLE_STATIC_ASSERT(sizeof(CLandscapeLayerMesh) == 0x54);

#endif
