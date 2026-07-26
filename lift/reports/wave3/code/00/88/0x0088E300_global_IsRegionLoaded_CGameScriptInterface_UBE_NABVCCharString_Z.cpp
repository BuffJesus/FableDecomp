#include <cstddef>
#include <cstdint>

class CCharString;
class CIDrawEnvironment;
class CWorldMap;
class CWorld;

class CWorldMap
{
public:
    CIDrawEnvironment* __thiscall GetRegionNumberFromName(const CCharString& regionName);
};

class CWorld
{
public:
    CIDrawEnvironment* __thiscall DrawGetEnvironment();
};

struct CGameScriptInterface_WorldProvider
{
    void** vtable;
};

struct CGameScriptInterface_Layout
{
    std::uint8_t pad_0[0x4];
    CGameScriptInterface_WorldProvider* worldProvider;
};

static_assert(offsetof(CGameScriptInterface_Layout, worldProvider) == 0x4);

class CGameScriptInterface
{
public:
    bool __thiscall IsRegionLoaded(const CCharString& regionName) const;
};

namespace
{
using Fn_GetWorldObject = void* (__thiscall*)(CGameScriptInterface_WorldProvider* self);
}

bool __thiscall CGameScriptInterface::IsRegionLoaded(const CCharString& regionName) const
{
    const auto* const self = reinterpret_cast<const CGameScriptInterface_Layout*>(this);
    auto* const worldProvider = self->worldProvider;

    const auto getWorldObject =
        reinterpret_cast<Fn_GetWorldObject>(worldProvider->vtable[0x34 / sizeof(void*)]);

    CWorldMap* const worldMap = static_cast<CWorldMap*>(getWorldObject(worldProvider));
    CIDrawEnvironment* const regionEnvironment = worldMap->GetRegionNumberFromName(regionName);

    CWorld* const world = static_cast<CWorld*>(getWorldObject(worldProvider));
    CIDrawEnvironment* const currentEnvironment = world->DrawGetEnvironment();

    return regionEnvironment == currentEnvironment;
}