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
    long __thiscall GetHeroExperienceAvailableToSpend() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_GetHeroExperienceAvailableToSpend_SelfOverlay
{
    std::byte m_Pad00[0x14];
    union
    {
        void* m_Field14; // 0x14
        long m_ExperienceAvailableToSpend; // 0x14
    };
};
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroExperienceAvailableToSpend_SelfOverlay,
        m_Field14) == 0x14);

struct CGameScriptInterface_GetHeroExperienceAvailableToSpend_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroExperienceAvailableToSpend_TCInterfaceEntryOverlay,
        m_Key) == 0x00);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroExperienceAvailableToSpend_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_GetHeroExperienceAvailableToSpend_TargetOverlay
{
    std::byte m_Pad00[0x2C];
    std::uint32_t m_Flags2C; // 0x2C
    std::byte m_Pad30[0x14];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_GetHeroExperienceAvailableToSpend_TCInterfaceEntryOverlay*
        m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroExperienceAvailableToSpend_TargetOverlay,
        m_Flags2C) == 0x2C);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroExperienceAvailableToSpend_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroExperienceAvailableToSpend_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroExperienceAvailableToSpend_TargetOverlay,
        m_Flags91) == 0x91);

struct CGameScriptInterface_GetHeroExperienceAvailableToSpend_InterfaceOverlay
{
    std::byte m_Pad00[0x14];
    long m_ExperienceAvailableToSpend; // 0x14
};
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroExperienceAvailableToSpend_InterfaceOverlay,
        m_ExperienceAvailableToSpend) == 0x14);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DD4 =
    CGameScriptInterface_GetHeroExperienceAvailableToSpend_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DD4 CALL_00487DD4 =
    reinterpret_cast<Fn_00487DD4>(0x00487DD4);
}

long __thiscall CGameScriptInterface::GetHeroExperienceAvailableToSpend() const
{
    const auto* const self =
        reinterpret_cast<
            const CGameScriptInterface_GetHeroExperienceAvailableToSpend_SelfOverlay*>(this);
    auto* const target =
        CALL_00487DD4(CALL_00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        if ((target->m_Flags2C & 0x00000100) != 0)
        {
            std::int32_t interfaceTypeValue = 0x68;

            auto* pEntry =
                reinterpret_cast<
                    CGameScriptInterface_GetHeroExperienceAvailableToSpend_TCInterfaceEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

            if ((pEntry == target->m_pTCInterfaceMapEnd) || (0x68 < pEntry->m_Key))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            const auto* const pInterface =
                reinterpret_cast<
                    const CGameScriptInterface_GetHeroExperienceAvailableToSpend_InterfaceOverlay*>(
                    pEntry->m_pInterface);

            if (pInterface != nullptr)
            {
                return pInterface->m_ExperienceAvailableToSpend;
            }

            return 0;
        }

        return self->m_ExperienceAvailableToSpend;
    }

    return 0;
}