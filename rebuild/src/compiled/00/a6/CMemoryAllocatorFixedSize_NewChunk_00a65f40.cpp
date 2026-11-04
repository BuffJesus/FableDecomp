#include "candidates/fixed_size_allocator_accessors_compiled.h"
extern "C" __declspec(dllimport) void* __stdcall GetProcessHeap();
extern "C" __declspec(dllimport) void* __stdcall HeapAlloc(void*,unsigned long,unsigned long);
extern "C" __declspec(dllimport) unsigned long __stdcall HeapSize(void*,unsigned long,const void*);
unsigned long CMemoryAllocatorFixedSize::InternalAlloc(unsigned long size,void** memory)
{
    unsigned long extra=(Alignment-1)&~7UL;
    unsigned __int64 total=static_cast<unsigned __int64>(extra)+size;
    if(total>0xffffffffUL)return 0;
    *memory=HeapAlloc(GetProcessHeap(),1,size+extra);
    if(*memory)return HeapSize(GetProcessHeap(),1,*memory);
    return 0;
}
inline void CMemoryAllocatorFixedSize::InsertChunkAtListHeadInline(CMemoryAreaChunk* chunk)
{
    chunk->PNextChunk=PFirstChunk;
    if(PFirstChunk)PFirstChunk->PPreviousChunk=chunk;
    chunk->PPreviousChunk=0;
    if(!PFirstChunk)PLastChunk=chunk;
    PFirstChunk=chunk;
}
CMemoryAllocatorFixedSize::CMemoryAreaChunk* CMemoryAllocatorFixedSize::NewChunk()
{
    void* memory;
    if(InternalAlloc(ChunkSize,&memory)){
        CMemoryAreaChunk* chunk=CreateChunk(memory);
        InsertChunkAtListHeadInline(chunk);
        ++NoEmptyChunks;
        return chunk;
    }
    CMemoryAllocator::Get()->OnFailedAllocation();
    return 0;
}
