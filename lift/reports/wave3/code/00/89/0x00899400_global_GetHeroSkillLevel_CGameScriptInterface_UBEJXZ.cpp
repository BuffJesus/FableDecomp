#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;
class CTCHeroExperience;

enum ETCInterfaceType : std::int32_t;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    void* __thiscall LowerBound(const TKey* pKey);
};

class CTCHeroExperience
{
public:
    long __thiscall GetTotalStatLevel(long statIndex) const;
};

class CGameScriptInterface
{
public:
    long __thiscall GetHeroSkillLevel() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_GetHeroSkillLevel_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroSkillLevel_SelfOverlay,
        m_Field14) == 0x14);

struct CGameScriptInterface_GetHeroSkillLevel_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroSkillLevel_TCInterfaceEntryOverlay,
        m_Key) == 0x00);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroSkillLevel_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);
static_assert(
    sizeof(CGameScriptInterface_GetHeroSkillLevel_TCInterfaceEntryOverlay) == 0x08);

struct CGameScriptInterface_GetHeroSkillLevel_TargetOverlay
{
    std::byte m_Pad00[0x2C];
    std::uint32_t m_Flags2C; // 0x2C
    std::byte m_Pad30[0x14];
    TCInterfaceMap m_TCInterfaceMap; // 0x44
    CGameScriptInterface_GetHeroSkillLevel_TCInterfaceEntryOverlay*
        m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroSkillLevel_TargetOverlay,
        m_Flags2C) == 0x2C);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroSkillLevel_TargetOverlay,
        m_TCInterfaceMap) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroSkillLevel_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroSkillLevel_TargetOverlay,
        m_Flags91) == 0x91);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DC0 =
    CGameScriptInterface_GetHeroSkillLevel_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DC0 CALL_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);
} // namespace

long __thiscall CGameScriptInterface::GetHeroSkillLevel() const
{
    const auto* const self =
        reinterpret_cast<
            const CGameScriptInterface_GetHeroSkillLevel_SelfOverlay*>(this);
    auto* const target =
        CALL_00487DC0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        const CTCHeroExperience* pHeroExperience =
            reinterpret_cast<const CTCHeroExperience*>(this);

        if ((target->m_Flags2C & 0x00000100) != 0)
        {
            std::int32_t interfaceTypeValue = 0x68;

            auto* pEntry =
                reinterpret_cast<
                    CGameScriptInterface_GetHeroSkillLevel_TCInterfaceEntryOverlay*>(
                    target->m_TCInterfaceMap.LowerBound(
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

            if ((pEntry == target->m_pTCInterfaceMapEnd) || (0x68 < pEntry->m_Key))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            pHeroExperience =
                reinterpret_cast<const CTCHeroExperience*>(pEntry->m_pInterface);
        }

        return pHeroExperience->GetTotalStatLevel(1);
    }

    return -1;
}