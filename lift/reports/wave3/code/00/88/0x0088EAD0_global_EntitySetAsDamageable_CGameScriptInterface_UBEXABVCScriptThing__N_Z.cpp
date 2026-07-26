#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;

namespace
{
struct CScriptThingVTableOverlay
{
    std::byte m_Pad00[0x2C];
    void* (__thiscall* GetThing)(const CScriptThing* pThis); // 0x2C
    std::byte m_Pad30[0x12C - 0x30];
    bool (__thiscall* Method12C)(const CScriptThing* pThis); // 0x12C
};

static_assert(offsetof(CScriptThingVTableOverlay, GetThing) == 0x2C);
static_assert(offsetof(CScriptThingVTableOverlay, Method12C) == 0x12C);

struct CScriptThingOverlay
{
    const CScriptThingVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThingOverlay, m_pVTable) == 0x00);

struct CThingDamageableOverlay
{
    std::byte m_Pad00[0x6C];
    std::uint8_t m_Byte6C;             // 0x6C
    std::byte m_Pad6D[0xBC - 0x6D];
    std::uint8_t m_ByteBC;             // 0xBC
};

static_assert(offsetof(CThingDamageableOverlay, m_Byte6C) == 0x6C);
static_assert(offsetof(CThingDamageableOverlay, m_ByteBC) == 0xBC);

struct CGameScriptInterfaceVTableOverlay
{
    std::byte m_Pad00[0x838];
    void (__thiscall* Method838)(
        const CGameScriptInterface* pThis,
        const CScriptThing& entity,
        bool damageable); // 0x838
    void (__thiscall* Method83C)(
        const CGameScriptInterface* pThis,
        const CScriptThing& entity,
        bool damageable); // 0x83C
};

static_assert(offsetof(CGameScriptInterfaceVTableOverlay, Method838) == 0x838);
static_assert(offsetof(CGameScriptInterfaceVTableOverlay, Method83C) == 0x83C);
}

void __thiscall CGameScriptInterface::EntitySetAsDamageable(
    const CScriptThing& entity,
    bool damageable) const
{
    const auto* const entityOverlay = reinterpret_cast<const CScriptThingOverlay*>(&entity);

    if (entityOverlay->m_pVTable->Method12C(&entity))
    {
        auto* const pThing = static_cast<CThingDamageableOverlay*>(
            entityOverlay->m_pVTable->GetThing(&entity));

        if ((pThing->m_Byte6C & 0x02) != 0)
        {
            pThing->m_ByteBC =
                pThing->m_ByteBC ^
                ((pThing->m_ByteBC ^ static_cast<std::uint8_t>(damageable)) & 0x01);

            const auto* const pVTable =
                *reinterpret_cast<const CGameScriptInterfaceVTableOverlay* const*>(this);

            pVTable->Method838(this, entity, damageable);
            pVTable->Method83C(this, entity, damageable);
        }
    }
}