#include <cstddef>
#include <cstdint>

class CScriptThing;

class CGameScriptInterface
{
public:
    void __thiscall EntitySetNegateAllHits(const CScriptThing& entity, bool negate_all_hits) const;
};

namespace
{
using CScriptThing_GetThingFn = void* (__thiscall*)(const CScriptThing*);

struct CScriptThing_VTable_Overlay
{
    std::byte m_Pad00[0x2C];
    CScriptThing_GetThingFn m_GetThing; // 0x2C
};

static_assert(offsetof(CScriptThing_VTable_Overlay, m_GetThing) == 0x2C);

struct CThingNegateAllHits_Overlay
{
    std::byte m_Pad00[0x6C];
    std::uint8_t m_Flags6C; // 0x6C
    std::byte m_Pad6D[0x91 - 0x6D];
    std::uint8_t m_Flags91; // 0x91
    std::byte m_Pad92[0xBC - 0x92];
    std::uint8_t m_FlagsBC; // 0xBC
};

static_assert(offsetof(CThingNegateAllHits_Overlay, m_Flags6C) == 0x6C);
static_assert(offsetof(CThingNegateAllHits_Overlay, m_Flags91) == 0x91);
static_assert(offsetof(CThingNegateAllHits_Overlay, m_FlagsBC) == 0xBC);
}

void __thiscall CGameScriptInterface::EntitySetNegateAllHits(const CScriptThing& entity, bool negate_all_hits) const
{
    const auto* const vtable = *reinterpret_cast<CScriptThing_VTable_Overlay* const*>(&entity);
    void* const thing = vtable->m_GetThing(&entity);

    if (thing != nullptr)
    {
        auto* const thing_overlay = static_cast<CThingNegateAllHits_Overlay*>(thing);
        if (((thing_overlay->m_Flags91 & 0x01) == 0) && ((thing_overlay->m_Flags6C & 0x02) != 0))
        {
            thing_overlay->m_FlagsBC =
                static_cast<std::uint8_t>(
                    thing_overlay->m_FlagsBC ^
                    (((static_cast<std::uint8_t>(negate_all_hits) << 4) ^ thing_overlay->m_FlagsBC) & 0x10));
        }
    }
}