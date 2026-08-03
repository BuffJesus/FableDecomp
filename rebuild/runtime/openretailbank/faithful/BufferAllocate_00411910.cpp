#include <cstdlib>
#include <cstring>

struct BufferOverlay {
    void* field_0x0;
    unsigned char* field_0x4;
    unsigned char* field_0x8;
};

BufferOverlay* __fastcall Buffer_AllocateAndZero(BufferOverlay* self, void* /*edx*/, unsigned int size)
{
    void* allocation = 0;
    unsigned int sizeCopy = size;

    self->field_0x0 = 0;
    self->field_0x4 = 0;
    self->field_0x8 = 0;

    if (size != 0) {
        allocation = std::malloc(size);
    }

    self->field_0x0 = allocation;

    std::memset(allocation, 0, size);

    self->field_0x8 = reinterpret_cast<unsigned char*>(allocation) + sizeCopy;
    self->field_0x4 = reinterpret_cast<unsigned char*>(allocation) + sizeCopy;
    return self;
}
