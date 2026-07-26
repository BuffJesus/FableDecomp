#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CPlayer;

class CPlayer
{
public:
    bool __thiscall HasMode(std::int32_t mode);
};

class CGameScriptInterface
{
public:
    bool __thiscall IsHeroInTavernGame() const;
};

namespace
{
struct CGameScriptInterface_IsHeroInTavernGame_GlobalOverlay
{
    std::byte m_Pad00[0x1C];
    void* m_Field1C; // 0x1C
};
static_assert(
    offsetof(CGameScriptInterface_IsHeroInTavernGame_GlobalOverlay, m_Field1C) == 0x1C);

using CGameScriptInterface_IsHeroInTavernGame_Helper0040D959Fn =
    CPlayer*(__thiscall*)(void*);

auto* const CGameScriptInterface_IsHeroInTavernGame_Helper0040D959 =
    reinterpret_cast<CGameScriptInterface_IsHeroInTavernGame_Helper0040D959Fn>(0x0040D959);
auto* const g_CGameScriptInterface_IsHeroInTavernGame_Global =
    reinterpret_cast<CGameScriptInterface_IsHeroInTavernGame_GlobalOverlay**>(0x013B86A0);
}

bool __thiscall CGameScriptInterface::IsHeroInTavernGame() const
{
    CPlayer* const pHero =
        CGameScriptInterface_IsHeroInTavernGame_Helper0040D959(
            (*g_CGameScriptInterface_IsHeroInTavernGame_Global)->m_Field1C);

    if (pHero != nullptr)
    {
        if (!pHero->HasMode(0x1A))
        {
            return false;
        }
    }

    return true;
}