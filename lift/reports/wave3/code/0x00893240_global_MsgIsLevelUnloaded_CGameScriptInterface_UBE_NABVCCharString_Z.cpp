#include <cstddef>
#include <cstdint>

class CCharString;
class CGameScriptInterface;
class CMessageEvent;
class CMessageEventManager;
class CWorldMap;

class CWorldMap
{
public:
    long GetMapNumberFromScriptName(const CCharString* map_name);
};

class CMessageEventManager
{
public:
    template <class TFilter>
    CMessageEvent* FindMostRecentMessage(TFilter* filter);
};

class CGameScriptInterface
{
public:
    long GetMaxWorldFrameForMessages() const;
    bool MsgIsLevelUnloaded(const CCharString& level_name) const;
};

namespace
{
using CWorldMapProvider_VFunc34 = CWorldMap* (__thiscall*)(void*);

struct CWorldMapProvider_VTable
{
    std::byte m_Pad0[0x34];
    CWorldMapProvider_VFunc34 m_VFunc34; // 0x34
};

static_assert(offsetof(CWorldMapProvider_VTable, m_VFunc34) == 0x34);

struct CGameScriptInterface_WorldMapProviderOverlay
{
    std::byte m_Pad0[0x04];
    void* m_WorldMapProvider; // 0x04
};

static_assert(offsetof(CGameScriptInterface_WorldMapProviderOverlay, m_WorldMapProvider) == 0x04);
static_assert(sizeof(CGameScriptInterface_WorldMapProviderOverlay) == 0x08);

struct CWorldMapProvider_MessageEventManagerOverlay
{
    std::byte m_Pad0[0x60];
    CMessageEventManager* m_MessageEventManager; // 0x60
};

static_assert(
    offsetof(CWorldMapProvider_MessageEventManagerOverlay, m_MessageEventManager) == 0x60);

struct CMsgIsLevelUnloaded_FilterBindOverlay
{
    const CCharString** m_Filter0; // 0x00
    long* m_Filter1;               // 0x04
    long* m_Filter2;               // 0x08
};

static_assert(offsetof(CMsgIsLevelUnloaded_FilterBindOverlay, m_Filter0) == 0x00);
static_assert(offsetof(CMsgIsLevelUnloaded_FilterBindOverlay, m_Filter1) == 0x04);
static_assert(offsetof(CMsgIsLevelUnloaded_FilterBindOverlay, m_Filter2) == 0x08);
static_assert(sizeof(CMsgIsLevelUnloaded_FilterBindOverlay) == 0x0C);
}

bool CGameScriptInterface::MsgIsLevelUnloaded(const CCharString& level_name) const
{
    const auto* const interface_overlay =
        reinterpret_cast<const CGameScriptInterface_WorldMapProviderOverlay*>(this);
    void* const world_map_provider = interface_overlay->m_WorldMapProvider;

    const auto* const world_map_provider_vtable =
        *reinterpret_cast<const CWorldMapProvider_VTable* const*>(world_map_provider);
    CWorldMap* const world_map = world_map_provider_vtable->m_VFunc34(world_map_provider);

    long lStack_18 = world_map->GetMapNumberFromScriptName(&level_name);
    if (lStack_18 == 0) {
        return false;
    }

    const long lVar1 = GetMaxWorldFrameForMessages();
    long lStack_14;
    long lStack_10 = GetMaxWorldFrameForMessages();
    (void)lStack_10;

    const CCharString* param_2 = &level_name;

    CMsgIsLevelUnloaded_FilterBindOverlay filter_bind;
    filter_bind.m_Filter1 = &lStack_14;
    filter_bind.m_Filter0 = &param_2;
    filter_bind.m_Filter2 = &lStack_18;

    param_2 = reinterpret_cast<const CCharString*>(0x25);
    lStack_14 = lVar1;

    const auto* const world_overlay =
        reinterpret_cast<const CWorldMapProvider_MessageEventManagerOverlay*>(world_map_provider);
    CMessageEvent* const message =
        world_overlay->m_MessageEventManager->FindMostRecentMessage(&filter_bind);

    return message != nullptr;
}