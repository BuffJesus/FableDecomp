#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;

enum ETCInterfaceType : std::int32_t;
enum EHeroTrainableStatType : std::int32_t;
enum EHeroAbility : std::int32_t;

class CGameScriptInterface
{
public:
    long __thiscall GetHeroStatLevel(EHeroTrainableStatType statType) const;
};

namespace
{
struct CGameScriptInterface_GetHeroStatLevel_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(CGameScriptInterface_GetHeroStatLevel_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_GetHeroStatLevel_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_Value;      // 0x04
};
static_assert(
    offsetof(CGameScriptInterface_GetHeroStatLevel_TCInterfaceEntryOverlay, m_Key) == 0x00);
static_assert(
    offsetof(CGameScriptInterface_GetHeroStatLevel_TCInterfaceEntryOverlay, m_Value) == 0x04);
static_assert(
    sizeof(CGameScriptInterface_GetHeroStatLevel_TCInterfaceEntryOverlay) == 0x08);

struct CGameScriptInterface_GetHeroStatLevel_TCInterfaceMapOverlay
{
    std::byte m_Field00[0x04];
    CGameScriptInterface_GetHeroStatLevel_TCInterfaceEntryOverlay* m_End; // 0x04
};
static_assert(
    offsetof(CGameScriptInterface_GetHeroStatLevel_TCInterfaceMapOverlay, m_End) == 0x04);

struct CGameScriptInterface_GetHeroStatLevel_TargetOverlay
{
    std::byte m_Pad00[0x2C];
    std::uint32_t m_Flags2C; // 0x2C
    std::byte m_Pad30[0x14];
    CGameScriptInterface_GetHeroStatLevel_TCInterfaceMapOverlay m_InterfaceMap44; // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(CGameScriptInterface_GetHeroStatLevel_TargetOverlay, m_Flags2C) == 0x2C);
static_assert(
    offsetof(CGameScriptInterface_GetHeroStatLevel_TargetOverlay, m_InterfaceMap44) == 0x44);
static_assert(
    offsetof(CGameScriptInterface_GetHeroStatLevel_TargetOverlay, m_Flags91) == 0x91);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DC0 =
    CGameScriptInterface_GetHeroStatLevel_TargetOverlay* (__thiscall*)(void*);
using Fn_00436570 =
    CGameScriptInterface_GetHeroStatLevel_TCInterfaceEntryOverlay* (__thiscall*)(
        CGameScriptInterface_GetHeroStatLevel_TCInterfaceMapOverlay*,
        const ETCInterfaceType*);
using Fn_006D6520 = long (__thiscall*)(void*, EHeroAbility);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DC0 CALL_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);
inline constexpr Fn_00436570 CALL_00436570 =
    reinterpret_cast<Fn_00436570>(0x00436570);
inline constexpr Fn_006D6520 CALL_006D6520 =
    reinterpret_cast<Fn_006D6520>(0x006D6520);
}

long __thiscall CGameScriptInterface::GetHeroStatLevel(
    EHeroTrainableStatType statType) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_GetHeroStatLevel_SelfOverlay*>(this);
    auto* const target = CALL_00487DC0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        if ((target->m_Flags2C & 0x00000100) != 0)
        {
            const ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x68);
            auto* entry = CALL_00436570(&target->m_InterfaceMap44, &interfaceType);

            if ((entry == target->m_InterfaceMap44.m_End) || (0x68 < entry->m_Key))
            {
                entry = target->m_InterfaceMap44.m_End;
            }

            return CALL_006D6520(
                entry->m_Value,
                static_cast<EHeroAbility>(statType));
        }

        return CALL_006D6520(
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(statType)),
            static_cast<EHeroAbility>(statType));
    }

    return -1;
}