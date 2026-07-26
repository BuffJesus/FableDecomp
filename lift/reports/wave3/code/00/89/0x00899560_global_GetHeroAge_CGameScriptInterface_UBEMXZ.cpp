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
    float __thiscall GetHeroAge() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_GetHeroAge_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
    std::byte m_Pad18[0x18];
    float m_HeroAge; // 0x30
};
static_assert(offsetof(CGameScriptInterface_GetHeroAge_SelfOverlay, m_Field14) == 0x14);
static_assert(offsetof(CGameScriptInterface_GetHeroAge_SelfOverlay, m_HeroAge) == 0x30);

struct CGameScriptInterface_GetHeroAge_TCInterfaceEntryOverlay
{
    std::int32_t m_Key; // 0x00
    CTCBase* m_pValue;  // 0x04
};
static_assert(offsetof(CGameScriptInterface_GetHeroAge_TCInterfaceEntryOverlay, m_pValue) == 0x04);

struct CGameScriptInterface_GetHeroAge_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20; // 0x20
    std::byte m_Pad21[0x23];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_GetHeroAge_TCInterfaceEntryOverlay* m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(offsetof(CGameScriptInterface_GetHeroAge_TargetOverlay, m_Flags20) == 0x20);
static_assert(offsetof(CGameScriptInterface_GetHeroAge_TargetOverlay, m_TCInterfaceMapStorage) == 0x44);
static_assert(offsetof(CGameScriptInterface_GetHeroAge_TargetOverlay, m_pTCInterfaceMapEnd) == 0x48);
static_assert(offsetof(CGameScriptInterface_GetHeroAge_TargetOverlay, m_Flags91) == 0x91);

struct CGameScriptInterface_GetHeroAge_InterfaceOverlay
{
    std::byte m_Pad00[0x30];
    float m_Field30; // 0x30
};
static_assert(offsetof(CGameScriptInterface_GetHeroAge_InterfaceOverlay, m_Field30) == 0x30);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DE0 = CGameScriptInterface_GetHeroAge_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DE0 CALL_00487DE0 =
    reinterpret_cast<Fn_00487DE0>(0x00487DE0);

inline constexpr auto DAT_0122DEDC =
    reinterpret_cast<const float*>(0x0122DEDC);
}

float __thiscall CGameScriptInterface::GetHeroAge() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_GetHeroAge_SelfOverlay*>(this);
    CGameScriptInterface_GetHeroAge_TargetOverlay* const target =
        CALL_00487DE0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        if ((target->m_Flags20 & 0x10) != 0)
        {
            ETCInterfaceType searchKey = static_cast<ETCInterfaceType>(4);

            auto* pEntry =
                reinterpret_cast<CGameScriptInterface_GetHeroAge_TCInterfaceEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                        &searchKey));

            if ((pEntry == target->m_pTCInterfaceMapEnd) || (pEntry->m_Key > 4))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            return reinterpret_cast<const CGameScriptInterface_GetHeroAge_InterfaceOverlay*>(
                       pEntry->m_pValue)
                ->m_Field30;
        }

        return self->m_HeroAge;
    }

    return *DAT_0122DEDC;
}