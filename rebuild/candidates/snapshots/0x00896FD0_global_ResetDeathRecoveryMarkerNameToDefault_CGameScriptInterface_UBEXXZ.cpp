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
    void* __thiscall LowerBound(const TKey* pKey);
};

class CGameScriptInterface
{
public:
    void __thiscall ResetDeathRecoveryMarkerNameToDefault() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_ResetDeathRecoveryMarkerNameToDefault_SelfOverlay
{
    std::byte pad_00[0x14];
    void* field_14;
};
static_assert(
    offsetof(
        CGameScriptInterface_ResetDeathRecoveryMarkerNameToDefault_SelfOverlay,
        field_14) == 0x14);

struct TCInterfaceEntryOverlay
{
    std::int32_t key;
    CTCBase* value;
};
static_assert(offsetof(TCInterfaceEntryOverlay, key) == 0x00);
static_assert(offsetof(TCInterfaceEntryOverlay, value) == 0x04);

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

using Helper00449970Fn = void* (__thiscall*)(void*);
using Helper00487DC0Fn = TargetOverlay* (__thiscall*)(void*);
using Call0057A5A8Fn = void (__thiscall*)(void*);

auto* const Helper00449970 =
    reinterpret_cast<Helper00449970Fn>(0x00449970);
auto* const Helper00487DC0 =
    reinterpret_cast<Helper00487DC0Fn>(0x00487DC0);
auto* const Call0057A5A8 =
    reinterpret_cast<Call0057A5A8Fn>(0x0057A5A8);
}

void __thiscall CGameScriptInterface::ResetDeathRecoveryMarkerNameToDefault() const
{
    const auto* const self =
        reinterpret_cast<
            const CGameScriptInterface_ResetDeathRecoveryMarkerNameToDefault_SelfOverlay*>(this);

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

        Call0057A5A8(entry->value);
    }
}