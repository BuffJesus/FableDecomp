#include "candidates/fixed_size_allocator_accessors_compiled.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
class LifetimeAccess:public CMemoryAllocatorFixedSize{public:using CMemoryAllocatorFixedSize::CreateChunk;using CMemoryAllocatorFixedSize::DeleteChunk;};
static FILE* input;static unsigned long args[7],calls;static unsigned char expected[0x8000],snapshot[0x8000];
static unsigned char* const region=reinterpret_cast<unsigned char*>(0x2004000);
static void read(void* p,unsigned long n){if(fread(p,1,n,input)!=n)exit(2);}
static void boundary(unsigned long kind,unsigned long a,unsigned long b,unsigned long c)
{
    unsigned long event[4];read(event,sizeof(event));read(snapshot,sizeof(snapshot));
    if(event[0]!=kind||event[1]!=a||event[2]!=b||event[3]!=c||memcmp(region,snapshot,sizeof(snapshot)))exit(7);++calls;
}
static void* __stdcall GetHeapRecording()
{
    boundary(1,0,0,0);
    if(args[2]){*reinterpret_cast<unsigned long*>(args[1])=args[1]-16;*reinterpret_cast<unsigned long*>(region+0xc24)=args[1];*reinterpret_cast<long*>(region+0xc48)=-3;}
    return reinterpret_cast<void*>(args[3]);
}
static int __stdcall HeapFreeRecording(void* heap,unsigned long flags,void* memory)
{boundary(2,reinterpret_cast<unsigned long>(heap),flags,reinterpret_cast<unsigned long>(memory));return args[4];}
extern "C"{void*(__stdcall* GetHeapSlot)()=GetHeapRecording;int(__stdcall* HeapFreeSlot)(void*,unsigned long,void*)=HeapFreeRecording;}
int main(int argc,char** argv)
{
    if(argc!=2)return 2;input=fopen(argv[1],"rb");if(!input)return 2;
    if(VirtualAlloc(reinterpret_cast<void*>(0x2000000),0x20000,0x3000,4)!=reinterpret_cast<void*>(0x2000000))return 3;
    LifetimeAccess* owner=reinterpret_cast<LifetimeAccess*>(region);unsigned long cases;read(&cases,4);
    for(unsigned long i=0;i<cases;++i){
        read(args,sizeof(args));read(region,sizeof(expected));read(expected,sizeof(expected));calls=0;
        if(args[0]==0){if(reinterpret_cast<unsigned long>(owner->CreateChunk(reinterpret_cast<void*>(args[1])))!=args[5])return 4;}
        else owner->DeleteChunk(reinterpret_cast<CMemoryAllocatorFixedSize::CMemoryAreaChunk*>(args[1]));
        if(calls!=args[6]||memcmp(region,expected,sizeof(expected))){printf("fixed chunk lifetime case %lu failed\n",i);return 5;}
    }
    if(fgetc(input)!=EOF)return 6;printf("FIXED_CHUNK_LIFETIME_OK cases=%lu\n",cases);return 0;
}
