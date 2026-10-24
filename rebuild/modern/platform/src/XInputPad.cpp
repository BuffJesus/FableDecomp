#include <fable/platform/XInputPad.hpp>

#if defined(_WIN32)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <xinput.h>

#include <initializer_list>

namespace fable::platform {
namespace {

using GetStateFn = DWORD(WINAPI*)(DWORD, XINPUT_STATE*);

GetStateFn get_state() noexcept
{
    static GetStateFn fn = [] {
        for (const wchar_t* dll : {L"xinput1_4.dll", L"xinput9_1_0.dll"}) {
            if (HMODULE module = LoadLibraryW(dll)) {
                if (auto f = reinterpret_cast<GetStateFn>(reinterpret_cast<void*>(GetProcAddress(module, "XInputGetState")))) {
                    return f;
                }
            }
        }
        return GetStateFn{nullptr};
    }();
    return fn;
}

std::uint32_t g_packets[XUSER_MAX_COUNT]{};

} // namespace

bool XInputPad::available() noexcept
{
    return get_state() != nullptr;
}

PadState XInputPad::poll(unsigned slot) noexcept
{
    PadState out;
    const auto fn = get_state();
    XINPUT_STATE s{};
    if (!fn || slot >= XUSER_MAX_COUNT || fn(slot, &s) != ERROR_SUCCESS) {
        return out;
    }
    g_packets[slot] = s.dwPacketNumber;
    const XINPUT_GAMEPAD& g = s.Gamepad;
    out.connected = true;
    // Retail convention: +y is down (DirectInput), XInput's +y is up.
    out.left_x = retail_dead_zone(stick_axis(g.sThumbLX));
    out.left_y = retail_dead_zone(-stick_axis(g.sThumbLY));
    out.right_x = stick_axis(g.sThumbRX);
    out.right_y = -stick_axis(g.sThumbRY);
    out.left_trigger = static_cast<float>(g.bLeftTrigger) / 255.0f;
    out.right_trigger = static_cast<float>(g.bRightTrigger) / 255.0f;
    static constexpr struct { WORD xinput; std::uint32_t pad; } map[] = {
        {XINPUT_GAMEPAD_A, pad_a}, {XINPUT_GAMEPAD_B, pad_b}, {XINPUT_GAMEPAD_X, pad_x}, {XINPUT_GAMEPAD_Y, pad_y},
        {XINPUT_GAMEPAD_LEFT_SHOULDER, pad_left_shoulder}, {XINPUT_GAMEPAD_RIGHT_SHOULDER, pad_right_shoulder},
        {XINPUT_GAMEPAD_BACK, pad_back}, {XINPUT_GAMEPAD_START, pad_start},
        {XINPUT_GAMEPAD_LEFT_THUMB, pad_left_thumb}, {XINPUT_GAMEPAD_RIGHT_THUMB, pad_right_thumb},
        {XINPUT_GAMEPAD_DPAD_UP, pad_dpad_up}, {XINPUT_GAMEPAD_DPAD_DOWN, pad_dpad_down},
        {XINPUT_GAMEPAD_DPAD_LEFT, pad_dpad_left}, {XINPUT_GAMEPAD_DPAD_RIGHT, pad_dpad_right},
    };
    for (const auto& m : map) {
        if (g.wButtons & m.xinput) {
            out.buttons |= m.pad;
        }
    }
    return out;
}

std::uint32_t XInputPad::last_packet(unsigned slot) noexcept
{
    return slot < XUSER_MAX_COUNT ? g_packets[slot] : 0;
}

} // namespace fable::platform

#endif
