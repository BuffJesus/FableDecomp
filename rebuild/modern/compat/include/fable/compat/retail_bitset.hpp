#pragma once

// std::bitset's interface over VC7.1's storage: ceil(N/32) 32-bit words, bit i in word i/32 at
// bit i%32. The reconstructed headers embed std::bitset in persisted/engine layouts (CThing::
// UsingComponents, 274 bits = 36 bytes under VC7.1). Current standard libraries store 64-bit
// words (40 bytes for 274 bits), which moves every later member. The fork uses this type where
// a retail layout must hold (MODERN_BEHAVIOR_CENSUS, "retail bitset").

#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace fable::compat {

template <std::size_t N>
class retail_bitset {
public:
    class reference {
    public:
        reference(retail_bitset& owner, std::size_t pos) noexcept : owner_(owner), pos_(pos) {}
        reference& operator=(bool value) noexcept
        {
            owner_.set(pos_, value);
            return *this;
        }
        reference& operator=(const reference& other) noexcept { return *this = static_cast<bool>(other); }
        operator bool() const noexcept { return owner_.test_unchecked(pos_); }
        bool operator~() const noexcept { return !static_cast<bool>(*this); }
        reference& flip() noexcept
        {
            owner_.flip(pos_);
            return *this;
        }

    private:
        retail_bitset& owner_;
        std::size_t pos_;
    };

    constexpr retail_bitset() noexcept = default;

    [[nodiscard]] constexpr std::size_t size() const noexcept { return N; }

    [[nodiscard]] bool test(std::size_t pos) const
    {
        if (pos >= N) {
            throw std::out_of_range("retail_bitset::test");
        }
        return test_unchecked(pos);
    }
    [[nodiscard]] bool operator[](std::size_t pos) const noexcept { return test_unchecked(pos); }
    reference operator[](std::size_t pos) noexcept { return reference(*this, pos); }

    retail_bitset& set() noexcept
    {
        for (auto& w : words_) {
            w = 0xFFFFFFFFU;
        }
        trim();
        return *this;
    }
    retail_bitset& set(std::size_t pos, bool value = true)
    {
        if (pos >= N) {
            throw std::out_of_range("retail_bitset::set");
        }
        const std::uint32_t bit = std::uint32_t{1} << (pos % 32);
        words_[pos / 32] = value ? (words_[pos / 32] | bit) : (words_[pos / 32] & ~bit);
        return *this;
    }
    retail_bitset& reset() noexcept
    {
        for (auto& w : words_) {
            w = 0;
        }
        return *this;
    }
    retail_bitset& reset(std::size_t pos) { return set(pos, false); }
    retail_bitset& flip(std::size_t pos) { return set(pos, !test(pos)); }

    [[nodiscard]] std::size_t count() const noexcept
    {
        std::size_t n = 0;
        for (std::uint32_t w : words_) {
            for (; w; w &= w - 1) {
                ++n;
            }
        }
        return n;
    }
    [[nodiscard]] bool any() const noexcept { return count() != 0; }
    [[nodiscard]] bool none() const noexcept { return count() == 0; }

    // Written out (not defaulted) so the header also builds as C++17, like the reconstructed code.
    friend bool operator==(const retail_bitset& a, const retail_bitset& b) noexcept
    {
        for (std::size_t i = 0; i < word_count; ++i) {
            if (a.words_[i] != b.words_[i]) {
                return false;
            }
        }
        return true;
    }
    friend bool operator!=(const retail_bitset& a, const retail_bitset& b) noexcept { return !(a == b); }

private:
    static constexpr std::size_t word_count = (N + 31) / 32;

    [[nodiscard]] bool test_unchecked(std::size_t pos) const noexcept
    {
        return ((words_[pos / 32] >> (pos % 32)) & 1U) != 0;
    }
    void trim() noexcept
    {
        if constexpr (N % 32 != 0) {
            words_[word_count - 1] &= (std::uint32_t{1} << (N % 32)) - 1;
        }
    }

    std::uint32_t words_[word_count]{};
};

static_assert(sizeof(retail_bitset<274>) == 36, "VC7.1 stores std::bitset<274> in 9 32-bit words");

} // namespace fable::compat
