#include <cstddef>
#include <cstdint>

struct CAppearanceModifierGraphics {
    struct CEntry {
        std::uint32_t words[21];
    };
};

static_assert(sizeof(CAppearanceModifierGraphics::CEntry) == 0x54);

struct Vector84Overlay {
    std::byte* field_0x0;
    CAppearanceModifierGraphics::CEntry* last;        // +0x04
    CAppearanceModifierGraphics::CEntry* end_storage; // +0x08
};

static_assert(offsetof(Vector84Overlay, last) == 0x04);
static_assert(offsetof(Vector84Overlay, end_storage) == 0x08);

static inline CAppearanceModifierGraphics::CEntry* AddEntries(
    CAppearanceModifierGraphics::CEntry* ptr,
    std::ptrdiff_t count) {
    return reinterpret_cast<CAppearanceModifierGraphics::CEntry*>(
        reinterpret_cast<std::byte*>(ptr) + count * 0x54);
}

static inline void CopyEntryWords(
    CAppearanceModifierGraphics::CEntry* dst,
    const CAppearanceModifierGraphics::CEntry* src) {
    for (std::size_t i = 0; i != 21; ++i) {
        dst->words[i] = src->words[i];
    }
}

extern void __thiscall std__vector_insert_reallocate_84byte(
    void* self,
    CAppearanceModifierGraphics::CEntry* position,
    const CAppearanceModifierGraphics::CEntry* value,
    bool* local_flag,
    unsigned int count,
    int zero);

extern void __fastcall std___Cons_val__allocator_CAppearanceModifierGraphics_CEntry(
    CAppearanceModifierGraphics::CEntry* dst,
    unsigned int count,
    const CAppearanceModifierGraphics::CEntry* value);

void __thiscall Vector_Insert(
    void* self,
    CAppearanceModifierGraphics::CEntry* position,
    unsigned int count,
    const CAppearanceModifierGraphics::CEntry* value) {
    auto* const vec = static_cast<Vector84Overlay*>(self);

    if (count == 0) {
        return;
    }

    if (static_cast<unsigned int>(
            (reinterpret_cast<std::uintptr_t>(vec->end_storage) -
             reinterpret_cast<std::uintptr_t>(vec->last)) /
            0x54) < count) {
        bool local_flag = false;
        std__vector_insert_reallocate_84byte(self, position, value, &local_flag, count, 0);
        return;
    }

    CAppearanceModifierGraphics::CEntry local_value{};
    CopyEntryWords(&local_value, value);

    const unsigned int tail_count = static_cast<unsigned int>(
        (reinterpret_cast<std::uintptr_t>(vec->last) -
         reinterpret_cast<std::uintptr_t>(position)) /
        0x54);
    CAppearanceModifierGraphics::CEntry* const old_last = vec->last;

    if (count < tail_count) {
        const unsigned int inserted_bytes = count * 0x54;
        auto* split = reinterpret_cast<CAppearanceModifierGraphics::CEntry*>(
            reinterpret_cast<std::byte*>(old_last) - inserted_bytes);
        auto* dst = old_last;

        for (auto* src = split; src != old_last; src = AddEntries(src, 1)) {
            if (dst != nullptr) {
                CopyEntryWords(dst, src);
            }
            dst = AddEntries(dst, 1);
        }

        int move_count = static_cast<int>(
            (reinterpret_cast<std::uintptr_t>(split) -
             reinterpret_cast<std::uintptr_t>(position)) /
            0x54);

        vec->last = reinterpret_cast<CAppearanceModifierGraphics::CEntry*>(
            reinterpret_cast<std::byte*>(vec->last) + inserted_bytes);

        if (0 < move_count) {
            auto* move_dst = old_last;
            auto* move_src = split;
            do {
                move_dst = AddEntries(move_dst, -1);
                move_src = AddEntries(move_src, -1);
                --move_count;
                CopyEntryWords(move_dst, move_src);
            } while (move_count != 0);
        }

        auto* fill_end = reinterpret_cast<CAppearanceModifierGraphics::CEntry*>(
            reinterpret_cast<std::byte*>(position) + inserted_bytes);

        for (; position != fill_end; position = AddEntries(position, 1)) {
            CopyEntryWords(position, &local_value);
        }
    } else {
        const unsigned int extra_count = count - tail_count;
        std___Cons_val__allocator_CAppearanceModifierGraphics_CEntry(
            old_last,
            extra_count,
            &local_value);

        vec->last = AddEntries(vec->last, static_cast<std::ptrdiff_t>(extra_count));

        auto* write = vec->last;
        for (auto* src = position; src != old_last; src = AddEntries(src, 1)) {
            if (write != nullptr) {
                CopyEntryWords(write, src);
            }
            write = AddEntries(write, 1);
        }

        vec->last = AddEntries(vec->last, static_cast<std::ptrdiff_t>(tail_count));

        while (position != old_last) {
            auto* const next = AddEntries(position, 1);
            CopyEntryWords(position, &local_value);
            position = next;
        }
    }
}