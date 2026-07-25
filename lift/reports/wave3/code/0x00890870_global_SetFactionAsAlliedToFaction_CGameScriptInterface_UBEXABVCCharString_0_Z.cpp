#include <cstddef>

class CCharString;

void* __thiscall FactionManager_FindFactionByHash(void* factionManager, const CCharString& factionName);
void __thiscall Faction_Copy(void* faction, const void* otherFaction);

struct CGameScriptInterfaceOwnerOverlay
{
    std::byte m_Pad00[0x4];
    void* m_pOwner; // 0x4
};
static_assert(offsetof(CGameScriptInterfaceOwnerOverlay, m_pOwner) == 0x4);

struct CGameScriptInterfaceFactionManagerOverlay
{
    std::byte m_Pad00[0x54];
    void* m_pFactionManager; // 0x54
};
static_assert(offsetof(CGameScriptInterfaceFactionManagerOverlay, m_pFactionManager) == 0x54);

class CGameScriptInterface
{
public:
    void __thiscall SetFactionAsAlliedToFaction(
        const CCharString& factionName,
        const CCharString& alliedFactionName) const;
};

void __thiscall CGameScriptInterface::SetFactionAsAlliedToFaction(
    const CCharString& factionName,
    const CCharString& alliedFactionName) const
{
    const auto* const ownerOverlay =
        reinterpret_cast<const CGameScriptInterfaceOwnerOverlay*>(this);
    const auto* const factionManagerOverlay =
        reinterpret_cast<const CGameScriptInterfaceFactionManagerOverlay*>(ownerOverlay->m_pOwner);

    void* const faction = FactionManager_FindFactionByHash(
        factionManagerOverlay->m_pFactionManager,
        factionName);
    void* const alliedFaction = FactionManager_FindFactionByHash(
        factionManagerOverlay->m_pFactionManager,
        alliedFactionName);

    if ((faction != nullptr) && (alliedFaction != nullptr))
    {
        Faction_Copy(faction, alliedFaction);
        Faction_Copy(alliedFaction, faction);
    }
}