#include <cstddef>
#include <cstdint>

struct CCharString;

void __thiscall Std_Vector_Erase_Range(void* this_, int* first, int* last);
void __thiscall vector_resize_with_fill(void* this_, int* end, unsigned int count, std::uint32_t fill_value);

struct StdVectorOverlay {
    int* first;
    int* last;
};

static_assert(offsetof(StdVectorOverlay, first) == 0x0);
static_assert(offsetof(StdVectorOverlay, last) == 0x4);

void __thiscall Std_Vector_Resize(StdVectorOverlay* self, unsigned int new_size)
{
    StdVectorOverlay* local_4 = self;
    const std::uint32_t fill_value =
        CCharString::CCharString(reinterpret_cast<CCharString*>(&local_4));

    int* const first = self->first;
    if (new_size < static_cast<unsigned int>((self->last - first))) {
        Std_Vector_Erase_Range(self, first + new_size, self->last);
        CCharString::~CCharString(reinterpret_cast<CCharString*>(&local_4));
        return;
    }

    vector_resize_with_fill(
        self,
        self->last,
        new_size - static_cast<unsigned int>((self->last - first)),
        fill_value);

    CCharString::~CCharString(reinterpret_cast<CCharString*>(&local_4));
}