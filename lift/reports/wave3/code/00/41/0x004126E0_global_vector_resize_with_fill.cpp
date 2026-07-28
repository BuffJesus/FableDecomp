#include <cstddef>
#include <cstdint>
#include <new>

struct CCharString {
    ~CCharString();
};

struct CWideString {
    CWideString(const CWideString&);
};

struct VectorResizeWithFillOverlay {
    std::byte pad_00[0x04];
    CWideString* finish;          // +0x04
    CWideString* end_of_storage;  // +0x08
};

static_assert(offsetof(VectorResizeWithFillOverlay, finish) == 0x04);
static_assert(offsetof(VectorResizeWithFillOverlay, end_of_storage) == 0x08);

// Matches the observed 4-argument helper shape from the binary.
CWideString* std__Uninit_copy(
    CWideString* first,
    CWideString* last,
    CWideString* dest,
    CWideString** scratch_out);

// Matches std::_Fill_n<unsigned_long*,unsigned_int,unsigned_long>(...)
void std__Fill_n(
    CWideString* dest,
    std::uint32_t count,
    const CWideString* value);

// Matches std::fill<...>(...)
void std__fill(
    CWideString* first,
    CWideString* last,
    const CWideString* value);

void CWideString_FillBackward(CWideString* first, CWideString** scratch);
void Std_Vector_Insert_Range_Copies(
    CWideString* position,
    const CWideString* value,
    const std::uint32_t* count_ptr,
    std::uint32_t count,
    int zero);

void __thiscall vector_resize_with_fill(
    void* self_,
    CWideString* position,
    std::uint32_t count,
    const CWideString* value)
{
    auto* const self = static_cast<VectorResizeWithFillOverlay*>(self_);

    if (count != 0) {
        if (count <= static_cast<std::uint32_t>(self->end_of_storage - self->finish)) {
            alignas(CWideString) std::byte value_copy_storage[sizeof(CWideString)];
            auto* const value_copy = reinterpret_cast<CWideString*>(value_copy_storage);
            ::new (value_copy) CWideString(*value);

            CWideString* fill_last = self->finish;
            const std::uint32_t elems_after =
                static_cast<std::uint32_t>(self->finish - position);

            if (count < elems_after) {
                const std::ptrdiff_t count_delta = static_cast<std::ptrdiff_t>(count);
                CWideString* scratch = fill_last - count_delta;

                std__Uninit_copy(scratch, fill_last, fill_last, &scratch);
                self->finish = self->finish + count_delta;
                CWideString_FillBackward(fill_last, &scratch);
                fill_last = position + count_delta;
            } else {
                std::uint32_t remaining = count - elems_after;

                std__Fill_n(fill_last, remaining, value_copy);

                CWideString* const new_finish =
                    self->finish + static_cast<std::ptrdiff_t>(remaining);
                self->finish = new_finish;

                std__Uninit_copy(
                    position,
                    fill_last,
                    new_finish,
                    reinterpret_cast<CWideString**>(&remaining));
                self->finish =
                    self->finish + static_cast<std::ptrdiff_t>(elems_after);
            }

            std__fill(position, fill_last, value_copy);
            reinterpret_cast<CCharString*>(value_copy)->~CCharString();
            return;
        }

        Std_Vector_Insert_Range_Copies(position, value, &count, count, 0);
    }
}