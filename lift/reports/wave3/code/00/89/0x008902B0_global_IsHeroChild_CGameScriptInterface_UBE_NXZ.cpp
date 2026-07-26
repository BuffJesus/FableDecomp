#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CThingCreatureBase;

class CThingCreatureBase
{
public:
    bool __thiscall IsChild();
};

class CGameScriptInterface
{
public:
    bool __thiscall IsHeroChild() const;
};

namespace
{
struct CGameScriptInterface_IsHeroChild_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(offsetof(CGameScriptInterface_IsHeroChild_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_IsHeroChild_TargetOverlay
{
    std::byte m_Pad00[0x91];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(offsetof(CGameScriptInterface_IsHeroChild_TargetOverlay, m_Flags91) == 0x91);

using CGameScriptInterface_IsHeroChild_Helper00449970Fn =
    void* (__thiscall*)(void*);
using CGameScriptInterface_IsHeroChild_Helper00487DC1Fn =
    CThingCreatureBase* (__thiscall*)(void*);

auto* const CGameScriptInterface_IsHeroChild_Helper00449970 =
    reinterpret_cast<CGameScriptInterface_IsHeroChild_Helper00449970Fn>(0x00449970);
auto* const CGameScriptInterface_IsHeroChild_Helper00487DC1 =
    reinterpret_cast<CGameScriptInterface_IsHeroChild_Helper00487DC1Fn>(0x00487DC1);
}

bool __thiscall CGameScriptInterface::IsHeroChild() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_IsHeroChild_SelfOverlay*>(this);

    auto* const target =
        CGameScriptInterface_IsHeroChild_Helper00487DC1(
            CGameScriptInterface_IsHeroChild_Helper00449970(self->m_Field14));

    if ((target != nullptr) &&
        ((reinterpret_cast<const CGameScriptInterface_IsHeroChild_TargetOverlay*>(target)->m_Flags91 & 0x01) == 0))
    {
        return target->IsChild();
    }

    return false;
}