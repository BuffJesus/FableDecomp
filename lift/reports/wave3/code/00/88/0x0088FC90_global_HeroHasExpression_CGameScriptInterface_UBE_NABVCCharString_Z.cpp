#include <cstddef>
#include <cstdint>

class CCharString;
class CThingPlayerCreature;

class CGameScriptInterface
{
public:
    bool __thiscall HeroHasExpression(const CCharString& expression) const;
};

class CThingPlayerCreature
{
public:
    bool __thiscall IsExpressionAvailable(const CCharString& expression);
};

namespace
{
struct CGameScriptInterface_HeroHasExpression_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(CGameScriptInterface_HeroHasExpression_SelfOverlay, m_Field14) == 0x14);

struct CThingPlayerCreature_HeroHasExpression_Overlay
{
    std::byte m_Pad00[0x91];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(CThingPlayerCreature_HeroHasExpression_Overlay, m_Flags91) == 0x91);

using CGameScriptInterface_HeroHasExpression_Helper00449970Fn =
    void* (__thiscall*)(void*);
using CGameScriptInterface_HeroHasExpression_Helper00487DC1Fn =
    CThingPlayerCreature* (__thiscall*)(void*);

auto* const CGameScriptInterface_HeroHasExpression_Helper00449970 =
    reinterpret_cast<CGameScriptInterface_HeroHasExpression_Helper00449970Fn>(0x00449970);
auto* const CGameScriptInterface_HeroHasExpression_Helper00487DC1 =
    reinterpret_cast<CGameScriptInterface_HeroHasExpression_Helper00487DC1Fn>(0x00487DC1);
} // namespace

bool __thiscall CGameScriptInterface::HeroHasExpression(
    const CCharString& expression) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_HeroHasExpression_SelfOverlay*>(this);

    auto* const hero =
        CGameScriptInterface_HeroHasExpression_Helper00487DC1(
            CGameScriptInterface_HeroHasExpression_Helper00449970(self->m_Field14));

    if ((hero != nullptr) &&
        ((reinterpret_cast<const CThingPlayerCreature_HeroHasExpression_Overlay*>(hero)->m_Flags91 & 0x01) == 0))
    {
        return hero->IsExpressionAvailable(expression);
    }

    return false;
}