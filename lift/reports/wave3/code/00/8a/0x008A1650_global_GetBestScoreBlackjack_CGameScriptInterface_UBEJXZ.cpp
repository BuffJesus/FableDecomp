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
    static void* __thiscall LowerBound(CVectorMap* pThis, const TKey* pKey);
};

class CGameScriptInterface
{
public:
    long __thiscall GetBestScoreBlackjack() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_GetBestScoreBlackjack_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(CGameScriptInterface_GetBestScoreBlackjack_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_GetBestScoreBlackjack_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_GetBestScoreBlackjack_TCInterfaceEntryOverlay,
        m_Key) == 0x00);
static_assert(
    offsetof(
        CGameScriptInterface_GetBestScoreBlackjack_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_GetBestScoreBlackjack_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20; // 0x20
    std::byte m_Pad21[0x23];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_GetBestScoreBlackjack_TCInterfaceEntryOverlay* m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(CGameScriptInterface_GetBestScoreBlackjack_TargetOverlay, m_Flags20) == 0x20);
static_assert(
    offsetof(
        CGameScriptInterface_GetBestScoreBlackjack_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_GetBestScoreBlackjack_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(CGameScriptInterface_GetBestScoreBlackjack_TargetOverlay, m_Flags91) == 0x91);

struct CGameScriptInterface_GetBestScoreBlackjack_InterfaceOverlay
{
    std::byte m_Pad00[0x180];
    float m_BestScoreBlackjack; // 0x180
};
static_assert(
    offsetof(
        CGameScriptInterface_GetBestScoreBlackjack_InterfaceOverlay,
        m_BestScoreBlackjack) == 0x180);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DD0 =
    CGameScriptInterface_GetBestScoreBlackjack_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DD0 CALL_00487DD0 =
    reinterpret_cast<Fn_00487DD0>(0x00487DD0);
}

long __thiscall CGameScriptInterface::GetBestScoreBlackjack() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_GetBestScoreBlackjack_SelfOverlay*>(this);
    auto* const target =
        CALL_00487DD0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) &&
        ((target->m_Flags91 & 0x01) == 0) &&
        ((target->m_Flags20 & 0x10) != 0))
    {
        ETCInterfaceType searchKey = static_cast<ETCInterfaceType>(4);

        auto* pEntry =
            reinterpret_cast<CGameScriptInterface_GetBestScoreBlackjack_TCInterfaceEntryOverlay*>(
                TCInterfaceMap::LowerBound(
                    reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                    &searchKey));

        if ((pEntry == target->m_pTCInterfaceMapEnd) || (pEntry->m_Key > 4))
        {
            pEntry = target->m_pTCInterfaceMapEnd;
        }

        long result;
        __asm
        {
            mov eax, pEntry
            mov eax, [eax + 0x04]
            fld dword ptr [eax + 0x180]
            call __ftol2
            mov result, eax
        }

        return result;
    }

    return 0;
}