#include "fable_variable_size_allocator.h"
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// All unrecovered virtual operations are fail-fast link boundaries.
void* CMemoryAllocatorVariableSize::Allocate(unsigned long){abort();return 0;}
void CMemoryAllocatorVariableSize::Deallocate(void*){abort();}
unsigned long CMemoryAllocatorVariableSize::GetAllAllocatedAreas(void**,unsigned long){abort();return 0;}
unsigned long CMemoryAllocatorVariableSize::GetAllocationSize(void*){abort();return 0;}
unsigned long CMemoryAllocatorVariableSize::GetAllocationSizeIncludingHeader(void*){abort();return 0;}
unsigned long CMemoryAllocatorVariableSize::GetLargestFreeAreaSize(){abort();return 0;}
unsigned long CMemoryAllocatorVariableSize::GetTotalMemoryControlledByAllocator(){abort();return 0;}
unsigned long CMemoryAllocatorVariableSize::GetTotalMemoryAllocated(){abort();return 0;}
unsigned long CMemoryAllocatorVariableSize::GetNoFreeAreas(){abort();return 0;}
unsigned long CMemoryAllocatorVariableSize::GetNoAllocatedAreas(){abort();return 0;}
float CMemoryAllocatorVariableSize::GetFragmentation(){abort();return 0;}
void* CMemoryAllocatorVariableSize::GetAllocatedArea(long){abort();return 0;}
unsigned long CMemoryAllocatorVariableSize::GetHeaderOverhead(){abort();return 0;}
void CMemoryAllocatorVariableSize::Validate(){abort();}
CMemoryAllocatorVariableSize::~CMemoryAllocatorVariableSize(){abort();}
// RETAIL_EXPECTATIONS
static int currentCase,calls;static unsigned long kind,recordedMemory,recordedSize;
static CMemoryAllocatorVariableSize::CMemoryAreaChunk chunk;
static void record(CMemoryAllocatorVariableSize* owner,unsigned long type,void* memory,long size)
{
    ++calls;kind=type;recordedMemory=reinterpret_cast<unsigned long>(memory);recordedSize=size;
    if(memcmp(reinterpret_cast<unsigned char*>(owner)+4,expected[currentCase]+4,0xc34))abort();
    if(g_FableVariableAllocatorAlignment_013D28D4!=expectedGlobals[currentCase][0] || g_FableVariableAllocatorAlignmentShift_013D28D8!=expectedGlobals[currentCase][1])abort();
    for(int q=0;q<0x34;++q)if(reinterpret_cast<unsigned char*>(&chunk)[q]!=(currentCase%2?0xa5:0x5a))abort();
}
CMemoryAllocatorVariableSize::CMemoryAreaChunk* CMemoryAllocatorVariableSize::NewChunk(long size)
{record(this,1,0,size);return &chunk;}
CMemoryAllocatorVariableSize::CMemoryAreaChunk* CMemoryAllocatorVariableSize::NewChunkDemandPaged(void* memory,long size)
{record(this,2,memory,size);return &chunk;}
int main()
{
    if(g_FableVariableAllocatorAlignment_013D28D4 || g_FableVariableAllocatorAlignmentShift_013D28D8)return 6;
    unsigned long storage[(0xc38+8)/4];unsigned char* bytes=reinterpret_cast<unsigned char*>(storage);
    for(currentCase=0;currentCase<40;++currentCase) {
        unsigned long* args=arguments[currentCase/2];calls=0;kind=0;recordedMemory=0;recordedSize=0;
        memset(bytes,currentCase%2?0xa5:0x5a,sizeof(storage));memset(bytes,0x17,4);memset(bytes+4+0xc38,0x31,4);memset(&chunk,currentCase%2?0xa5:0x5a,sizeof(chunk));
        g_FableVariableAllocatorAlignment_013D28D4=0x77777777;g_FableVariableAllocatorAlignmentShift_013D28D8=0x77777777;
        CMemoryAllocatorVariableSize* owner=::new(bytes+4)CMemoryAllocatorVariableSize(reinterpret_cast<void*>(0x12345678),args[0],reinterpret_cast<void*>(args[1]),args[2],args[3],args[4],args[5]);
        if(memcmp(bytes+8,expected[currentCase]+4,0xc34))return 1;
        if(memcmp(&chunk,expectedChunk[currentCase],0x34))return 2;
        if(calls!=expectedCalls[currentCase] || kind!=expectedKind[currentCase] || recordedMemory!=expectedMemory[currentCase] || recordedSize!=expectedSize[currentCase])return 3;
        if(g_FableVariableAllocatorAlignment_013D28D4!=expectedGlobals[currentCase][0] || g_FableVariableAllocatorAlignmentShift_013D28D8!=expectedGlobals[currentCase][1])return 4;
        for(int q=0;q<4;++q)if(bytes[q]!=0x17 || bytes[4+0xc38+q]!=0x31)return 5;
        // No destruction: the variable allocator destructor is unrecovered.
    }
    puts("VARIABLE_ALLOCATOR_CTOR_OK cases=40");return 0;
}
