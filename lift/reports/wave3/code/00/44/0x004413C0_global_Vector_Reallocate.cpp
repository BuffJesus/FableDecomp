#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>

struct RefCountedOverlay {
    std::uint32_t pad_0x0;
    std::int32_t ref_count;
};
static_assert(offsetof(RefCountedOverlay, ref_count) == 0x4);

struct ScriptThingOverlay;
using ScriptThingDtor = void(__thiscall*)(ScriptThingOverlay*, unsigned int);

struct ScriptThingOverlay {
    ScriptThingDtor* vftable;
    RefCountedOverlay* ref_object;
};
static_assert(offsetof(ScriptThingOverlay, vftable) == 0x0);
static_assert(offsetof(ScriptThingOverlay, ref_object) == 0x4);
static_assert(sizeof(ScriptThingOverlay) == 0x8);

struct VectorOverlay {
    ScriptThingOverlay* begin;
    ScriptThingOverlay* end;
    ScriptThingOverlay* capacity_end;
};
static_assert(offsetof(VectorOverlay, begin) == 0x0);
static_assert(offsetof(VectorOverlay, end) == 0x4);
static_assert(offsetof(VectorOverlay, capacity_end) == 0x8);
static_assert(sizeof(VectorOverlay) == 0xC);

extern ScriptThingDtor PTR__scalar_deleting_destructor__0122f598[];

void __thiscall Vector_Reallocate(
    VectorOverlay* self,
    ScriptThingOverlay* insert_pos,
    const ScriptThingOverlay* value,
    std::uint32_t /*unused*/,
    std::uint32_t insert_count,
    char skip_suffix_copy)
{
    std::uint32_t old_count = static_cast<std::uint32_t>(self->end - self->begin);
    std::uint32_t growth = insert_count;

    if (insert_count <= old_count) {
        growth = old_count;
    }

    std::uint32_t new_capacity = growth + old_count;

    ScriptThingOverlay* new_begin;
    if (new_capacity == 0) {
        new_begin = nullptr;
    } else {
        new_begin = static_cast<ScriptThingOverlay*>(std::malloc(new_capacity * 8));
    }

    ScriptThingOverlay* new_current = std::uninitialized_copy(self->begin, insert_pos, new_begin);

    if (insert_count == 1) {
        if (new_current != nullptr) {
            new_current->vftable = PTR__scalar_deleting_destructor__0122f598;
            RefCountedOverlay* const ref_object = value->ref_object;
            new_current->ref_object = ref_object;
            if (ref_object != nullptr) {
                ++ref_object->ref_count;
            }
        }
        new_current = new_current + 1;
    } else {
        new_current = std::uninitialized_fill_n(new_current, insert_count, *value);
    }

    if (skip_suffix_copy == '\0') {
        new_current = std::uninitialized_copy(insert_pos, self->end, new_current);
    }

    for (ScriptThingOverlay* it = self->begin; it != self->end; ++it) {
        it->vftable[0](it, 0);
    }

    if (self->begin != nullptr) {
        std::free(self->begin);
    }

    self->end = new_current;
    self->begin = new_begin;
    self->capacity_end = new_begin + new_capacity;
}