#include <cstddef>
#include <cstdint>

class CCharString;
class CGameScriptInterface;

class CWorldMap
{
public:
    void SetRegionsAsConnectedInGraph(
        const CCharString& region_one,
        const CCharString& region_two,
        bool allow_route);
};

struct CGameScriptInterface_WorldMapProviderOverlay
{
    std::byte m_Pad0[0x04];
    void* m_WorldMapProvider; // 0x04
};

static_assert(offsetof(CGameScriptInterface_WorldMapProviderOverlay, m_WorldMapProvider) == 0x04);
static_assert(sizeof(CGameScriptInterface_WorldMapProviderOverlay) == 0x08);

void CGameScriptInterface::MiniMapAllowRouteBetweenRegions(
    const CCharString& region_one,
    const CCharString& region_two,
    bool allow_route) const
{
    auto* const overlay =
        reinterpret_cast<const CGameScriptInterface_WorldMapProviderOverlay*>(this);

    using GetWorldMapForRouteFn =
        CWorldMap*(__thiscall*)(void*, const CCharString&, const CCharString&, bool);

    auto* const world_map_provider_vftable =
        *reinterpret_cast<void***>(overlay->m_WorldMapProvider);
    auto* const world_map =
        reinterpret_cast<GetWorldMapForRouteFn>(world_map_provider_vftable[0x30 / sizeof(void*)])(
            overlay->m_WorldMapProvider,
            region_one,
            region_two,
            allow_route);

    world_map->SetRegionsAsConnectedInGraph(region_one, region_two, allow_route);
}