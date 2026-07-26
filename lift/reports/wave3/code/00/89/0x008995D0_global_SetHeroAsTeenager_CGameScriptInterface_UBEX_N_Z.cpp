#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;
class CTCHeroMorph;
class CTCHeroStats;

enum ETCInterfaceType : std::int32_t
{
    ETCInterfaceType_HeroMorph = 3,
    ETCInterfaceType_HeroStats = 4,
};

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

class CTCHeroMorph
{
public:
    void __thiscall SetAsTeenager(bool isTeenager);
};

class CTCHeroStats
{
public:
    void __thiscall ForceStaminaTo(float stamina);
    void __thiscall SoberUp();
};

class CGameScriptInterface
{
public:
    void __thiscall SetHeroAsTeenager(bool isTeenager) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_SetHeroAsTeenager_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};

static_assert(
    offsetof(
        CGameScriptInterface_SetHeroAsTeenager_SelfOverlay,
        m_Field14) == 0x14);

struct CGameScriptInterface_SetHeroAsTeenager_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};

static_assert(
    offsetof(
        CGameScriptInterface_SetHeroAsTeenager_TCInterfaceEntryOverlay,
        m_Key) == 0x00);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroAsTeenager_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_SetHeroAsTeenager_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20; // 0x20
    std::byte m_Pad21[0x23];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_SetHeroAsTeenager_TCInterfaceEntryOverlay*
        m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(
    offsetof(
        CGameScriptInterface_SetHeroAsTeenager_TargetOverlay,
        m_Flags20) == 0x20);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroAsTeenager_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroAsTeenager_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroAsTeenager_TargetOverlay,
        m_Flags91) == 0x91);

struct SetHeroAsTeenager_BoolArgOverlay
{
    bool m_Value;              // 0x00
    std::byte m_Pad01[0x03];
};

static_assert(offsetof(SetHeroAsTeenager_BoolArgOverlay, m_Value) == 0x00);
static_assert(sizeof(SetHeroAsTeenager_BoolArgOverlay) == 0x04);

union SetHeroAsTeenager_SecondArgStorage
{
    std::uint32_t m_Raw;
    ETCInterfaceType m_InterfaceType;
};

static_assert(offsetof(SetHeroAsTeenager_SecondArgStorage, m_Raw) == 0x00);
static_assert(
    offsetof(SetHeroAsTeenager_SecondArgStorage, m_InterfaceType) == 0x00);
static_assert(sizeof(SetHeroAsTeenager_SecondArgStorage) == 0x04);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DC0 =
    CGameScriptInterface_SetHeroAsTeenager_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DC0 CALL_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);
}

void __thiscall CGameScriptInterface::SetHeroAsTeenager(bool isTeenager) const
{
    const auto* const self =
        reinterpret_cast<
            const CGameScriptInterface_SetHeroAsTeenager_SelfOverlay*>(this);
    const auto* const argOverlay =
        reinterpret_cast<const SetHeroAsTeenager_BoolArgOverlay*>(&isTeenager);
    auto* const target =
        CALL_00487DC0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        SetHeroAsTeenager_SecondArgStorage secondArgStorage{};
        secondArgStorage.m_Raw =
            *reinterpret_cast<const std::uint32_t*>(argOverlay);

        CTCHeroMorph* pHeroMorph =
            reinterpret_cast<CTCHeroMorph*>(
                static_cast<std::uintptr_t>(secondArgStorage.m_Raw));

        if ((target->m_Flags20 & 0x08) != 0)
        {
            ETCInterfaceType interfaceType = ETCInterfaceType_HeroMorph;

            auto* pEntry =
                reinterpret_cast<
                    CGameScriptInterface_SetHeroAsTeenager_TCInterfaceEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(
                            target->m_TCInterfaceMapStorage),
                        &interfaceType));

            if ((pEntry == target->m_pTCInterfaceMapEnd) ||
                (ETCInterfaceType_HeroMorph < pEntry->m_Key))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            pHeroMorph = reinterpret_cast<CTCHeroMorph*>(pEntry->m_pInterface);
        }

        pHeroMorph->SetAsTeenager(argOverlay->m_Value);

        if ((target->m_Flags20 & 0x10) != 0)
        {
            secondArgStorage.m_InterfaceType = ETCInterfaceType_HeroStats;

            auto* pEntry =
                reinterpret_cast<
                    CGameScriptInterface_SetHeroAsTeenager_TCInterfaceEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(
                            target->m_TCInterfaceMapStorage),
                        &secondArgStorage.m_InterfaceType));

            if ((pEntry == target->m_pTCInterfaceMapEnd) ||
                (ETCInterfaceType_HeroStats < pEntry->m_Key))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            auto* const pHeroStats =
                reinterpret_cast<CTCHeroStats*>(pEntry->m_pInterface);
            pHeroStats->ForceStaminaTo(1.0f);
            pHeroStats->SoberUp();
        }
    }
}