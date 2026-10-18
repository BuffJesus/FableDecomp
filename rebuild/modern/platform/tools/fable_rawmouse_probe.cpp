// fable_rawmouse_probe [--seconds N]
//
// Opens a window, registers Win32 Raw Input for the mouse (background delivery on), and takes one
// MouseFrame per 15 Hz "world tick" like the game loop does. At the end it prints the total
// reports and the largest frame: with retail's CMouseDX, a frame above 256 records would have
// overflowed (DI_BUFFEROVERFLOW); here every report must be counted.

#include <fable/platform/MouseAccumulator.hpp>
#include <fable/platform/RawMouseWin32.hpp>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

namespace {

fable::platform::MouseAccumulator g_mouse;
fable::platform::KeyboardAccumulator g_keyboard;

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
    if (msg == WM_INPUT) {
        if (!fable::platform::RawMouseWin32::handle(l, g_mouse)) {
            fable::platform::RawKeyboardWin32::handle(l, g_keyboard);
        }
    } else if (msg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, w, l);
}

} // namespace

int main(int argc, char** argv)
{
    double seconds = 10;
    for (int i = 1; i + 1 < argc; ++i) {
        if (std::string(argv[i]) == "--seconds") {
            seconds = std::stod(argv[++i]);
        }
    }
    WNDCLASSW wc{};
    wc.lpfnWndProc = wnd_proc;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.lpszClassName = L"FableRawMouseProbe";
    RegisterClassW(&wc);
    HWND hwnd = CreateWindowW(wc.lpszClassName, L"fable raw mouse probe", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 320, 200,
                              nullptr, nullptr, wc.hInstance, nullptr);
    if (!fable::platform::RawMouseWin32::register_for(hwnd, true) ||
        !fable::platform::RawKeyboardWin32::register_for(hwnd, true)) {
        std::cerr << "RegisterRawInputDevices failed\n";
        return 1;
    }
    using clock = std::chrono::steady_clock;
    const auto start = clock::now();
    auto next_tick = start;
    std::uint64_t total = 0, max_frame = 0, ticks = 0;
    std::int64_t sum_dx = 0;
    std::uint64_t key_events = 0, f24_taps = 0;
    MSG msg{};
    while (std::chrono::duration<double>(clock::now() - start).count() < seconds) {
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        if (clock::now() >= next_tick) {   // the 15 Hz world tick
            const auto frame = g_mouse.take();
            total += frame.reports;
            max_frame = std::max<std::uint64_t>(max_frame, frame.reports);
            sum_dx += frame.dx;
            const auto keys = g_keyboard.take();
            key_events += keys.events.size();
            for (const auto& e : keys.events) {
                f24_taps += (e.key == 0x87 && e.down) ? 1 : 0;   // VK_F24, used by the injector
            }
            ++ticks;
            next_tick += std::chrono::microseconds(66667);
        }
        MsgWaitForMultipleObjects(0, nullptr, FALSE, 1, QS_ALLINPUT);
    }
    std::cout << "RAWMOUSE ticks=" << ticks << " reports=" << total << " max_reports_per_tick=" << max_frame
              << " net_dx=" << sum_dx << " key_events=" << key_events << " f24_taps=" << f24_taps << '\n';
    return 0;
}
