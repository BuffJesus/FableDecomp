#include <fable/platform/MouseAccumulator.hpp>

#include <iostream>

int main()
{
    int failures = 0;
    auto expect = [&](bool ok, const char* what) {
        if (!ok) {
            std::cerr << "FAIL: " << what << '\n';
            ++failures;
        }
    };
    fable::platform::MouseAccumulator mouse;
    // An 8 kHz mouse over one 15 Hz world tick: about 533 reports, far past retail's 256 buffer.
    for (int i = 0; i < 2000; ++i) {
        mouse.add_motion(i % 2 == 0 ? 3 : -1, 1);
    }
    mouse.button(0, true);
    mouse.add_wheel(120);
    auto frame = mouse.take();
    expect(frame.reports == 2002 + 0 && frame.dx == 2000 && frame.dy == 2000, "every report is summed, none dropped");
    expect(frame.wheel == 120 && frame.buttons_down == 1 && frame.buttons_pressed == 1, "buttons and wheel");
    frame = mouse.take();
    expect(frame.reports == 0 && frame.dx == 0 && frame.buttons_down == 1 && frame.buttons_pressed == 0,
           "take() starts a fresh frame but keeps held buttons");
    mouse.button(0, false);
    mouse.button(0, true);
    mouse.button(0, false);
    frame = mouse.take();
    expect(frame.buttons_down == 0 && frame.buttons_pressed == 1, "a click inside one tick is not lost");
    if (failures == 0) {
        std::cout << "fable_platform: all tests passed\n";
    }
    return failures == 0 ? 0 : 1;
}
