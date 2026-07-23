#include <cstddef>

struct CEngineLandscapeRenderer_OnReleaseDefaultPoolResources_MapEntryOverlay
{
    CEngineLandscapeMap* m_pLandscapeMap; // 0x00
};

struct CEngineLandscapeRenderer_OnReleaseDefaultPoolResources_ListOverlay
{
    std::byte pad_0000[0x20];
    CEngineLandscapeRenderer_OnReleaseDefaultPoolResources_MapEntryOverlay** m_ppBegin; // 0x20
    CEngineLandscapeRenderer_OnReleaseDefaultPoolResources_MapEntryOverlay** m_ppEnd;   // 0x24
};

static_assert(
    offsetof(CEngineLandscapeRenderer_OnReleaseDefaultPoolResources_MapEntryOverlay, m_pLandscapeMap) == 0x00);
static_assert(
    offsetof(CEngineLandscapeRenderer_OnReleaseDefaultPoolResources_ListOverlay, m_ppBegin) == 0x20);
static_assert(
    offsetof(CEngineLandscapeRenderer_OnReleaseDefaultPoolResources_ListOverlay, m_ppEnd) == 0x24);

extern CEngineLandscapeRenderer_OnReleaseDefaultPoolResources_ListOverlay* DAT_01436e8c;

void CEngineLandscapeRenderer::OnReleaseDefaultPoolResources()
{
    int index = 0;
    auto* list = DAT_01436e8c;

    if (0 < static_cast<int>(
                (reinterpret_cast<int>(list->m_ppEnd) - reinterpret_cast<int>(list->m_ppBegin)) &
                0xFFFFFFFCU))
    {
        do
        {
            auto* entry = *(reinterpret_cast<CEngineLandscapeRenderer_OnReleaseDefaultPoolResources_MapEntryOverlay**>(
                reinterpret_cast<int>(list->m_ppBegin) + index * 4));

            if (entry != nullptr)
            {
                CEngineLandscapeMap* const map = entry->m_pLandscapeMap;
                if (map != nullptr)
                {
                    CEngineLandscapeMap::ReleaseDefaultPoolSurfaces(map);
                    list = DAT_01436e8c;
                }
            }

            index = index + 1;
        } while (index <
                 (reinterpret_cast<int>(list->m_ppEnd) - reinterpret_cast<int>(list->m_ppBegin)) >> 2);
    }
}