#include <fable/codec/Lzo1x.hpp>

#include <cstdint>

namespace fable::codec {
namespace {

// LZO1X stream decoder. Token grammar (t = the token byte):
//   first byte > 17       initial literal run of t - 17 bytes
//   t < 16 at loop top    literal run of t + 3 bytes (t == 0: zero-byte extension, base 15)
//   t >= 64               M2: length (t >> 5) + 1, distance 1 + ((t >> 2) & 7) + (next << 3)
//   t >= 32               M3: length (t & 31) + 2 (extension base 31), 14-bit distance + 1
//   t >= 16               M4: length (t & 7) + 2 (extension base 7), distance 0x4000 + ...;
//                         an encoded distance of 0 is the end of the stream
//   t < 16 after a run    a 3-byte match at distance 0x801 + (t >> 2) + (next << 2)
//   t < 16 after a match  a 2-byte match at distance 1 + (t >> 2) + (next << 2)
// After every match, the low two bits of the byte two before the input position give 0-3
// literals to copy; a nonzero count is followed directly by the next match token.
class Decoder final {
public:
    Decoder(std::span<const std::byte> in, std::size_t expected) : in_(in) { out_.reserve(expected); }

    std::expected<std::vector<std::byte>, Lzo1xError> run()
    {
        enum class State { loop, after_run, match, after_match };
        State state = State::loop;
        std::size_t t = 0;

        if (!in_.empty() && byte_at(0) > 17) {
            t = byte_at(0) - 17;
            ip_ = 1;
            if (!literals(t)) {
                return std::unexpected(Lzo1xError::truncated);
            }
            state = State::after_run;
            if (t < 4) {
                // A short initial run plays the role of a match's trailing literals.
                if (!next(t)) {
                    return std::unexpected(Lzo1xError::truncated);
                }
                state = State::match;
            }
        }

        for (;;) {
            switch (state) {
            case State::loop: {
                if (!next(t)) {
                    return std::unexpected(Lzo1xError::truncated);
                }
                if (t >= 16) {
                    state = State::match;
                    break;
                }
                if (t == 0 && !extend(t, 15)) {
                    return std::unexpected(Lzo1xError::truncated);
                }
                if (!literals(t + 3)) {
                    return std::unexpected(Lzo1xError::truncated);
                }
                state = State::after_run;
                break;
            }
            case State::after_run: {
                std::size_t h = 0;
                if (!next(t)) {
                    return std::unexpected(Lzo1xError::truncated);
                }
                if (t >= 16) {
                    state = State::match;
                    break;
                }
                if (!next(h)) {
                    return std::unexpected(Lzo1xError::truncated);
                }
                if (!copy_match(0x801 + (t >> 2) + (h << 2), 3)) {
                    return std::unexpected(Lzo1xError::bad_distance);
                }
                state = State::after_match;
                break;
            }
            case State::match: {
                std::size_t distance = 0;
                std::size_t length = 0;
                std::size_t h = 0;
                if (t >= 64) {
                    if (!next(h)) {
                        return std::unexpected(Lzo1xError::truncated);
                    }
                    distance = 1 + ((t >> 2) & 7) + (h << 3);
                    length = (t >> 5) + 1;
                } else if (t >= 32) {
                    std::size_t n = t & 31;
                    if (n == 0 && !extend(n, 31)) {
                        return std::unexpected(Lzo1xError::truncated);
                    }
                    std::size_t lo = 0;
                    if (!next(lo) || !next(h)) {
                        return std::unexpected(Lzo1xError::truncated);
                    }
                    distance = 1 + ((h << 6) | (lo >> 2));
                    length = n + 2;
                } else if (t >= 16) {
                    std::size_t n = t & 7;
                    if (n == 0 && !extend(n, 7)) {
                        return std::unexpected(Lzo1xError::truncated);
                    }
                    std::size_t lo = 0;
                    if (!next(lo) || !next(h)) {
                        return std::unexpected(Lzo1xError::truncated);
                    }
                    const std::size_t encoded = ((t & 8) << 11) + ((h << 6) | (lo >> 2));
                    if (encoded == 0) {
                        return finish();
                    }
                    distance = 0x4000 + encoded;
                    length = n + 2;
                } else {
                    if (!next(h)) {
                        return std::unexpected(Lzo1xError::truncated);
                    }
                    distance = 1 + (t >> 2) + (h << 2);
                    length = 2;
                }
                if (!copy_match(distance, length)) {
                    return std::unexpected(Lzo1xError::bad_distance);
                }
                state = State::after_match;
                break;
            }
            case State::after_match: {
                const std::size_t trailing = byte_at(ip_ - 2) & 3;
                if (trailing == 0) {
                    state = State::loop;
                    break;
                }
                if (!literals(trailing) || !next(t)) {
                    return std::unexpected(Lzo1xError::truncated);
                }
                state = State::match;
                break;
            }
            }
        }
    }

private:
    std::size_t byte_at(std::size_t at) const { return std::to_integer<std::size_t>(in_[at]); }

    bool next(std::size_t& value)
    {
        if (ip_ >= in_.size()) {
            return false;
        }
        value = byte_at(ip_++);
        return true;
    }

    // Zero bytes add 255 each; the first nonzero byte adds itself plus `base`.
    bool extend(std::size_t& value, std::size_t base)
    {
        std::size_t total = 0;
        while (ip_ < in_.size() && in_[ip_] == std::byte{0}) {
            total += 255;
            ++ip_;
        }
        std::size_t last = 0;
        if (!next(last)) {
            return false;
        }
        value = total + base + last;
        return true;
    }

    bool literals(std::size_t count)
    {
        if (count > in_.size() - ip_) {
            return false;
        }
        out_.insert(out_.end(), in_.begin() + static_cast<std::ptrdiff_t>(ip_),
                    in_.begin() + static_cast<std::ptrdiff_t>(ip_ + count));
        ip_ += count;
        return true;
    }

    bool copy_match(std::size_t distance, std::size_t length)
    {
        if (distance > out_.size()) {
            return false;
        }
        std::size_t from = out_.size() - distance;
        for (std::size_t i = 0; i < length; ++i) {
            out_.push_back(out_[from++]);   // overlapping copies repeat, as in the format
        }
        return true;
    }

    std::expected<std::vector<std::byte>, Lzo1xError> finish()
    {
        if (ip_ != in_.size()) {
            return std::unexpected(Lzo1xError::trailing_bytes);
        }
        return std::move(out_);
    }

    std::span<const std::byte> in_;
    std::size_t ip_ = 0;
    std::vector<std::byte> out_;
};

} // namespace

std::expected<std::vector<std::byte>, Lzo1xError> lzo1x_decompress(std::span<const std::byte> stream,
                                                                  std::size_t expected_size)
{
    auto out = Decoder(stream, expected_size).run();
    if (out && out->size() != expected_size) {
        return std::unexpected(Lzo1xError::size_mismatch);
    }
    return out;
}

} // namespace fable::codec
