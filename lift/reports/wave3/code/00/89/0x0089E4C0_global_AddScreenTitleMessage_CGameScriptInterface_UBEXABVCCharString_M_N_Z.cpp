#include <cstddef>
#include <cstdint>

class CCharString;
class CGameScriptInterface;

class CTCRegionDisplay
{
public:
    void __thiscall SetCurrentAreaName(
        const CCharString& areaName,
        float fadeTime,
        bool unknownFlag,
        bool forceImmediate,
        bool isScreenTitle);
};

enum class ETCInterfaceType : std::int32_t
{
    RegionDisplay = 0x8E,
};

struct CGameScriptInterface_Overlay
{
    std::byte pad_0000[0x14];
    void* field_0014;
};

static_assert(offsetof(CGameScriptInterface_Overlay, field_0014) == 0x14);

struct InterfaceEntryOverlay
{
    std::int32_t key;
    CTCRegionDisplay* value;
};

static_assert(offsetof(InterfaceEntryOverlay, key) == 0x0);
static_assert(offsetof(InterfaceEntryOverlay, value) == 0x4);
static_assert(sizeof(InterfaceEntryOverlay) == 0x8);

struct InterfaceOwnerOverlay
{
    std::byte pad_0000[0x30];
    std::uint32_t flags_0030;
    std::byte pad_0034[0x10];
    InterfaceEntryOverlay* interfacesBegin_0044;
    InterfaceEntryOverlay* interfacesEnd_0048;
    std::byte pad_004C[0x45];
    std::uint8_t flags_0091;
};

static_assert(offsetof(InterfaceOwnerOverlay, flags_0030) == 0x30);
static_assert(offsetof(InterfaceOwnerOverlay, interfacesBegin_0044) == 0x44);
static_assert(offsetof(InterfaceOwnerOverlay, interfacesEnd_0048) == 0x48);
static_assert(offsetof(InterfaceOwnerOverlay, flags_0091) == 0x91);
static_assert(sizeof(CTCRegionDisplay*) == 4);

extern void* __thiscall sub_00449970(void* valueAt14);
extern InterfaceOwnerOverlay* __thiscall sub_00487DD0(void* value);
extern InterfaceEntryOverlay* __thiscall CVectorMap_LowerBound_0040F000(
    void* vectorMapAt0044,
    const ETCInterfaceType* key);

class CGameScriptInterface
{
public:
    void __thiscall AddScreenTitleMessage(
        const CCharString& areaName,
        float fadeTime,
        CTCRegionDisplay* regionDisplay) const;
};

void __thiscall CGameScriptInterface::AddScreenTitleMessage(
    const CCharString& areaName,
    float fadeTime,
    CTCRegionDisplay* regionDisplay) const
{
    const auto* const self = reinterpret_cast<const CGameScriptInterface_Overlay*>(this);
    InterfaceOwnerOverlay* const interfaceOwner = sub_00487DD0(sub_00449970(self->field_0014));
    if (interfaceOwner == nullptr) {
        return;
    }

    if ((interfaceOwner->flags_0091 & 0x01) != 0) {
        return;
    }

    CTCRegionDisplay* targetDisplay = regionDisplay;
    if ((interfaceOwner->flags_0030 & 0x4000) != 0) {
        const ETCInterfaceType key = ETCInterfaceType::RegionDisplay;
        InterfaceEntryOverlay* entry =
            CVectorMap_LowerBound_0040F000(&interfaceOwner->interfacesBegin_0044, &key);

        if (entry == interfaceOwner->interfacesEnd_0048 || entry->key > 0x8E) {
            entry = interfaceOwner->interfacesEnd_0048;
        }

        targetDisplay = entry->value;
    }

    if (targetDisplay != nullptr) {
        const std::uint32_t rawParam4 =
            static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(regionDisplay));
        const bool param4LowByte = static_cast<bool>(static_cast<std::uint8_t>(rawParam4));

        targetDisplay->SetCurrentAreaName(areaName, fadeTime, param4LowByte, false, true);
    }
}