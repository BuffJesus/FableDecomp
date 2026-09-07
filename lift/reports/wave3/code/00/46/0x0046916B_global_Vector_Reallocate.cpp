#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>

struct CRGBColour;

struct VectorOverlay {
    CRGBColour* begin;
    CRGBColour* end;
    CRGBColour* capacity_end;
};

static_assert(offsetof(VectorOverlay, begin) == 0x0);
static_assert(offsetof(VectorOverlay, end) == 0x4);
static_assert(offsetof(VectorOverlay, capacity_end) == 0x8);

void __thiscall Vector_Reallocate(
    VectorOverlay* self,
    CRGBColour* insert_pos,
    const CRGBColour* value,
    unsigned int /*unused*/,
    std::size_t insert_count,
    unsigned char skip_suffix_copy)
{
    const std::size_t old_count =
        (static_cast<std::uintptr_t>(reinterpret_cast<std::uintptr_t>(self->end)) -
         static_cast<std::uintptr_t>(reinterpret_cast<std::uintptr_t>(self->begin))) >> 2;

    const std::size_t growth = (insert_count <= old_count) ? old_count : insert_count;
    const std::size_t new_capacity = growth + old_count;

    CRGBColour* new_begin =
        (new_capacity == 0)
            ? nullptr
            : static_cast<CRGBColour*>(std::malloc(new_capacity * 4));

    CRGBColour* new_cur = new_begin;

    for (CRGBColour* src = self->begin; src != insert_pos; ++src) {
        std::construct_at(new_cur, *src);
        ++new_cur;
    }

    if (insert_count == 1) {
        std::construct_at(new_cur, *value);
        ++new_cur;
    } else {
        for (std::size_t remaining = insert_count; remaining != 0; --remaining) {
            std::construct_at(new_cur, *value);
            ++new_cur;
        }
    }

    if (static_cast<char>(skip_suffix_copy) == '\0') {
        CRGBColour* old_end = self->end;
        for (CRGBColour* src = insert_pos; src != old_end; ++src) {
            std::construct_at(new_cur, *src);
            ++new_cur;
        }
    }

    if (self->begin != nullptr) {
        std::free(self->begin);
    }

    self->begin = new_begin;
    self->end = new_cur;
    self->capacity_end =
        (new_capacity == 0)
            ? nullptr
            : reinterpret_cast<CRGBColour*>(
                  static_cast<std::uintptr_t>(reinterpret_cast<std::uintptr_t>(new_begin)) +
                  new_capacity * 4);
}