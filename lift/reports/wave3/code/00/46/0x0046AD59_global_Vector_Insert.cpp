#include <cstddef>
#include <cstdint>

struct CAppearanceModifierGraphics {
    struct CEntry {
        std::uint32_t words[5];
    };
};

static_assert(sizeof(CAppearanceModifierGraphics::CEntry) == 0x14);

struct VectorInsertOverlay {
    std::byte pad_0[0x4];
    CAppearanceModifierGraphics::CEntry* finish;
    CAppearanceModifierGraphics::CEntry* end_of_storage;
};

static_assert(offsetof(VectorInsertOverlay, finish) == 0x4);
static_assert(offsetof(VectorInsertOverlay, end_of_storage) == 0x8);
static_assert(sizeof(VectorInsertOverlay) == 0x0C);

static inline void CopyEntryWords(
    CAppearanceModifierGraphics::CEntry* dst,
    const CAppearanceModifierGraphics::CEntry* src)
{
    dst->words[0] = src->words[0];
    dst->words[1] = src->words[1];
    dst->words[2] = src->words[2];
    dst->words[3] = src->words[3];
    dst->words[4] = src->words[4];
}

extern void __thiscall Vector_Reallocate(
    VectorInsertOverlay* self,
    CAppearanceModifierGraphics::CEntry* position,
    const CAppearanceModifierGraphics::CEntry* value,
    bool* flag,
    unsigned int count,
    int zero);

namespace std {
extern void __fastcall _Cons_val(
    CAppearanceModifierGraphics::CEntry* finish,
    unsigned int count,
    const CAppearanceModifierGraphics::CEntry* value);
}

void __thiscall Vector_Insert(
    VectorInsertOverlay* self,
    CAppearanceModifierGraphics::CEntry* position,
    unsigned int count,
    const CAppearanceModifierGraphics::CEntry* value)
{
    if (count == 0) {
        return;
    }

    if (static_cast<unsigned int>(
            (reinterpret_cast<std::intptr_t>(self->end_of_storage) -
             reinterpret_cast<std::intptr_t>(self->finish)) / 0x14) < count) {
        bool local_flag = false;
        Vector_Reallocate(self, position, value, &local_flag, count, 0);
        return;
    }

    CAppearanceModifierGraphics::CEntry local_value{};
    CopyEntryWords(&local_value, value);

    const unsigned int trailing_count =
        static_cast<unsigned int>(self->finish - position);
    CAppearanceModifierGraphics::CEntry* finish = self->finish;

    if (count < trailing_count) {
        const unsigned int inserted_bytes = count * 0x14;
        auto* src = reinterpret_cast<CAppearanceModifierGraphics::CEntry*>(
            reinterpret_cast<std::byte*>(finish) - inserted_bytes);
        auto* dst = finish;

        for (auto* cur = src; cur != finish; cur = reinterpret_cast<CAppearanceModifierGraphics::CEntry*>(
                 reinterpret_cast<std::byte*>(cur) + 0x14)) {
            if (dst != nullptr) {
                CopyEntryWords(dst, cur);
            }
            dst = reinterpret_cast<CAppearanceModifierGraphics::CEntry*>(
                reinterpret_cast<std::byte*>(dst) + 0x14);
        }

        int move_count = static_cast<int>(src - position);
        self->finish = reinterpret_cast<CAppearanceModifierGraphics::CEntry*>(
            reinterpret_cast<std::byte*>(self->finish) + inserted_bytes);

        if (move_count > 0) {
            do {
                finish = reinterpret_cast<CAppearanceModifierGraphics::CEntry*>(
                    reinterpret_cast<std::byte*>(finish) - 0x14);
                src = reinterpret_cast<CAppearanceModifierGraphics::CEntry*>(
                    reinterpret_cast<std::byte*>(src) - 0x14);
                --move_count;
                CopyEntryWords(finish, src);
            } while (move_count != 0);
        }

        auto* fill_end = reinterpret_cast<CAppearanceModifierGraphics::CEntry*>(
            reinterpret_cast<std::byte*>(position) + inserted_bytes);
        for (; position != fill_end; position = reinterpret_cast<CAppearanceModifierGraphics::CEntry*>(
                 reinterpret_cast<std::byte*>(position) + 0x14)) {
            CopyEntryWords(position, &local_value);
        }
    } else {
        const unsigned int append_count = count - trailing_count;
        std::_Cons_val(finish, append_count, &local_value);

        self->finish = reinterpret_cast<CAppearanceModifierGraphics::CEntry*>(
            reinterpret_cast<std::byte*>(self->finish) + append_count * 0x14);

        auto* out = self->finish;
        for (auto* cur = position; cur != finish; cur = reinterpret_cast<CAppearanceModifierGraphics::CEntry*>(
                 reinterpret_cast<std::byte*>(cur) + 0x14)) {
            if (out != nullptr) {
                CopyEntryWords(out, cur);
            }
            out = reinterpret_cast<CAppearanceModifierGraphics::CEntry*>(
                reinterpret_cast<std::byte*>(out) + 0x14);
        }

        self->finish = reinterpret_cast<CAppearanceModifierGraphics::CEntry*>(
            reinterpret_cast<std::byte*>(self->finish) + trailing_count * 0x14);

        while (position != finish) {
            auto* next = reinterpret_cast<CAppearanceModifierGraphics::CEntry*>(
                reinterpret_cast<std::byte*>(position) + 0x14);
            CopyEntryWords(position, &local_value);
            position = next;
        }
    }
}