#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct Vector8Storage32 {
    std::uint32_t begin;
    std::uint32_t end;
    std::uint32_t capacity_end;
};

struct Vector8Element {
    std::uint32_t word0;
    std::uint32_t word1;
};

static_assert(offsetof(Vector8Storage32, begin) == 0x0);
static_assert(offsetof(Vector8Storage32, end) == 0x4);
static_assert(offsetof(Vector8Storage32, capacity_end) == 0x8);
static_assert(sizeof(Vector8Storage32) == 0xC);
static_assert(sizeof(Vector8Element) == 0x8);

static inline Vector8Element* AsElementPtr(std::uint32_t address) {
    return reinterpret_cast<Vector8Element*>(static_cast<std::uintptr_t>(address));
}

void __thiscall Vector_Reallocate(
    Vector8Storage32* self,
    Vector8Element* insert_at,
    const Vector8Element* value,
    std::uint32_t /*unused*/,
    std::uint32_t count,
    char preserve_tail)
{
    const std::uint32_t old_size = (self->end - self->begin) >> 3;
    const std::uint32_t growth = (count <= old_size) ? old_size : count;
    const std::uint32_t new_capacity = growth + old_size;

    Vector8Element* new_begin;
    if (new_capacity == 0) {
        new_begin = nullptr;
    } else {
        new_begin = static_cast<Vector8Element*>(std::malloc(new_capacity * 8u));
    }

    Vector8Element* write_ptr = new_begin;
    Vector8Element* src = AsElementPtr(self->begin);

    for (; src != insert_at; ++src) {
        if (write_ptr != nullptr) {
            write_ptr->word0 = src->word0;
            write_ptr->word1 = src->word1;
        }
        write_ptr += 1;
    }

    if (count == 1) {
        if (write_ptr != nullptr) {
            write_ptr->word0 = value->word0;
            write_ptr->word1 = value->word1;
        }
        write_ptr += 1;
    } else {
        for (; count != 0; --count) {
            if (write_ptr != nullptr) {
                write_ptr->word0 = value->word0;
                write_ptr->word1 = value->word1;
            }
            write_ptr += 1;
        }
    }

    if (preserve_tail == '\0') {
        Vector8Element* const old_end = AsElementPtr(self->end);
        if (insert_at != old_end) {
            const std::intptr_t delta =
                reinterpret_cast<std::intptr_t>(insert_at) -
                reinterpret_cast<std::intptr_t>(write_ptr);

            do {
                if (write_ptr != nullptr) {
                    write_ptr->word0 =
                        *reinterpret_cast<const std::uint32_t*>(delta + reinterpret_cast<std::intptr_t>(write_ptr));
                    write_ptr->word1 =
                        *reinterpret_cast<const std::uint32_t*>(delta + 4 + reinterpret_cast<std::intptr_t>(write_ptr));
                }
                write_ptr += 1;
            } while (reinterpret_cast<Vector8Element*>(delta + reinterpret_cast<std::intptr_t>(write_ptr)) != old_end);
        }
    }

    if (self->begin != 0) {
        std::free(AsElementPtr(self->begin));
    }

    self->begin = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(new_begin));
    self->end = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(write_ptr));
    self->capacity_end =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(new_begin + new_capacity));
}