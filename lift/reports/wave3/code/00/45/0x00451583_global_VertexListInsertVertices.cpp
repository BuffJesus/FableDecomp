#include <cstddef>
#include <cstdint>

struct VertexListBufferOverlay {
    std::uint8_t* begin; // 0x00
    std::uint8_t* end;   // 0x04
};

static_assert(offsetof(VertexListBufferOverlay, begin) == 0x00);
static_assert(offsetof(VertexListBufferOverlay, end) == 0x04);
static_assert(sizeof(VertexListBufferOverlay) == 0x08);

extern void __thiscall Buffer_InitializeWithSize(VertexListBufferOverlay* self, int count, std::uint32_t sizeHint);

VertexListBufferOverlay* __thiscall VertexListInsertVertices(
    VertexListBufferOverlay* self,
    int count,
    std::uint8_t* valueSource,
    std::uint32_t sizeHint)
{
    Buffer_InitializeWithSize(self, count, sizeHint);

    std::uint8_t* write = self->begin;
    for (; count != 0; --count) {
        *write = *valueSource;
        ++write;
    }

    self->end = write;
    return self;
}