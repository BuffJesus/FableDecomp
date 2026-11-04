#include "fable_variable_size_allocator.h"
unsigned long CMemoryAllocatorVariableSize::GetLargestFreeAreaSize()
{
    unsigned long largest=0;
    for(CMemoryAreaChunk* chunk=PFirstChunk;chunk;chunk=chunk->PNextChunk){
        unsigned long size=chunk->LargestFreeAreaSize;
        if(size>largest)largest=size;
    }
    return largest;
}
