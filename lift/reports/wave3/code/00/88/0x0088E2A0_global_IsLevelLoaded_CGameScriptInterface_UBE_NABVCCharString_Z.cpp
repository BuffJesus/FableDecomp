#include <cstddef>
#include <cstdint>

class CCharString;
class CGameScriptInterface;
class CWorldMap;

class CWorldMap
{
public:
    long __thiscall GetMapNumberFromScriptName(const CCharString* pMapName);
    bool __thiscall IsMapLoadedOnPlayerProximity(long mapNumber);
};

namespace
{
struct CGameScriptInterface_0x04_Overlay
{
    std::byte pad_0x00[0x04];
    void* field_0x04; // 0x04
};

static_assert(offsetof(CGameScriptInterface_0x04_Overlay, field_0x04) == 0x04);

using InterfaceGetterFn = void* (__thiscall*)(void*);

struct Field04VftableOverlay
{
    std::byte pad_0x00[0x00];
    InterfaceGetterFn fn_0x34; // 0x34
};

static_assert(offsetof(Field04VftableOverlay, fn_0x34) == 0x34);

using MapStateCheckFn = char (__thiscall*)(void*, long);

struct MapStateQueryVftableOverlay
{
    std::byte pad_0x00[0x28];
    MapStateCheckFn fn_0x28; // 0x28
};

static_assert(offsetof(MapStateQueryVftableOverlay, fn_0x28) == 0x28);

struct MapStateQueryOverlay
{
    MapStateQueryVftableOverlay* vfptr; // 0x00
};

static_assert(offsetof(MapStateQueryOverlay, vfptr) == 0x00);
static_assert(sizeof(MapStateQueryOverlay) == 0x04);
}

class CGameScriptInterface
{
public:
    int __thiscall IsLevelLoaded(const CCharString& levelName) const;
};

int __thiscall CGameScriptInterface::IsLevelLoaded(const CCharString& levelName) const
{
    const auto* const self = reinterpret_cast<const CGameScriptInterface_0x04_Overlay*>(this);

    CWorldMap* const pWorldMap =
        reinterpret_cast<CWorldMap*>(
            (*reinterpret_cast<Field04VftableOverlay**>(self->field_0x04))->fn_0x34(self->field_0x04));

    const long mapNumber = pWorldMap->GetMapNumberFromScriptName(&levelName);
    if (mapNumber == 0)
    {
        return 0;
    }

    MapStateQueryOverlay* const pMapStateQuery =
        reinterpret_cast<MapStateQueryOverlay*>(
            (*reinterpret_cast<Field04VftableOverlay**>(self->field_0x04))->fn_0x34(self->field_0x04));

    const char isPresent = pMapStateQuery->vfptr->fn_0x28(pMapStateQuery, mapNumber);
    if (isPresent != '\0')
    {
        CWorldMap* const pWorldMapAgain =
            reinterpret_cast<CWorldMap*>(
                (*reinterpret_cast<Field04VftableOverlay**>(self->field_0x04))->fn_0x34(self->field_0x04));

        if (!pWorldMapAgain->IsMapLoadedOnPlayerProximity(mapNumber))
        {
            return 1;
        }
    }

    return 0;
}