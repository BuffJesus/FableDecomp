#include <cstddef>

class CCharString;

class CGameScriptInterface
{
public:
    virtual void vf0();
    void GiveHeroWeapon(const CCharString& s, bool equipNow) const;
};

typedef void (CGameScriptInterface::*GiveHeroWeaponSlot_t)(const CCharString& s, int slot, bool equipNow) const;

struct CGameScriptInterface_vftable
{
    unsigned char pad0[0x1E4];
    void* giveHeroWeaponSlot;
};

void CGameScriptInterface::GiveHeroWeapon(const CCharString& s, bool equipNow) const
{
    const CGameScriptInterface_vftable* vft = *reinterpret_cast<CGameScriptInterface_vftable* const*>(this);
    void* raw = vft->giveHeroWeaponSlot;
    GiveHeroWeaponSlot_t fn;
    *reinterpret_cast<void**>(&fn) = raw;
    (this->*fn)(s, -1, equipNow);
}