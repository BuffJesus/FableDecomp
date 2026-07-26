#include "candidates/landscape_patch_render_foreground_compiled.h"

void CEngineLandscapePatch::RenderForeground()
{
    if ((Flags & 4) == 0)
        return;

    BeginLandscapeForegroundRenderState();
    if (!ClipLandscapeForegroundPatch(this))
    {
        RestoreLandscapeForegroundRenderState();
        return;
    }

    int diffusePass = 0;
    int projectedPass = 0;
    SetupLandscapeForegroundLighting(this, &diffusePass, &projectedPass);
    SetLandscapeForegroundWorldAndConstants(this);

    int currentPass = -1;
    int currentVariant = -1;
    for (CForegroundRenderEntry* entry = Foreground; entry != 0; entry = entry->Next)
    {
        UpdateLandscapeForegroundGlobalLighting(entry->GlobalLightScale);
        int pass = entry->ProjectedFrame == 0 ? projectedPass : diffusePass;
        if (pass != currentPass)
        {
            ApplyLandscapeForegroundShaderPass(pass);
            currentPass = pass;
        }
        if (entry->ShaderVariantIndex != currentVariant)
        {
            ApplyLandscapeForegroundShaderVariant(entry->ShaderVariantIndex, this);
            currentVariant = entry->ShaderVariantIndex;
        }

        bool bindProjected = pass == 4 && entry->ProjectedFrame != 0;
        if (bindProjected && !PrepareLandscapeForegroundTexture(
                entry->ProjectedFrame, entry->ProjectedLodIndex, true))
        {
            Flags = static_cast<fable_u8>(Flags & 0xFB);
            break;
        }
        if (entry->DiffuseFrame == 0 || !PrepareLandscapeForegroundTexture(
                entry->DiffuseFrame, entry->DiffuseLodIndex, false))
        {
            Flags = static_cast<fable_u8>(Flags & 0xFB);
            break;
        }

        BindLandscapeForegroundEntry(entry, bindProjected);
        DrawLandscapeForegroundEntry(entry->BaseVertexIndex, entry->PrimitiveCount);
    }

    DisableLandscapeForegroundPointLights();
    UnbindLandscapeForegroundResources();
    RenderLandscapeForegroundWater(WaterPatch);
    RestoreLandscapeForegroundRenderState();
}
