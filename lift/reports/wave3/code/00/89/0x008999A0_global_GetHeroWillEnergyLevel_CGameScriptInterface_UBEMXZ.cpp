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
    float __thiscall GetHeroWillEnergyLevel() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_GetHeroWillEnergyLevel_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
    std::byte m_Pad18[0x40];
    std::int32_t m_Field58; // 0x58
    std::int32_t m_Field5C; // 0x5C
};
static_assert(
    offsetof(CGameScriptInterface_GetHeroWillEnergyLevel_SelfOverlay, m_Field14) == 0x14);
static_assert(
    offsetof(CGameScriptInterface_GetHeroWillEnergyLevel_SelfOverlay, m_Field58) == 0x58);
static_assert(
    offsetof(CGameScriptInterface_GetHeroWillEnergyLevel_SelfOverlay, m_Field5C) == 0x5C);

struct CGameScriptInterface_GetHeroWillEnergyLevel_TCInterfaceEntryOverlay
{
    std::int32_t m_Key; // 0x00
    CTCBase* m_pValue;  // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroWillEnergyLevel_TCInterfaceEntryOverlay,
        m_pValue) == 0x04);

struct CGameScriptInterface_GetHeroWillEnergyLevel_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20; // 0x20
    std::byte m_Pad21[0x23];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_GetHeroWillEnergyLevel_TCInterfaceEntryOverlay* m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(CGameScriptInterface_GetHeroWillEnergyLevel_TargetOverlay, m_Flags20) == 0x20);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroWillEnergyLevel_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroWillEnergyLevel_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(CGameScriptInterface_GetHeroWillEnergyLevel_TargetOverlay, m_Flags91) == 0x91);

using CGameScriptInterface_GetHeroWillEnergyLevel_InterfaceOverlay =
    CGameScriptInterface_GetHeroWillEnergyLevel_SelfOverlay;

using CGameScriptInterface_GetHeroWillEnergyLevel_Helper00449970Fn =
    void* (__thiscall*)(void*);
using CGameScriptInterface_GetHeroWillEnergyLevel_Helper00487DD0Fn =
    CGameScriptInterface_GetHeroWillEnergyLevel_TargetOverlay* (__thiscall*)(void*);

inline constexpr auto CGameScriptInterface_GetHeroWillEnergyLevel_Helper00449970 =
    reinterpret_cast<CGameScriptInterface_GetHeroWillEnergyLevel_Helper00449970Fn>(0x00449970);
inline constexpr auto CGameScriptInterface_GetHeroWillEnergyLevel_Helper00487DD0 =
    reinterpret_cast<CGameScriptInterface_GetHeroWillEnergyLevel_Helper00487DD0Fn>(0x00487DD0);

inline constexpr auto DAT_0122DEDC =
    reinterpret_cast<const float*>(0x0122DEDC);
}

float __thiscall CGameScriptInterface::GetHeroWillEnergyLevel() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_GetHeroWillEnergyLevel_SelfOverlay*>(this);
    auto* const target =
        CGameScriptInterface_GetHeroWillEnergyLevel_Helper00487DD0(
            CGameScriptInterface_GetHeroWillEnergyLevel_Helper00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        if ((target->m_Flags20 & 0x10) != 0)
        {
            ETCInterfaceType searchKey = static_cast<ETCInterfaceType>(4);

            auto* pEntry =
                reinterpret_cast<
                    CGameScriptInterface_GetHeroWillEnergyLevel_TCInterfaceEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                        &searchKey));

            if ((pEntry == target->m_pTCInterfaceMapEnd) || (pEntry->m_Key > 4))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            const auto* const pInterface =
                reinterpret_cast<
                    const CGameScriptInterface_GetHeroWillEnergyLevel_InterfaceOverlay*>(
                    pEntry->m_pValue);

            return static_cast<float>(pInterface->m_Field58) /
                   static_cast<float>(pInterface->m_Field5C);
        }

        return static_cast<float>(self->m_Field58) / static_cast<float>(self->m_Field5C);
    }

    return *DAT_0122DEDC;
}