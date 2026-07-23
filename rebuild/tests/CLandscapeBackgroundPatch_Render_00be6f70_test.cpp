#include <stdio.h>
#include <string.h>
#include "candidates/landscape_background_patch_render_compiled.h"

static CTexture g_texture;
static CIndexBuffer g_indices[5];
static CVertexBuffer g_vertices[5];
static int g_beginCalls, g_pixelCalls, g_textureCalls, g_shaderCalls, g_constantCalls;
static int g_bindCalls, g_baseDrawCalls, g_layerDrawCalls, g_restoreCalls;
static bool g_specialShader;
static fable_u8 g_stride;
static unsigned long g_base104, g_base106;
static CVertexBuffer* g_layerVertices[4];
static CIndexBuffer* g_layerIndices[4];
static unsigned long g_layerCounts[4];

void BeginLandscapeBackgroundRenderState() { ++g_beginCalls; }
void ApplyLandscapeBackgroundPixelShader() { ++g_pixelCalls; }
void PrepareLandscapeBackgroundTexture(CTexture* texture)
{
    ++g_textureCalls;
    if (texture != &g_texture) g_textureCalls = -100;
}
void ApplyLandscapeBackgroundVertexShader(bool specialPath)
{
    ++g_shaderCalls; g_specialShader = specialPath;
}
void SetLandscapeBackgroundRenderConstants() { ++g_constantCalls; }
void BindLandscapeBackgroundBuffers(CVertexBuffer* vertices, CIndexBuffer* indices)
{
    ++g_bindCalls;
    if (vertices != &g_vertices[0] || indices != &g_indices[0]) g_bindCalls = -100;
}
fable_u8 GetLandscapeBackgroundVertexStride(CVertexBuffer*) { return g_stride; }
void DrawLandscapeBackgroundBase(unsigned long count104, unsigned long count106)
{
    ++g_baseDrawCalls; g_base104 = count104; g_base106 = count106;
}
void DrawLandscapeBackgroundLayer(CVertexBuffer* vertices, CIndexBuffer* indices, unsigned long count)
{
    int slot = g_layerDrawCalls++;
    g_layerVertices[slot] = vertices; g_layerIndices[slot] = indices; g_layerCounts[slot] = count;
}
void RestoreLandscapeBackgroundRenderState() { ++g_restoreCalls; }

static void ResetCounters()
{
    g_beginCalls = g_pixelCalls = g_textureCalls = g_shaderCalls = g_constantCalls = 0;
    g_bindCalls = g_baseDrawCalls = g_layerDrawCalls = g_restoreCalls = 0;
    g_specialShader = false; g_stride = 24; g_base104 = g_base106 = 0;
    memset(g_layerVertices, 0, sizeof(g_layerVertices));
    memset(g_layerIndices, 0, sizeof(g_layerIndices));
    memset(g_layerCounts, 0, sizeof(g_layerCounts));
}

int main()
{
    CLandscapeBackgroundPatch patch;
    CBackgroundRenderTreeNode parent;
    CBackgroundRenderLandscapeMap map;
    CBackgroundForegroundPatchProbe probes[8];
    memset(&patch, 0, sizeof(patch)); memset(&parent, 0, sizeof(parent));
    memset(&map, 0, sizeof(map)); memset(probes, 0, sizeof(probes));
    parent.LandscapeMap = &map; map.ForegroundPatches = probes; map.PatchColumns = 4;
    patch.Parent = &parent; patch.Texture = &g_texture;
    patch.VertexBuffer = &g_vertices[0]; patch.IndexBuffer = &g_indices[0];

    ResetCounters();
    if (patch.Render(false)) return 1;
    if (g_beginCalls != 0 || g_restoreCalls != 0) return 2;

    patch.Count104 = 12; patch.Count106 = 34;
    patch.ShaderVariant = 0;
    patch.LayerVertexBuffers[0] = &g_vertices[1]; patch.LayerIndexBuffers[0] = &g_indices[1]; patch.LayerIndexCounts[0] = 7;
    patch.LayerVertexBuffers[2] = &g_vertices[3]; patch.LayerIndexBuffers[2] = &g_indices[3]; patch.LayerIndexCounts[2] = 9;
    ResetCounters();
    if (!patch.Render(true)) return 3;
    if (g_beginCalls != 1 || g_pixelCalls != 1 || g_textureCalls != 1 ||
        g_shaderCalls != 1 || g_specialShader || g_constantCalls != 1 || g_bindCalls != 1) return 4;
    if (g_baseDrawCalls != 1 || g_base104 != 12 || g_base106 != 34 || g_layerDrawCalls != 2) return 5;
    if (g_layerVertices[0] != &g_vertices[1] || g_layerCounts[0] != 7 ||
        g_layerVertices[1] != &g_vertices[3] || g_layerCounts[1] != 9 || g_restoreCalls != 1) return 6;

    patch.SectionX = 16; patch.SectionY = 16; patch.ShaderVariant = 1;
    probes[5].Flags = 4;
    ResetCounters();
    if (!patch.Render(false) || !g_specialShader) return 7;

    patch.DisableSpecialShaderPath = 1;
    ResetCounters();
    if (!patch.Render(false) || g_specialShader) return 8;

    patch.DisableSpecialShaderPath = 0; g_stride = 0;
    ResetCounters(); g_stride = 0;
    if (patch.Render(false)) return 9;
    if (g_baseDrawCalls != 1 || g_restoreCalls != 1) return 10;

    printf("LANDSCAPE_BACKGROUND_PATCH_RENDER_TEST PASS\n");
    return 0;
}
