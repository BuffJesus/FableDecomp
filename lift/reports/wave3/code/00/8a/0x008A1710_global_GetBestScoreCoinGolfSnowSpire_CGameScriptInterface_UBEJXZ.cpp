#include <cstddef>
#include <cstdint>

extern "C" std::int32_t __cdecl __ftol2();

class CGameScriptInterface;
class CTCBase;

enum ETCInterfaceType : std::int32_t;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    void* __thiscall LowerBound(const TKey* pKey);
};

class CGameScriptInterface
{
public:
    long __thiscall GetBestScoreCoinGolfSnowSpire() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_GetBestScoreCoinGolfSnowSpire_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(
        CGameScriptInterface_GetBestScoreCoinGolfSnowSpire_SelfOverlay,
        m_Field14) == 0x14);

struct CGameScriptInterface_GetBestScoreCoinGolfSnowSpire_TCInterfaceEntryOverlay
{
    ETCInterfaceType m_Key; // 0x00
    CTCBase* m_pInterface;  // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_GetBestScoreCoinGolfSnowSpire_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_GetBestScoreCoinGolfSnowSpire_TCInterfaceMapOverlay
{
    CGameScriptInterface_GetBestScoreCoinGolfSnowSpire_TCInterfaceEntryOverlay* m_pBegin; // 0x00
    CGameScriptInterface_GetBestScoreCoinGolfSnowSpire_TCInterfaceEntryOverlay* m_pEnd;   // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_GetBestScoreCoinGolfSnowSpire_TCInterfaceMapOverlay,
        m_pEnd) == 0x04);

struct CGameScriptInterface_GetBestScoreCoinGolfSnowSpire_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20; // 0x20
    std::byte m_Pad21[0x23];
    CGameScriptInterface_GetBestScoreCoinGolfSnowSpire_TCInterfaceMapOverlay m_TCInterfaceMap; // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(
        CGameScriptInterface_GetBestScoreCoinGolfSnowSpire_TargetOverlay,
        m_Flags20) == 0x20);
static_assert(
    offsetof(
        CGameScriptInterface_GetBestScoreCoinGolfSnowSpire_TargetOverlay,
        m_TCInterfaceMap) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_GetBestScoreCoinGolfSnowSpire_TargetOverlay,
        m_Flags91) == 0x91);

struct CGameScriptInterface_GetBestScoreCoinGolfSnowSpire_InterfaceOverlay
{
    std::byte m_Pad00[0x188];
    float m_BestScoreCoinGolfSnowSpire; // 0x188
};
static_assert(
    offsetof(
        CGameScriptInterface_GetBestScoreCoinGolfSnowSpire_InterfaceOverlay,
        m_BestScoreCoinGolfSnowSpire) == 0x188);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DD0 =
    CGameScriptInterface_GetBestScoreCoinGolfSnowSpire_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DD0 CALL_00487DD0 =
    reinterpret_cast<Fn_00487DD0>(0x00487DD0);

__forceinline long CallFtol2FromInterfaceScore(
    const CGameScriptInterface_GetBestScoreCoinGolfSnowSpire_InterfaceOverlay* pInterface)
{
    __asm
    {
        mov eax, pInterface
        fld dword ptr [eax + 0x188]
        call __ftol2
    }
}
}

long __thiscall CGameScriptInterface::GetBestScoreCoinGolfSnowSpire() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_GetBestScoreCoinGolfSnowSpire_SelfOverlay*>(this);
    auto* const target =
        CALL_00487DD0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) &&
        ((target->m_Flags91 & 0x01) == 0) &&
        ((target->m_Flags20 & 0x10) != 0))
    {
        const ETCInterfaceType searchKey = static_cast<ETCInterfaceType>(4);

        auto* pEntry = reinterpret_cast<
            CGameScriptInterface_GetBestScoreCoinGolfSnowSpire_TCInterfaceEntryOverlay*>(
            reinterpret_cast<TCInterfaceMap*>(&target->m_TCInterfaceMap)->LowerBound(&searchKey));

        auto* const pEnd = target->m_TCInterfaceMap.m_pEnd;
        if ((pEntry == pEnd) || (static_cast<std::int32_t>(pEntry->m_Key) > 4))
        {
            pEntry = pEnd;
        }

        const auto* const pInterface =
            reinterpret_cast<
                const CGameScriptInterface_GetBestScoreCoinGolfSnowSpire_InterfaceOverlay*>(
                pEntry->m_pInterface);
        return CallFtol2FromInterfaceScore(pInterface);
    }

    return 0;
}