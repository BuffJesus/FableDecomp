#include "fable_variable_size_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
class AllocatorAccess:public CMemoryAllocatorVariableSize {public:using CMemoryAllocatorVariableSize::DeleteChunk;};
static FILE* input;static unsigned long args[5],calls,statusCalls;static void* statusPointer;
static unsigned char expected[0x8000],snapshot[0x8000];
static unsigned char* const region=reinterpret_cast<unsigned char*>(0x2004000);
static void read(void* p,unsigned long n){if(fread(p,1,n,input)!=n)exit(2);}
static void boundary(unsigned long kind,unsigned long a,unsigned long b,unsigned long c)
{
    unsigned long event[4];read(event,sizeof(event));read(snapshot,sizeof(snapshot));
    if(event[0]!=kind||event[1]!=a||event[2]!=b||event[3]!=c||memcmp(region,snapshot,sizeof(snapshot)))exit(7);++calls;
}
void CMemoryAllocatorVariableSize::RemoveFromTree(CMemoryArea* area)
{boundary(1,reinterpret_cast<unsigned long>(this),reinterpret_cast<unsigned long>(area),0);}
void CMemoryAllocatorVariableSize::InsertIntoTree(CMemoryArea* root,CMemoryArea* area)
{boundary(2,reinterpret_cast<unsigned long>(this),reinterpret_cast<unsigned long>(root),reinterpret_cast<unsigned long>(area));}
static void __stdcall StatusRecording(void* pointer)
{
    ++statusCalls;if(statusCalls==1)statusPointer=pointer;else if(pointer!=statusPointer)exit(8);
    boundary(3,0,0,0);unsigned long values[8]={32,17,1000,900,800,700,600,500};memcpy(pointer,values,32);
    if(statusCalls==1&&args[3])*reinterpret_cast<unsigned char*>(0x200901d)^=1;
}
static int __stdcall FreeRecording(void* pointer,unsigned long size,unsigned long flags)
{boundary(4,reinterpret_cast<unsigned long>(pointer),size,flags);return args[4];}
extern "C" {
    void (__stdcall* StatusSlot)(void*)=StatusRecording;
    int (__stdcall* FreeSlot)(void*,unsigned long,unsigned long)=FreeRecording;
}
int main(int argc,char** argv)
{
    if(argc!=2)return 2;input=fopen(argv[1],"rb");if(!input)return 2;
    if(VirtualAlloc(reinterpret_cast<void*>(0x2000000),0x20000,0x3000,4)!=reinterpret_cast<void*>(0x2000000))return 3;
    AllocatorAccess* owner=reinterpret_cast<AllocatorAccess*>(region);
    unsigned long count;read(&count,4);
    for(unsigned long i=0;i<count;++i){
        unsigned long expectedCalls,globals[2];read(args,sizeof(args));read(&expectedCalls,4);read(globals,8);read(region,0x8000);read(expected,sizeof(expected));calls=0;statusCalls=0;statusPointer=0;
        g_FableVariableAllocatorAlignment_013D28D4=args[0]?64:args[1];g_FableVariableAllocatorAlignmentShift_013D28D8=args[0]?6:args[1]==4?2:args[1]==8?3:args[1]==16?4:5;
        if(args[0]==0)owner->DeleteChunk(reinterpret_cast<CMemoryAllocatorVariableSize::CMemoryAreaChunk*>(args[2]));
        else owner->CMemoryAllocatorVariableSize::Deallocate(reinterpret_cast<void*>(args[2]));
        if(calls!=expectedCalls||memcmp(region,expected,sizeof(expected))||g_FableVariableAllocatorAlignment_013D28D4!=globals[0]||g_FableVariableAllocatorAlignmentShift_013D28D8!=globals[1]){printf("case %lu mismatch\n",i);return 5;}
    }
    if(fgetc(input)!=EOF)return 6;printf("VARIABLE_DEALLOCATION_OK cases=%lu\n",count);return 0;
}
