#include "fable_variable_size_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char before[0x8000];
static unsigned long args[8],expected[3][6],eventCount,eventIndex;
static bool newCalled;
static void event(unsigned long kind,unsigned long owner,unsigned long a,unsigned long b)
{
    unsigned long got[6]={kind,owner,a,b,g_FableVariableAllocatorAlignment_013D28D4,g_FableVariableAllocatorAlignmentShift_013D28D8};
    if(eventIndex>=eventCount||memcmp(got,expected[eventIndex],sizeof(got))||memcmp(before,reinterpret_cast<void*>(0x2004000),sizeof(before)))exit(8);
    ++eventIndex;
}
CMemoryAllocatorVariableSize::CMemoryArea* CMemoryAllocatorVariableSize::CMemoryAreaChunk::AllocAreaFromArea(CMemoryArea* area,long size)
{
    event(1,reinterpret_cast<unsigned long>(this),reinterpret_cast<unsigned long>(area),size);
    return reinterpret_cast<CMemoryArea*>((newCalled?args[3]:args[1])?0x200b800:0);
}
CMemoryAllocatorVariableSize::CMemoryAreaChunk* CMemoryAllocatorVariableSize::NewChunk(long size)
{
    event(2,reinterpret_cast<unsigned long>(this),size,0);newCalled=true;
    if(args[4]){g_FableVariableAllocatorAlignment_013D28D4=64;g_FableVariableAllocatorAlignmentShift_013D28D8=6;}
    return reinterpret_cast<CMemoryAreaChunk*>(args[2]?0x200b000:0);
}
static void read(FILE* f,void* p,unsigned long n){if(fread(p,1,n,f)!=n)exit(2);}
int main(int argc,char** argv)
{
    if(argc!=2)return 2;FILE* f=fopen(argv[1],"rb");if(!f)return 2;
    if(VirtualAlloc(reinterpret_cast<void*>(0x2000000),0x20000,0x3000,4)!=reinterpret_cast<void*>(0x2000000))return 3;
    CMemoryAllocatorVariableSize* owner=reinterpret_cast<CMemoryAllocatorVariableSize*>(0x2004000);
    unsigned long count;read(f,&count,4);
    for(unsigned long i=0;i<count;++i){
        read(f,args,sizeof(args));read(f,&eventCount,4);if(eventCount>3)return 4;read(f,before,sizeof(before));read(f,expected,eventCount*sizeof(expected[0]));
        memcpy(owner,before,sizeof(before));eventIndex=0;newCalled=false;g_FableVariableAllocatorAlignment_013D28D4=128;g_FableVariableAllocatorAlignmentShift_013D28D8=7;
        unsigned long result=reinterpret_cast<unsigned long>(owner->CMemoryAllocatorVariableSize::Allocate(args[0]));
        if(result!=args[5]||eventIndex!=eventCount||memcmp(owner,before,sizeof(before))||g_FableVariableAllocatorAlignment_013D28D4!=args[6]||g_FableVariableAllocatorAlignmentShift_013D28D8!=args[7]){printf("case %lu mismatch\n",i);return 5;}
    }
    if(fgetc(f)!=EOF)return 6;
    printf("VARIABLE_VIRTUAL_ALLOCATE_OK cases=%lu\n",count);return 0;
}
