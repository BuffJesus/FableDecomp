#include <fable/multiplayer/GameEvent.hpp>

#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <span>
#include <string_view>
#include <vector>

namespace {

using fable::multiplayer::GameEvent;
using fable::multiplayer::GameEventCodec;
using fable::multiplayer::GameEventDecodeError;
using fable::multiplayer::GameEventError;

int failures = 0;

void expect(const bool condition, const std::string_view message)
{
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        ++failures;
    }
}

template <typename Error>
void expect_error(
    const auto& result,
    const Error error,
    const std::string_view message)
{
    expect(!result && result.error() == error, message);
}

std::byte byte(const unsigned int value)
{
    return static_cast<std::byte>(value);
}

void exact_retail_wire_bytes_are_preserved()
{
    const std::array payload{byte(0x10), byte(0x20), byte(0x30)};
    const auto event = GameEvent::create(0x1234, 0x7A, payload, true);
    expect(event.has_value(), "representative event should be constructible");
    if (!event) {
        return;
    }

    const std::vector expected{
        byte(0x34), // little-endian 0x9234
        byte(0x92),
        byte(0x7A),
        byte(0x03),
        byte(0x10),
        byte(0x20),
        byte(0x30),
    };
    expect(
        GameEventCodec::encode(*event) == expected,
        "encoder should reproduce the recovered retail record");

    const auto decoded = GameEventCodec::decode(expected);
    expect(decoded.has_value(), "representative retail record should decode");
    if (decoded) {
        expect(decoded->event == *event, "decoded event should equal the source");
        expect(
            decoded->bytes_consumed == expected.size(),
            "decoder should report the exact record length");
    }
}

void empty_and_maximum_payloads_round_trip()
{
    const std::array<std::byte, 0> empty{};
    const auto empty_event = GameEvent::create(0, 0xFF, empty);
    expect(empty_event.has_value(), "empty payload should be accepted");
    if (empty_event) {
        const auto bytes = GameEventCodec::encode(*empty_event);
        expect(bytes.size() == GameEventCodec::header_size, "empty record size");
        const auto decoded = GameEventCodec::decode(bytes);
        expect(
            decoded && decoded->event == *empty_event,
            "empty record should round trip");
    }

    std::array<std::byte, GameEvent::max_payload_size> maximum{};
    for (std::size_t index = 0; index < maximum.size(); ++index) {
        maximum[index] = static_cast<std::byte>(index);
    }

    const auto maximum_event =
        GameEvent::create(GameEvent::max_event_id, 3, maximum, true);
    expect(maximum_event.has_value(), "32-byte retail payload should be accepted");
    if (maximum_event) {
        const auto bytes = GameEventCodec::encode(*maximum_event);
        const auto decoded = GameEventCodec::decode(bytes);
        expect(
            decoded && decoded->event == *maximum_event,
            "maximum-size record should round trip");
    }
}

void construction_enforces_retail_invariants()
{
    const std::array<std::byte, 1> payload{};
    expect_error(
        GameEvent::create(0x8000, 0, payload),
        GameEventError::event_id_out_of_range,
        "event identifiers must fit below the replacement bit");

    const std::array<std::byte, GameEvent::max_payload_size + 1> oversized{};
    expect_error(
        GameEvent::create(1, 0, oversized),
        GameEventError::payload_too_large,
        "payload must fit the retail object's 32-byte storage");
}

void malformed_records_are_rejected()
{
    const std::array short_header{byte(0), byte(0), byte(0)};
    expect_error(
        GameEventCodec::decode(short_header),
        GameEventDecodeError::truncated_header,
        "records shorter than four bytes should fail");

    const std::array oversized{
        byte(1), byte(0), byte(0), byte(GameEvent::max_payload_size + 1)};
    expect_error(
        GameEventCodec::decode(oversized),
        GameEventDecodeError::payload_too_large,
        "an unsafe wire payload length should fail");

    const std::array truncated{
        byte(1), byte(0), byte(0), byte(2), byte(0xAA)};
    expect_error(
        GameEventCodec::decode(truncated),
        GameEventDecodeError::truncated_payload,
        "a record missing payload bytes should fail");
}

void concatenated_records_can_be_walked()
{
    const std::array first_payload{byte(0xA1)};
    const std::array second_payload{byte(0xB1), byte(0xB2)};
    const auto first = GameEvent::create(1, 0, first_payload);
    const auto second = GameEvent::create(2, 1, second_payload, true);
    expect(first.has_value() && second.has_value(), "stream fixtures should build");
    if (!first || !second) {
        return;
    }

    auto stream = GameEventCodec::encode(*first);
    const auto second_bytes = GameEventCodec::encode(*second);
    stream.insert(stream.end(), second_bytes.begin(), second_bytes.end());

    const auto decoded_first = GameEventCodec::decode(stream);
    expect(
        decoded_first && decoded_first->event == *first,
        "first concatenated record should decode");
    if (!decoded_first) {
        return;
    }

    const auto remaining = std::span(stream).subspan(
        decoded_first->bytes_consumed);
    const auto decoded_second = GameEventCodec::decode(remaining);
    expect(
        decoded_second && decoded_second->event == *second,
        "bytes-consumed should locate the next record");
}

} // namespace

int main()
{
    exact_retail_wire_bytes_are_preserved();
    empty_and_maximum_payloads_round_trip();
    construction_enforces_retail_invariants();
    malformed_records_are_rejected();
    concatenated_records_can_be_walked();

    if (failures == 0) {
        std::cout << "All GameEvent codec tests passed.\n";
    }
    return failures == 0 ? 0 : 1;
}
