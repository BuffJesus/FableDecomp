#include <cstddef>
#include <cstdint>

class CScriptThing;
class CGameScriptInterface;
class CThing;

using CGameScriptInterface_VFunc2C = CThing* (__thiscall*)(const CGameScriptInterface*);
using CThing_VFuncEC = void (__thiscall*)(CThing*, const CScriptThing&, float);

struct CGameScriptInterface_VTable
{
    std::byte m_Pad0[0x2C];
    CGameScriptInterface_VFunc2C m_VFunc2C; // 0x2C
};

static_assert(offsetof(CGameScriptInterface_VTable, m_VFunc2C) == 0x2C);

struct CGameScriptInterface_VTableOverlay
{
    const CGameScriptInterface_VTable* m_VTable; // 0x00
};

static_assert(offsetof(CGameScriptInterface_VTableOverlay, m_VTable) == 0x00);
static_assert(sizeof(CGameScriptInterface_VTableOverlay) == 0x04);

struct CThing_VTable
{
    std::byte m_Pad0[0xEC];
    CThing_VFuncEC m_VFuncEC; // 0xEC
};

static_assert(offsetof(CThing_VTable, m_VFuncEC) == 0xEC);

struct CThing_VTableOverlay
{
    const CThing_VTable* m_VTable; // 0x00
};

static_assert(offsetof(CThing_VTableOverlay, m_VTable) == 0x00);
static_assert(sizeof(CThing_VTableOverlay) == 0x04);

struct CThing_ModifyThingHealthOverlay
{
    std::byte m_Pad0[0x6C];
    std::uint8_t m_InterfaceFlags; // 0x6C
    std::byte m_Pad1[0x24];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(CThing_ModifyThingHealthOverlay, m_InterfaceFlags) == 0x6C);
static_assert(offsetof(CThing_ModifyThingHealthOverlay, m_Flags91) == 0x91);
static_assert(sizeof(CThing_ModifyThingHealthOverlay) == 0x92);

void CGameScriptInterface::ModifyThingHealth(
    const CScriptThing& script_thing,
    float amount,
    bool /*unused*/) const
{
    auto* const interface_overlay =
        reinterpret_cast<const CGameScriptInterface_VTableOverlay*>(this);
    CThing* const thing = interface_overlay->m_VTable->m_VFunc2C(this);

    if (thing != nullptr) {
        auto* const thing_overlay =
            reinterpret_cast<const CThing_ModifyThingHealthOverlay*>(thing);

        if (((thing_overlay->m_Flags91 & 0x01) == 0) &&
            ((thing_overlay->m_InterfaceFlags & 0x02) != 0)) {
            auto* const thing_vtable =
                reinterpret_cast<const CThing_VTableOverlay*>(thing)->m_VTable;
            thing_vtable->m_VFuncEC(thing, script_thing, amount);
        }
    }
}