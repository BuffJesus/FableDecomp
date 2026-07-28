#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

struct StdDequeOverlay {
    std::byte pad_00[0x04];
    std::int32_t value_at_begin;       // 0x04
    std::int32_t value_at_begin_end;   // 0x08
    std::uint32_t begin_raw;           // 0x0C
    std::byte pad_10[0x04];
    std::int32_t value_at_end;         // 0x14
    std::int32_t value_at_end_end;     // 0x18
    std::uint32_t current_raw;         // 0x1C
    std::uint32_t storage_raw;         // 0x20
    std::uint32_t capacity;            // 0x24
};

static_assert(offsetof(StdDequeOverlay, value_at_begin) == 0x04);
static_assert(offsetof(StdDequeOverlay, value_at_begin_end) == 0x08);
static_assert(offsetof(StdDequeOverlay, begin_raw) == 0x0C);
static_assert(offsetof(StdDequeOverlay, value_at_end) == 0x14);
static_assert(offsetof(StdDequeOverlay, value_at_end_end) == 0x18);
static_assert(offsetof(StdDequeOverlay, current_raw) == 0x1C);
static_assert(offsetof(StdDequeOverlay, storage_raw) == 0x20);
static_assert(offsetof(StdDequeOverlay, capacity) == 0x24);

void __thiscall Std_Deque_Resize_WithGap(void* self_ptr, std::uint32_t gap_count, char bias_to_front) {
    auto* const self = static_cast<StdDequeOverlay*>(self_ptr);

    auto* current = reinterpret_cast<std::int32_t*>(static_cast<std::uintptr_t>(self->current_raw));
    auto* begin = reinterpret_cast<std::int32_t*>(static_cast<std::uintptr_t>(self->begin_raw));

    const std::int32_t element_index = static_cast<std::int32_t>(current - begin);
    const std::int32_t element_count = element_index + 1;
    const std::int32_t new_span = static_cast<std::int32_t>(gap_count) + element_count;
    const std::uint32_t capacity = self->capacity;

    std::int32_t* dst;

    if ((capacity > (static_cast<std::uint32_t>(new_span) * 2u))) {
        dst = reinterpret_cast<std::int32_t*>(
            static_cast<std::uintptr_t>(self->storage_raw) +
            ((((capacity - static_cast<std::uint32_t>(new_span)) >> 1) +
              (-(static_cast<std::uint32_t>(bias_to_front != '\0')) & gap_count)) * sizeof(std::int32_t)));

        if (dst < begin) {
            if ((current + 1) != begin) {
                std::memmove(dst, begin, static_cast<std::size_t>(current - begin + 1) * sizeof(std::int32_t));
            }
        } else {
            const std::size_t move_size =
                static_cast<std::size_t>((reinterpret_cast<std::uintptr_t>(current) -
                                          reinterpret_cast<std::uintptr_t>(begin)) +
                                         sizeof(std::int32_t));
            if (move_size > 0) {
                std::memmove(
                    reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(dst) +
                                            ((element_count * static_cast<std::int32_t>(sizeof(std::int32_t))) -
                                             static_cast<std::int32_t>(move_size))),
                    begin,
                    move_size);
            }
        }
    } else {
        const std::uint32_t* growth_source = &self->capacity;
        if (self->capacity < gap_count) {
            growth_source = &gap_count;
        }

        const std::uint32_t new_capacity = capacity + 2u + *growth_source;
        void* new_storage;

        if (new_capacity == 0) {
            new_storage = nullptr;
        } else {
            new_storage = std::malloc(static_cast<std::size_t>(new_capacity) * sizeof(std::int32_t));
        }

        void* const src = reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->begin_raw));
        dst = reinterpret_cast<std::int32_t*>(
            reinterpret_cast<std::uintptr_t>(new_storage) +
            ((((new_capacity - static_cast<std::uint32_t>(new_span)) >> 1) +
              (-(static_cast<std::uint32_t>(bias_to_front != '\0')) & gap_count)) * sizeof(std::int32_t)));

        void* const src_end = reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(current) + sizeof(std::int32_t));
        if (src_end != src) {
            std::memmove(dst, src, reinterpret_cast<std::uintptr_t>(src_end) - reinterpret_cast<std::uintptr_t>(src));
        }

        if (self->storage_raw != 0) {
            std::free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->storage_raw)));
        }

        self->storage_raw = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(new_storage));
        self->capacity = new_capacity;
    }

    self->begin_raw = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(dst));

    std::int32_t value = *dst;
    self->value_at_begin = value;
    self->value_at_begin_end = value + 0x80;

    self->current_raw = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(dst + element_index));

    value = dst[element_index];
    self->value_at_end = value;
    self->value_at_end_end = value + 0x80;
}