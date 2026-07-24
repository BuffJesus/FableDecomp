#include <cstddef>
#include <cstdint>

class CCharString;
class CGameScriptInterface;

using CGameScriptInterface_GiveHeroWeaponSlot_t =
    void(__thiscall*)(const CGameScriptInterface* this_ptr, const CCharString& string, int unknown, bool equip_now);

struct CGameScriptInterface_vftable
{
    std::byte m_Pad0[0x1E4];
    CGameScriptInterface_GiveHeroWeaponSlot_t m_GiveHeroWeaponSlot; // 0x1E4
};

static_assert(offsetof(CGameScriptInterface_vftable, m_GiveHeroWeaponSlot) == 0x1E4);

class CGameScriptInterface
{
public:
    void GiveHeroWeapon(const CCharString& string, bool equip_now) const;
};

void CGameScriptInterface::GiveHeroWeapon(const CCharString& string, bool equip_now) const
{
    const auto* const vftable = *reinterpret_cast<CGameScriptInterface_vftable* const*>(this);
    vftable->m_GiveHeroWeaponSlot(this, string, -1, equip_now);
}