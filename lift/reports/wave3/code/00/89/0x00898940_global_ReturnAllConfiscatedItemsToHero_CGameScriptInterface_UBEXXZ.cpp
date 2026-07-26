#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;
class CTCHeroStats;

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

class CTCHeroStats
{
public:
    void __thiscall SetMoneyAsConfiscated(bool isConfiscated);
};

class CGameScriptInterface
{
public:
    void __thiscall ReturnAllConfiscatedItemsToHero() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_ReturnAllConfiscatedItemsToHero_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(
        CGameScriptInterface_ReturnAllConfiscatedItemsToHero_SelfOverlay,
        m_Field14) == 0x14);

struct CGameScriptInterface_ReturnAllConfiscatedItemsToHero_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_ReturnAllConfiscatedItemsToHero_TCInterfaceEntryOverlay,
        m_Key) == 0x00);
static_assert(
    offsetof(
        CGameScriptInterface_ReturnAllConfiscatedItemsToHero_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_ReturnAllConfiscatedItemsToHero_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint32_t m_Flags20; // 0x20
    std::byte m_Pad24[0x20];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_ReturnAllConfiscatedItemsToHero_TCInterfaceEntryOverlay*
        m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(
        CGameScriptInterface_ReturnAllConfiscatedItemsToHero_TargetOverlay,
        m_Flags20) == 0x20);
static_assert(
    offsetof(
        CGameScriptInterface_ReturnAllConfiscatedItemsToHero_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_ReturnAllConfiscatedItemsToHero_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(
        CGameScriptInterface_ReturnAllConfiscatedItemsToHero_TargetOverlay,
        m_Flags91) == 0x91);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DC0 =
    CGameScriptInterface_ReturnAllConfiscatedItemsToHero_TargetOverlay* (__thiscall*)(void*);
using RawInterfaceCall124Fn = void (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DC0 CALL_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);
}

void __thiscall CGameScriptInterface::ReturnAllConfiscatedItemsToHero() const
{
    const auto* const self =
        reinterpret_cast<
            const CGameScriptInterface_ReturnAllConfiscatedItemsToHero_SelfOverlay*>(this);
    auto* const target =
        CALL_00487DC0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        if ((target->m_Flags20 & 0x00020000) != 0)
        {
            std::int32_t interfaceTypeValue = 0x11;

            auto* pEntry =
                reinterpret_cast<
                    CGameScriptInterface_ReturnAllConfiscatedItemsToHero_TCInterfaceEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

            if ((pEntry == target->m_pTCInterfaceMapEnd) || (0x11 < pEntry->m_Key))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            auto* const pInterface = pEntry->m_pInterface;
            auto* const vftable = *reinterpret_cast<void***>(pInterface);
            reinterpret_cast<RawInterfaceCall124Fn>(vftable[0x124 / sizeof(void*)])(pInterface);
        }

        if ((target->m_Flags20 & 0x00080000) != 0)
        {
            std::int32_t interfaceTypeValue = 0x13;

            auto* pEntry =
                reinterpret_cast<
                    CGameScriptInterface_ReturnAllConfiscatedItemsToHero_TCInterfaceEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

            if ((pEntry == target->m_pTCInterfaceMapEnd) || (0x13 < pEntry->m_Key))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            auto* const pInterface = pEntry->m_pInterface;
            auto* const vftable = *reinterpret_cast<void***>(pInterface);
            reinterpret_cast<RawInterfaceCall124Fn>(vftable[0x124 / sizeof(void*)])(pInterface);
        }

        if ((target->m_Flags20 & 0x00040000) != 0)
        {
            std::int32_t interfaceTypeValue = 0x12;

            auto* pEntry =
                reinterpret_cast<
                    CGameScriptInterface_ReturnAllConfiscatedItemsToHero_TCInterfaceEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

            if ((pEntry == target->m_pTCInterfaceMapEnd) || (0x12 < pEntry->m_Key))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            auto* const pInterface = pEntry->m_pInterface;
            auto* const vftable = *reinterpret_cast<void***>(pInterface);
            reinterpret_cast<RawInterfaceCall124Fn>(vftable[0x124 / sizeof(void*)])(pInterface);
        }

        if ((target->m_Flags20 & 0x00000010) != 0)
        {
            std::int32_t interfaceTypeValue = 0x04;

            auto* pEntry =
                reinterpret_cast<
                    CGameScriptInterface_ReturnAllConfiscatedItemsToHero_TCInterfaceEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

            if ((pEntry == target->m_pTCInterfaceMapEnd) || (0x04 < pEntry->m_Key))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            reinterpret_cast<CTCHeroStats*>(pEntry->m_pInterface)->SetMoneyAsConfiscated(false);
        }
    }
}