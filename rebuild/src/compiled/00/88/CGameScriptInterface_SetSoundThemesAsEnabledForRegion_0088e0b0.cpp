#include <cstddef>

class CCharString;
class CWorld;
class CCategory;

namespace NInventory
{
class CTCInventoryBase
{
public:
    CCategory* GetCategoryWithIndex(long index);
};
}

class CWorldMap
{
public:
    long GetRegionNumberFromName(const CCharString* regionName);
};

namespace
{
struct CWorldVTableOverlay
{
    unsigned char m_Pad00[0x30];
    NInventory::CTCInventoryBase* (__fastcall* m_GetTCInventoryBase)(CWorld*); // 0x30
    CWorldMap* (__fastcall* m_GetWorldMap)(CWorld*);                          // 0x34
};

struct CWorldOverlay
{
    CWorldVTableOverlay* m_pVTable; // 0x00
};

struct CGameScriptInterfaceOverlay
{
    unsigned char m_Pad00[0x04];
    CWorld* m_pWorld; // 0x04
};

struct CCategorySoundThemesEnabledOverlay
{
    unsigned char m_Pad00[0x56];
    unsigned char m_SoundThemesEnabled; // 0x56
};
}

class CGameScriptInterface
{
public:
    void SetSoundThemesAsEnabledForRegion(const CCharString& theName, bool enabled) const;
};

void CGameScriptInterface::SetSoundThemesAsEnabledForRegion(
    const CCharString& theName,
    bool enabled) const
{
    const CGameScriptInterfaceOverlay* const self =
        reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);

    const CCharString* const pName = &theName;
    CWorld* const pWorld = self->m_pWorld;
    const long regionNumber =
        reinterpret_cast<CWorldOverlay*>(pWorld)->m_pVTable->m_GetWorldMap(pWorld)
            ->GetRegionNumberFromName(pName);
    if (regionNumber > 0)
    {
        CWorld* const pWorld2 = self->m_pWorld;
        CCategory* const pCategory =
            reinterpret_cast<CWorldOverlay*>(pWorld2)->m_pVTable->m_GetTCInventoryBase(pWorld2)
                ->GetCategoryWithIndex(regionNumber);
        reinterpret_cast<CCategorySoundThemesEnabledOverlay*>(pCategory)->m_SoundThemesEnabled =
            static_cast<unsigned char>(enabled);
    }
}