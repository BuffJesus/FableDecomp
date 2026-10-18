// fable_pad_probe [--seconds N]
//
// Polls the four XInput slots at about 1 kHz and takes one PadFrame per 15 Hz tick, like the game
// loop would. Prints which slots are connected, the resting stick values after retail's dead zone,
// and every tick where something changed (buttons pressed, sticks moved, triggers).

#include <fable/platform/Pad.hpp>
#include <fable/platform/XInputPad.hpp>

#include <chrono>
#include <cmath>
#include <cstdio>
#include <string>
#include <thread>

int main(int argc, char** argv)
{
    double seconds = 10;
    for (int i = 1; i + 1 < argc; ++i) {
        if (std::string(argv[i]) == "--seconds") {
            seconds = std::stod(argv[++i]);
        }
    }
    if (!fable::platform::XInputPad::available()) {
        std::printf("PAD no XInput runtime\n");
        return 1;
    }
    fable::platform::PadAccumulator pads[4];
    for (unsigned s = 0; s < 4; ++s) {
        const auto st = fable::platform::XInputPad::poll(s);
        std::printf("PAD slot=%u connected=%d left=(%.3f,%.3f) right=(%.3f,%.3f) triggers=(%.2f,%.2f) buttons=0x%04x\n", s,
                    st.connected ? 1 : 0, st.left_x, st.left_y, st.right_x, st.right_y, st.left_trigger, st.right_trigger,
                    st.buttons);
    }
    using clock = std::chrono::steady_clock;
    const auto start = clock::now();
    auto next_tick = start;
    unsigned ticks = 0, active_ticks = 0;
    while (std::chrono::duration<double>(clock::now() - start).count() < seconds) {
        for (unsigned s = 0; s < 4; ++s) {
            pads[s].poll(fable::platform::XInputPad::poll(s));
        }
        if (clock::now() >= next_tick) {
            for (unsigned s = 0; s < 4; ++s) {
                const auto f = pads[s].take();
                const auto& st = f.state;
                const bool moved = std::fabs(st.left_x) + std::fabs(st.left_y) + std::fabs(st.right_x) + std::fabs(st.right_y) > 0.25f;
                if (st.connected && (f.pressed || moved || st.left_trigger > 0.1f || st.right_trigger > 0.1f)) {
                    ++active_ticks;
                    std::printf("TICK %u slot=%u pressed=0x%04x held=0x%04x left=(%.2f,%.2f) right=(%.2f,%.2f) lt=%.2f rt=%.2f\n", ticks,
                                s, f.pressed, st.buttons, st.left_x, st.left_y, st.right_x, st.right_y, st.left_trigger,
                                st.right_trigger);
                }
            }
            ++ticks;
            next_tick += std::chrono::microseconds(66667);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::printf("PAD done ticks=%u active_ticks=%u\n", ticks, active_ticks);
    return 0;
}
