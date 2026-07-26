#include <cstddef>
#include <cstdint>

class CCharString;
class CRegion;
class CWorld;
class CWorldMap;
class CGameScriptInterface;

class CWorldMap
{
public:
    long __thiscall GetRegionNumberFromName(const CCharString& regionName);
    CRegion& __thiscall GetRegion(long regionNumber);
};

class CQuestManager
{
public:
    void __thiscall AddQuestRegion(
        const CCharString& questName,
        const CRegion* pRegion);
};

extern CQuestManager* DAT_013b89fc;

namespace
{
struct CWorldVTableOverlay
{
    std::byte m_Pad00[0x34];
    CWorldMap* (__thiscall* m_GetWorldMap)(CWorld*); // 0x34
};

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
}

class CGameScriptInterface
{
public:
    void __thiscall AddQuestRegion(
        const CCharString& questName,
        const CCharString& regionName) const;
};

void __thiscall CGameScriptInterface::AddQuestRegion(
    const CCharString& questName,
    const CCharString& regionName) const
{
    CWorld* const pWorld =
        reinterpret_cast<const CGameScriptInterfaceOverlay*>(this)->m_pWorld;

    CWorldMap* const pWorldMap =
        reinterpret_cast<CWorldOverlay*>(pWorld)->m_pVTable->m_GetWorldMap(pWorld);
    const long regionNumber = pWorldMap->GetRegionNumberFromName(regionName);
    if (regionNumber > 0)
    {
        CWorldMap* const currentWorldMap =
            reinterpret_cast<CWorldOverlay*>(pWorld)->m_pVTable->m_GetWorldMap(pWorld);
        CRegion& region = currentWorldMap->GetRegion(regionNumber);
        DAT_013b89fc->AddQuestRegion(questName, &region);
    }
}
