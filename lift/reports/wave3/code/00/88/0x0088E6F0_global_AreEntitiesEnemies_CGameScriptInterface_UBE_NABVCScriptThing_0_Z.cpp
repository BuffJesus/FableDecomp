#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CThing;

class CTCEnemy
{
public:
    static bool __cdecl AreThingsEnemies(CThing* thing1, CThing* thing2);
};

class CGameScriptInterface
{
public:
    bool __thiscall AreEntitiesEnemies(const CScriptThing& entity1, const CScriptThing& entity2) const;
};

namespace
{
struct CScriptThing_VTable_Overlay
{
    std::byte pad_0000[0x2C];
    CThing* (__thiscall* GetThing)(const CScriptThing* scriptThing);
    std::byte pad_0030[0xFC];
    bool (__thiscall* HasThing)(const CScriptThing* scriptThing);
};

static_assert(offsetof(CScriptThing_VTable_Overlay, GetThing) == 0x2C);
static_assert(offsetof(CScriptThing_VTable_Overlay, HasThing) == 0x12C);

struct CScriptThing_Overlay
{
    const CScriptThing_VTable_Overlay* vftable;
};

static_assert(offsetof(CScriptThing_Overlay, vftable) == 0x00);
}

bool __thiscall CGameScriptInterface::AreEntitiesEnemies(const CScriptThing& entity1, const CScriptThing& entity2) const
{
    const auto* const entity1Overlay = reinterpret_cast<const CScriptThing_Overlay*>(&entity1);
    if (!entity1Overlay->vftable->HasThing(&entity1))
    {
        return false;
    }

    const auto* const entity2Overlay = reinterpret_cast<const CScriptThing_Overlay*>(&entity2);
    if (!entity2Overlay->vftable->HasThing(&entity2))
    {
        return false;
    }

    CThing* const entity2Thing = entity2Overlay->vftable->GetThing(&entity2);
    CThing* const entity1Thing = entity1Overlay->vftable->GetThing(&entity1);
    return CTCEnemy::AreThingsEnemies(entity1Thing, entity2Thing);
}