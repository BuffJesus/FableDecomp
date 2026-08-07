#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <type_traits>

struct ArrayOverlay {
    std::uint32_t begin;
    std::uint32_t current;
    std::uint32_t end;
};

static_assert(std::is_standard_layout_v<ArrayOverlay>);
static_assert(offsetof(ArrayOverlay, begin) == 0x0);
static_assert(offsetof(ArrayOverlay, current) == 0x4);
static_assert(offsetof(ArrayOverlay, end) == 0x8);
static_assert(sizeof(ArrayOverlay) == 0xC);

ArrayOverlay* __thiscall Array_Initialize(ArrayOverlay* self, int count) {
    void* allocation = nullptr;

    self->begin = 0;
    self->current = 0;
    self->end = 0;

    if (count != 0) {
        allocation = std::malloc(static_cast<std::uint32_t>(count) << 2);
    }

    const auto allocation_value =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(allocation));

    self->begin = allocation_value;
    self->current = allocation_value;
    self->end = allocation_value + static_cast<std::uint32_t>(count) * 4;

    return self;
}