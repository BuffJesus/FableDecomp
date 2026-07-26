#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CThing;
class CThingCreatureBase;
class CCreatureActionBase;

class CThingCreatureBase
{
public:
    void __thiscall SetCurrentAction(CCreatureActionBase* pAction);
};

class CGameScriptInterface
{
public:
    void __thiscall SheatheHeroWeapons() const;
};

namespace
{
struct CGameScriptInterface_SheatheHeroWeapons_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(CGameScriptInterface_SheatheHeroWeapons_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_SheatheHeroWeapons_TargetOverlay
{
    std::byte m_Pad00[0x91];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(CGameScriptInterface_SheatheHeroWeapons_TargetOverlay, m_Flags91) == 0x91);

struct CGameScriptInterface_SheatheHeroWeapons_ActionStorage
{
    std::byte m_Bytes[0xAC];
};
static_assert(sizeof(CGameScriptInterface_SheatheHeroWeapons_ActionStorage) == 0xAC);

using Helper00449970Fn = void* (__thiscall*)(void*);
using Helper00487DC0Fn =
    CGameScriptInterface_SheatheHeroWeapons_TargetOverlay* (__thiscall*)(void*);
using RawIsThingCarryingWeaponFn = bool (__thiscall*)(CThing*);
using RawCall00663600Fn = void (__thiscall*)(CThingCreatureBase*);
using RawCall00662650Fn = void (__thiscall*)(CThingCreatureBase*);
using RawCall0066339CFn = void (__thiscall*)(CThingCreatureBase*);
using SheatheItemToInventoryCtorFn =
    CCreatureActionBase* (__thiscall*)(
        void*,
        CThingCreatureBase*,
        CCreatureActionBase*,
        int);
using DestroyTempActionFn = void (__thiscall*)(void*);

inline constexpr auto Helper00449970 =
    reinterpret_cast<Helper00449970Fn>(0x00449970);
inline constexpr auto Helper00487DC0 =
    reinterpret_cast<Helper00487DC0Fn>(0x00487DC0);
inline constexpr auto RawIsThingCarryingWeapon =
    reinterpret_cast<RawIsThingCarryingWeaponFn>(0x006A1540);
inline constexpr auto RawCall00663600 =
    reinterpret_cast<RawCall00663600Fn>(0x00663600);
inline constexpr auto RawCall00662650 =
    reinterpret_cast<RawCall00662650Fn>(0x00662650);
inline constexpr auto RawCall0066339C =
    reinterpret_cast<RawCall0066339CFn>(0x0066339C);
inline constexpr auto SheatheItemToInventoryCtor =
    reinterpret_cast<SheatheItemToInventoryCtorFn>(0x0069FFC9);
inline constexpr auto DestroyTempAction =
    reinterpret_cast<DestroyTempActionFn>(0x0048C949);
}

void __thiscall CGameScriptInterface::SheatheHeroWeapons() const
{
    const auto* const self =
        reinterpret_cast<
            const CGameScriptInterface_SheatheHeroWeapons_SelfOverlay*>(this);
    auto* const target = Helper00487DC0(Helper00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        if (RawIsThingCarryingWeapon(reinterpret_cast<CThing*>(target)))
        {
            RawCall00663600(reinterpret_cast<CThingCreatureBase*>(target));
            RawCall00662650(reinterpret_cast<CThingCreatureBase*>(target));
            RawCall00662650(reinterpret_cast<CThingCreatureBase*>(target));
            RawCall0066339C(reinterpret_cast<CThingCreatureBase*>(target));

            CGameScriptInterface_SheatheHeroWeapons_ActionStorage actionStorage;
            auto* const pAction = SheatheItemToInventoryCtor(
                &actionStorage,
                reinterpret_cast<CThingCreatureBase*>(target),
                nullptr,
                100);
            reinterpret_cast<CThingCreatureBase*>(target)->SetCurrentAction(pAction);
            DestroyTempAction(&actionStorage);
        }
    }
}