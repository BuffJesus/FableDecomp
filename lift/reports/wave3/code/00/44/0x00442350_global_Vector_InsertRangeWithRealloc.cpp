#include <cstddef>
#include <cstdint>

struct CountedOverlay;

struct ElementOverlay {
    std::uint32_t word0;
    CountedOverlay* counted;
};

static_assert(offsetof(ElementOverlay, word0) == 0x0);
static_assert(offsetof(ElementOverlay, counted) == 0x4);
static_assert(sizeof(ElementOverlay) == 0x8);

struct CountedVftableOverlay {
    void* slot00;
    void (__thiscall* slot04)(void* self);
    void* slot08;
    void (__thiscall* slot0C)(void* self, ElementOverlay* value);
};

static_assert(offsetof(CountedVftableOverlay, slot04) == 0x4);
static_assert(offsetof(CountedVftableOverlay, slot0C) == 0xC);

struct CountedOverlay {
    CountedVftableOverlay* vftable;
    std::int32_t ref_count;
};

static_assert(offsetof(CountedOverlay, vftable) == 0x0);
static_assert(offsetof(CountedOverlay, ref_count) == 0x4);

struct VectorOverlay {
    ElementOverlay* begin;
    ElementOverlay* end;
    ElementOverlay* capacity;
};

static_assert(offsetof(VectorOverlay, begin) == 0x0);
static_assert(offsetof(VectorOverlay, end) == 0x4);
static_assert(offsetof(VectorOverlay, capacity) == 0x8);

extern "C" std::byte PTR__scalar_deleting_destructor__0122f598;

extern "C" void __cdecl ghidra_std_uninit_copy(
    ElementOverlay* first,
    ElementOverlay* last,
    ElementOverlay* dest,
    void* opaque_out);

extern "C" void __cdecl ghidra_std_move_5(
    ElementOverlay* first,
    ElementOverlay* last,
    ElementOverlay* dest,
    void* opaque_out,
    int zero);

extern "C" void __cdecl ghidra_std_uninit_fill_n(
    ElementOverlay* dest,
    std::uint32_t count,
    const ElementOverlay* value);

extern "C" void __cdecl ghidra_std_move_3(
    ElementOverlay* first,
    ElementOverlay* last,
    ElementOverlay* value);

extern "C" void __thiscall Vector_Reallocate(
    VectorOverlay* self,
    ElementOverlay* insert_at,
    const ElementOverlay* value_src,
    const ElementOverlay** value_src_out,
    std::uint32_t count,
    int zero);

extern "C" void __thiscall ghidra_local_value_cleanup(ElementOverlay* value);

void __thiscall Vector_InsertRangeWithRealloc(
    VectorOverlay* self,
    ElementOverlay* insert_at,
    std::uint32_t count,
    const ElementOverlay* value_src)
{
    if (count != 0) {
        if (count <= static_cast<std::uint32_t>(
                         (reinterpret_cast<std::uintptr_t>(self->capacity) -
                          reinterpret_cast<std::uintptr_t>(self->end)) >>
                         3)) {
            ElementOverlay local_value{};
            local_value.counted = value_src->counted;
            local_value.word0 = static_cast<std::uint32_t>(
                reinterpret_cast<std::uintptr_t>(&PTR__scalar_deleting_destructor__0122f598));

            if (local_value.counted != nullptr) {
                local_value.counted->ref_count = local_value.counted->ref_count + 1;
            }

            const std::uint32_t tail_count = static_cast<std::uint32_t>(
                (reinterpret_cast<std::uintptr_t>(self->end) -
                 reinterpret_cast<std::uintptr_t>(insert_at)) >>
                3);

            ElementOverlay* old_end = self->end;
            ElementOverlay* fill_end = old_end;

            if (count < tail_count) {
                ElementOverlay* move_src = reinterpret_cast<ElementOverlay*>(
                    reinterpret_cast<std::uintptr_t>(old_end) +
                    count * static_cast<std::intptr_t>(-8));

                ghidra_std_uninit_copy(move_src, old_end, old_end, &move_src);
                self->end = reinterpret_cast<ElementOverlay*>(
                    reinterpret_cast<std::uintptr_t>(self->end) + count * 8);
                ghidra_std_move_5(insert_at, move_src, old_end, &move_src, 0);
                fill_end = reinterpret_cast<ElementOverlay*>(
                    reinterpret_cast<std::uintptr_t>(insert_at) + count * 8);
            } else {
                const std::uint32_t spill_count = count - tail_count;
                ghidra_std_uninit_fill_n(old_end, spill_count, &local_value);

                ElementOverlay* grown_end = reinterpret_cast<ElementOverlay*>(
                    reinterpret_cast<std::uintptr_t>(self->end) + spill_count * 8);
                self->end = grown_end;

                ghidra_std_uninit_copy(
                    insert_at,
                    old_end,
                    grown_end,
                    const_cast<std::uint32_t*>(&spill_count));

                self->end = reinterpret_cast<ElementOverlay*>(
                    reinterpret_cast<std::uintptr_t>(self->end) + tail_count * 8);
            }

            ghidra_std_move_3(insert_at, fill_end, &local_value);

            local_value.word0 = static_cast<std::uint32_t>(
                reinterpret_cast<std::uintptr_t>(&PTR__scalar_deleting_destructor__0122f598));

            if (local_value.counted != nullptr) {
                if (local_value.counted->ref_count == 1) {
                    local_value.counted->vftable->slot0C(local_value.counted, &local_value);
                }

                if (local_value.counted != nullptr) {
                    const std::int32_t old_ref = local_value.counted->ref_count;
                    local_value.counted->ref_count = old_ref - 1;
                    if (old_ref - 1 == 0) {
                        local_value.counted->vftable->slot04(local_value.counted);
                    }
                }
            }

            local_value.counted = nullptr;
            ghidra_local_value_cleanup(&local_value);
            return;
        }

        Vector_Reallocate(self, insert_at, value_src, &value_src, count, 0);
    }
}