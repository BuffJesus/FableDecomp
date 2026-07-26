bool __thiscall CEngineLandscapePatch::LoadTextures(float distanceSquared)
{
    // 0x0C = m_pForegroundTextureIndices
    // 0x10 = m_nForegroundTextureCount
    // 0x3C = m_Flags

    if ((this->m_Flags & 0x01) == 0)
    {
        this->m_Flags = static_cast<std::uint8_t>(this->m_Flags | 0x01);

        int textureIndex = 0;
        if (this->m_nForegroundTextureCount != 0)
        {
            std::uint16_t* pTextureIndex = this->m_pForegroundTextureIndices;
            do
            {
                const auto textureManager = *reinterpret_cast<std::uint8_t**>(DAT_01436e98 + 0x954);
                const auto entriesBegin = *reinterpret_cast<std::uintptr_t*>(textureManager + 0x1E0);
                const auto entriesEnd = *reinterpret_cast<std::uintptr_t*>(textureManager + 0x1E4);

                const std::uint32_t entryIndex = static_cast<std::uint32_t>(*pTextureIndex);
                std::uintptr_t handleValue;

                if ((entryIndex < static_cast<std::uint32_t>((entriesEnd - entriesBegin) / 0x2C)) &&
                    ((handleValue = entryIndex * 0x2C + entriesBegin), handleValue != 0))
                {
                    handleValue = *reinterpret_cast<std::uintptr_t*>(handleValue + 0x28);
                }
                else
                {
                    handleValue = 0;
                }

                const auto resourceValue = *reinterpret_cast<std::uintptr_t*>(handleValue + 0x08);
                if ((resourceValue == 0) ||
                    (*reinterpret_cast<std::int32_t*>(resourceValue + 0x1C) == 0))
                {
                    this->m_Flags = static_cast<std::uint8_t>(this->m_Flags & 0xFE);
                }

                ++textureIndex;
                ++pTextureIndex;
            } while (textureIndex < static_cast<int>(this->m_nForegroundTextureCount));
        }
    }

    CEngineLandscapeRenderer::AddFrameForegroundTexturesUsed(
        DAT_01436ea8,
        this->m_pForegroundTextureIndices,
        static_cast<std::uint32_t>(this->m_nForegroundTextureCount),
        distanceSquared);

    return (this->m_Flags & 0x01) != 0;
}