#pragma once

// Win32 Raw Input backend for MouseAccumulator. Register once for a window, then feed every
// WM_INPUT to handle(); each RAWMOUSE report goes straight into the accumulator, so the OS
// delivers every report at the device's polling rate and nothing is buffered on our side.

#if defined(_WIN32)

#include <fable/platform/KeyboardAccumulator.hpp>
#include <fable/platform/MouseAccumulator.hpp>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

namespace fable::platform {

class RawMouseWin32 {
public:
    // Registers the generic mouse (usage page 1, usage 2) for `hwnd`. `background` keeps reports
    // flowing while the window is not focused (RIDEV_INPUTSINK).
    [[nodiscard]] static bool register_for(HWND hwnd, bool background = false) noexcept;

    // Call from the window procedure for WM_INPUT (lParam = HRAWINPUT). Returns true when the
    // message was a mouse report.
    static bool handle(LPARAM lparam, MouseAccumulator& into) noexcept;
};

// The keyboard counterpart: registers usage page 1 / usage 6 and feeds KeyboardAccumulator.
class RawKeyboardWin32 {
public:
    [[nodiscard]] static bool register_for(HWND hwnd, bool background = false) noexcept;
    static bool handle(LPARAM lparam, KeyboardAccumulator& into);
};

} // namespace fable::platform

#endif
