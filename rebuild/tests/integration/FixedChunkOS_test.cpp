#include "candidates/fixed_size_allocator_accessors_compiled.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall GetProcessHeap();
extern "C" __declspec(dllimport) void* __stdcall HeapAlloc(void*,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall HeapFree(void*,unsigned long,void*);
extern "C" __declspec(dllimport) int __stdcall HeapValidate(void*,unsigned long,const void*);
class ChunkAccess:public CMemoryAllocatorFixedSize{public:using CMemoryAllocatorFixedSize::CreateChunk;using CMemoryAllocatorFixedSize::DeleteChunk;using CMemoryAllocatorFixedSize::InsertChunkAtListHead;};
static ChunkAccess* owner;static void* raw;static void* heap;static unsigned long calls,releases;static unsigned char expected[17000];
static void require(bool value){if(!value)exit(5);}
static void* __stdcall RealHeap()
{
    require(owner->PFirstChunk==0&&owner->PLastChunk==0);++calls;
    return GetProcessHeap();
}
static int __stdcall RealFree(void* h,unsigned long flags,void* memory)
{
    require(h==heap&&flags==1&&memory==raw);++calls;
    int result=HeapFree(h,flags,memory);require(result!=0);++releases;return result;
}
extern "C"{void*(__stdcall* GetHeapSlot)()=RealHeap;int(__stdcall* HeapFreeSlot)(void*,unsigned long,void*)=RealFree;}
int main()
{
    heap=GetProcessHeap();require(heap!=0);unsigned long cases=0;
    const unsigned long counts[7]={0,1,3,254,255,256,257};
    for(unsigned long alignment=4;alignment<=16;alignment*=2)
     for(unsigned long stride=16;stride<=64;stride*=4)
      for(unsigned long n=0;n<7;++n)
       for(unsigned long current=0;current<2;++current){
        unsigned long storage[0xc4c/4];memset(storage,0,sizeof(storage));owner=reinterpret_cast<ChunkAccess*>(storage);
        storage[0xc14/4]=alignment;storage[0xc18/4]=alignment==4?2:alignment==8?3:4;
        unsigned long header=(28+alignment-1)/alignment*alignment-4,count=counts[n];
        owner->ChunkHeaderSize=header;owner->StatAreaSize=stride;owner->NoAreasPerChunk=count;owner->NoEmptyChunks=99;
        unsigned long bytes=alignment-1+header+count*stride+64;require(bytes<=sizeof(expected));
        raw=HeapAlloc(heap,0,bytes);require(raw!=0);unsigned char pattern=cases%2?0x5a:0xa5;memset(raw,pattern,bytes);memset(expected,pattern,bytes);
        unsigned long aligned=(reinterpret_cast<unsigned long>(raw)+alignment-1)/alignment*alignment,offset=aligned-reinterpret_cast<unsigned long>(raw);
        *reinterpret_cast<unsigned long*>(expected+offset)=reinterpret_cast<unsigned long>(raw);
        for(unsigned long off=4;off<24;off+=4)*reinterpret_cast<unsigned long*>(expected+offset+off)=0;
        for(unsigned long i=0;i<count;++i){
            unsigned long at=offset+header+i*stride;expected[at]=static_cast<unsigned char>(i);expected[at+1]=i?static_cast<unsigned char>(i-1):255;expected[at+2]=255;expected[at+3]=0;
            if(i)expected[at-stride+2]=static_cast<unsigned char>(i);
        }
        *reinterpret_cast<unsigned long*>(expected+offset+16)=count?aligned+header+(count-1)*stride:0;
        CMemoryAllocatorFixedSize::CMemoryAreaChunk* chunk=owner->CreateChunk(raw);
        require(reinterpret_cast<unsigned long>(chunk)==aligned&&memcmp(raw,expected,bytes)==0);
        owner->InsertChunkAtListHead(chunk);owner->PCurrentAllocatorChunk=current?chunk:reinterpret_cast<CMemoryAllocatorFixedSize::CMemoryAreaChunk*>(storage);
        CMemoryAllocatorFixedSize::CMemoryAreaChunk* preserved=owner->PCurrentAllocatorChunk;calls=0;
        owner->DeleteChunk(chunk);
        require(calls==2&&owner->PFirstChunk==0&&owner->PLastChunk==0&&owner->NoEmptyChunks==99);
        require(owner->PCurrentAllocatorChunk==(current?0:preserved));require(HeapValidate(heap,0,0)!=0);++cases;
    }
    printf("FIXED_CHUNK_OS_OK cases=%lu actual_releases=%lu\n",cases,releases);return 0;
}
