#include <fable/save/TextSections.hpp>

#include "Lzo.hpp"

#include <fable/codec/Lzo1x.hpp>

#include <algorithm>
#include <array>
#include <string>

namespace fable::save {
namespace {

constexpr std::size_t raw_tail = 3;
constexpr std::uint32_t max_short_clen = 0x7FFF;

std::span<const std::byte> as_bytes(std::string_view text)
{
    return {reinterpret_cast<const std::byte*>(text.data()), text.size()};
}

std::string as_text(std::span<const std::byte> bytes)
{
    std::string out(bytes.size(), '\0');
    std::transform(bytes.begin(), bytes.end(), out.begin(), [](std::byte b) { return static_cast<char>(b); });
    return out;
}

void write_stored(PersistWriter& out, std::string_view text)
{
    out.u32(static_cast<std::uint32_t>(text.size()));
    out.bytes_raw(std::array{std::byte{0}, std::byte{0}});
    out.bytes_raw(as_bytes(text));
}

} // namespace

bool lzo_compressor_available() noexcept
{
    return detail::lzo_compressor_available();
}

std::expected<std::string, PersistError> read_lz_text(PersistReader& in)
{
    const auto size = in.u32();
    if (!size) {
        return std::unexpected(size.error());
    }
    if (*size == 0) {
        return std::string();
    }
    const auto short_clen = in.raw(2);
    if (!short_clen) {
        return std::unexpected(short_clen.error());
    }
    const auto clen16 = static_cast<std::int16_t>(std::to_integer<unsigned>((*short_clen)[0])
                                                  | std::to_integer<unsigned>((*short_clen)[1]) << 8);
    std::int64_t clen = clen16;
    if (clen16 == -1) {
        const auto wide = in.u32();
        if (!wide) {
            return std::unexpected(wide.error());
        }
        clen = *wide;
    }
    if (clen <= 0) {
        const auto raw = in.raw(*size);
        if (!raw) {
            return std::unexpected(raw.error());
        }
        return as_text(*raw);
    }
    if (*size <= raw_tail) {
        return std::unexpected(PersistError::bad_section);
    }
    const auto stream = in.raw(static_cast<std::size_t>(clen));
    const auto tail = in.raw(raw_tail);
    if (!stream || !tail) {
        return std::unexpected(PersistError::truncated);
    }
    auto head = detail::lzo1x_decompress(*stream, *size - raw_tail);
    if (!head) {
        return std::unexpected(PersistError::bad_section);
    }
    std::string text = as_text(*head);
    text += as_text(*tail);
    return text;
}

void write_lz_text(PersistWriter& out, std::string_view text)
{
    if (text.empty()) {
        out.u32(0);
        return;
    }
    if (text.size() <= raw_tail) {
        write_stored(out, text);
        return;
    }
    const std::string_view head = text.substr(0, text.size() - raw_tail);
    // Upstream lzo1x_999 (optional GPL link) gives retail's exact bytes; otherwise the MIT encoder
    // gives a valid LZO1X stream the retail reader accepts.
    auto stream = detail::lzo1x_999_compress(as_bytes(head));
    if (!stream) {
        stream = codec::lzo1x_compress(as_bytes(head));
    }
    if (!stream || stream->size() >= head.size()) {
        write_stored(out, text);
        return;
    }
    out.u32(static_cast<std::uint32_t>(text.size()));
    if (stream->size() <= max_short_clen) {
        const auto clen = static_cast<std::uint32_t>(stream->size());
        out.bytes_raw(std::array{static_cast<std::byte>(clen & 0xFFU), static_cast<std::byte>(clen >> 8)});
    } else {
        out.bytes_raw(std::array{std::byte{0xFF}, std::byte{0xFF}});
        out.u32(static_cast<std::uint32_t>(stream->size()));
    }
    out.bytes_raw(*stream);
    out.bytes_raw(as_bytes(text.substr(text.size() - raw_tail)));
}

std::expected<QuestsSection, PersistError> QuestsSection::decode(std::span<const std::byte> section)
{
    PersistReader in(section);
    auto text = read_lz_text(in);
    if (!text) {
        return std::unexpected(text.error());
    }
    QuestsSection out;
    out.text = std::move(*text);
    if (const auto tag = in.tag("NumAcceptedBoasts"); !tag) {
        return std::unexpected(tag.error());
    }
    const auto count = in.u32();
    if (!count) {
        return std::unexpected(count.error());
    }
    if (*count > in.remaining() / 19) {   // 4 tags, a NUL, a u32 and 2 bools per boast
        return std::unexpected(PersistError::bad_section);
    }
    for (std::uint32_t i = 0; i < *count; ++i) {
        const std::string index = std::to_string(i);
        AcceptedBoast boast;
        auto name = in.tag("BoastScriptName" + index).and_then([&] { return in.cstring(); });
        auto id = in.tag("BoastId" + index).and_then([&] { return in.u32(); });
        auto failed = in.tag("BoastFailed" + index).and_then([&] { return in.boolean(); });
        auto completed = in.tag("BoastCompleted" + index).and_then([&] { return in.boolean(); });
        if (!name || !id || !failed || !completed) {
            return std::unexpected(!name ? name.error() : !id ? id.error() : !failed ? failed.error() : completed.error());
        }
        boast.script_name = std::move(*name);
        boast.id = *id;
        boast.failed = *failed;
        boast.completed = *completed;
        out.accepted_boasts.push_back(std::move(boast));
    }
    if (in.remaining() != 0) {
        return std::unexpected(PersistError::bad_section);
    }
    return out;
}

std::vector<std::byte> QuestsSection::encode() const
{
    PersistWriter out;
    write_lz_text(out, text);
    out.tag("NumAcceptedBoasts");
    out.u32(static_cast<std::uint32_t>(accepted_boasts.size()));
    for (std::size_t i = 0; i < accepted_boasts.size(); ++i) {
        const std::string index = std::to_string(i);
        const AcceptedBoast& boast = accepted_boasts[i];
        out.tag("BoastScriptName" + index);
        out.cstring(boast.script_name);
        out.tag("BoastId" + index);
        out.u32(boast.id);
        out.tag("BoastFailed" + index);
        out.boolean(boast.failed);
        out.tag("BoastCompleted" + index);
        out.boolean(boast.completed);
    }
    return out.take();
}

std::expected<FactionsSection, PersistError> FactionsSection::decode(std::span<const std::byte> section)
{
    PersistReader in(section);
    if (const auto tag = in.tag("NumberOfFactions"); !tag) {
        return std::unexpected(tag.error());
    }
    const auto count = in.u32();
    if (!count) {
        return std::unexpected(count.error());
    }
    // Each record is at least a tag and a size.
    if (*count > in.remaining() / 8) {
        return std::unexpected(PersistError::bad_section);
    }
    FactionsSection out;
    out.factions.reserve(*count);
    for (std::uint32_t i = 0; i < *count; ++i) {
        if (const auto tag = in.tag("Faction" + std::to_string(i)); !tag) {
            return std::unexpected(tag.error());
        }
        auto text = read_lz_text(in);
        if (!text) {
            return std::unexpected(text.error());
        }
        out.factions.push_back(std::move(*text));
    }
    if (in.remaining() != 0) {
        return std::unexpected(PersistError::bad_section);
    }
    return out;
}

std::vector<std::byte> FactionsSection::encode() const
{
    PersistWriter out;
    out.tag("NumberOfFactions");
    out.u32(static_cast<std::uint32_t>(factions.size()));
    for (std::size_t i = 0; i < factions.size(); ++i) {
        out.tag("Faction" + std::to_string(i));
        write_lz_text(out, factions[i]);
    }
    return out.take();
}

} // namespace fable::save
