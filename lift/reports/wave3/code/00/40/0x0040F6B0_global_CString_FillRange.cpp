#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <type_traits>

struct CStringRangeOverlay {
    char* begin;
    char* end;
    char* capacity_end;
};

static_assert(std::is_standard_layout_v<CStringRangeOverlay>);
static_assert(offsetof(CStringRangeOverlay, begin) == 0x0);
static_assert(offsetof(CStringRangeOverlay, end) == 0x4);
static_assert(offsetof(CStringRangeOverlay, capacity_end) == 0x8);
static_assert(sizeof(CStringRangeOverlay) == 0xC);

void __thiscall CString_FillRange(
    CStringRangeOverlay* self,
    void* insert_at,
    const std::uint8_t* fill_byte_ptr,
    std::uint32_t /*unused*/,
    std::uint32_t fill_count,
    char skip_tail_copy)
{
    const std::uint8_t fill_byte = *fill_byte_ptr;
    const std::uint32_t old_length =
        static_cast<std::uint32_t>(self->end - self->begin);

    const std::uint32_t growth_base =
        (fill_count <= old_length) ? old_length : fill_count;
    const std::uint32_t allocation_size = growth_base + old_length;

    void* new_buffer = (allocation_size == 0)
        ? nullptr
        : std::malloc(allocation_size);

    void* write_ptr = new_buffer;
    void* const old_begin = self->begin;

    if (insert_at != old_begin) {
        void* const moved_prefix = std::memmove(
            new_buffer,
            old_begin,
            static_cast<std::size_t>(
                static_cast<const char*>(insert_at) - static_cast<const char*>(old_begin)));
        write_ptr = static_cast<char*>(moved_prefix) +
                    (static_cast<const char*>(insert_at) - static_cast<const char*>(old_begin));
    }

    std::uint32_t* dword_fill_ptr = static_cast<std::uint32_t*>(write_ptr);
    for (std::uint32_t count = fill_count >> 2; count != 0; --count) {
        *dword_fill_ptr = static_cast<std::uint32_t>(fill_byte) |
                          (static_cast<std::uint32_t>(fill_byte) << 8) |
                          (static_cast<std::uint32_t>(fill_byte) << 16) |
                          (static_cast<std::uint32_t>(fill_byte) << 24);
        ++dword_fill_ptr;
    }

    for (std::uint32_t count = fill_count & 3; count != 0; --count) {
        *reinterpret_cast<std::uint8_t*>(dword_fill_ptr) = fill_byte;
        dword_fill_ptr = reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uint8_t*>(dword_fill_ptr) + 1);
    }

    char* new_end = static_cast<char*>(write_ptr) + fill_count;
    if ((skip_tail_copy == '\0') && (self->end != insert_at)) {
        const std::size_t tail_size =
            static_cast<std::size_t>(self->end - static_cast<char*>(insert_at));
        void* const moved_tail = std::memmove(new_end, insert_at, tail_size);
        new_end = static_cast<char*>(moved_tail) + tail_size;
    }

    if (self->begin != nullptr) {
        std::free(self->begin);
    }

    self->begin = static_cast<char*>(new_buffer);
    self->end = new_end;
    self->capacity_end = static_cast<char*>(new_buffer) + allocation_size;
}