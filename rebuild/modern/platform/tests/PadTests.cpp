#include <fable/platform/Pad.hpp>

#include <cmath>
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
    using fable::platform::retail_dead_zone;
    expect(retail_dead_zone(0.19f) == 0.0f && retail_dead_zone(-0.2f) == 0.0f, "inside 20 % reads centre");
    expect(std::fabs(retail_dead_zone(0.6f) - 0.5f) < 1e-6f, "outside is rescaled: 0.6 -> 0.5");
    expect(retail_dead_zone(1.0f) == 1.0f && retail_dead_zone(-1.0f) == -1.0f, "full travel stays full");
    expect(std::fabs(retail_dead_zone(0.2001f)) < 1e-3f, "continuous at the edge");
    using fable::platform::stick_axis;
    expect(stick_axis(-32768) == -1.0f && stick_axis(32767) == 1.0f && stick_axis(0) == 0.0f, "XInput range to -1..1");
    fable::platform::PadAccumulator acc;
    fable::platform::PadState s;
    s.connected = true;
    acc.poll(s);
    s.buttons = fable::platform::pad_a;
    acc.poll(s);   // A down...
    s.buttons = 0;
    acc.poll(s);   // ...and up again before the tick
    auto f = acc.take();
    expect(f.pressed == fable::platform::pad_a && f.state.buttons == 0, "a press between ticks is kept");
    f = acc.take();
    expect(f.pressed == 0, "pressed resets each tick");
    if (failures == 0) {
        std::cout << "fable_platform.Pad: all tests passed\n";
    }
    return failures == 0 ? 0 : 1;
}
