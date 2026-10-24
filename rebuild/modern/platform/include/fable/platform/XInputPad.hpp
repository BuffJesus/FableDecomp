#pragma once

#if defined(_WIN32)

#include <fable/platform/Pad.hpp>

#include <cstdint>

namespace fable::platform {

// XInput backend (xinput1_4.dll, falling back to xinput9_1_0.dll, loaded at runtime so the
// executable has no hard dependency). Polls one of the four XInput slots into retail's PadState
// convention.
class XInputPad {
public:
    [[nodiscard]] static bool available() noexcept;
    [[nodiscard]] static PadState poll(unsigned slot) noexcept;
    // Packet number of the last poll (changes whenever the pad's state changes).
    [[nodiscard]] static std::uint32_t last_packet(unsigned slot) noexcept;
};

} // namespace fable::platform

#endif
