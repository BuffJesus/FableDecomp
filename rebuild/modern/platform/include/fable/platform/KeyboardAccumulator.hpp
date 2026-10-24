#pragma once

// The fork's keyboard model, matching what retail's CKeyboardDX gives the game each world tick
// (Update 0xAB69F0): the held-key snapshot (UpdateKeyStates 0xAB5FF0, GetDeviceState of 256 keys)
// and the ordered key events since the last tick (UpdateKeyEvents 0xAB6240, buffered
// GetDeviceData), so a tap that goes down and up between two 15 Hz ticks is still seen.
// Keys are Windows virtual-key codes (0-255).

#include <bitset>
#include <cstdint>
#include <vector>

namespace fable::platform {

struct KeyEvent {
    std::uint8_t key = 0;   // virtual-key code
    bool down = false;
};

struct KeyboardFrame {
    std::bitset<256> held;            // keys down at the end of the tick
    std::bitset<256> pressed;         // keys that went down at least once during the tick
    std::vector<KeyEvent> events;     // every transition, in arrival order (auto-repeat excluded)
};

class KeyboardAccumulator {
public:
    // One OS report. Auto-repeat (a down for a key already held) is not a new event.
    void key(std::uint8_t vk, bool down)
    {
        if (down == held_.test(vk)) {
            return;
        }
        held_.set(vk, down);
        if (down) {
            frame_.pressed.set(vk);
        }
        frame_.events.push_back(KeyEvent{vk, down});
    }

    // Focus loss: everything is released (retail re-acquires the device and re-reads its state).
    void release_all()
    {
        for (unsigned vk = 0; vk < 256; ++vk) {
            if (held_.test(vk)) {
                key(static_cast<std::uint8_t>(vk), false);
            }
        }
    }

    [[nodiscard]] KeyboardFrame take()
    {
        KeyboardFrame out = std::move(frame_);
        out.held = held_;
        frame_ = KeyboardFrame{};
        return out;
    }

private:
    KeyboardFrame frame_;
    std::bitset<256> held_;
};

} // namespace fable::platform
