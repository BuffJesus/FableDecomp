#include "candidates/landscape_patch_compiled.h"

bool CEngineLandscapePatch::LoadTextures(float distanceSquared)
{
    if ((m_Flags & 0x01) == 0)
    {
        m_Flags = static_cast<fable_u8>(m_Flags | 0x01);

        int textureIndex = 0;
        if (m_nForegroundTextureCount != 0)
        {
            fable_u16* pTextureIndex = m_pForegroundTextureIndices;
            do
            {
                fable_u8* textureManager = *reinterpret_cast<fable_u8**>(DAT_01436e98 + 0x954);
                const fable_u32 entriesBegin = *reinterpret_cast<fable_u32*>(textureManager + 0x1E0);
                const fable_u32 entriesEnd = *reinterpret_cast<fable_u32*>(textureManager + 0x1E4);

                const fable_u32 entryIndex = static_cast<fable_u32>(*pTextureIndex);
                fable_u32 handleValue;

                if ((entryIndex < (entriesEnd - entriesBegin) / 0x2C) &&
                    ((handleValue = entryIndex * 0x2C + entriesBegin), handleValue != 0))
                {
                    handleValue = *reinterpret_cast<fable_u32*>(handleValue + 0x28);
                }
                else
                {
                    handleValue = 0;
                }

                const fable_u32 resourceValue = *reinterpret_cast<fable_u32*>(handleValue + 0x08);
                if ((resourceValue == 0) ||
                    (*reinterpret_cast<fable_i32*>(resourceValue + 0x1C) == 0))
                {
                    m_Flags = static_cast<fable_u8>(m_Flags & 0xFE);
                }

                ++textureIndex;
                ++pTextureIndex;
            } while (textureIndex < static_cast<int>(m_nForegroundTextureCount));
        }
    }

    CEngineLandscapeRenderer::AddFrameForegroundTexturesUsed(
        DAT_01436ea8,
        m_pForegroundTextureIndices,
        static_cast<fable_u32>(m_nForegroundTextureCount),
        distanceSquared);

    return (m_Flags & 0x01) != 0;
}
