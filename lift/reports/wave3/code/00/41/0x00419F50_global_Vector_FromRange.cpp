#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct U32Vector {
    std::uint32_t* begin;
    std::uint32_t* end;
    std::uint32_t* capacity_end;
};
static_assert(offsetof(U32Vector, begin) == 0x0);
static_assert(offsetof(U32Vector, end) == 0x4);
static_assert(offsetof(U32Vector, capacity_end) == 0x8);
static_assert(sizeof(U32Vector) == 0xC);

struct U32Range {
    std::uint32_t* first;
    std::uint32_t* last;
};
static_assert(offsetof(U32Range, first) == 0x0);
static_assert(offsetof(U32Range, last) == 0x4);
static_assert(sizeof(U32Range) == 0x8);

U32Vector* __thiscall Vector_FromRange(U32Vector* self, const U32Range* range)
{
    const std::int32_t count =
        (static_cast<std::int32_t>(reinterpret_cast<std::uint32_t>(range->last)) -
         static_cast<std::int32_t>(reinterpret_cast<std::uint32_t>(range->first))) >> 2;

    self->begin = nullptr;
    self->end = nullptr;
    self->capacity_end = nullptr;

    std::uint32_t* storage;
    if (count == 0) {
        storage = nullptr;
    } else {
        storage = static_cast<std::uint32_t*>(std::malloc(static_cast<std::size_t>(count) * 4u));
    }

    self->begin = storage;
    self->end = storage;
    self->capacity_end = reinterpret_cast<std::uint32_t*>(
        reinterpret_cast<std::uint32_t>(storage) + static_cast<std::uint32_t>(count * 4));

    std::uint32_t* dst = storage;
    std::uint32_t* src = range->first;
    std::uint32_t* const src_end = range->last;

    for (; src != src_end; src = reinterpret_cast<std::uint32_t*>(
             reinterpret_cast<std::uint32_t>(src) + 4u)) {
        if (dst != nullptr) {
            *dst = *src;
        }

        dst = reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uint32_t>(dst) + 4u);
    }

    self->end = dst;
    return self;
}