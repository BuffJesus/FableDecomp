#pragma once

// The fork's mouse input model: every relative-motion report from the OS is added as it arrives,
// and the world tick takes one summed frame (MODERNIZATION step 3, input service). Retail's
// CMouseDX instead reads a 256-record DirectInput buffer once per 15 Hz world frame, which a
// 4-8 kHz mouse overflows (docs/modern/FRAME_TIMING.md "Live results"). Here there is no buffer
// to overflow: motion is folded into running sums immediately, whatever the polling rate.

#include <cstdint>

namespace fable::platform {

struct MouseFrame {
    std::int64_t dx = 0;          // summed relative motion since the previous take()
    std::int64_t dy = 0;
    std::int32_t wheel = 0;       // summed wheel delta (WHEEL_DELTA units)
    std::uint32_t buttons_down = 0;    // buttons held at the end of the frame (bit i = button i)
    std::uint32_t buttons_pressed = 0; // buttons that went down at least once during the frame
    std::uint64_t reports = 0;    // OS reports folded into this frame
};

class MouseAccumulator {
public:
    // One OS report: relative motion and the button transitions it carries.
    void add_motion(std::int32_t dx, std::int32_t dy) noexcept
    {
        frame_.dx += dx;
        frame_.dy += dy;
        ++frame_.reports;
    }
    void add_wheel(std::int32_t delta) noexcept
    {
        frame_.wheel += delta;
        ++frame_.reports;
    }
    void button(unsigned index, bool down) noexcept
    {
        if (index >= 32) {
            return;
        }
        const std::uint32_t bit = std::uint32_t{1} << index;
        if (down) {
            frame_.buttons_pressed |= bit;
            held_ |= bit;
        } else {
            held_ &= ~bit;
        }
        ++frame_.reports;
    }

    // The world tick's read: everything since the last take(), then a fresh frame that keeps the
    // held buttons.
    [[nodiscard]] MouseFrame take() noexcept
    {
        MouseFrame out = frame_;
        out.buttons_down = held_;
        frame_ = MouseFrame{};
        return out;
    }

private:
    MouseFrame frame_;
    std::uint32_t held_ = 0;
};

} // namespace fable::platform
