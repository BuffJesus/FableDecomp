#include <cstdio>
#include <cstring>
#include <cstdlib>

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

int main()
{
    BufferOverlay ov;
    memset(&ov, 0xCD, sizeof(ov));

    BufferOverlay* r = Buffer_AllocateAndZero(&ov, 0, 10);

    bool ok = (r == &ov);
    ok = ok && (ov.field_0x0 != 0);
    ok = ok && (ov.field_0x4 == static_cast<unsigned char*>(ov.field_0x0) + 10);
    ok = ok && (ov.field_0x8 == static_cast<unsigned char*>(ov.field_0x0) + 10);

    unsigned char* bytes = static_cast<unsigned char*>(ov.field_0x0);
    for (int i = 0; i < 10 && ok; ++i) {
        if (bytes[i] != 0) ok = false;
    }

    BufferOverlay ov2;
    memset(&ov2, 0xCD, sizeof(ov2));
    BufferOverlay* r2 = Buffer_AllocateAndZero(&ov2, 0, 0);
    ok = ok && (r2 == &ov2);
    ok = ok && (ov2.field_0x0 == 0);

    BufferOverlay ov3;
    memset(&ov3, 0xCD, sizeof(ov3));
    BufferOverlay* r3 = Buffer_AllocateAndZero(&ov3, 0, 7);
    ok = ok && (r3 == &ov3);
    unsigned char* bytes3 = static_cast<unsigned char*>(ov3.field_0x0);
    for (int i = 0; i < 7 && ok; ++i) {
        if (bytes3[i] != 0) ok = false;
    }

    if (ok) {
        printf("BUFFER_ALLOC_ZERO_OK\n");
    } else {
        printf("BUFFER_ALLOC_ZERO_FAIL\n");
    }
    return 0;
}
