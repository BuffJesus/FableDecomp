#include <new>
#include "candidates/landscape_renderer_build_themes_compiled.h"

unsigned long CEngineThemeVector::Size() const
{
    return Begin == 0 ? 0 : static_cast<unsigned long>(End - Begin);
}

bool CEngineThemeVector::Resize(unsigned long count)
{
    unsigned long oldCount = Size();
    if (Begin != 0 && static_cast<unsigned long>(Capacity - Begin) >= count)
    {
        if (count > oldCount)
        {
            for (unsigned long index = oldCount; index < count; ++index)
            {
                CEngineThemeEntry* entry = Begin + index;
                for (unsigned long byte = 0; byte < sizeof(CEngineThemeEntry); ++byte)
                    reinterpret_cast<fable_u8*>(entry)[byte] = 0;
            }
        }
        End = Begin + count;
        return true;
    }

    CEngineThemeEntry* replacement = 0;
    if (count != 0)
    {
        replacement = static_cast<CEngineThemeEntry*>(operator new(count * sizeof(CEngineThemeEntry)));
        if (replacement == 0)
            return false;
        for (unsigned long index = 0; index < count; ++index)
        {
            CEngineThemeEntry* entry = replacement + index;
            for (unsigned long byte = 0; byte < sizeof(CEngineThemeEntry); ++byte)
                reinterpret_cast<fable_u8*>(entry)[byte] = 0;
        }
        unsigned long copyCount = oldCount < count ? oldCount : count;
        for (unsigned long index = 0; index < copyCount; ++index)
            replacement[index] = Begin[index];
    }

    if (Begin != 0)
        operator delete(Begin);
    Begin = replacement;
    End = replacement == 0 ? 0 : replacement + count;
    Capacity = End;
    return true;
}

static int CalculateThemeMipShift(fable_i32 textureId, fable_i32 tiles)
{
    if (tiles <= 0)
        return 0;
    unsigned long quotient =
        static_cast<unsigned long>(GetEngineThemeTextureWidth(textureId)) /
        static_cast<unsigned long>(tiles);
    if (quotient == 0)
        return -1;
    if (quotient >= 0x80000000UL)
        return 31;

    int shift = 0;
    while (quotient > 1)
    {
        quotient >>= 1;
        ++shift;
    }
    return shift;
}

void CEngineLandscapeRenderer::BuildThemes()
{
    long count = GetEngineThemeCount();
    if (count < 0)
        count = 0;
    if (!Themes.Resize(static_cast<unsigned long>(count)))
        return;

    for (unsigned long index = 1; index < Themes.Size(); ++index)
    {
        CSoundThemeDefRuntime* definition = AcquireEngineThemeDefinition(index);
        if (definition == 0)
            continue;

        CEngineThemeEntry& theme = Themes.Begin[index];
        theme.AmbientTextureId = definition->AmbientTextureId;
        theme.AmbientMaterialId = definition->AmbientMaterialId;
        theme.DetailTextureId = definition->DetailTextureId;
        theme.DetailMaterialId = definition->DetailMaterialId;
        theme.CliffTextureId = definition->CliffTextureId;
        theme.CliffMaterialId = definition->CliffMaterialId;
        theme.FalloffMaterialId = definition->FalloffMaterialId;
        theme.FalloffTextureId = definition->FalloffTextureId;
        theme.AmbientMipShift = 0;
        theme.DetailMipShift = 0;
        theme.CliffMipShift = 0;
        theme.FalloffMipShift = 0;
        theme.AmbientScale = definition->AmbientScale;
        theme.DetailScale = definition->DetailScale;

        if (definition->AmbientTiles > 0)
            theme.AmbientMipShift = CalculateThemeMipShift(theme.AmbientTextureId, definition->AmbientTiles);
        if (definition->DetailTiles > 0)
            theme.DetailMipShift = CalculateThemeMipShift(theme.DetailTextureId, definition->DetailTiles);
        if (definition->CliffTiles > 0)
            theme.CliffMipShift = CalculateThemeMipShift(theme.CliffTextureId, definition->CliffTiles);
        if (definition->FalloffTiles > 0)
            theme.FalloffMipShift = CalculateThemeMipShift(theme.CliffMaterialId, definition->FalloffTiles);

        --definition->ReferenceCount;
        if (definition->ReferenceCount == 0)
            definition->VTable->Destroy(definition);
    }
}
