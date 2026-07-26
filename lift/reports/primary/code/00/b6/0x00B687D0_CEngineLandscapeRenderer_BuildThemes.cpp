namespace
{
struct CEngineThemeEntry
{
    std::int32_t m_ambientTextureId;      // 0x00
    std::int32_t m_ambientMaterialId;     // 0x04
    std::int32_t m_detailTextureId;       // 0x08
    std::int32_t m_detailMaterialId;      // 0x0C
    std::int32_t m_cliffTextureId;        // 0x10
    std::int32_t m_cliffMaterialId;       // 0x14
    std::int32_t m_ambientMipShift;       // 0x18
    std::int32_t m_detailMipShift;        // 0x1C
    std::int32_t m_cliffMipShift;         // 0x20
    std::int32_t m_falloffMipShift;       // 0x24
    std::int32_t m_ambientScale;          // 0x28
    std::int32_t m_detailScale;           // 0x2C
    std::int32_t m_falloffTextureId;      // 0x30
    std::int32_t m_falloffMaterialId;     // 0x34
};

struct CSoundThemeDefVTable
{
    void* m_unused0;
    void(__thiscall* m_destroy)(void*);
};

struct CSoundThemeDefRuntime
{
    CSoundThemeDefVTable* m_vtable;       // 0x00
    std::int32_t m_refCount;             // 0x04
    std::byte m_pad08[0x34];             // 0x08
    std::int32_t m_ambientTextureId;     // 0x3C
    std::int32_t m_ambientMaterialId;    // 0x40
    std::int32_t m_detailTextureId;      // 0x44
    std::int32_t m_detailMaterialId;     // 0x48
    std::int32_t m_cliffTextureId;       // 0x4C
    std::int32_t m_cliffMaterialId;      // 0x50
    std::int32_t m_ambientTiles;         // 0x54
    std::int32_t m_detailTiles;          // 0x58
    std::int32_t m_cliffTiles;           // 0x5C
    std::int32_t m_falloffTiles;         // 0x60
    std::int32_t m_ambientScale;         // 0x64
    std::int32_t m_detailScale;          // 0x68
    std::int32_t m_falloffMaterialId;    // 0x6C
    std::int32_t m_falloffTextureId;     // 0x70
};

struct CTextureHandleRuntime
{
    std::byte m_pad00[0x0C];
    void* m_pTexture;                    // 0x0C
};

struct CTextureRuntime
{
    std::byte m_pad00[0x06];
    std::uint16_t m_width;               // 0x06
};

struct CTextureRecord
{
    std::byte m_pad00[0x28];
    CTextureHandleRuntime* m_pHandle;    // 0x28
};

struct CTextureRepositoryRuntime
{
    std::byte m_pad00[0x1E0];
    CTextureRecord* m_pBegin;            // 0x1E0
    CTextureRecord* m_pEnd;              // 0x1E4
};

static_assert(sizeof(CEngineThemeEntry) == 0x38);
static_assert(offsetof(CSoundThemeDefRuntime, m_ambientTextureId) == 0x3C);
static_assert(offsetof(CSoundThemeDefRuntime, m_falloffTextureId) == 0x70);
static_assert(offsetof(CTextureRecord, m_pHandle) == 0x28);
static_assert(offsetof(CTextureHandleRuntime, m_pTexture) == 0x0C);
static_assert(offsetof(CTextureRuntime, m_width) == 0x06);
}

void CEngineLandscapeRenderer::BuildThemes()
{
    CCharString themeSymbol("ENGINE_THEME", -1);
    const long themeCount = LTextSystem::GetIDFromSymbol(DAT_01436e28, themeSymbol);

    CEngineThemeEntry defaultTheme{};
    m_themes.resize(themeCount, defaultTheme);

    std::uint32_t themeIndex = 1;
    if (m_themes.size() > 1)
    {
        std::size_t themeOffset = sizeof(CEngineThemeEntry);
        do
        {
            CSoundThemeDefRuntime* pThemeDef = nullptr;
            CCharString defName("ENGINE_THEME", -1);
            CDefinitionManager::GetDef<CSoundThemeDef>(
                reinterpret_cast<CDefinitionManager*>(DAT_01436e28),
                defName,
                themeIndex,
                reinterpret_cast<CDefPointer<CSoundThemeDef const>*>(&pThemeDef));

            auto& theme = *reinterpret_cast<CEngineThemeEntry*>(
                reinterpret_cast<std::byte*>(m_themes.data()) + themeOffset);

            theme.m_ambientTextureId = pThemeDef->m_ambientTextureId;
            theme.m_ambientMaterialId = pThemeDef->m_ambientMaterialId;
            theme.m_detailTextureId = pThemeDef->m_detailTextureId;
            theme.m_detailMaterialId = pThemeDef->m_detailMaterialId;
            theme.m_cliffTextureId = pThemeDef->m_cliffTextureId;
            theme.m_cliffMaterialId = pThemeDef->m_cliffMaterialId;
            theme.m_falloffMaterialId = pThemeDef->m_falloffMaterialId;
            theme.m_falloffTextureId = pThemeDef->m_falloffTextureId;
            theme.m_ambientMipShift = 0;
            theme.m_detailMipShift = 0;
            theme.m_cliffMipShift = 0;
            theme.m_falloffMipShift = 0;
            theme.m_ambientScale = pThemeDef->m_ambientScale;
            theme.m_detailScale = pThemeDef->m_detailScale;

            if (pThemeDef->m_ambientTiles > 0)
            {
                std::uint32_t textureId = static_cast<std::uint32_t>(theme.m_ambientTextureId);
                auto* const pRepository = *reinterpret_cast<CTextureRepositoryRuntime**>(
                    reinterpret_cast<std::byte*>(DAT_01436e98) + 0x954);
                CTextureHandleRuntime* pHandle;
                if ((textureId < static_cast<std::uint32_t>((pRepository->m_pEnd - pRepository->m_pBegin))) &&
                    ((reinterpret_cast<std::uintptr_t>(pRepository->m_pBegin) + textureId * sizeof(CTextureRecord)) != 0))
                {
                    auto* const pRecord = reinterpret_cast<CTextureRecord*>(
                        reinterpret_cast<std::byte*>(pRepository->m_pBegin) + textureId * sizeof(CTextureRecord));
                    pHandle = pRecord->m_pHandle;
                }
                else
                {
                    pHandle = nullptr;
                }

                std::uint32_t quotient =
                    static_cast<std::uint32_t>(reinterpret_cast<CTextureRuntime*>(pHandle->m_pTexture)->m_width) /
                    static_cast<std::uint32_t>(pThemeDef->m_ambientTiles);

                std::int32_t mipShift;
                if (quotient == 0)
                {
                    mipShift = -1;
                }
                else if (quotient < 0x80000000U)
                {
                    std::int32_t value = static_cast<std::int32_t>(quotient + 1);
                    std::uint32_t bits = (0x10000U - static_cast<std::uint32_t>(value)) >> 31 & 0x10;
                    mipShift = static_cast<std::int32_t>(bits + (((1 << (static_cast<char>(bits) + 8)) - value) >> 31 & 8U));
                    mipShift = mipShift + ((((1 << (static_cast<char>(mipShift) + 4)) - value) >> 31) & 4U);
                    mipShift = mipShift + ((((1 << (static_cast<char>(mipShift) + 2)) - value) >> 31) & 2U);
                    mipShift = static_cast<std::int32_t>((((1 << (static_cast<char>(mipShift) + 1)) - value) >> 31) & 1U) + mipShift;
                }
                else
                {
                    mipShift = 0x1F;
                }

                theme.m_ambientMipShift = mipShift;
            }

            if (pThemeDef->m_detailTiles > 0)
            {
                std::uint32_t textureId = static_cast<std::uint32_t>(theme.m_detailTextureId);
                auto* const pRepository = *reinterpret_cast<CTextureRepositoryRuntime**>(
                    reinterpret_cast<std::byte*>(DAT_01436e98) + 0x954);
                CTextureHandleRuntime* pHandle;
                if ((textureId < static_cast<std::uint32_t>((pRepository->m_pEnd - pRepository->m_pBegin))) &&
                    ((reinterpret_cast<std::uintptr_t>(pRepository->m_pBegin) + textureId * sizeof(CTextureRecord)) != 0))
                {
                    auto* const pRecord = reinterpret_cast<CTextureRecord*>(
                        reinterpret_cast<std::byte*>(pRepository->m_pBegin) + textureId * sizeof(CTextureRecord));
                    pHandle = pRecord->m_pHandle;
                }
                else
                {
                    pHandle = nullptr;
                }

                std::uint32_t quotient =
                    static_cast<std::uint32_t>(reinterpret_cast<CTextureRuntime*>(pHandle->m_pTexture)->m_width) /
                    static_cast<std::uint32_t>(pThemeDef->m_detailTiles);

                std::int32_t mipShift;
                if (quotient == 0)
                {
                    mipShift = -1;
                }
                else if (quotient < 0x80000000U)
                {
                    std::int32_t value = static_cast<std::int32_t>(quotient + 1);
                    std::uint32_t bits = (0x10000U - static_cast<std::uint32_t>(value)) >> 31 & 0x10;
                    mipShift = static_cast<std::int32_t>(bits + (((1 << (static_cast<char>(bits) + 8)) - value) >> 31 & 8U));
                    mipShift = mipShift + ((((1 << (static_cast<char>(mipShift) + 4)) - value) >> 31) & 4U);
                    mipShift = mipShift + ((((1 << (static_cast<char>(mipShift) + 2)) - value) >> 31) & 2U);
                    mipShift = static_cast<std::int32_t>((((1 << (static_cast<char>(mipShift) + 1)) - value) >> 31) & 1U) + mipShift;
                }
                else
                {
                    mipShift = 0x1F;
                }

                theme.m_detailMipShift = mipShift;
            }

            if (pThemeDef->m_cliffTiles > 0)
            {
                std::uint32_t textureId = static_cast<std::uint32_t>(theme.m_cliffTextureId);
                auto* const pRepository = *reinterpret_cast<CTextureRepositoryRuntime**>(
                    reinterpret_cast<std::byte*>(DAT_01436e98) + 0x954);
                CTextureHandleRuntime* pHandle;
                if ((textureId < static_cast<std::uint32_t>((pRepository->m_pEnd - pRepository->m_pBegin))) &&
                    ((reinterpret_cast<std::uintptr_t>(pRepository->m_pBegin) + textureId * sizeof(CTextureRecord)) != 0))
                {
                    auto* const pRecord = reinterpret_cast<CTextureRecord*>(
                        reinterpret_cast<std::byte*>(pRepository->m_pBegin) + textureId * sizeof(CTextureRecord));
                    pHandle = pRecord->m_pHandle;
                }
                else
                {
                    pHandle = nullptr;
                }

                std::uint32_t quotient =
                    static_cast<std::uint32_t>(reinterpret_cast<CTextureRuntime*>(pHandle->m_pTexture)->m_width) /
                    static_cast<std::uint32_t>(pThemeDef->m_cliffTiles);

                std::int32_t mipShift;
                if (quotient == 0)
                {
                    mipShift = -1;
                }
                else if (quotient < 0x80000000U)
                {
                    std::int32_t value = static_cast<std::int32_t>(quotient + 1);
                    std::uint32_t bits = (0x10000U - static_cast<std::uint32_t>(value)) >> 31 & 0x10;
                    mipShift = static_cast<std::int32_t>(bits + (((1 << (static_cast<char>(bits) + 8)) - value) >> 31 & 8U));
                    mipShift = mipShift + ((((1 << (static_cast<char>(mipShift) + 4)) - value) >> 31) & 4U);
                    mipShift = mipShift + ((((1 << (static_cast<char>(mipShift) + 2)) - value) >> 31) & 2U);
                    mipShift = static_cast<std::int32_t>((((1 << (static_cast<char>(mipShift) + 1)) - value) >> 31) & 1U) + mipShift;
                }
                else
                {
                    mipShift = 0x1F;
                }

                theme.m_cliffMipShift = mipShift;
            }

            if (pThemeDef->m_falloffTiles > 0)
            {
                std::uint32_t textureId = static_cast<std::uint32_t>(theme.m_cliffMaterialId);
                auto* const pRepository = *reinterpret_cast<CTextureRepositoryRuntime**>(
                    reinterpret_cast<std::byte*>(DAT_01436e98) + 0x954);
                CTextureHandleRuntime* pHandle;
                if (textureId < static_cast<std::uint32_t>((pRepository->m_pEnd - pRepository->m_pBegin)))
                {
                    const std::uintptr_t recordAddress =
                        reinterpret_cast<std::uintptr_t>(pRepository->m_pBegin) + textureId * sizeof(CTextureRecord);
                    if (recordAddress == 0)
                    {
                        pHandle = nullptr;
                    }
                    else
                    {
                        auto* const pRecord = reinterpret_cast<CTextureRecord*>(recordAddress);
                        pHandle = pRecord->m_pHandle;
                    }
                }
                else
                {
                    pHandle = nullptr;
                }

                std::uint32_t quotient =
                    static_cast<std::uint32_t>(reinterpret_cast<CTextureRuntime*>(pHandle->m_pTexture)->m_width) /
                    static_cast<std::uint32_t>(pThemeDef->m_falloffTiles);

                std::int32_t mipShift;
                if (quotient == 0)
                {
                    mipShift = -1;
                }
                else if (quotient < 0x80000000U)
                {
                    std::int32_t value = static_cast<std::int32_t>(quotient + 1);
                    std::uint32_t bits = (0x10000U - static_cast<std::uint32_t>(value)) >> 31 & 0x10;
                    mipShift = static_cast<std::int32_t>(bits + (((1 << (static_cast<char>(bits) + 8)) - value) >> 31 & 8U));
                    mipShift = mipShift + ((((1 << (static_cast<char>(mipShift) + 4)) - value) >> 31) & 4U);
                    mipShift = mipShift + ((((1 << (static_cast<char>(mipShift) + 2)) - value) >> 31) & 2U);
                    mipShift = static_cast<std::int32_t>((((1 << (static_cast<char>(mipShift) + 1)) - value) >> 31) & 1U) + mipShift;
                }
                else
                {
                    mipShift = 0x1F;
                }

                theme.m_falloffMipShift = mipShift;
            }

            pThemeDef->m_refCount = pThemeDef->m_refCount - 1;
            if (pThemeDef->m_refCount == 0)
            {
                pThemeDef->m_vtable->m_destroy(pThemeDef);
            }

            ++themeIndex;
            themeOffset = themeOffset + sizeof(CEngineThemeEntry);
        } while (themeIndex < m_themes.size());
    }
}