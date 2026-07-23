#include <stdio.h>
#include <string.h>
#include "candidates/landscape_background_patch_load_compiled.h"

static CTexture g_texture;
static CVertexBuffer g_vertexBuffer;
static CIndexBuffer g_indexBuffer, g_sharedIndexBuffer;
static CEngineWaterBackgroundSubPatch g_water;
static CBackgroundLandscapeVertex g_vertices[4];
static fable_u8 g_indices[64];
static bool g_failTexture, g_failVertex, g_failIndex, g_failEdge, g_failWater;
static int g_textureCalls, g_vertexCalls, g_indexCalls, g_edgeCalls, g_waterCalls;
static int g_vertexUnlocks, g_indexUnlocks, g_decompressCalls, g_normalCalls;
static unsigned long g_textureWidth, g_textureHeight, g_vertexCount, g_vertexStride;
static unsigned long g_indexBytes, g_sharedWidth, g_sharedHeight;
static unsigned long g_waterWidth, g_waterHeight, g_waterMapX, g_waterMapY;
static float g_waterMinZ, g_waterMaxZ;
static bool g_textureDxt1;
static fable_u32 g_normals[4];

CTexture* AllocateLandscapeBackgroundTexture(
    CLandscapeBackgroundPatch*, unsigned long width, unsigned long height, bool dxt1)
{
    ++g_textureCalls; g_textureWidth = width; g_textureHeight = height; g_textureDxt1 = dxt1;
    return g_failTexture ? 0 : &g_texture;
}
bool LoadLandscapeBackgroundTexture(CTexture*, CDataInputStream&) { return true; }
CVertexBuffer* AllocateLandscapeBackgroundVertexBuffer(
    CLandscapeBackgroundPatch*, unsigned long count, unsigned long stride)
{
    ++g_vertexCalls; g_vertexCount = count; g_vertexStride = stride;
    memset(g_vertices, 0, sizeof(g_vertices));
    return g_failVertex ? 0 : &g_vertexBuffer;
}
void* LockLandscapeBackgroundVertexBuffer(CVertexBuffer*) { return g_vertices; }
void UnlockLandscapeBackgroundVertexBuffer(CVertexBuffer*) { ++g_vertexUnlocks; }
void SetLandscapeBackgroundTargetNormal(CBackgroundLandscapeVertex* vertex, fable_u32 normal)
{
    g_normals[g_normalCalls++] = normal;
    vertex->Unknown08[0] = static_cast<fable_u8>(normal);
}
bool DecompressLandscapeRange(
    const fable_u8* source, unsigned long sourceSize,
    unsigned long elementCount, unsigned long elementSize, void* destination)
{
    ++g_decompressCalls;
    unsigned long outputBytes = elementCount * elementSize;
    if (sourceSize < outputBytes) return false;
    memcpy(destination, source, outputBytes);
    return true;
}
CIndexBuffer* GetLandscapeBackgroundSharedIndexBuffer(unsigned long width, unsigned long height)
{
    g_sharedWidth = width; g_sharedHeight = height; return &g_sharedIndexBuffer;
}
CIndexBuffer* AllocateLandscapeBackgroundIndexBuffer(
    CLandscapeBackgroundPatch*, unsigned long bytes, unsigned long, unsigned long)
{
    ++g_indexCalls; g_indexBytes = bytes; memset(g_indices, 0, sizeof(g_indices));
    return g_failIndex ? 0 : &g_indexBuffer;
}
void* LockLandscapeBackgroundIndexBuffer(CIndexBuffer*) { return g_indices; }
void UnlockLandscapeBackgroundIndexBuffer(CIndexBuffer*) { ++g_indexUnlocks; }
CEngineWaterBackgroundSubPatch* CreateLandscapeWaterBackgroundSubPatch(CEngineLandscapeMap*)
{
    return &g_water;
}
bool LoadLandscapeWaterBackgroundSubPatch(
    CEngineWaterBackgroundSubPatch*, CDataInputStream&, unsigned long width, unsigned long height,
    unsigned long mapX, unsigned long mapY, float minZ, float maxZ)
{
    ++g_waterCalls; g_waterWidth = width; g_waterHeight = height;
    g_waterMapX = mapX; g_waterMapY = mapY; g_waterMinZ = minZ; g_waterMaxZ = maxZ;
    return !g_failWater;
}
bool CPatchTesselationEdgeStrip::Load(CDataInputStream& input, CLandscapeBackgroundPatch*)
{
    int edge = g_edgeCalls++;
    fable_u8 marker = 0;
    if (!input.Read8(marker)) return false;
    return !g_failEdge || edge != 2;
}

static void Put8(fable_u8*& out, fable_u8 value) { *out++ = value; }
static void Put16(fable_u8*& out, fable_u16 value)
{
    Put8(out, static_cast<fable_u8>(value)); Put8(out, static_cast<fable_u8>(value >> 8));
}
static void Put32(fable_u8*& out, fable_u32 value)
{
    Put16(out, static_cast<fable_u16>(value)); Put16(out, static_cast<fable_u16>(value >> 16));
}

static int BuildPayload(fable_u8* bytes, bool waterOnly, fable_u8 lod, bool water)
{
    fable_u8* out = bytes;
    Put16(out, 2); Put16(out, 3); Put16(out, 4); Put16(out, 5);
    Put8(out, waterOnly ? 1 : 0); Put8(out, lod);
    if (!waterOnly)
    {
        Put16(out, 2); Put16(out, 1); Put8(out, 8); Put8(out, 9); Put8(out, 1);
        Put32(out, 16);
        Put16(out, 10); Put16(out, 20); Put32(out, 0x10203040); Put32(out, 0x50607080);
        Put16(out, 0x31); Put16(out, 0x32);
        if (lod != 1)
        {
            Put32(out, 12);
            for (int index = 0; index < 6; ++index) Put16(out, static_cast<fable_u16>(100 + index));
        }
    }
    Put8(out, 0xA0); Put8(out, 0xA1); Put8(out, 0xA2); Put8(out, 0xA3);
    Put8(out, water ? 1 : 0);
    return static_cast<int>(out - bytes);
}

static void Reset()
{
    g_failTexture = g_failVertex = g_failIndex = g_failEdge = g_failWater = false;
    g_textureCalls = g_vertexCalls = g_indexCalls = g_edgeCalls = g_waterCalls = 0;
    g_vertexUnlocks = g_indexUnlocks = g_decompressCalls = g_normalCalls = 0;
    g_textureWidth = g_textureHeight = g_vertexCount = g_vertexStride = 0;
    g_indexBytes = g_sharedWidth = g_sharedHeight = 0;
    g_waterWidth = g_waterHeight = g_waterMapX = g_waterMapY = 0;
    g_waterMinZ = g_waterMaxZ = 0.0f; g_textureDxt1 = false;
    memset(g_normals, 0, sizeof(g_normals));
}

int main()
{
    fable_u8 bytes[128];
    CEngineLandscapeRuntime runtime; memset(&runtime, 0, sizeof(runtime));
    runtime.BaseMapX = 1000; runtime.BaseMapY = 2000;
    CEngineLandscapeMap map; memset(&map, 0, sizeof(map)); map.Runtime = &runtime;
    CLandscapeBackgroundTreeNode parent; memset(&parent, 0, sizeof(parent)); parent.LandscapeMap = &map;
    CLandscapeBackgroundPatch patch;

    Reset();
    int size = BuildPayload(bytes, false, 0, true);
    CDataInputStream stream(bytes, size);
    memset(&patch, 0, sizeof(patch)); patch.Parent = &parent;
    if (!patch.Load(stream, -3.0f, 7.0f)) return 1;
    if (patch.Width != 2 || patch.Height != 3 || patch.MapX != 4 || patch.MapY != 5) return 2;
    if (patch.MinZ != -3.0f || patch.MaxZ != 7.0f || patch.VertexCount != 1 || patch.PolyCountIncDegenerates != 2) return 3;
    if (g_textureWidth != 8 || g_textureHeight != 9 || !g_textureDxt1) return 4;
    if (g_vertexCount != 1 || g_vertexStride != 0x18 || g_vertices[0].X != 10 ||
        g_vertices[0].Y != 20 || g_vertices[0].Z != 0x10203040) return 5;
    if (g_normals[0] != 0x50607080 || g_vertices[0].Byte12 != 0x31 || g_vertices[0].Byte11 != 0x32) return 6;
    if (g_indexBytes != 12 || *reinterpret_cast<fable_u16*>(g_indices) != 100 ||
        *reinterpret_cast<fable_u16*>(g_indices + 10) != 105) return 7;
    if (g_decompressCalls != 2 || g_vertexUnlocks != 1 || g_indexUnlocks != 1 || g_edgeCalls != 4) return 8;
    if (g_waterCalls != 1 || g_waterWidth != 2 || g_waterHeight != 3 ||
        g_waterMapX != 1004 || g_waterMapY != 2005 || g_waterMinZ != -3.0f || g_waterMaxZ != 7.0f) return 9;

    Reset();
    size = BuildPayload(bytes, false, 1, false);
    CDataInputStream sharedStream(bytes, size);
    memset(&patch, 0, sizeof(patch)); patch.Parent = &parent;
    if (!patch.Load(sharedStream, 1.0f, 2.0f)) return 10;
    if (!patch.SharedIndexBuffer || patch.IndexBuffer != &g_sharedIndexBuffer ||
        g_sharedWidth != 2 || g_sharedHeight != 3 || g_indexCalls != 0) return 11;

    Reset();
    size = BuildPayload(bytes, true, 3, true);
    CDataInputStream waterOnlyStream(bytes, size);
    memset(&patch, 0, sizeof(patch)); patch.Parent = &parent;
    if (!patch.Load(waterOnlyStream, 4.0f, 8.0f)) return 12;
    if (!patch.WaterOnlyPatch || g_textureCalls != 0 || g_vertexCalls != 0 || g_edgeCalls != 4 || g_waterCalls != 1) return 13;

    Reset();
    size = BuildPayload(bytes, false, 0, false);
    CDataInputStream failStream(bytes, size);
    memset(&patch, 0, sizeof(patch)); patch.Parent = &parent; g_failIndex = true;
    if (patch.Load(failStream, 0.0f, 0.0f) || g_indexCalls != 1 || g_edgeCalls != 0) return 14;

    Reset();
    CDataInputStream edgeFailStream(bytes, size);
    memset(&patch, 0, sizeof(patch)); patch.Parent = &parent; g_failEdge = true;
    if (patch.Load(edgeFailStream, 0.0f, 0.0f) || g_edgeCalls != 3) return 15;

    printf("LANDSCAPE_BACKGROUND_PATCH_LOAD_TEST PASS\n");
    return 0;
}
