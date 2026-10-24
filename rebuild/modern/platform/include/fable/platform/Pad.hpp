#pragma once

// The fork's game-pad model (MODERNIZATION step 3/4b). Retail reads pads through DirectInput
// (CJoystickDX): EnumDevices callback 0xAB6B10 sets DIPROP_RANGE 0..0x10000 and DIPROP_DEADZONE
// 2000 (20 %) on X and Y only, i.e. the left stick, plus a 64-event buffer; UpdateEvents 0xAB6E40
// stores each axis as 2 * raw / 65536 - 1 (so Y grows downward, as DirectInput reports it). On an
// Xbox pad DirectInput also folds both triggers into one axis. The fork reads XInput instead and
// reproduces retail's stick values (same dead zone, same signs), while reporting the triggers
// separately.

#include <cmath>
#include <cstdint>

namespace fable::platform {

// Buttons in retail's EXboxControllerButton spirit; bit positions are the fork's own.
enum PadButton : std::uint32_t {
    pad_a = 1U << 0, pad_b = 1U << 1, pad_x = 1U << 2, pad_y = 1U << 3,
    pad_left_shoulder = 1U << 4, pad_right_shoulder = 1U << 5,
    pad_back = 1U << 6, pad_start = 1U << 7,
    pad_left_thumb = 1U << 8, pad_right_thumb = 1U << 9,
    pad_dpad_up = 1U << 10, pad_dpad_down = 1U << 11, pad_dpad_left = 1U << 12, pad_dpad_right = 1U << 13,
};

struct PadState {
    bool connected = false;
    float left_x = 0, left_y = 0;     // -1..1, retail convention: +x right, +y down; 20 % dead zone
    float right_x = 0, right_y = 0;   // -1..1, no dead zone (retail sets none on the right stick)
    float left_trigger = 0, right_trigger = 0;   // 0..1, separate (retail's DirectInput path shares one axis)
    std::uint32_t buttons = 0;        // held
};

// DirectInput's axial dead zone: inside it the axis reports centre, outside it the remaining
// travel is rescaled to the full range (so the value is continuous at the dead-zone edge).
[[nodiscard]] inline float retail_dead_zone(float value, float dead_zone = 0.2f) noexcept
{
    const float magnitude = std::fabs(value);
    if (magnitude <= dead_zone) {
        return 0.0f;
    }
    const float scaled = (magnitude - dead_zone) / (1.0f - dead_zone);
    return std::copysign(scaled > 1.0f ? 1.0f : scaled, value);
}

// XInput's signed 16-bit stick value to retail's -1..1 axis.
[[nodiscard]] inline float stick_axis(std::int16_t raw) noexcept
{
    return raw < 0 ? static_cast<float>(raw) / 32768.0f : static_cast<float>(raw) / 32767.0f;
}

// Per-tick pad frame: buttons pressed at any poll since the last tick are kept, like the mouse and
// keyboard accumulators, so a quick press between two 15 Hz ticks is not lost.
struct PadFrame {
    PadState state;                 // the latest poll
    std::uint32_t pressed = 0;      // buttons that went down during the tick
};

class PadAccumulator {
public:
    void poll(const PadState& state) noexcept
    {
        frame_.pressed |= state.buttons & ~last_.buttons;
        last_ = state;
    }
    [[nodiscard]] PadFrame take() noexcept
    {
        PadFrame out = frame_;
        out.state = last_;
        frame_ = PadFrame{};
        return out;
    }

private:
    PadState last_;
    PadFrame frame_;
};

} // namespace fable::platform
