#include <cstddef>

enum ECreatureGroup : int;

class CWorld
{
public:
    void __thiscall SetCreatureGenerationGroupAsEnabled(
        ECreatureGroup creatureGroup,
        bool enabled);
};

class CGameScriptInterface
{
public:
    virtual void __thiscall SetCreatureGeneratorsCreatureGroupAsEnabled(
        ECreatureGroup creatureGroup,
        bool enabled) const;
};

namespace
{
struct CGameScriptInterfaceOverlay
{
    std::byte m_Pad00[0x04];
    CWorld* m_pWorld; // 0x04
};

static_assert(offsetof(CGameScriptInterfaceOverlay, m_pWorld) == 0x04);
}

void __thiscall CGameScriptInterface::SetCreatureGeneratorsCreatureGroupAsEnabled(
    ECreatureGroup creatureGroup,
    bool enabled) const
{
    reinterpret_cast<const CGameScriptInterfaceOverlay*>(this)->m_pWorld
        ->SetCreatureGenerationGroupAsEnabled(creatureGroup, enabled);
}