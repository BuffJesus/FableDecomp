#include <stdio.h>
#include <string.h>
#include "candidates/landscape_patch_render_foreground_compiled.h"

static bool g_visible, g_failDiffuse, g_failProjected;
static int g_diffusePass, g_projectedPass;
static int g_beginCalls, g_clipCalls, g_setupCalls, g_worldCalls, g_lightCalls;
static int g_passCalls, g_variantCalls, g_prepareCalls, g_bindCalls, g_drawCalls;
static int g_disableCalls, g_unbindCalls, g_waterCalls, g_restoreCalls;
static int g_passes[8], g_variants[8], g_lods[8];
static bool g_projectedPrepare[8], g_projectedBind[8];
static float g_lightScales[8];
static unsigned long g_baseVertices[8], g_primitiveCounts[8];
static void* g_waterPointer;

void BeginLandscapeForegroundRenderState() { ++g_beginCalls; }
bool ClipLandscapeForegroundPatch(const CEngineLandscapePatch*) { ++g_clipCalls; return g_visible; }
void SetupLandscapeForegroundLighting(const CEngineLandscapePatch*, int* diffusePass, int* projectedPass)
{
    ++g_setupCalls; *diffusePass = g_diffusePass; *projectedPass = g_projectedPass;
}
void SetLandscapeForegroundWorldAndConstants(const CEngineLandscapePatch*) { ++g_worldCalls; }
void UpdateLandscapeForegroundGlobalLighting(float scale) { g_lightScales[g_lightCalls++] = scale; }
void ApplyLandscapeForegroundShaderPass(int pass) { g_passes[g_passCalls++] = pass; }
void ApplyLandscapeForegroundShaderVariant(int variant, const CEngineLandscapePatch*)
{
    g_variants[g_variantCalls++] = variant;
}
bool PrepareLandscapeForegroundTexture(CGraphicInfoFrame*, int lodIndex, bool projected)
{
    int slot = g_prepareCalls++;
    g_lods[slot] = lodIndex; g_projectedPrepare[slot] = projected;
    return projected ? !g_failProjected : !g_failDiffuse;
}
void BindLandscapeForegroundEntry(const CForegroundRenderEntry*, bool projected)
{
    g_projectedBind[g_bindCalls++] = projected;
}
void DrawLandscapeForegroundEntry(unsigned long baseVertex, unsigned long primitiveCount)
{
    int slot = g_drawCalls++;
    g_baseVertices[slot] = baseVertex; g_primitiveCounts[slot] = primitiveCount;
}
void DisableLandscapeForegroundPointLights() { ++g_disableCalls; }
void UnbindLandscapeForegroundResources() { ++g_unbindCalls; }
void RenderLandscapeForegroundWater(void* waterPatch) { ++g_waterCalls; g_waterPointer = waterPatch; }
void RestoreLandscapeForegroundRenderState() { ++g_restoreCalls; }

static void Reset()
{
    g_visible = true; g_failDiffuse = g_failProjected = false;
    g_diffusePass = 3; g_projectedPass = 4;
    g_beginCalls = g_clipCalls = g_setupCalls = g_worldCalls = g_lightCalls = 0;
    g_passCalls = g_variantCalls = g_prepareCalls = g_bindCalls = g_drawCalls = 0;
    g_disableCalls = g_unbindCalls = g_waterCalls = g_restoreCalls = 0;
    memset(g_passes, 0, sizeof(g_passes)); memset(g_variants, 0, sizeof(g_variants));
    memset(g_lods, 0, sizeof(g_lods)); memset(g_projectedPrepare, 0, sizeof(g_projectedPrepare));
    memset(g_projectedBind, 0, sizeof(g_projectedBind)); memset(g_lightScales, 0, sizeof(g_lightScales));
    memset(g_baseVertices, 0, sizeof(g_baseVertices)); memset(g_primitiveCounts, 0, sizeof(g_primitiveCounts));
    g_waterPointer = 0;
}

int main()
{
    CEngineLandscapePatch patch;
    CForegroundRenderEntry entries[3];
    CGraphicInfoFrame diffuse[3], projected;
    int water;
    memset(&patch, 0, sizeof(patch)); memset(entries, 0, sizeof(entries));
    patch.WaterPatch = &water;

    Reset();
    patch.RenderForeground();
    if (g_beginCalls != 0 || g_restoreCalls != 0) return 1;

    patch.Flags = 4; g_visible = false;
    patch.RenderForeground();
    if (g_beginCalls != 1 || g_clipCalls != 1 || g_restoreCalls != 1) return 2;
    if (g_setupCalls || g_waterCalls) return 3;

    Reset();
    entries[0].DiffuseFrame = &diffuse[0]; entries[0].ProjectedFrame = &projected;
    entries[0].DiffuseLodIndex = 2; entries[0].ProjectedLodIndex = 5;
    entries[0].GlobalLightScale = 0.5f; entries[0].ShaderVariantIndex = 7;
    entries[0].BaseVertexIndex = 10; entries[0].PrimitiveCount = 11; entries[0].Next = &entries[1];
    entries[1].DiffuseFrame = &diffuse[1]; entries[1].ProjectedFrame = 0;
    entries[1].DiffuseLodIndex = 3; entries[1].GlobalLightScale = 0.75f;
    entries[1].ShaderVariantIndex = 7; entries[1].BaseVertexIndex = 20; entries[1].PrimitiveCount = 21;
    patch.Foreground = entries; patch.Flags = 4;
    patch.RenderForeground();
    if (g_setupCalls != 1 || g_worldCalls != 1 || g_lightCalls != 2) return 4;
    if (g_lightScales[0] != 0.5f || g_lightScales[1] != 0.75f) return 5;
    if (g_passCalls != 2 || g_passes[0] != 3 || g_passes[1] != 4 ||
        g_variantCalls != 1 || g_variants[0] != 7) return 6;
    if (g_prepareCalls != 2 || g_lods[0] != 2 || g_projectedPrepare[0] ||
        g_lods[1] != 3 || g_projectedPrepare[1]) return 7;
    if (g_bindCalls != 2 || g_projectedBind[0] || g_projectedBind[1] || g_drawCalls != 2) return 8;
    if (g_baseVertices[0] != 10 || g_primitiveCounts[1] != 21) return 9;
    if (g_disableCalls != 1 || g_unbindCalls != 1 || g_waterCalls != 1 ||
        g_waterPointer != &water || g_restoreCalls != 1 || patch.Flags != 4) return 10;

    Reset();
    g_diffusePass = 4;
    entries[0].Next = &entries[1]; entries[0].ProjectedFrame = &projected;
    g_failProjected = true; patch.Flags = 4;
    patch.RenderForeground();
    if ((patch.Flags & 4) != 0 || g_prepareCalls != 1 || !g_projectedPrepare[0]) return 11;
    if (g_drawCalls != 0 || g_disableCalls != 1 || g_waterCalls != 1 || g_restoreCalls != 1) return 12;

    Reset();
    entries[0].ProjectedFrame = 0; entries[0].Next = &entries[1];
    g_failDiffuse = true; patch.Flags = 4;
    patch.RenderForeground();
    if ((patch.Flags & 4) != 0 || g_prepareCalls != 1 || g_drawCalls != 0) return 13;
    if (g_disableCalls != 1 || g_unbindCalls != 1 || g_waterCalls != 1 || g_restoreCalls != 1) return 14;

    Reset();
    patch.Foreground = 0; patch.Flags = 4;
    patch.RenderForeground();
    if (g_lightCalls != 0 || g_drawCalls != 0 || g_waterCalls != 1 || g_restoreCalls != 1) return 15;

    printf("LANDSCAPE_PATCH_RENDER_FOREGROUND_TEST PASS\n");
    return 0;
}
