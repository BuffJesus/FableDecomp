#include "candidates/fixed_size_allocator_accessors_compiled.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
class NewAccess:public CMemoryAllocatorFixedSize{public:using CMemoryAllocatorFixedSize::InternalAlloc;using CMemoryAllocatorFixedSize::NewChunk;};
static FILE* input;static unsigned long args[9],calls,heapCalls;static unsigned char expected[0x8000],snapshot[0x8000];
static unsigned char* const region=reinterpret_cast<unsigned char*>(0x2004000);
CMemoryAllocator* g_FableMemoryAllocator_013CAA90;
static void read(void* p,unsigned long n){if(fread(p,1,n,input)!=n)exit(2);}
static void boundary(unsigned long kind,unsigned long a,unsigned long b,unsigned long c)
{
    if(calls>=args[8])exit(7);unsigned long event[4];read(event,sizeof(event));read(snapshot,sizeof(snapshot));
    if(event[0]!=kind||event[1]!=a||event[2]!=b||event[3]!=c||memcmp(region,snapshot,sizeof(snapshot)))exit(8);++calls;
}
static void* __stdcall WrongHeap(){exit(9);return 0;}
extern "C" void*(__stdcall* GetHeapSlot)();
static void* __stdcall GetHeapRecording()
{
    boundary(1,0,0,0);
    if(args[5]&&heapCalls==0){
        static const unsigned long offsets[]={0xc14,0xc18,0xc1c,0xc20,0xc2c,0xc30,0xc34,0xc38,0xc48};
        static const unsigned long values[]={16,4,0x2007900,0x2007900,64,28,3,999,0xfffffffd};
        for(unsigned long i=0;i<9;++i)*reinterpret_cast<unsigned long*>(region+offsets[i])=values[i];
        g_FableMemoryAllocator_013CAA90=reinterpret_cast<CMemoryAllocator*>(0x2007b00);GetHeapSlot=WrongHeap;
    }
    ++heapCalls;return reinterpret_cast<void*>(args[6]);
}
static void* __stdcall HeapAllocRecording(void* heap,unsigned long flags,unsigned long bytes)
{boundary(2,reinterpret_cast<unsigned long>(heap),flags,bytes);return reinterpret_cast<void*>(args[3]);}
static unsigned long __stdcall HeapSizeRecording(void* heap,unsigned long flags,const void* memory)
{boundary(3,reinterpret_cast<unsigned long>(heap),flags,reinterpret_cast<unsigned long>(memory));return args[4];}
void CMemoryAllocator::OnFailedAllocation(){boundary(4,reinterpret_cast<unsigned long>(this),0,0);}
extern "C"{
void*(__stdcall* GetHeapSlot)()=GetHeapRecording;
void*(__stdcall* HeapAllocSlot)(void*,unsigned long,unsigned long)=HeapAllocRecording;
unsigned long(__stdcall* HeapSizeSlot)(void*,unsigned long,const void*)=HeapSizeRecording;
}
int main(int argc,char** argv)
{
    if(argc!=2)return 2;input=fopen(argv[1],"rb");if(!input)return 2;
    if(VirtualAlloc(reinterpret_cast<void*>(0x2000000),0x20000,0x3000,4)!=reinterpret_cast<void*>(0x2000000))return 3;
    NewAccess* owner=reinterpret_cast<NewAccess*>(region);unsigned long cases;read(&cases,4);
    for(unsigned long i=0;i<cases;++i){
        read(args,sizeof(args));read(region,sizeof(expected));read(expected,sizeof(expected));calls=heapCalls=0;GetHeapSlot=GetHeapRecording;
        g_FableMemoryAllocator_013CAA90=reinterpret_cast<CMemoryAllocator*>(0x2007800);
        unsigned long result=args[0]?reinterpret_cast<unsigned long>(owner->NewChunk()):owner->InternalAlloc(args[1],reinterpret_cast<void**>(0x2007c00));
        if(result!=args[7]||calls!=args[8]||memcmp(region,expected,sizeof(expected))){printf("fixed new chunk case %lu failed\n",i);return 5;}
    }
    if(fgetc(input)!=EOF)return 6;printf("FIXED_NEW_CHUNK_OK cases=%lu\n",cases);return 0;
}
