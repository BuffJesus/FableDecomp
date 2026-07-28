#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <type_traits>

struct Vector28Overlay {
    void* first; // +0x00
    void* last;  // +0x04
    void* end;   // +0x08
};

static_assert(offsetof(Vector28Overlay, first) == 0x00);
static_assert(offsetof(Vector28Overlay, last) == 0x04);
static_assert(offsetof(Vector28Overlay, end) == 0x08);
static_assert(sizeof(Vector28Overlay) == 0x0C);

struct Element28 {
    std::uint32_t word[7];
};

static_assert(sizeof(Element28) == 0x1C);

void __thiscall Std_Vector_Insert_Range_Copies_28byte(
    Vector28Overlay* self,
    Element28* insert_at,
    const Element28* source_value,
    std::uint32_t /*unused_param_4*/,
    std::uint32_t copy_count,
    char skip_suffix_copy)
{
    std::uint32_t old_count =
        (static_cast<std::uint32_t>(
             reinterpret_cast<std::uintptr_t>(self->last) -
             reinterpret_cast<std::uintptr_t>(self->first)) /
         0x1Cu);

    Vector28Overlay* const self_saved = self;
    std::uint32_t growth = copy_count;
    if (copy_count <= old_count) {
        growth = old_count;
    }

    const std::uint32_t new_capacity_count = growth + old_count;

    Element28* new_storage;
    if (new_capacity_count == 0) {
        new_storage = nullptr;
    } else {
        new_storage = static_cast<Element28*>(std::malloc(new_capacity_count * 0x1Cu));
    }

    Element28* out = new_storage;

    for (Element28* cur = static_cast<Element28*>(self->first); cur != insert_at; ++cur) {
        if (out != nullptr) {
            for (int i = 0; i != 7; ++i) {
                out->word[i] = cur->word[i];
            }
        }
        ++out;
    }

    if (copy_count == 1) {
        if (out != nullptr) {
            for (int i = 0; i != 7; ++i) {
                out->word[i] = source_value->word[i];
            }
        }
        ++out;
    } else {
        for (; copy_count != 0; --copy_count) {
            if (out != nullptr) {
                for (int i = 0; i != 7; ++i) {
                    out->word[i] = source_value->word[i];
                }
            }
            ++out;
        }
    }

    if (skip_suffix_copy == '\0') {
        Element28* const old_last = static_cast<Element28*>(self_saved->last);
        self = self_saved;
        if (insert_at != old_last) {
            const std::ptrdiff_t delta =
                reinterpret_cast<const std::byte*>(insert_at) -
                reinterpret_cast<const std::byte*>(out);

            do {
                if (out != nullptr) {
                    const auto* src =
                        reinterpret_cast<const Element28*>(
                            reinterpret_cast<const std::byte*>(out) + delta);
                    for (int i = 0; i != 7; ++i) {
                        out->word[i] = src->word[i];
                    }
                }
                ++out;
            } while (reinterpret_cast<const Element28*>(
                         reinterpret_cast<const std::byte*>(out) + delta) != old_last);
        }
    }

    if (self->first != nullptr) {
        std::free(self->first);
    }

    self->end = new_storage + new_capacity_count;
    self->last = out;
    self->first = new_storage;
}