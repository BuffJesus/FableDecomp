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
    bool MsgIsLevelLoaded(const CCharString& level_name) const;
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

static_assert(
    offsetof(CGameScriptInterface_WorldMapProviderOverlay, m_WorldMapProvider) == 0x04);
static_assert(sizeof(CGameScriptInterface_WorldMapProviderOverlay) == 0x08);

struct CWorldMapProvider_MessageEventManagerOverlay
{
    std::byte m_Pad0[0x60];
    CMessageEventManager* m_MessageEventManager; // 0x60
};

static_assert(
    offsetof(
        CWorldMapProvider_MessageEventManagerOverlay,
        m_MessageEventManager) == 0x60);

struct CMsgIsLevelLoaded_FilterBindOverlay
{
    void* m_Filter0;       // 0x00
    long* m_Filter1;       // 0x04
    long* m_Filter2;       // 0x08
};

static_assert(offsetof(CMsgIsLevelLoaded_FilterBindOverlay, m_Filter0) == 0x00);
static_assert(offsetof(CMsgIsLevelLoaded_FilterBindOverlay, m_Filter1) == 0x04);
static_assert(offsetof(CMsgIsLevelLoaded_FilterBindOverlay, m_Filter2) == 0x08);
static_assert(sizeof(CMsgIsLevelLoaded_FilterBindOverlay) == 0x0C);

struct CMsgIsLevelLoaded_Filter2Locals
{
    long m_MapNumber;      // 0x00
    long m_MaxWorldFrame;  // 0x04
};

static_assert(offsetof(CMsgIsLevelLoaded_Filter2Locals, m_MapNumber) == 0x00);
static_assert(offsetof(CMsgIsLevelLoaded_Filter2Locals, m_MaxWorldFrame) == 0x04);
static_assert(sizeof(CMsgIsLevelLoaded_Filter2Locals) == 0x08);
}

bool CGameScriptInterface::MsgIsLevelLoaded(const CCharString& level_name) const
{
    const auto* const interface_overlay =
        reinterpret_cast<const CGameScriptInterface_WorldMapProviderOverlay*>(this);
    auto* const world_map_provider = interface_overlay->m_WorldMapProvider;

    const auto* const world_map_provider_vtable =
        *reinterpret_cast<const CWorldMapProvider_VTable* const*>(world_map_provider);
    CWorldMap* const world_map = world_map_provider_vtable->m_VFunc34(world_map_provider);

    CMsgIsLevelLoaded_Filter2Locals filter2_locals;
    filter2_locals.m_MapNumber = world_map->GetMapNumberFromScriptName(&level_name);
    if (filter2_locals.m_MapNumber == 0) {
        return false;
    }

    const long max_world_frame = GetMaxWorldFrameForMessages();
    filter2_locals.m_MaxWorldFrame = GetMaxWorldFrameForMessages();

    CCharString* filter0_slot = const_cast<CCharString*>(&level_name);
    CMsgIsLevelLoaded_FilterBindOverlay filter_bind;
    filter_bind.m_Filter1 = const_cast<long*>(&max_world_frame);
    filter_bind.m_Filter0 = &filter0_slot;
    filter_bind.m_Filter2 = &filter2_locals.m_MapNumber;
    filter0_slot = reinterpret_cast<CCharString*>(0x24);

    const auto* const world_overlay =
        reinterpret_cast<const CWorldMapProvider_MessageEventManagerOverlay*>(world_map_provider);
    CMessageEvent* const message =
        world_overlay->m_MessageEventManager->FindMostRecentMessage(&filter_bind);

    return message != nullptr;
}