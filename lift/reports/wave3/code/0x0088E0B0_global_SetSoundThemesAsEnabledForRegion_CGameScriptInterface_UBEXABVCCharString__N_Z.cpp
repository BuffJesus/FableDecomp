#include <cstddef>

class CCharString;
class CGameScriptInterface;
class CWorld;
class CWorldMap;
class CCategory;

namespace NInventory
{
class CTCInventoryBase
{
public:
    CCategory* __thiscall GetCategoryWithIndex(long index);
};
}

class CWorldMap
{
public:
    long __thiscall GetRegionNumberFromName(const CCharString* regionName);
};

namespace
{
struct CWorldVTableOverlay
{
    std::byte m_Pad00[0x30];
    NInventory::CTCInventoryBase* (__thiscall* m_GetTCInventoryBase)(CWorld*); // 0x30
    CWorldMap* (__thiscall* m_GetWorldMap)(CWorld*);                            // 0x34
};

static_assert(offsetof(CWorldVTableOverlay, m_GetTCInventoryBase) == 0x30);
static_assert(offsetof(CWorldVTableOverlay, m_GetWorldMap) == 0x34);

struct CWorldOverlay
{
    CWorldVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CWorldOverlay, m_pVTable) == 0x00);

struct CGameScriptInterfaceOverlay
{
    std::byte m_Pad00[0x04];
    CWorld* m_pWorld; // 0x04
};

static_assert(offsetof(CGameScriptInterfaceOverlay, m_pWorld) == 0x04);

struct CCategorySoundThemesEnabledOverlay
{
    std::byte m_Pad00[0x56];
    bool m_SoundThemesEnabled; // 0x56
};

static_assert(offsetof(CCategorySoundThemesEnabledOverlay, m_SoundThemesEnabled) == 0x56);
}

class CGameScriptInterface
{
public:
    void __thiscall SetSoundThemesAsEnabledForRegion(
        const CCharString& regionName,
        bool enabled) const;
};

void __thiscall CGameScriptInterface::SetSoundThemesAsEnabledForRegion(
    const CCharString& regionName,
    bool enabled) const
{
    const auto* const self = reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);
    CWorld* const pWorld = self->m_pWorld;

    CWorldMap* const pWorldMap =
        reinterpret_cast<CWorldOverlay*>(pWorld)->m_pVTable->m_GetWorldMap(pWorld);
    const long regionNumber = pWorldMap->GetRegionNumberFromName(&regionName);
    if (regionNumber > 0)
    {
        NInventory::CTCInventoryBase* const pInventory =
            reinterpret_cast<CWorldOverlay*>(pWorld)->m_pVTable->m_GetTCInventoryBase(pWorld);
        CCategory* const pCategory = pInventory->GetCategoryWithIndex(regionNumber);
        reinterpret_cast<CCategorySoundThemesEnabledOverlay*>(pCategory)->m_SoundThemesEnabled =
            enabled;
    }
}