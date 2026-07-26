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
    long __thiscall GetBestScoreShoveHaPenny() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_GetBestScoreShoveHaPenny_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(
        CGameScriptInterface_GetBestScoreShoveHaPenny_SelfOverlay,
        m_Field14) == 0x14);

struct CGameScriptInterface_GetBestScoreShoveHaPenny_TCInterfaceEntryOverlay
{
    ETCInterfaceType m_Key; // 0x00
    CTCBase* m_pInterface;  // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_GetBestScoreShoveHaPenny_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_GetBestScoreShoveHaPenny_TCInterfaceMapOverlay
{
    CGameScriptInterface_GetBestScoreShoveHaPenny_TCInterfaceEntryOverlay* m_pBegin; // 0x00
    CGameScriptInterface_GetBestScoreShoveHaPenny_TCInterfaceEntryOverlay* m_pEnd;   // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_GetBestScoreShoveHaPenny_TCInterfaceMapOverlay,
        m_pEnd) == 0x04);

struct CGameScriptInterface_GetBestScoreShoveHaPenny_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20; // 0x20
    std::byte m_Pad21[0x23];
    CGameScriptInterface_GetBestScoreShoveHaPenny_TCInterfaceMapOverlay m_TCInterfaceMap; // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(
        CGameScriptInterface_GetBestScoreShoveHaPenny_TargetOverlay,
        m_Flags20) == 0x20);
static_assert(
    offsetof(
        CGameScriptInterface_GetBestScoreShoveHaPenny_TargetOverlay,
        m_TCInterfaceMap) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_GetBestScoreShoveHaPenny_TargetOverlay,
        m_Flags91) == 0x91);

struct CGameScriptInterface_GetBestScoreShoveHaPenny_InterfaceOverlay
{
    std::byte m_Pad00[0x18C];
    float m_BestScoreShoveHaPenny; // 0x18C
};
static_assert(
    offsetof(
        CGameScriptInterface_GetBestScoreShoveHaPenny_InterfaceOverlay,
        m_BestScoreShoveHaPenny) == 0x18C);

using Fn_00449971 = void* (__thiscall*)(void*);
using Fn_00487DD1 =
    CGameScriptInterface_GetBestScoreShoveHaPenny_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449971 CALL_00449971 =
    reinterpret_cast<Fn_00449971>(0x00449971);
inline constexpr Fn_00487DD1 CALL_00487DD1 =
    reinterpret_cast<Fn_00487DD1>(0x00487DD1);

__forceinline long CallFtol2FromInterfaceScore(
    const CGameScriptInterface_GetBestScoreShoveHaPenny_InterfaceOverlay* pInterface)
{
    __asm
    {
        mov eax, pInterface
        fld dword ptr [eax + 0x18C]
        call __ftol2
    }
}
}

long __thiscall CGameScriptInterface::GetBestScoreShoveHaPenny() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_GetBestScoreShoveHaPenny_SelfOverlay*>(this);
    auto* const target =
        CALL_00487DD1(CALL_00449971(self->m_Field14));

    if ((target != nullptr) &&
        ((target->m_Flags91 & 0x01) == 0) &&
        ((target->m_Flags20 & 0x10) != 0))
    {
        const ETCInterfaceType searchKey = static_cast<ETCInterfaceType>(4);

        auto* pEntry = reinterpret_cast<
            CGameScriptInterface_GetBestScoreShoveHaPenny_TCInterfaceEntryOverlay*>(
            reinterpret_cast<TCInterfaceMap*>(&target->m_TCInterfaceMap)->LowerBound(&searchKey));

        auto* const pEnd = target->m_TCInterfaceMap.m_pEnd;
        if ((pEntry == pEnd) || (static_cast<std::int32_t>(pEntry->m_Key) > 4))
        {
            pEntry = pEnd;
        }

        const auto* const pInterface =
            reinterpret_cast<
                const CGameScriptInterface_GetBestScoreShoveHaPenny_InterfaceOverlay*>(
                pEntry->m_pInterface);
        return CallFtol2FromInterfaceScore(pInterface);
    }

    return 0;
}