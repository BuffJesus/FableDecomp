#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct CMemoryBlock_Overlay {
    void* vftable;
    std::byte padding_0x04[0x25];
    void* heap_block;
};

static_assert(offsetof(CMemoryBlock_Overlay, vftable) == 0x00);
static_assert(offsetof(CMemoryBlock_Overlay, heap_block) == 0x29);

extern void* PTR__vector_deleting_destructor__01230ba0;

CSimpleMemoryPool::CMemoryBlock* __thiscall
CMemoryBlock_Destructor(CSimpleMemoryPool::CMemoryBlock* self, std::uint8_t flags)
{
    auto* const overlay = reinterpret_cast<CMemoryBlock_Overlay*>(self);

    if (overlay->heap_block != nullptr) {
        std::free(overlay->heap_block);
    }

    overlay->vftable = &PTR__vector_deleting_destructor__01230ba0;
    self->CSimpleMemoryPool::CMemoryBlock::~CMemoryBlock();

    if ((flags & 1u) != 0) {
        operator delete(self);
    }

    return self;
}