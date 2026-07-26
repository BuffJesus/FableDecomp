#include <fable/multiplayer/GameEvent.hpp>

#include <cassert>
#include <limits>
#include <utility>

namespace fable::multiplayer {
namespace {

// Keep byte order and cursor arithmetic in one compatibility-boundary type.
// Code that understands game events should only see the named write operations.
class EventWireWriter final {
public:
    explicit EventWireWriter(const std::size_t expected_size)
    {
        bytes_.reserve(expected_size);
    }

    void write_header(const std::uint16_t header)
    {
        write_unsigned_16_little_endian(header);
    }

    void write_source(const std::uint8_t source)
    {
        write_unsigned_8(source);
    }

    void write_payload_size(const std::size_t payload_size)
    {
        assert(payload_size <= GameEvent::max_payload_size);
        write_unsigned_8(static_cast<std::uint8_t>(payload_size));
    }

    void write_payload(const std::span<const std::byte> payload)
    {
        bytes_.insert(bytes_.end(), payload.begin(), payload.end());
    }

    [[nodiscard]] std::vector<std::byte> finish() &&
    {
        return std::move(bytes_);
    }

private:
    void write_unsigned_8(const std::uint8_t value)
    {
        bytes_.push_back(static_cast<std::byte>(value));
    }

    void write_unsigned_16_little_endian(const std::uint16_t value)
    {
        constexpr auto bits_per_byte =
            std::numeric_limits<std::uint8_t>::digits;

        write_unsigned_8(static_cast<std::uint8_t>(value));
        write_unsigned_8(static_cast<std::uint8_t>(value >> bits_per_byte));
    }

    std::vector<std::byte> bytes_;
};

class EventWireReader final {
public:
    explicit EventWireReader(const std::span<const std::byte> bytes)
        : bytes_(bytes)
    {
    }

    [[nodiscard]] std::uint16_t read_header()
    {
        return read_unsigned_16_little_endian();
    }

    [[nodiscard]] std::uint8_t read_source()
    {
        return read_unsigned_8();
    }

    [[nodiscard]] std::size_t read_payload_size()
    {
        return read_unsigned_8();
    }

    [[nodiscard]] std::span<const std::byte> read_payload(
        const std::size_t payload_size)
    {
        assert(remaining() >= payload_size);
        const auto result = bytes_.subspan(position_, payload_size);
        position_ += payload_size;
        return result;
    }

    [[nodiscard]] std::size_t remaining() const noexcept
    {
        return bytes_.size() - position_;
    }

private:
    [[nodiscard]] std::uint8_t read_unsigned_8()
    {
        assert(remaining() >= sizeof(std::uint8_t));
        return std::to_integer<std::uint8_t>(bytes_[position_++]);
    }

    [[nodiscard]] std::uint16_t read_unsigned_16_little_endian()
    {
        constexpr auto bits_per_byte =
            std::numeric_limits<std::uint8_t>::digits;

        const auto low_byte = read_unsigned_8();
        const auto high_byte = read_unsigned_8();
        return static_cast<std::uint16_t>(
            low_byte
            | (static_cast<std::uint16_t>(high_byte) << bits_per_byte));
    }

    std::span<const std::byte> bytes_;
    std::size_t position_ = 0;
};

struct EventHeader final {
    GameEvent::EventId id;
    bool replacement;
};

[[nodiscard]] std::uint16_t encode_header(const GameEvent& event)
{
    auto header = event.id();
    if (event.replacement()) {
        header = static_cast<std::uint16_t>(
            header | GameEventCodec::replacement_mask);
    }
    return header;
}

[[nodiscard]] EventHeader decode_header(const std::uint16_t wire_header)
{
    return {
        .id = static_cast<GameEvent::EventId>(
            wire_header & GameEvent::max_event_id),
        .replacement =
            (wire_header & GameEventCodec::replacement_mask) != 0,
    };
}

} // namespace

std::expected<GameEvent, GameEventError> GameEvent::create(
    const EventId id,
    const std::uint8_t source,
    const std::span<const std::byte> payload,
    const bool replacement)
{
    if (id > max_event_id) {
        return std::unexpected(GameEventError::event_id_out_of_range);
    }
    if (payload.size() > max_payload_size) {
        return std::unexpected(GameEventError::payload_too_large);
    }

    return GameEvent(id, source, payload, replacement);
}

GameEvent::GameEvent(
    const EventId id,
    const std::uint8_t source,
    const std::span<const std::byte> payload,
    const bool replacement)
    : id_(id),
      source_(source),
      replacement_(replacement),
      payload_(payload.begin(), payload.end())
{
}

GameEvent::EventId GameEvent::id() const noexcept
{
    return id_;
}

std::uint8_t GameEvent::source() const noexcept
{
    return source_;
}

bool GameEvent::replacement() const noexcept
{
    return replacement_;
}

std::span<const std::byte> GameEvent::payload() const noexcept
{
    return payload_;
}

std::vector<std::byte> GameEventCodec::encode(const GameEvent& event)
{
    EventWireWriter writer(header_size + event.payload().size());
    writer.write_header(encode_header(event));
    writer.write_source(event.source());
    writer.write_payload_size(event.payload().size());
    writer.write_payload(event.payload());
    return std::move(writer).finish();
}

std::expected<DecodedGameEvent, GameEventDecodeError>
GameEventCodec::decode(const std::span<const std::byte> bytes)
{
    if (bytes.size() < header_size) {
        return std::unexpected(GameEventDecodeError::truncated_header);
    }

    EventWireReader reader(bytes);
    const auto wire_header = reader.read_header();
    const auto source = reader.read_source();
    const auto payload_size = reader.read_payload_size();

    if (payload_size > GameEvent::max_payload_size) {
        return std::unexpected(GameEventDecodeError::payload_too_large);
    }

    if (reader.remaining() < payload_size) {
        return std::unexpected(GameEventDecodeError::truncated_payload);
    }

    const auto header = decode_header(wire_header);
    const auto payload = reader.read_payload(payload_size);

    auto event = GameEvent::create(
        header.id,
        source,
        payload,
        header.replacement);

    // Header masking and the payload bound above make construction infallible.
    if (!event) {
        return std::unexpected(GameEventDecodeError::payload_too_large);
    }

    return DecodedGameEvent{
        .event = std::move(*event),
        .bytes_consumed = header_size + payload_size,
    };
}

} // namespace fable::multiplayer
