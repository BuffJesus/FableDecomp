#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;

enum ETCInterfaceType : std::int32_t;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    static void* __thiscall LowerBound(
        CVectorMap* pThis,
        const TKey* pKey);
};

class CGameScriptInterface
{
public:
    void __thiscall SetHeroWillEnergyAsAbleToRefill(std::uint32_t param3Raw) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_SetHeroWillEnergyAsAbleToRefill_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};

static_assert(
    offsetof(
        CGameScriptInterface_SetHeroWillEnergyAsAbleToRefill_SelfOverlay,
        m_Field14) == 0x14);

struct CGameScriptInterface_SetHeroWillEnergyAsAbleToRefill_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};

static_assert(
    offsetof(
        CGameScriptInterface_SetHeroWillEnergyAsAbleToRefill_TCInterfaceEntryOverlay,
        m_Key) == 0x00);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroWillEnergyAsAbleToRefill_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_SetHeroWillEnergyAsAbleToRefill_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20; // 0x20
    std::byte m_Pad21[0x23];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_SetHeroWillEnergyAsAbleToRefill_TCInterfaceEntryOverlay*
        m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(
    offsetof(
        CGameScriptInterface_SetHeroWillEnergyAsAbleToRefill_TargetOverlay,
        m_Flags20) == 0x20);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroWillEnergyAsAbleToRefill_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroWillEnergyAsAbleToRefill_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroWillEnergyAsAbleToRefill_TargetOverlay,
        m_Flags91) == 0x91);

struct CGameScriptInterface_SetHeroWillEnergyAsAbleToRefill_RefillTargetOverlay
{
    std::byte m_Pad00[0x68];
    std::uint8_t m_IsAbleToRefill; // 0x68
};

static_assert(
    offsetof(
        CGameScriptInterface_SetHeroWillEnergyAsAbleToRefill_RefillTargetOverlay,
        m_IsAbleToRefill) == 0x68);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DC0 =
    CGameScriptInterface_SetHeroWillEnergyAsAbleToRefill_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DC0 CALL_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);
}

void __thiscall CGameScriptInterface::SetHeroWillEnergyAsAbleToRefill(
    std::uint32_t param3Raw) const
{
    const auto* const self =
        reinterpret_cast<
            const CGameScriptInterface_SetHeroWillEnergyAsAbleToRefill_SelfOverlay*>(this);
    auto* const target =
        CALL_00487DC0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        if ((target->m_Flags20 & 0x10) != 0)
        {
            std::int32_t interfaceTypeValue = 4;

            auto* pEntry =
                reinterpret_cast<
                    CGameScriptInterface_SetHeroWillEnergyAsAbleToRefill_TCInterfaceEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(
                            target->m_TCInterfaceMapStorage),
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

            if ((pEntry == target->m_pTCInterfaceMapEnd) || (4 < pEntry->m_Key))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            reinterpret_cast<
                CGameScriptInterface_SetHeroWillEnergyAsAbleToRefill_RefillTargetOverlay*>(
                pEntry->m_pInterface)
                ->m_IsAbleToRefill =
                static_cast<std::uint8_t>(
                    static_cast<std::uint8_t>(param3Raw) == static_cast<std::uint8_t>(0));
            return;
        }

        reinterpret_cast<
            CGameScriptInterface_SetHeroWillEnergyAsAbleToRefill_RefillTargetOverlay*>(
            static_cast<std::uintptr_t>(param3Raw))
            ->m_IsAbleToRefill =
            static_cast<std::uint8_t>(
                static_cast<std::uint8_t>(param3Raw) == static_cast<std::uint8_t>(0));
    }
}