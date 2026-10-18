#include <fable/platform/RawMouseWin32.hpp>

#if defined(_WIN32)

namespace fable::platform {

bool RawMouseWin32::register_for(HWND hwnd, bool background) noexcept
{
    RAWINPUTDEVICE device{};
    device.usUsagePage = 0x01;   // generic desktop
    device.usUsage = 0x02;       // mouse
    device.dwFlags = background ? RIDEV_INPUTSINK : 0;
    device.hwndTarget = hwnd;
    return RegisterRawInputDevices(&device, 1, sizeof(device)) != FALSE;
}

bool RawMouseWin32::handle(LPARAM lparam, MouseAccumulator& into) noexcept
{
    RAWINPUT input{};
    UINT size = sizeof(input);
    if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, &input, &size, sizeof(RAWINPUTHEADER)) == static_cast<UINT>(-1) ||
        input.header.dwType != RIM_TYPEMOUSE) {
        return false;
    }
    const RAWMOUSE& m = input.data.mouse;
    if ((m.usFlags & MOUSE_MOVE_ABSOLUTE) == 0 && (m.lLastX != 0 || m.lLastY != 0)) {
        into.add_motion(m.lLastX, m.lLastY);
    }
    const USHORT f = m.usButtonFlags;
    static constexpr struct { USHORT down, up; unsigned index; } buttons[] = {
        {RI_MOUSE_BUTTON_1_DOWN, RI_MOUSE_BUTTON_1_UP, 0}, {RI_MOUSE_BUTTON_2_DOWN, RI_MOUSE_BUTTON_2_UP, 1},
        {RI_MOUSE_BUTTON_3_DOWN, RI_MOUSE_BUTTON_3_UP, 2}, {RI_MOUSE_BUTTON_4_DOWN, RI_MOUSE_BUTTON_4_UP, 3},
        {RI_MOUSE_BUTTON_5_DOWN, RI_MOUSE_BUTTON_5_UP, 4},
    };
    for (const auto& b : buttons) {
        if (f & b.down) {
            into.button(b.index, true);
        }
        if (f & b.up) {
            into.button(b.index, false);
        }
    }
    if (f & RI_MOUSE_WHEEL) {
        into.add_wheel(static_cast<SHORT>(m.usButtonData));
    }
    return true;
}

} // namespace fable::platform

#endif
