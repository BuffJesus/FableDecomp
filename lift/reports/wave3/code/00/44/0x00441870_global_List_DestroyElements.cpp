#include <cstddef>
#include <cstdlib>

struct ListDestroyElementSlot {
    using DestroyThunk = void(__thiscall*)(ListDestroyElementSlot*, int);

    void** header;
    std::byte unknown_04[0x4];
};

static_assert(sizeof(ListDestroyElementSlot) == 0x8);
static_assert(offsetof(ListDestroyElementSlot, header) == 0x0);

struct ListDestroyElementsOverlay {
    std::byte pad_00[0x1C];
    ListDestroyElementSlot* begin_1C;
    ListDestroyElementSlot* end_20;
    std::byte pad_24[0x4];
    ListDestroyElementSlot* begin_28;
    ListDestroyElementSlot* end_2C;
    std::byte pad_30[0x4];
    ListDestroyElementSlot* begin_34;
    ListDestroyElementSlot* end_38;
};

static_assert(offsetof(ListDestroyElementsOverlay, begin_1C) == 0x1C);
static_assert(offsetof(ListDestroyElementsOverlay, end_20) == 0x20);
static_assert(offsetof(ListDestroyElementsOverlay, begin_28) == 0x28);
static_assert(offsetof(ListDestroyElementsOverlay, end_2C) == 0x2C);
static_assert(offsetof(ListDestroyElementsOverlay, begin_34) == 0x34);
static_assert(offsetof(ListDestroyElementsOverlay, end_38) == 0x38);

void __fastcall List_DestroyElements(ListDestroyElementsOverlay* self) {
    ListDestroyElementSlot* end = self->end_38;
    for (ListDestroyElementSlot* it = self->begin_34; it != end; ++it) {
        const auto destroy = reinterpret_cast<ListDestroyElementSlot::DestroyThunk>(it->header[0]);
        destroy(it, 0);
    }
    if (self->begin_34 != nullptr) {
        std::free(self->begin_34);
    }

    end = self->end_2C;
    for (ListDestroyElementSlot* it = self->begin_28; it != end; ++it) {
        const auto destroy = reinterpret_cast<ListDestroyElementSlot::DestroyThunk>(it->header[0]);
        destroy(it, 0);
    }
    if (self->begin_28 != nullptr) {
        std::free(self->begin_28);
    }

    end = self->end_20;
    for (ListDestroyElementSlot* it = self->begin_1C; it != end; ++it) {
        const auto destroy = reinterpret_cast<ListDestroyElementSlot::DestroyThunk>(it->header[0]);
        destroy(it, 0);
    }
    if (self->begin_1C != nullptr) {
        std::free(self->begin_1C);
    }
}