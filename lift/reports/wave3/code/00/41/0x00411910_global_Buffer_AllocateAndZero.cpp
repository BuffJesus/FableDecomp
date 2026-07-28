#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct BufferOverlay {
    void* field_0x0;
    std::byte* field_0x4;
    std::byte* field_0x8;
};

static_assert(offsetof(BufferOverlay, field_0x0) == 0x0);
static_assert(offsetof(BufferOverlay, field_0x4) == 0x4);
static_assert(offsetof(BufferOverlay, field_0x8) == 0x8);
static_assert(sizeof(BufferOverlay) == 0xC);

BufferOverlay* __thiscall Buffer_AllocateAndZero(BufferOverlay* self, std::uint32_t size)
{
    std::uint32_t* allocation = nullptr;

    self->field_0x0 = nullptr;
    self->field_0x4 = nullptr;
    self->field_0x8 = nullptr;

    if (size != 0) {
        allocation = static_cast<std::uint32_t*>(std::malloc(size));
    }

    self->field_0x8 = reinterpret_cast<std::byte*>(allocation) + size;

    std::uint32_t dwordCount = size >> 2;
    self->field_0x0 = allocation;

    std::uint32_t* writePtr = allocation;
    for (; dwordCount != 0; --dwordCount) {
        *writePtr = 0;
        ++writePtr;
    }

    for (dwordCount = (size & 3); dwordCount != 0; --dwordCount) {
        *reinterpret_cast<std::byte*>(writePtr) = std::byte{0};
        writePtr = reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uintptr_t>(writePtr) + 1
        );
    }

    self->field_0x4 = reinterpret_cast<std::byte*>(allocation) + size;
    return self;
}