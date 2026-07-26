#include <cstddef>
#include <cstdint>

class CTCBase;

enum ETCInterfaceType : std::int32_t;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    void* __thiscall LowerBound(const TKey* pKey);
};

class CCharString
{
public:
    CCharString(const CCharString& other);
    CCharString(const char* pText, int length);
};

static_assert(sizeof(CCharString) == 0x04);

class CGameScriptInterface
{
public:
    CCharString __thiscall GetDeathRecoveryMarkerName() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_GetDeathRecoveryMarkerName_SelfOverlay
{
    std::byte pad_00[0x14];
    void* field_14;
};

static_assert(
    offsetof(
        CGameScriptInterface_GetDeathRecoveryMarkerName_SelfOverlay,
        field_14) == 0x14);

struct TCInterfaceEntryOverlay
{
    std::int32_t key;
    CTCBase* value;
};

static_assert(offsetof(TCInterfaceEntryOverlay, key) == 0x00);
static_assert(offsetof(TCInterfaceEntryOverlay, value) == 0x04);
static_assert(sizeof(TCInterfaceEntryOverlay) == 0x08);

struct TargetOverlay
{
    std::byte pad_00[0x20];
    std::uint8_t flags_20;
    std::byte pad_21[0x23];
    std::byte tcInterfaceMap_44[0x04];
    TCInterfaceEntryOverlay* tcInterfaceMapEnd_48;
    std::byte pad_4C[0x45];
    std::uint8_t flags_91;
};

static_assert(offsetof(TargetOverlay, flags_20) == 0x20);
static_assert(offsetof(TargetOverlay, tcInterfaceMap_44) == 0x44);
static_assert(offsetof(TargetOverlay, tcInterfaceMapEnd_48) == 0x48);
static_assert(offsetof(TargetOverlay, flags_91) == 0x91);

struct DeathRecoveryInterfaceOverlay
{
    std::byte pad_00[0x170];
    CCharString markerName_170;
};

static_assert(offsetof(DeathRecoveryInterfaceOverlay, markerName_170) == 0x170);

using Helper00449970Fn = void* (__thiscall*)(void*);
using Helper00487DC0Fn = TargetOverlay* (__thiscall*)(void*);

inline auto* const Helper00449970 =
    reinterpret_cast<Helper00449970Fn>(0x00449970);
inline auto* const Helper00487DC0 =
    reinterpret_cast<Helper00487DC0Fn>(0x00487DC0);
}

CCharString __thiscall CGameScriptInterface::GetDeathRecoveryMarkerName() const
{
    const auto* const self =
        reinterpret_cast<
            const CGameScriptInterface_GetDeathRecoveryMarkerName_SelfOverlay*>(this);

    auto* const target =
        Helper00487DC0(Helper00449970(self->field_14));

    if ((target != nullptr) &&
        ((target->flags_91 & 0x01) == 0) &&
        ((target->flags_20 & 0x10) != 0))
    {
        std::int32_t interfaceTypeValue = 4;

        auto* entry =
            reinterpret_cast<TCInterfaceEntryOverlay*>(
                reinterpret_cast<TCInterfaceMap*>(target->tcInterfaceMap_44)
                    ->LowerBound(
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

        if ((entry == target->tcInterfaceMapEnd_48) || (4 < entry->key))
        {
            entry = target->tcInterfaceMapEnd_48;
        }

        return CCharString(
            reinterpret_cast<const DeathRecoveryInterfaceOverlay*>(entry->value)
                ->markerName_170);
    }

    return CCharString("", -1);
}