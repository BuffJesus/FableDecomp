#include <stdio.h>
#include <string.h>
#include "candidates/landscape_renderer_build_themes_compiled.h"

static long g_themeCount;
static CSoundThemeDefRuntime g_definitions[3];
static fable_u16 g_widths[32];
static int g_acquireCalls, g_destroyCalls;

static void DestroyDefinition(void*) { ++g_destroyCalls; }
static CSoundThemeDefVTable g_vtable = { 0, DestroyDefinition };

long GetEngineThemeCount() { return g_themeCount; }

CSoundThemeDefRuntime* AcquireEngineThemeDefinition(unsigned long index)
{
    ++g_acquireCalls;
    if (index >= 3)
        return 0;
    return &g_definitions[index];
}

fable_u16 GetEngineThemeTextureWidth(fable_i32 textureId)
{
    if (textureId < 0 || textureId >= 32)
        return 0;
    return g_widths[textureId];
}

static void Reset()
{
    memset(g_definitions, 0, sizeof(g_definitions));
    memset(g_widths, 0, sizeof(g_widths));
    g_acquireCalls = g_destroyCalls = 0;
    for (int index = 0; index < 3; ++index)
    {
        g_definitions[index].VTable = &g_vtable;
        g_definitions[index].ReferenceCount = 1;
    }
}

int main()
{
    CEngineLandscapeRenderer renderer;
    memset(&renderer, 0, sizeof(renderer));
    Reset();

    g_themeCount = 3;
    CSoundThemeDefRuntime& first = g_definitions[1];
    first.AmbientTextureId = 1; first.AmbientMaterialId = 11;
    first.DetailTextureId = 2; first.DetailMaterialId = 12;
    first.CliffTextureId = 3; first.CliffMaterialId = 4;
    first.FalloffTextureId = 5; first.FalloffMaterialId = 15;
    first.AmbientTiles = 2; first.DetailTiles = 3;
    first.CliffTiles = 4; first.FalloffTiles = 5;
    first.AmbientScale = 21; first.DetailScale = 22;
    g_widths[1] = 256;
    g_widths[2] = 192;
    g_widths[3] = 4;
    g_widths[4] = 4;

    CSoundThemeDefRuntime& second = g_definitions[2];
    second.AmbientTextureId = 6;
    second.AmbientTiles = 4;
    second.DetailTextureId = 7;
    second.DetailTiles = 0;
    second.ReferenceCount = 2;
    g_widths[6] = 2;

    renderer.BuildThemes();
    if (renderer.Themes.Size() != 3 || g_acquireCalls != 2) return 1;
    const CEngineThemeEntry& theme = renderer.Themes.Begin[1];
    if (theme.AmbientTextureId != 1 || theme.AmbientMaterialId != 11 ||
        theme.DetailTextureId != 2 || theme.DetailMaterialId != 12 ||
        theme.CliffTextureId != 3 || theme.CliffMaterialId != 4) return 2;
    if (theme.FalloffTextureId != 5 || theme.FalloffMaterialId != 15 ||
        theme.AmbientScale != 21 || theme.DetailScale != 22) return 3;
    if (theme.AmbientMipShift != 7 || theme.DetailMipShift != 6 ||
        theme.CliffMipShift != 0 || theme.FalloffMipShift != -1) return 4;
    if (renderer.Themes.Begin[2].AmbientMipShift != -1 ||
        renderer.Themes.Begin[2].DetailMipShift != 0) return 5;
    if (g_destroyCalls != 1 || first.ReferenceCount != 0 || second.ReferenceCount != 1) return 6;

    g_themeCount = 1;
    renderer.BuildThemes();
    if (renderer.Themes.Size() != 1 || renderer.Themes.Begin[0].AmbientTextureId != 0) return 7;

    g_themeCount = 0;
    renderer.BuildThemes();
    if (renderer.Themes.Size() != 0) return 8;

    printf("LANDSCAPE_RENDERER_BUILD_THEMES_TEST PASS\n");
    return 0;
}
