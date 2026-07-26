#pragma once

#include <cstddef>
#include <cstdint>
#include <expected>
#include <span>
#include <vector>

namespace fable::multiplayer {

enum class GameEventError {
    event_id_out_of_range,
    payload_too_large,
};

// A safe, ownership-aware model of retail CGameEvent's serialized state.
//
// This is intentionally not an ABI-compatible replacement for the retail
// 0x28-byte class. Fixed-width fields preserve the wire contract, while
// std::vector and size_t make runtime storage independent of pointer width.
class GameEvent final {
public:
    using EventId = std::uint16_t;

    static constexpr EventId max_event_id = 0x7FFF;
    static constexpr std::size_t max_payload_size = 32;

    [[nodiscard]] static std::expected<GameEvent, GameEventError> create(
        EventId id,
        std::uint8_t source,
        std::span<const std::byte> payload,
        bool replacement = false);

    [[nodiscard]] EventId id() const noexcept;
    [[nodiscard]] std::uint8_t source() const noexcept;
    [[nodiscard]] bool replacement() const noexcept;
    [[nodiscard]] std::span<const std::byte> payload() const noexcept;

    friend bool operator==(const GameEvent&, const GameEvent&) = default;

private:
    GameEvent(
        EventId id,
        std::uint8_t source,
        std::span<const std::byte> payload,
        bool replacement);

    EventId id_;
    std::uint8_t source_;
    bool replacement_;
    std::vector<std::byte> payload_;
};

enum class GameEventDecodeError {
    truncated_header,
    payload_too_large,
    truncated_payload,
};

struct DecodedGameEvent {
    GameEvent event;
    std::size_t bytes_consumed;
};

class GameEventCodec final {
public:
    static constexpr std::size_t header_size = 4;
    static constexpr std::uint16_t replacement_mask = 0x8000;

    [[nodiscard]] static std::vector<std::byte> encode(const GameEvent& event);

    // Decodes the first record and reports its length so callers can safely
    // walk the retail format's concatenated event stream.
    [[nodiscard]] static std::expected<DecodedGameEvent, GameEventDecodeError>
    decode(std::span<const std::byte> bytes);
};

} // namespace fable::multiplayer
