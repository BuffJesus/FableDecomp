#include <stdlib.h>

extern void* PTR__vector_deleting_destructor__01230ba0;
void __cdecl operator delete(void* memory);

#pragma pack(push,1)
struct CMemoryBlockOverlay
{
    void* vftable;
    unsigned char unused[0x25];
    void* heap_block;

    void BaseDestructor();
    CMemoryBlockOverlay* CMemoryBlock_Destructor(unsigned char flags);
};
#pragma pack(pop)

CMemoryBlockOverlay* CMemoryBlockOverlay::CMemoryBlock_Destructor(unsigned char flags)
{
    if (heap_block != 0) {
        free(heap_block);
    }
    vftable = &PTR__vector_deleting_destructor__01230ba0;
    BaseDestructor();
    if ((flags & 1) != 0) {
        operator delete(this);
    }
    return this;
}