#include <cstddef>

class CCharString;
class CGameScriptInterface;
class CScriptThing;

class CCharString
{
public:
    CCharString(const char* string, int length);
    ~CCharString();
};

using CGameScriptInterface_Slot380_t =
    void(__thiscall*)(const CGameScriptInterface* this_ptr,
                      const CScriptThing& thing,
                      const CCharString& string,
                      const CCharString& slot);

struct CGameScriptInterface_vftable
{
    std::byte m_Pad0[0x380];
    CGameScriptInterface_Slot380_t m_Offset380; // 0x380
};

static_assert(offsetof(CGameScriptInterface_vftable, m_Offset380) == 0x380);

class CGameScriptInterface
{
public:
    void GiveThingItemInHand(const CScriptThing& thing, const CCharString& string, bool right_hand) const;
};

void CGameScriptInterface::GiveThingItemInHand(
    const CScriptThing& thing,
    const CCharString& string,
    bool right_hand) const
{
    const auto* const vftable = *reinterpret_cast<CGameScriptInterface_vftable* const*>(this);

    if (right_hand)
    {
        CCharString slot("CARRY_SLOT_RIGHT_HAND", -1);
        vftable->m_Offset380(this, thing, string, slot);
        return;
    }

    CCharString slot("CARRY_SLOT_LEFT_HAND", -1);
    vftable->m_Offset380(this, thing, string, slot);
}