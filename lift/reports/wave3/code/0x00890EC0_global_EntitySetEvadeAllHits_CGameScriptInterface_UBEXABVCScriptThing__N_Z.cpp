#include <cstddef>
#include <cstdint>

class CScriptThing;

class CGameScriptInterface
{
public:
    void __thiscall EntitySetEvadeAllHits(const CScriptThing& entity, bool enabled) const;
};

namespace
{
using EntitySetEvadeAllHits_TargetAccessorFn = void* (__thiscall*)(const CGameScriptInterface*);

struct CGameScriptInterface_VTable_Overlay
{
    std::byte m_Pad00[0x2C];
    EntitySetEvadeAllHits_TargetAccessorFn m_Unknown2C; // 0x2C
};
static_assert(offsetof(CGameScriptInterface_VTable_Overlay, m_Unknown2C) == 0x2C);

struct EntitySetEvadeAllHits_Target_Overlay
{
    std::byte m_Pad00[0x6C];
    std::uint8_t m_Byte6C;  // 0x6C
    std::byte m_Pad6D[0x91 - 0x6D];
    std::uint8_t m_Byte91;  // 0x91
    std::byte m_Pad92[0xBC - 0x92];
    std::uint8_t m_ByteBC;  // 0xBC
};
static_assert(offsetof(EntitySetEvadeAllHits_Target_Overlay, m_Byte6C) == 0x6C);
static_assert(offsetof(EntitySetEvadeAllHits_Target_Overlay, m_Byte91) == 0x91);
static_assert(offsetof(EntitySetEvadeAllHits_Target_Overlay, m_ByteBC) == 0xBC);
}

void __thiscall CGameScriptInterface::EntitySetEvadeAllHits(const CScriptThing& entity, bool enabled) const
{
    (void)entity;

    const auto* const vtable =
        *reinterpret_cast<CGameScriptInterface_VTable_Overlay* const*>(this);
    auto* const target =
        static_cast<EntitySetEvadeAllHits_Target_Overlay*>(vtable->m_Unknown2C(this));

    if ((target != nullptr) &&
        ((target->m_Byte91 & 0x01u) == 0) &&
        ((target->m_Byte6C & 0x02u) != 0))
    {
        target->m_ByteBC =
            target->m_ByteBC ^ (((static_cast<std::uint8_t>(enabled) << 3) ^ target->m_ByteBC) & 0x08u);
    }
}