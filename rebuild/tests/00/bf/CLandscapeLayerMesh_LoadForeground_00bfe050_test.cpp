#include <stdio.h>
#include <string.h>
#include "candidates/landscape_layer_mesh_load_foreground_compiled.h"

static CVertexBuffer g_vertexBuffer;
static CIndexBuffer g_indexBuffer, g_sharedIndexBuffer;
static CGraphicInfoFrame g_frames[4];
static CLandscapeForegroundVertex g_vertices[4];
static fable_u8 g_indices[32];
static bool g_failVertexAllocation, g_failIndexAllocation;
static int g_vertexAllocCalls, g_vertexUnlockCalls, g_indexAllocCalls, g_indexUnlockCalls;
static int g_normalCalls, g_resolveCalls;
static unsigned long g_vertexCount, g_vertexStride, g_indexBytes, g_indexFormat, g_primitiveType;
static fable_u32 g_targetNormals[4];
static fable_i32 g_resolvedIndices[3];

CVertexBuffer* AllocateLandscapeForegroundVertexBuffer(
    CLandscapeLayerMesh*, unsigned long count, unsigned long stride)
{
    ++g_vertexAllocCalls;
    g_vertexCount = count;
    g_vertexStride = stride;
    memset(g_vertices, 0, sizeof(g_vertices));
    return g_failVertexAllocation ? 0 : &g_vertexBuffer;
}

void* LockLandscapeVertexBuffer(CVertexBuffer*) { return g_vertices; }
void UnlockLandscapeVertexBuffer(CVertexBuffer*) { ++g_vertexUnlockCalls; }

void SetLandscapeTargetVertexNormal(CLandscapeForegroundVertex* vertex, fable_u32 normal)
{
    g_targetNormals[g_normalCalls++] = normal;
    vertex->Unknown08[0] = static_cast<fable_u8>(normal);
}

CGraphicInfoFrame* ResolveLandscapeTextureFrame(fable_i32 textureIndex)
{
    g_resolvedIndices[g_resolveCalls] = textureIndex;
    return textureIndex == 0 ? 0 : &g_frames[g_resolveCalls++];
}

CIndexBuffer* GetSharedLandscapeIndexBuffer() { return &g_sharedIndexBuffer; }

CIndexBuffer* AllocateLandscapeForegroundIndexBuffer(
    CLandscapeLayerMesh*,
    unsigned long byteCount,
    unsigned long format,
    unsigned long primitiveType)
{
    ++g_indexAllocCalls;
    g_indexBytes = byteCount;
    g_indexFormat = format;
    g_primitiveType = primitiveType;
    memset(g_indices, 0, sizeof(g_indices));
    return g_failIndexAllocation ? 0 : &g_indexBuffer;
}

void* LockLandscapeIndexBuffer(CIndexBuffer*) { return g_indices; }
void UnlockLandscapeIndexBuffer(CIndexBuffer*) { ++g_indexUnlockCalls; }

static void Put8(fable_u8*& output, fable_u8 value) { *output++ = value; }
static void Put16(fable_u8*& output, fable_u16 value)
{
    Put8(output, static_cast<fable_u8>(value));
    Put8(output, static_cast<fable_u8>(value >> 8));
}
static void Put32(fable_u8*& output, fable_u32 value)
{
    Put16(output, static_cast<fable_u16>(value));
    Put16(output, static_cast<fable_u16>(value >> 16));
}

static int BuildPayload(fable_u8* bytes, bool shared)
{
    fable_u8* output = bytes;
    Put16(output, 2);
    Put16(output, 3);
    Put8(output, 4);
    Put32(output, 1); Put32(output, 2); Put32(output, 3);
    Put8(output, shared ? 1 : 0);
    Put32(output, 6); Put32(output, 7); Put32(output, 0x3F400000);
    for (int index = 0; index < 2; ++index)
    {
        Put16(output, static_cast<fable_u16>(10 + index));
        Put16(output, static_cast<fable_u16>(20 + index));
        Put32(output, 0x10000000 + index);
        Put32(output, 0x20000000 + index);
        Put8(output, static_cast<fable_u8>(30 + index));
        Put8(output, static_cast<fable_u8>(40 + index));
        Put8(output, static_cast<fable_u8>(50 + index));
    }
    if (!shared)
    {
        for (int index = 0; index < 5; ++index)
            Put16(output, static_cast<fable_u16>(100 + index));
    }
    return static_cast<int>(output - bytes);
}

static void Reset()
{
    g_failVertexAllocation = g_failIndexAllocation = false;
    g_vertexAllocCalls = g_vertexUnlockCalls = g_indexAllocCalls = g_indexUnlockCalls = 0;
    g_normalCalls = g_resolveCalls = 0;
    g_vertexCount = g_vertexStride = g_indexBytes = g_indexFormat = g_primitiveType = 0;
    memset(g_targetNormals, 0, sizeof(g_targetNormals));
    memset(g_resolvedIndices, 0, sizeof(g_resolvedIndices));
}

int main()
{
    fable_u8 bytes[128];
    CEngineTexturePaletteEntry entries[4];
    memset(entries, 0, sizeof(entries));
    entries[1].TextureId = 11;
    entries[2].TextureId = 22;
    entries[3].TextureId = 33;
    CEngineTexturePalette palette;
    memset(&palette, 0, sizeof(palette));
    palette.Entries = entries;

    Reset();
    int size = BuildPayload(bytes, false);
    CDataInputStream stream(bytes, size);
    CLandscapeLayerMesh mesh;
    memset(&mesh, 0, sizeof(mesh));
    if (!mesh.LoadForeground(stream, palette)) return 1;
    if (mesh.VertexCount != 2 || mesh.PolyCountIncDegenerates != 3 || mesh.MappingDirection != 4) return 2;
    if (mesh.ForegroundTextureIndex != 11 || mesh.BackgroundTextureIndex != 22 ||
        mesh.BumpMapTextureIndex != 33) return 3;
    if (mesh.MinTextureMipMap != 6 || mesh.MinBumpMapMipMap != 7 || mesh.SelfIllumination != 0.75f) return 4;
    if (g_vertexCount != 2 || g_vertexStride != 0x18 || g_vertexUnlockCalls != 1) return 5;
    if (g_vertices[0].Value0 != 10 || g_vertices[1].Value1 != 21 ||
        g_vertices[0].NormalBase != 0x10000000 || g_targetNormals[1] != 0x20000001) return 6;
    if (g_vertices[0].Byte16 != 30 || g_vertices[0].Byte15 != 40 || g_vertices[0].Byte14 != 50) return 7;
    if (g_resolvedIndices[0] != 11 || g_resolvedIndices[1] != 22 || g_resolvedIndices[2] != 33) return 8;
    if (g_indexBytes != 10 || g_indexFormat != 0x65 || g_primitiveType != 5 || g_indexUnlockCalls != 1) return 9;
    if (*reinterpret_cast<fable_u16*>(g_indices) != 100 ||
        *reinterpret_cast<fable_u16*>(g_indices + 8) != 104 || stream.Position != static_cast<unsigned long>(size)) return 10;

    Reset();
    size = BuildPayload(bytes, true);
    CDataInputStream sharedStream(bytes, size);
    memset(&mesh, 0, sizeof(mesh));
    palette.DirectMode = 1;
    if (!mesh.LoadForeground(sharedStream, palette)) return 11;
    if (mesh.ForegroundTextureIndex != 1 || mesh.BackgroundTextureIndex != 2 ||
        mesh.BumpMapTextureIndex != 3 || mesh.IndexBuffer != &g_sharedIndexBuffer) return 12;
    if (g_indexAllocCalls != 0) return 13;

    Reset();
    CDataInputStream failedVertexStream(bytes, size);
    memset(&mesh, 0, sizeof(mesh));
    g_failVertexAllocation = true;
    if (mesh.LoadForeground(failedVertexStream, palette)) return 14;

    Reset();
    size = BuildPayload(bytes, false);
    CDataInputStream failedIndexStream(bytes, size);
    memset(&mesh, 0, sizeof(mesh));
    g_failIndexAllocation = true;
    if (mesh.LoadForeground(failedIndexStream, palette)) return 15;
    if (g_vertexUnlockCalls != 1 || g_indexAllocCalls != 1) return 16;

    printf("LANDSCAPE_LAYER_MESH_LOAD_FOREGROUND_TEST PASS\n");
    return 0;
}
