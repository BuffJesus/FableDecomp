#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    void __thiscall SetWeaponOutCrimeEnabled(bool enabled) const;
};

namespace
{
struct CGameScriptInterface_SetWeaponOutCrimeEnabled_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(
        CGameScriptInterface_SetWeaponOutCrimeEnabled_SelfOverlay,
        m_Field14) == 0x14);

struct CGameScriptInterface_SetWeaponOutCrimeEnabled_TCEntryOverlay
{
    std::int32_t m_Key;   // 0x00
    void* m_Value;        // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_SetWeaponOutCrimeEnabled_TCEntryOverlay,
        m_Value) == 0x04);

struct CGameScriptInterface_SetWeaponOutCrimeEnabled_TCBaseOverlay
{
    std::byte m_Pad00[0x2C];
    std::uint8_t m_Field2C; // 0x2C
};
static_assert(
    offsetof(
        CGameScriptInterface_SetWeaponOutCrimeEnabled_TCBaseOverlay,
        m_Field2C) == 0x2C);

struct CGameScriptInterface_SetWeaponOutCrimeEnabled_TargetOverlay
{
    std::byte m_Pad00[0x28];
    std::uint32_t m_Flags28; // 0x28
    std::byte m_Pad2C[0x18];
    CGameScriptInterface_SetWeaponOutCrimeEnabled_TCEntryOverlay* m_MapBegin44; // 0x44
    CGameScriptInterface_SetWeaponOutCrimeEnabled_TCEntryOverlay* m_MapEnd48;   // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(
        CGameScriptInterface_SetWeaponOutCrimeEnabled_TargetOverlay,
        m_Flags28) == 0x28);
static_assert(
    offsetof(
        CGameScriptInterface_SetWeaponOutCrimeEnabled_TargetOverlay,
        m_MapBegin44) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_SetWeaponOutCrimeEnabled_TargetOverlay,
        m_MapEnd48) == 0x48);
static_assert(
    offsetof(
        CGameScriptInterface_SetWeaponOutCrimeEnabled_TargetOverlay,
        m_Flags91) == 0x91);
}

void __thiscall CGameScriptInterface::SetWeaponOutCrimeEnabled(bool enabled) const
{
    auto* const self =
        reinterpret_cast<const CGameScriptInterface_SetWeaponOutCrimeEnabled_SelfOverlay*>(this);

    const auto get_field14_result =
        reinterpret_cast<void* (__thiscall*)(void*)>(0x00449970);
    const auto get_target =
        reinterpret_cast<CGameScriptInterface_SetWeaponOutCrimeEnabled_TargetOverlay* (__thiscall*)(void*)>(0x00487DC0);
    const auto lower_bound =
        reinterpret_cast<CGameScriptInterface_SetWeaponOutCrimeEnabled_TCEntryOverlay* (__thiscall*)(
            void*,
            const std::int32_t*)>(0x0040F020);

    CGameScriptInterface_SetWeaponOutCrimeEnabled_TargetOverlay* const target =
        get_target(get_field14_result(self->m_Field14));

    if (target != nullptr &&
        (target->m_Flags91 & 0x01u) == 0 &&
        (target->m_Flags28 & 0x00020000u) != 0)
    {
        std::int32_t interfaceType = 0x51;

        CGameScriptInterface_SetWeaponOutCrimeEnabled_TCEntryOverlay* entry =
            lower_bound(&target->m_MapBegin44, &interfaceType);

        if (entry == target->m_MapEnd48 || entry->m_Key > 0x51)
        {
            entry = target->m_MapEnd48;
        }

        auto* const tcBase =
            reinterpret_cast<CGameScriptInterface_SetWeaponOutCrimeEnabled_TCBaseOverlay*>(entry->m_Value);
        tcBase->m_Field2C = static_cast<std::uint8_t>(enabled);
    }
}