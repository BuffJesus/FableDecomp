#include <cstddef>
#include <cstdint>

class CCharString;
class CGameScriptInterface;

using CGameScriptInterface_GiveHeroTitleSlot_t =
    void(__thiscall*)(const CGameScriptInterface* this_ptr, const CCharString& string, int unknown, int flag);

struct CGameScriptInterface_vftable
{
    std::byte m_Pad0[0x1E4];
    CGameScriptInterface_GiveHeroTitleSlot_t m_GiveHeroTitleSlot; // 0x1E4
};

static_assert(offsetof(CGameScriptInterface_vftable, m_GiveHeroTitleSlot) == 0x1E4);

class CGameScriptInterface
{
public:
    void GiveHeroTitle(const CCharString& string) const;
};

void CGameScriptInterface::GiveHeroTitle(const CCharString& string) const
{
    const auto* const vftable = *reinterpret_cast<CGameScriptInterface_vftable* const*>(this);
    vftable->m_GiveHeroTitleSlot(this, string, -1, 1);
}