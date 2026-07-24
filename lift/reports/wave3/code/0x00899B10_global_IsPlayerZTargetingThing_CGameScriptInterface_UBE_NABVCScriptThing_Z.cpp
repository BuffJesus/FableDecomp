#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CThing;
class CTCBase;

enum ETCInterfaceType : std::int32_t;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    struct CEntry
    {
        TKey m_Key;     // 0x00
        TValue m_Value; // 0x04
    };

    CEntry* __thiscall LowerBound(const TKey* pKey);

    std::byte pad_0x00[0x04];
    CEntry* field_0x04; // 0x04
};

template <typename TKey, typename TValue, typename TCompare>
static_assert(offsetof(CVectorMap<TKey, TValue, TCompare>, field_0x04) == 0x04);

using CTCInterfaceMap = CVectorMap<
    ETCInterfaceType,
    CTCBase*,
    CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

using CTCInterfaceMapEntry = CTCInterfaceMap::CEntry;

namespace
{
struct CGameScriptInterface_0x14_Overlay
{
    std::byte pad_0x00[0x14];
    void* field_0x14; // 0x14
};

static_assert(offsetof(CGameScriptInterface_0x14_Overlay, field_0x14) == 0x14);

struct CScriptThingVftableOverlay
{
    std::byte pad_0x00[0x2C];
    CThing* (__thiscall* fn_0x2C)(const CScriptThing*); // 0x2C
};

static_assert(offsetof(CScriptThingVftableOverlay, fn_0x2C) == 0x2C);

struct CTCBaseVftableOverlay
{
    std::byte pad_0x00[0x84];
    CThing* (__thiscall* fn_0x84)(CTCBase*); // 0x84
};

static_assert(offsetof(CTCBaseVftableOverlay, fn_0x84) == 0x84);

struct TargetingState_0x20A_Overlay
{
    std::byte pad_0x00[0x20A];
    std::uint8_t field_0x20A; // 0x20A
};

static_assert(offsetof(TargetingState_0x20A_Overlay, field_0x20A) == 0x20A);

struct CThing_0x20_Overlay
{
    std::byte pad_0x00[0x20];
    std::uint32_t field_0x20; // 0x20
};

static_assert(offsetof(CThing_0x20_Overlay, field_0x20) == 0x20);

struct CThing_0x44_Overlay
{
    std::byte pad_0x00[0x44];
    CTCInterfaceMap field_0x44; // 0x44
};

static_assert(offsetof(CThing_0x44_Overlay, field_0x44) == 0x44);

struct CThing_0x91_Overlay
{
    std::byte pad_0x00[0x91];
    std::uint8_t field_0x91; // 0x91
};

static_assert(offsetof(CThing_0x91_Overlay, field_0x91) == 0x91);

using Fn_00449970 = TargetingState_0x20A_Overlay* (__thiscall*)(void*);
using Fn_00487DBD = CThing* (__thiscall*)(TargetingState_0x20A_Overlay*);
}

bool __thiscall CGameScriptInterface::IsPlayerZTargetingThing(const CScriptThing& scriptThing) const
{
    const auto* const thisOverlay = reinterpret_cast<const CGameScriptInterface_0x14_Overlay*>(this);
    auto* const targetingState =
        reinterpret_cast<Fn_00449970>(0x00449970)(thisOverlay->field_0x14);

    const auto* const scriptThingVftable =
        *reinterpret_cast<const CScriptThingVftableOverlay* const*>(&scriptThing);
    CThing* const targetThing = scriptThingVftable->fn_0x2C(&scriptThing);

    if ((targetingState->field_0x20A != 0) &&
        (targetThing != nullptr) &&
        ((reinterpret_cast<const CThing_0x91_Overlay*>(targetThing)->field_0x91 & 0x01u) == 0))
    {
        CThing* const playerThing =
            reinterpret_cast<Fn_00487DBD>(0x00487DBD)(targetingState);

        if ((playerThing != nullptr) &&
            ((reinterpret_cast<const CThing_0x91_Overlay*>(playerThing)->field_0x91 & 0x01u) == 0) &&
            ((reinterpret_cast<const CThing_0x20_Overlay*>(playerThing)->field_0x20 & 0x100u) != 0))
        {
            ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(8);
            auto* const interfaceMap =
                &reinterpret_cast<CThing_0x44_Overlay*>(playerThing)->field_0x44;

            CTCInterfaceMapEntry* pEntry = interfaceMap->LowerBound(&interfaceType);
            if ((pEntry == interfaceMap->field_0x04) ||
                (static_cast<std::int32_t>(8) < static_cast<std::int32_t>(pEntry->m_Key)))
            {
                pEntry = interfaceMap->field_0x04;
            }

            CTCBase* const tcBase = pEntry->m_Value;
            const auto* const tcBaseVftable =
                *reinterpret_cast<const CTCBaseVftableOverlay* const*>(tcBase);
            if (tcBaseVftable->fn_0x84(tcBase) == targetThing)
            {
                return true;
            }
        }
    }

    return false;
}