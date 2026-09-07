#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

struct VectorUint32Overlay {
    std::uint32_t* begin;
    std::uint32_t* end;
    std::uint32_t* capacity_end;
};

static_assert(offsetof(VectorUint32Overlay, begin) == 0x0);
static_assert(offsetof(VectorUint32Overlay, end) == 0x4);
static_assert(offsetof(VectorUint32Overlay, capacity_end) == 0x8);
static_assert(sizeof(VectorUint32Overlay) == 0xC);

void __thiscall Vector_InsertN_uint32(
    VectorUint32Overlay* self,
    void* insert_at,
    std::uint32_t* value_src,
    std::uint32_t /*unused*/,
    std::uint32_t insert_count,
    char skip_tail_move
) {
    const std::uint32_t current_count = static_cast<std::uint32_t>(
        (reinterpret_cast<std::uintptr_t>(self->end) -
         reinterpret_cast<std::uintptr_t>(self->begin)) >> 2
    );

    std::uint32_t selected_count = insert_count;
    if (insert_count <= current_count) {
        selected_count = current_count;
    }

    const std::uint32_t new_count = selected_count + current_count;

    std::uint32_t* new_begin;
    if (new_count == 0) {
        new_begin = nullptr;
    } else {
        new_begin = static_cast<std::uint32_t*>(std::malloc(static_cast<std::size_t>(new_count) * 4));
    }

    void* old_begin = self->begin;
    std::uint32_t* dst = new_begin;
    std::uint32_t remaining = insert_count;

    if (insert_at != old_begin) {
        const std::size_t prefix_size =
            static_cast<std::size_t>(
                reinterpret_cast<std::uintptr_t>(insert_at) -
                reinterpret_cast<std::uintptr_t>(old_begin)
            );
        void* moved_prefix = std::memmove(new_begin, old_begin, prefix_size);
        dst = reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uintptr_t>(moved_prefix) + prefix_size
        );
        remaining = insert_count;
    }

    for (; remaining != 0; --remaining) {
        *dst = *value_src;
        ++dst;
    }

    if ((skip_tail_move == '\0') && (self->end != insert_at)) {
        const std::size_t tail_size =
            static_cast<std::size_t>(
                reinterpret_cast<std::uintptr_t>(self->end) -
                reinterpret_cast<std::uintptr_t>(insert_at)
            );
        void* moved_tail = std::memmove(dst, insert_at, tail_size);
        dst = reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uintptr_t>(moved_tail) + tail_size
        );
    }

    if (self->begin != nullptr) {
        std::free(self->begin);
    }

    self->begin = new_begin;
    self->end = dst;
    if (new_count == 0) {
        self->capacity_end = nullptr;
    } else {
        self->capacity_end = new_begin + new_count;
    }
}