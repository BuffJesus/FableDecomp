#include <cstddef>
#include <cstdint>

struct VectorOverlay {
    void* field_0x0;
    void* field_0x4;
};

static_assert(sizeof(void*) == 4);
static_assert(offsetof(VectorOverlay, field_0x4) == 0x4);

std::uint32_t* __thiscall Vector_Erase(
    VectorOverlay* self,
    std::uint32_t* element,
    std::uint32_t* next_element)
{
    int remaining = (
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self->field_0x4)) -
        static_cast<int>(reinterpret_cast<std::uintptr_t>(next_element))
    ) / 0x14;

    std::uint32_t* new_end = element;

    if (0 < remaining) {
        std::uint32_t* current = element;
        do {
            new_end = current + 5;
            remaining = remaining + -1;

            std::uint32_t* source = reinterpret_cast<std::uint32_t*>(
                (reinterpret_cast<std::uintptr_t>(next_element) - reinterpret_cast<std::uintptr_t>(element)) +
                reinterpret_cast<std::uintptr_t>(current));

            for (int copy_count = 5; copy_count != 0; copy_count = copy_count + -1) {
                *current = *source;
                source = source + 1;
                current = current + 1;
            }

            current = new_end;
        } while (remaining != 0);
    }

    self->field_0x4 = new_end;
    return element;
}