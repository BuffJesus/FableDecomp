#include "fable_variable_size_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" unsigned long RetailTable[16];
extern "C" void* __fastcall TestDeleting(CMemoryAllocatorVariableSize*,void*,unsigned long);
// Unused virtuals are fail-fast test boundaries; native destructor/chunk release are linked.
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

static FILE* input;static unsigned long args[7],calls,statusCalls,chain[7];static void* statusPointer;
static unsigned char expected[0x8000],snapshot[0x8000],normalized[0x8000];
static unsigned char* const region=reinterpret_cast<unsigned char*>(0x2004000);
static void read(void* p,unsigned long n){if(fread(p,1,n,input)!=n)exit(2);}
static bool stateMatches(const unsigned char* state)
{
    if(*reinterpret_cast<unsigned long*>(region)!=reinterpret_cast<unsigned long>(RetailTable))return false;
    memcpy(normalized,region,sizeof(normalized));*reinterpret_cast<unsigned long*>(normalized)=0x129db1c;
    return memcmp(normalized,state,sizeof(normalized))==0;
}
static void boundary(unsigned long kind,unsigned long a,unsigned long b,unsigned long c)
{
    unsigned long event[6];read(event,sizeof(event));read(snapshot,sizeof(snapshot));
    if(event[0]!=kind||event[1]!=a||event[2]!=b||event[3]!=c||event[4]!=g_FableVariableAllocatorAlignment_013D28D4||event[5]!=g_FableVariableAllocatorAlignmentShift_013D28D8||!stateMatches(snapshot))exit(7);++calls;
}
#ifndef FABLE_NATIVE_TREE
void CMemoryAllocatorVariableSize::RemoveFromTree(CMemoryArea* area)
{boundary(1,reinterpret_cast<unsigned long>(this),reinterpret_cast<unsigned long>(area),0);}
#endif
static void __stdcall StatusRecording(void* pointer)
{
    if(statusCalls>=args[3]*2)exit(8);unsigned long chunk=chain[statusCalls/2];
    if(statusCalls%2==0)statusPointer=pointer;else if(pointer!=statusPointer)exit(9);
    boundary(3,chunk,0,0);unsigned long values[8]={32,17,1000,900,800,700,600,500};memcpy(pointer,values,32);
    if(statusCalls%2==0&&args[1]){
        *reinterpret_cast<unsigned char*>(chunk+0x1d)^=1;
        *reinterpret_cast<unsigned long*>(region+0xc14)=128;*reinterpret_cast<unsigned long*>(region+0xc18)=7;
        g_FableVariableAllocatorAlignment_013D28D4=64;g_FableVariableAllocatorAlignmentShift_013D28D8=6;
    }
    ++statusCalls;
}
static int __stdcall FreeRecording(void* pointer,unsigned long size,unsigned long flags)
{boundary(4,reinterpret_cast<unsigned long>(pointer),size,flags);return args[2];}
extern "C" {
    void (__stdcall* StatusSlot)(void*)=StatusRecording;
    int (__stdcall* FreeSlot)(void*,unsigned long,unsigned long)=FreeRecording;
}
int main(int argc,char** argv)
{
    if(argc!=2)return 2;input=fopen(argv[1],"rb");if(!input)return 2;
    if(VirtualAlloc(reinterpret_cast<void*>(0x2000000),0x20000,0x3000,4)!=reinterpret_cast<void*>(0x2000000))return 3;
    CMemoryAllocatorVariableSize* owner=reinterpret_cast<CMemoryAllocatorVariableSize*>(region);
    unsigned long count;read(&count,4);
    for(unsigned long i=0;i<count;++i){
        read(args,sizeof(args));read(region,0x8000);read(expected,sizeof(expected));calls=0;statusCalls=0;statusPointer=0;
        unsigned long p=*reinterpret_cast<unsigned long*>(region+0xc1c);
        for(unsigned long j=0;j<args[3];++j){if(!p)return 4;chain[j]=p;p=*reinterpret_cast<unsigned long*>(p+0x20);}if(p)return 4;
        g_FableVariableAllocatorAlignment_013D28D4=256;g_FableVariableAllocatorAlignmentShift_013D28D8=8;
        if(args[0]==0xffffffff)owner->CMemoryAllocatorVariableSize::~CMemoryAllocatorVariableSize();
        else if(TestDeleting(owner,0,args[0])!=owner)return 10;
        if(calls!=args[4]||statusCalls!=args[3]*2||!stateMatches(expected)||g_FableVariableAllocatorAlignment_013D28D4!=args[5]||g_FableVariableAllocatorAlignmentShift_013D28D8!=args[6]){printf("case %lu mismatch\n",i);return 5;}
    }
    if(fgetc(input)!=EOF)return 6;printf("VARIABLE_DESTRUCTOR_OK cases=%lu\n",count);return 0;
}
