#include "candidates/fixed_size_allocator_accessors_compiled.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Test-only Win32 exception ABI; the VC7.1 compiler supplies these intrinsics.
extern "C" unsigned long __cdecl _exception_code();
extern "C" void* __cdecl _exception_info();
struct ExceptionRecord{unsigned long code,flags;ExceptionRecord* nested;void* address;unsigned long count,information[15];};
struct ExceptionPointers{ExceptionRecord* record;void* context;};
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static FILE* input;static unsigned long args[8],calls;static unsigned char expected[0x8000],initial[0x8000],snapshot[0x8000];
static unsigned char* const region=reinterpret_cast<unsigned char*>(0x2004000);
CMemoryAllocator* g_FableMemoryAllocator_013CAA90;
static void read(void* p,unsigned long n){if(fread(p,1,n,input)!=n)exit(2);}
static void boundary(unsigned long kind,unsigned long a,unsigned long b,unsigned long c)
{
    if(calls>=args[7])exit(7);unsigned long event[4];read(event,sizeof(event));read(snapshot,sizeof(snapshot));
    if(event[0]!=kind||event[1]!=a||event[2]!=b||event[3]!=c||memcmp(region,snapshot,sizeof(snapshot)))exit(8);++calls;
}
static void* __stdcall GetHeapRecording(){boundary(1,0,0,0);return reinterpret_cast<void*>(0x12345678);}
static void* __stdcall HeapAllocRecording(void* heap,unsigned long flags,unsigned long bytes)
{boundary(2,reinterpret_cast<unsigned long>(heap),flags,bytes);return reinterpret_cast<void*>(args[4]);}
static unsigned long __stdcall HeapSizeRecording(void* heap,unsigned long flags,const void* memory)
{boundary(3,reinterpret_cast<unsigned long>(heap),flags,reinterpret_cast<unsigned long>(memory));return args[1]>=2?0:args[5]+16;}
static int __stdcall HeapFreeRecording(void* heap,unsigned long flags,void* memory)
{boundary(4,reinterpret_cast<unsigned long>(heap),flags,reinterpret_cast<unsigned long>(memory));return 1;}
extern "C"{
void*(__stdcall* GetHeapSlot)()=GetHeapRecording;
void*(__stdcall* HeapAllocSlot)(void*,unsigned long,unsigned long)=HeapAllocRecording;
unsigned long(__stdcall* HeapSizeSlot)(void*,unsigned long,const void*)=HeapSizeRecording;
int(__stdcall* HeapFreeSlot)(void*,unsigned long,void*)=HeapFreeRecording;
}
int main(int argc,char** argv)
{
    if(argc!=2)return 2;input=fopen(argv[1],"rb");if(!input)return 2;
    if(VirtualAlloc(reinterpret_cast<void*>(0x2000000),0x20000,0x3000,4)!=reinterpret_cast<void*>(0x2000000))return 3;
    CMemoryAllocatorFixedSize* owner=reinterpret_cast<CMemoryAllocatorFixedSize*>(region);unsigned long cases,last=0xffffffff;read(&cases,4);
    for(unsigned long i=0;i<cases;++i){
        read(args,sizeof(args));read(initial,sizeof(initial));read(expected,sizeof(expected));calls=0;
        if(last!=args[0])memcpy(region,initial,sizeof(initial));else if(memcmp(region,initial,sizeof(initial)))return 4;
        last=args[0];
        if(args[1]==2){
            unsigned long exception=0,address=0;
            __try{owner->CMemoryAllocatorFixedSize::Allocate(args[2]);}
            __except((exception=_exception_code(),address=static_cast<ExceptionPointers*>(_exception_info())->record->information[1],1)){}
            if(exception!=0xc0000005||address!=args[3])return 10;
        }else if(args[1]==0||args[1]==3){if(reinterpret_cast<unsigned long>(owner->CMemoryAllocatorFixedSize::Allocate(args[2]))!=args[3])return 5;}
        else owner->CMemoryAllocatorFixedSize::Deallocate(reinterpret_cast<void*>(args[2]));
        if(calls!=args[7]||memcmp(region,expected,sizeof(expected))){printf("fixed allocation case %lu failed\n",i);return 6;}
    }
    if(fgetc(input)!=EOF)return 9;printf("FIXED_ALLOCATION_OK cases=%lu\n",cases);return 0;
}
