#include <cstddef>
#include <cstdint>

class CCharString;
class CGameScriptInterface;
class CWorldMap;
class CCategory;

class CWorldMap
{
public:
    long __thiscall GetRegionNumberFromName(const CCharString* pRegionName);
};

namespace NInventory
{
class CTCInventoryBase
{
public:
    CCategory* __thiscall GetCategoryWithIndex(long index);
};
}

namespace
{
struct CGameScriptInterface_0x04_Overlay
{
    std::byte pad_0x00[0x04];
    void* field_0x04; // 0x04
};

static_assert(offsetof(CGameScriptInterface_0x04_Overlay, field_0x04) == 0x04);

struct CCategory_0x24_Overlay
{
    std::byte pad_0x00[0x24];
    void* field_0x24; // 0x24
};

static_assert(offsetof(CCategory_0x24_Overlay, field_0x24) == 0x24);

struct HenchmenRegionState_Overlay
{
    void** vfptr;          // 0x00
    std::int32_t refCount; // 0x04
    std::byte pad_0x08[0x81];
    bool isAllowed;        // 0x89
};

static_assert(offsetof(HenchmenRegionState_Overlay, vfptr) == 0x00);
static_assert(offsetof(HenchmenRegionState_Overlay, refCount) == 0x04);
static_assert(offsetof(HenchmenRegionState_Overlay, isAllowed) == 0x89);
}

class CGameScriptInterface
{
public:
    bool __thiscall IsHeroAllowedHenchmenInRegion(const CCharString& regionName) const;
};

bool __thiscall CGameScriptInterface::IsHeroAllowedHenchmenInRegion(
    const CCharString& regionName) const
{
    using GetInterfaceAtVfptr34 = void* (__thiscall*)(void*);
    using ReleaseFn = void (__thiscall*)(void*);

    const auto* const self = reinterpret_cast<const CGameScriptInterface_0x04_Overlay*>(this);
    void* const interfaceObject = self->field_0x04;

    const auto getInterface =
        reinterpret_cast<GetInterfaceAtVfptr34>((*reinterpret_cast<void***>(interfaceObject))[0x0D]);

    CWorldMap* const pWorldMap = reinterpret_cast<CWorldMap*>(getInterface(interfaceObject));
    const long regionNumber = pWorldMap->GetRegionNumberFromName(&regionName);

    if (regionNumber > 0)
    {
        auto* const pInventory =
            reinterpret_cast<NInventory::CTCInventoryBase*>(getInterface(interfaceObject));
        auto* const pCategory = pInventory->GetCategoryWithIndex(regionNumber);

        auto* const pState =
            reinterpret_cast<HenchmenRegionState_Overlay*>(
                reinterpret_cast<CCategory_0x24_Overlay*>(pCategory)->field_0x24);

        if (pState != nullptr)
        {
            const std::int32_t refCount = pState->refCount;
            pState->refCount = refCount + 1;

            const bool isAllowed = pState->isAllowed;

            pState->refCount = refCount;
            if (refCount == 0)
            {
                const auto release =
                    reinterpret_cast<ReleaseFn>((*reinterpret_cast<void***>(pState))[1]);
                release(pState);
            }

            return isAllowed;
        }
    }

    return true;
}