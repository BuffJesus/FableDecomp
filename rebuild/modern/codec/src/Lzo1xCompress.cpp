#include <fable/codec/Lzo1x.hpp>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <vector>

// LZO1X encoder, ported unchanged in substance from FableForge `src/lzo1x.cpp` (MIT, by the
// same author as this tree; FableForge commit a358de0). Its output is not lzo1x_999's exact
// bytes, but any LZO1X decoder accepts it, the retail engine's included: FableForge verifies it
// through minilzo and its own clean-room decoder and ships it for in-game STB and texture edits.

namespace fable::codec {
namespace {

using std::size_t;
using std::uint8_t;
using std::uint32_t;

// ---------------------------------------------------------------- encoder
//
// Two stages. A hash-chain match finder records, for every position, the
// longest match overall and the longest within each cheaper distance class
// (M2 <= 2048, M3 <= 16384), the nearest 3-byte match in the M1L band
// (2049..3072) and whether a 2-byte match exists within 1024 (M1). Then a
// backward dynamic programme over the exact grammar: literal runs cost their
// bytes plus the run token (free for 0..3 literals riding in the previous
// match token, +1 for 4..18, +1 and length extension beyond), and the token
// that may follow a run depends on the run's length class:
//   after 0 literals    : M2 / M3 / M4
//   after 1..3 literals : also M1  (2 bytes at distance 1..1024, 2 bytes)
//   after a run >= 4    : also M1L (3 bytes at distance 2049..3072, 2 bytes)
// which is exactly why lzo1x_999 output is smaller than a plain parse. The
// forward pass emits tokens in that grammar. The point is size: retail STB
// frames were written with lzo1x_999 and an edited frame must fit its slot.

namespace {

struct Cand { uint32_t dist = 0, len = 0; };
struct Cands {
    Cand best, m2, m3;      // longest overall / within 2048 / within 16384
    uint32_t m1lDist = 0;   // 3-byte match at 2049..3072 (0 = none)
    uint32_t m1Dist = 0;    // 2-byte match at 1..1024 (0 = none)
};

constexpr size_t kMaxDist = 0xBFFF;
constexpr size_t kMinLen = 3;
constexpr size_t kNiceLen = 512;
constexpr uint32_t kChainDepth = 400;
constexpr uint32_t kInf = 0x3FFFFFFFu;

inline uint32_t extBytes(size_t rem) { return uint32_t((rem - 1) / 255 + 1); }

// bytes for an M2/M3/M4 token of `len` at `dist` (no literal-run bytes)
inline uint32_t tokenCost(size_t len, size_t dist) {
    if (dist <= 2048 && len <= 8) return 2;
    if (dist <= 16384) { const size_t t = len - 2; return 3 + (t > 31 ? extBytes(t - 31) : 0); }
    const size_t t = len - 2;
    return 3 + (t > 7 ? extBytes(t - 7) : 0);
}

void findMatches(const uint8_t* in, size_t n, std::vector<Cands>& out) {
    out.assign(n, Cands{});
    if (n < 2) return;
    // 2-byte pairs: last position of each pair, for M1
    std::vector<uint32_t> pairLast(65536, 0xFFFFFFFFu);
    for (size_t i = 0; i + 2 <= n; ++i) {
        const uint32_t key = uint32_t(in[i]) | (uint32_t(in[i + 1]) << 8);
        const uint32_t p = pairLast[key];
        if (p != 0xFFFFFFFFu && i - p <= 1024) out[i].m1Dist = uint32_t(i - p);
        pairLast[key] = uint32_t(i);
    }
    if (n < kMinLen) return;
    constexpr size_t kHashBits = 16;
    std::vector<uint32_t> head(size_t(1) << kHashBits, 0xFFFFFFFFu);
    std::vector<uint32_t> next(n, 0xFFFFFFFFu);
    // Positions advance monotonically. A match at distance d proves equality
    // up to its end for every later position inside that span at the same d.
    // Reuse that verified prefix without changing the longest-match result.
    std::vector<size_t> matchEnd(std::min(n,kMaxDist+1),0);
    auto hash = [&](size_t i) {
        const uint32_t v = uint32_t(in[i]) | (uint32_t(in[i + 1]) << 8) | (uint32_t(in[i + 2]) << 16);
        return size_t((v * 0x9E3779B1u) >> (32 - kHashBits));
    };
    for (size_t i = 0; i + kMinLen <= n; ++i) {
        const size_t h = hash(i);
        uint32_t p = head[h];
        Cands& c = out[i];
        uint32_t depth = 0;
        const size_t maxLen = n - i;
        while (p != 0xFFFFFFFFu && depth++ < kChainDepth) {
            const size_t dist = i - p;
            if (dist > kMaxDist) break;
            const bool canImprove = c.best.len < maxLen && in[p + c.best.len] == in[i + c.best.len];
            const bool wantClass = (dist <= 16384 && c.m3.len < maxLen) || (dist >= 2049 && dist <= 3072 && !c.m1lDist);
            if (canImprove || wantClass) {
                size_t len = matchEnd[dist]>i ? matchEnd[dist]-i : 0;
                while (len < maxLen && in[p + len] == in[i + len]) ++len;
                matchEnd[dist]=i+len;
                if (len >= kMinLen) {
                    if (len > c.best.len) c.best = {uint32_t(dist), uint32_t(len)};
                    if (dist <= 2048 && len > c.m2.len) c.m2 = {uint32_t(dist), uint32_t(len)};
                    if (dist <= 16384 && len > c.m3.len) c.m3 = {uint32_t(dist), uint32_t(len)};
                    if (dist >= 2049 && dist <= 3072 && !c.m1lDist) c.m1lDist = uint32_t(dist);
                    if (len >= kNiceLen) break;
                }
            }
            p = next[p];
        }
        next[i] = head[h];
        head[h] = uint32_t(i);
    }
}

} // namespace

} // namespace

namespace {

std::vector<uint8_t> compress_bytes(const uint8_t* in, size_t n) {
    std::vector<uint8_t> out;
    out.reserve(n / 2 + 16);
    if (n == 0) { out = {0x11, 0x00, 0x00}; return out; }

    std::vector<Cands> cands;
    findMatches(in, n, cands);

    // A[i]  : cheapest cost of in[i..n) starting right after a match token (0 literals so far)
    // B[c][i]: cheapest cost of in[i..n) starting with a match token in literal context c
    //          (0: no literals before, 1: 1..3 literals before, 2: run >= 4 before)
    // L[i]  : cheapest cost of in[i..n) while inside a literal run that is already
    //          >= 19 long (each further literal costs 1; ending it means a class-2 match)
    // Choices are packed for the forward pass.
    struct Choice { uint32_t len = 0, dist = 0; uint8_t kind = 0; };   // kind: 0 M2/3/4, 1 M1, 2 M1L
    std::vector<uint32_t> A(n + 1, kInf), L(n + 1, kInf);
    std::vector<uint32_t> B[3] = {std::vector<uint32_t>(n + 1, kInf), std::vector<uint32_t>(n + 1, kInf), std::vector<uint32_t>(n + 1, kInf)};
    std::vector<Choice> Bc[3] = {std::vector<Choice>(n + 1), std::vector<Choice>(n + 1), std::vector<Choice>(n + 1)};
    std::vector<uint32_t> Ak(n + 1, 0);        // literal count chosen at A[i]; 0xFFFFFFFF = long run
    constexpr uint32_t kLong = 0xFFFFFFFFu;
    // end marker costs 3 in every context; the tail literals are a plain run
    A[n] = 3; L[n] = 3;
    for (int c = 0; c < 3; ++c) B[c][n] = 3;

    auto runCost = [](size_t k) -> uint32_t {   // bytes for a literal run of k (token + literals)
        if (k == 0) return 0;
        if (k <= 3) return uint32_t(k);
        if (k <= 18) return uint32_t(k + 1);
        return uint32_t(k + 1 + extBytes(k - 18));
    };

    for (size_t i = n; i-- > 0;) {
        // ---- B: a match starts at i
        const Cands& c = cands[i];
        Choice bestCh[3]; uint32_t bestV[3] = {kInf, kInf, kInf};
        auto consider = [&](const Cand& cd) {
            if (cd.len < kMinLen) return;
            auto tryLen = [&](size_t Lm) {
                if (Lm < kMinLen || Lm > cd.len) return;
                const uint32_t v = tokenCost(Lm, cd.dist) + A[i + Lm];
                for (int k = 0; k < 3; ++k) if (v < bestV[k]) { bestV[k] = v; bestCh[k] = {uint32_t(Lm), cd.dist, 0}; }
            };
            tryLen(cd.len);
            const size_t lo = cd.len > 24 ? cd.len - 24 : kMinLen;
            for (size_t Lm = lo; Lm < cd.len; ++Lm) tryLen(Lm);
            static const size_t thresholds[] = {3, 4, 8, 9, 33};
            for (size_t t : thresholds) if (t < lo) tryLen(t);
        };
        consider(c.best); consider(c.m2); consider(c.m3);
        if (c.m1Dist && i + 2 <= n) {            // class 1 only
            const uint32_t v = 2 + A[i + 2];
            if (v < bestV[1]) { bestV[1] = v; bestCh[1] = {2, c.m1Dist, 1}; }
        }
        if (c.m1lDist && i + 3 <= n) {           // class 2 only
            const uint32_t v = 2 + A[i + 3];
            if (v < bestV[2]) { bestV[2] = v; bestCh[2] = {3, c.m1lDist, 2}; }
        }
        for (int k = 0; k < 3; ++k) { B[k][i] = bestV[k]; Bc[k][i] = bestCh[k]; }

        // ---- L: inside a long literal run
        L[i] = std::min(1 + L[i + 1], B[2][i]);

        // ---- A: choose how many literals precede the next match (or the end)
        uint32_t best = kInf, bk = 0;
        const size_t rem = n - i;
        // finish with literals only
        if (rem <= 18) { const uint32_t v = runCost(rem) + 3; if (v < best) { best = v; bk = uint32_t(rem); } }
        if (B[0][i] < best) { best = B[0][i]; bk = 0; }
        for (size_t k = 1; k <= 18 && k < rem; ++k) {
            // The very first run of 1..3 literals ("17+k" byte) is followed by a
            // plain match token in the C decoder but by an after-literal-run token
            // in LZO's i386 assembly decoder (lzo1x_decompress_asm_fast, which
            // Fable's landscape loader uses): the two disagree on tokens < 16
            // there, so only M2/M3/M4 (class 0) are allowed after it.
            const int cls = (i == 0 && k <= 3) ? 0 : (k <= 3 ? 1 : 2);
            const uint32_t v = runCost(k) + B[cls][i + k];
            if (v < best) { best = v; bk = uint32_t(k); }
        }
        if (rem >= 19) {
            // long run: 19 literals (+ token + first extension byte) then keep going or match
            const uint32_t v = 19 + 2 + L[i + 19];
            if (v < best) { best = v; bk = kLong; }
        }
        A[i] = best; Ak[i] = bk;
    }

    // ---- forward emission
    auto extend = [&](size_t remv) { while (remv > 255) { out.push_back(0); remv -= 255; } out.push_back(uint8_t(remv)); };
    size_t litBitsPos = SIZE_MAX;
    bool first = true;
    auto emitRun = [&](size_t start, size_t k) {   // k literals in[start..start+k)
        if (first) {
            first = false;
            if (k <= 3) out.push_back(uint8_t(17 + k));
            else if (k <= 18) out.push_back(uint8_t(k - 3));
            else { out.push_back(0); extend(k - 18); }
        } else if (k == 0) {
        } else if (k <= 3) {
            out[litBitsPos] |= uint8_t(k);
        } else if (k <= 18) {
            out.push_back(uint8_t(k - 3));
        } else {
            out.push_back(0); extend(k - 18);
        }
        out.insert(out.end(), in + start, in + start + k);
    };
    auto emitMatch = [&](const Choice& ch) {
        const size_t len = ch.len, dist = ch.dist;
        if (ch.kind == 1) {                                   // M1: 2 bytes, dist 1..1024
            const size_t d = dist - 1;
            litBitsPos = out.size();
            out.push_back(uint8_t((d & 3) << 2));
            out.push_back(uint8_t(d >> 2));
        } else if (ch.kind == 2) {                            // M1L: 3 bytes, dist 2049..3072
            const size_t d = dist - 0x801;
            litBitsPos = out.size();
            out.push_back(uint8_t((d & 3) << 2));
            out.push_back(uint8_t(d >> 2));
        } else if (dist <= 2048 && len <= 8) {                // M2
            litBitsPos = out.size();
            out.push_back(uint8_t(((len - 1) << 5) | (((dist - 1) & 7) << 2)));
            out.push_back(uint8_t((dist - 1) >> 3));
        } else if (dist <= 16384) {                           // M3
            const size_t t = len - 2;
            if (t <= 31) out.push_back(uint8_t(0x20 | t));
            else { out.push_back(0x20); extend(t - 31); }
            const size_t d = dist - 1;
            litBitsPos = out.size();
            out.push_back(uint8_t((d & 0x3F) << 2));
            out.push_back(uint8_t(d >> 6));
        } else {                                              // M4
            const size_t d = dist - 0x4000;
            const uint8_t t0 = uint8_t(0x10 | ((d >> 11) & 8));
            const size_t t = len - 2;
            if (t <= 7) out.push_back(uint8_t(t0 | t));
            else { out.push_back(t0); extend(t - 7); }
            const size_t d14 = d & 0x3FFF;
            litBitsPos = out.size();
            out.push_back(uint8_t((d14 & 0x3F) << 2));
            out.push_back(uint8_t(d14 >> 6));
        }
    };

    size_t i = 0;
    while (i < n) {
        uint32_t k = Ak[i];
        size_t runLen;
        int cls;
        if (k == kLong) {
            // extend the run while L says "one more literal" is the choice
            size_t j = i + 19;
            while (j < n && 1 + L[j + 1] <= B[2][j]) ++j;
            runLen = j - i; cls = 2;
        } else {
            runLen = k; cls = k == 0 ? 0 : (k <= 3 ? ((i == 0) ? 0 : 1) : 2);
        }
        emitRun(i, runLen);
        i += runLen;
        if (i >= n) break;
        const Choice ch = Bc[cls][i];
        if (ch.len == 0) {   // no match possible here (B == inf): cannot happen when A chose it; guard
            emitRun(i, n - i); i = n; break;
        }
        emitMatch(ch);
        i += ch.len;
    }
    out.push_back(0x11); out.push_back(0x00); out.push_back(0x00);   // end marker
    return out;
}

} // namespace

std::vector<std::byte> lzo1x_compress(std::span<const std::byte> data)
{
    const auto bytes = compress_bytes(reinterpret_cast<const uint8_t*>(data.data()), data.size());
    std::vector<std::byte> out(bytes.size());
    if (!bytes.empty()) {
        std::memcpy(out.data(), bytes.data(), bytes.size());
    }
    return out;
}

} // namespace fable::codec
