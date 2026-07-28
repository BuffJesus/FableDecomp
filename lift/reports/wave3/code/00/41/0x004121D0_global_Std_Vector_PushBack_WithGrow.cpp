#include <cstddef>
#include <cstdint>

struct StdVector4ByteOverlay {
    std::byte pad_0x00[0x04];
    std::uint32_t* end;
    std::uint32_t* capacity_end;
};

static_assert(offsetof(StdVector4ByteOverlay, end) == 0x04);
static_assert(offsetof(StdVector4ByteOverlay, capacity_end) == 0x08);

void __cdecl Std_Vector_Insert_Range_Copies_4byte(
    std::uint32_t* insert_at,
    const std::uint32_t* value_ptr,
    const std::uint32_t* const* value_ptr_ref,
    int count_a,
    int count_b);

void __thiscall Std_Vector_PushBack_WithGrow(StdVector4ByteOverlay* self, const std::uint32_t* value_ptr)
{
    std::uint32_t* end = self->end;

    if (end != self->capacity_end) {
        if (end != nullptr) {
            *end = *value_ptr;
        }

        self->end = self->end + 1;
        return;
    }

    Std_Vector_Insert_Range_Copies_4byte(end, value_ptr, &value_ptr, 1, 1);
}