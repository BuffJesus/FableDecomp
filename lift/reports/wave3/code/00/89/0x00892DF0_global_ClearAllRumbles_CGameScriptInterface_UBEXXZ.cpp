#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CThing;

class CThing
{
public:
    long __thiscall GetJoystickDeviceNumber();
};

class CGameScriptInterface
{
public:
    void __thiscall ClearAllRumbles() const;
};

extern std::uint32_t DAT_013b8388;

namespace
{
struct CGameScriptInterface_ClearAllRumbles_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(CGameScriptInterface_ClearAllRumbles_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_ClearAllRumbles_TargetOverlay
{
    std::byte m_Pad00[0x91];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(CGameScriptInterface_ClearAllRumbles_TargetOverlay, m_Flags91) == 0x91);

struct CGameScriptInterface_ClearAllRumbles_DeviceEntryOverlay
{
    void* m_Field00;          // 0x00
    std::uint32_t m_Field04;  // 0x04
};
static_assert(
    offsetof(CGameScriptInterface_ClearAllRumbles_DeviceEntryOverlay, m_Field00) == 0x00);
static_assert(
    offsetof(CGameScriptInterface_ClearAllRumbles_DeviceEntryOverlay, m_Field04) == 0x04);
static_assert(sizeof(CGameScriptInterface_ClearAllRumbles_DeviceEntryOverlay) == 0x08);

struct CGameScriptInterface_ClearAllRumbles_GlobalOverlay
{
    std::byte m_Pad00[0x2C];
    CGameScriptInterface_ClearAllRumbles_DeviceEntryOverlay* m_pDeviceEntries; // 0x2C
};
static_assert(
    offsetof(CGameScriptInterface_ClearAllRumbles_GlobalOverlay, m_pDeviceEntries) == 0x2C);

using Helper00449970Fn = void* (__thiscall*)(void*);
using Helper00487DC0Fn =
    CGameScriptInterface_ClearAllRumbles_TargetOverlay* (__thiscall*)(void*);
using RawClearAllRumblesFn = void (__thiscall*)(void*);

inline constexpr auto Helper00449970 =
    reinterpret_cast<Helper00449970Fn>(0x00449970);
inline constexpr auto Helper00487DC0 =
    reinterpret_cast<Helper00487DC0Fn>(0x00487DC0);
}

void __thiscall CGameScriptInterface::ClearAllRumbles() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_ClearAllRumbles_SelfOverlay*>(this);
    auto* const target =
        Helper00487DC0(Helper00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        const long joystickDeviceNumber =
            reinterpret_cast<CThing*>(target)->GetJoystickDeviceNumber();

        auto* const global =
            reinterpret_cast<CGameScriptInterface_ClearAllRumbles_GlobalOverlay*>(
                DAT_013b8388);
        auto* const device =
            global->m_pDeviceEntries[joystickDeviceNumber].m_Field00;
        auto* const vftable = *reinterpret_cast<void***>(device);

        reinterpret_cast<RawClearAllRumblesFn>(vftable[0x48 / sizeof(void*)])(device);
    }
}