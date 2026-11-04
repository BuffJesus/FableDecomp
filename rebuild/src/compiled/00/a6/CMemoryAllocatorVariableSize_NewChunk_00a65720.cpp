#include "fable_variable_size_allocator.h"
struct FableMemoryBasicInformation {
    void* BaseAddress;
    void* AllocationBase;
    unsigned long AllocationProtect;
    unsigned long RegionSize;
    unsigned long State,Protect,Type;
};
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) unsigned long __stdcall VirtualQuery(const void*,FableMemoryBasicInformation*,unsigned long);

static inline unsigned long ChunkHeaderSize()
{
    return (((g_FableVariableAllocatorAlignment_013D28D4+sizeof(CMemoryAllocatorVariableSize::CMemoryAreaChunk)-1)>>g_FableVariableAllocatorAlignmentShift_013D28D8)<<g_FableVariableAllocatorAlignmentShift_013D28D8);
}
static inline unsigned long AreaHeaderSize()
{
    return (((g_FableVariableAllocatorAlignment_013D28D4+sizeof(CMemoryAllocatorVariableSize::CMemoryArea)-1)>>g_FableVariableAllocatorAlignmentShift_013D28D8)<<g_FableVariableAllocatorAlignmentShift_013D28D8);
}

inline unsigned long CMemoryAllocatorBase::Align(unsigned long size) const
{
    return ((size+Alignment-1)>>AlignmentShift)<<AlignmentShift;
}

inline long CMemoryAllocatorVariableSize::InternalAlloc(long size,void** memory)
{
    unsigned long protection=0;
    if(Flags&2)protection|=2;
    else protection|=4;
    long actualSize=0;
    *memory=VirtualAlloc(0,size,0x103000,protection);
    if(*memory) {
        FableMemoryBasicInformation info;
        VirtualQuery(*memory,&info,sizeof(info));
        actualSize=info.RegionSize;
    }
    return actualSize;
}

CMemoryAllocatorVariableSize::CMemoryAreaChunk* CMemoryAllocatorVariableSize::NewChunk(long size)
{
    long initialSize=Align(
        (size+ChunkHeaderSize()+AreaHeaderSize())>static_cast<unsigned long>(MinChunkSize)
        ? (size+ChunkHeaderSize()+AreaHeaderSize()) : MinChunkSize);
    void* memory=0;
    long actualSize=0;
    long allocationSize=initialSize;
    while(!memory && allocationSize>=size) {
        actualSize=InternalAlloc(allocationSize,&memory);
        allocationSize/=2;
    }
    if(!memory) {
        CMemoryAllocator::Get()->OnFailedAllocation();
        return 0;
    }
    long dataSize=actualSize-ChunkHeaderSize();
    CMemoryAreaChunk* chunk=static_cast<CMemoryAreaChunk*>(memory);
    chunk->Init(reinterpret_cast<unsigned char*>(memory)+ChunkHeaderSize(),dataSize,this);
    chunk->PNextChunk=PFirstChunk;
    if(PFirstChunk)PFirstChunk->PPreviousChunk=chunk;
    chunk->PPreviousChunk=0;
    PFirstChunk=chunk;
    return chunk;
}
