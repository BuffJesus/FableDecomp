#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;
class CTCHeroStats;

enum ETCInterfaceType : std::int32_t;
enum EMorality : std::int32_t;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    static void* __thiscall LowerBound(CVectorMap* pThis, const TKey* pKey);
};

class CTCHeroStats
{
public:
    EMorality __thiscall GetMoralityCategory() const;
};

class CGameScriptInterface
{
public:
    EMorality __thiscall GetHeroMoralityCategory() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_GetHeroMoralityCategory_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroMoralityCategory_SelfOverlay,
        m_Field14) == 0x14);

struct CGameScriptInterface_GetHeroMoralityCategory_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroMoralityCategory_TCInterfaceEntryOverlay,
        m_Key) == 0x00);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroMoralityCategory_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);
static_assert(
    sizeof(CGameScriptInterface_GetHeroMoralityCategory_TCInterfaceEntryOverlay) == 0x08);

struct CGameScriptInterface_GetHeroMoralityCategory_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20; // 0x20
    std::byte m_Pad21[0x23];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_GetHeroMoralityCategory_TCInterfaceEntryOverlay*
        m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroMoralityCategory_TargetOverlay,
        m_Flags20) == 0x20);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroMoralityCategory_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroMoralityCategory_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroMoralityCategory_TargetOverlay,
        m_Flags91) == 0x91);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DC2 =
    CGameScriptInterface_GetHeroMoralityCategory_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DC2 CALL_00487DC2 =
    reinterpret_cast<Fn_00487DC2>(0x00487DC2);
} // namespace

EMorality __thiscall CGameScriptInterface::GetHeroMoralityCategory() const
{
    const auto* const self =
        reinterpret_cast<
            const CGameScriptInterface_GetHeroMoralityCategory_SelfOverlay*>(this);
    auto* const target =
        CALL_00487DC2(CALL_00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        if ((target->m_Flags20 & 0x10) == 0)
        {
            return reinterpret_cast<const CTCHeroStats*>(this)->GetMoralityCategory();
        }

        std::int32_t interfaceTypeValue = 0x04;

        auto* pEntry =
            reinterpret_cast<
                CGameScriptInterface_GetHeroMoralityCategory_TCInterfaceEntryOverlay*>(
                TCInterfaceMap::LowerBound(
                    reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                    reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

        if ((pEntry == target->m_pTCInterfaceMapEnd) || (0x04 < pEntry->m_Key))
        {
            pEntry = target->m_pTCInterfaceMapEnd;
        }

        return reinterpret_cast<const CTCHeroStats*>(pEntry->m_pInterface)
            ->GetMoralityCategory();
    }

    return static_cast<EMorality>(3);
}