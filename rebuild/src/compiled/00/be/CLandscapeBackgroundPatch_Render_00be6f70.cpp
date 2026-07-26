#include "candidates/landscape_background_patch_render_compiled.h"

static bool UseLandscapeBackgroundSpecialShader(const CLandscapeBackgroundPatch& patch)
{
    if (patch.DisableSpecialShaderPath != 0 || patch.ShaderVariant != 1 || patch.Parent == 0)
        return false;
    CBackgroundRenderLandscapeMap* map = patch.Parent->LandscapeMap;
    if (map == 0 || map->ForegroundPatches == 0 || map->PatchColumns <= 0)
        return false;
    unsigned long column = static_cast<unsigned long>(patch.SectionX) >> 4;
    unsigned long row = static_cast<unsigned long>(patch.SectionY) >> 4;
    CBackgroundForegroundPatchProbe* foreground =
        map->ForegroundPatches + row * static_cast<unsigned long>(map->PatchColumns) + column;
    return (foreground->Flags & 4) != 0;
}

bool CLandscapeBackgroundPatch::Render(bool)
{
    if (Count106 == 0)
        return false;

    bool specialShader = UseLandscapeBackgroundSpecialShader(*this);
    BeginLandscapeBackgroundRenderState();
    ApplyLandscapeBackgroundPixelShader();
    PrepareLandscapeBackgroundTexture(Texture);
    ApplyLandscapeBackgroundVertexShader(specialShader);
    SetLandscapeBackgroundRenderConstants();
    BindLandscapeBackgroundBuffers(VertexBuffer, IndexBuffer);

    fable_u8 vertexStride = GetLandscapeBackgroundVertexStride(VertexBuffer);
    DrawLandscapeBackgroundBase(Count104, Count106);
    for (int layer = 0; layer < 4; ++layer)
    {
        if (LayerVertexBuffers[layer] != 0)
        {
            DrawLandscapeBackgroundLayer(
                LayerVertexBuffers[layer],
                LayerIndexBuffers[layer],
                LayerIndexCounts[layer]);
        }
    }
    RestoreLandscapeBackgroundRenderState();
    return vertexStride != 0;
}
