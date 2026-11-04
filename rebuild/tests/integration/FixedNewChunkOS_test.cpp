#include "candidates/fixed_size_allocator_accessors_compiled.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall GetProcessHeap();
extern "C" __declspec(dllimport) void* __stdcall HeapAlloc(void*,unsigned long,unsigned long);
extern "C" __declspec(dllimport) unsigned long __stdcall HeapSize(void*,unsigned long,const void*);
extern "C" __declspec(dllimport) int __stdcall HeapValidate(void*,unsigned long,const void*);
class OSAccess:public CMemoryAllocatorFixedSize{public:using CMemoryAllocatorFixedSize::NewChunk;using CMemoryAllocatorFixedSize::DeleteChunk;};
CMemoryAllocator* g_FableMemoryAllocator_013CAA90;
static unsigned long requests,heapCalls,sizeCalls,wanted;static void* allocated;
static void* __stdcall GetHeapChecked(){++heapCalls;return GetProcessHeap();}
static void* __stdcall HeapAllocChecked(void* heap,unsigned long flags,unsigned long bytes)
{if(heap!=GetProcessHeap()||flags!=1||bytes!=wanted)exit(7);++requests;allocated=HeapAlloc(heap,flags,bytes);if(!allocated)exit(8);memset(allocated,0xa5,HeapSize(heap,0,allocated));return allocated;}
static unsigned long __stdcall HeapSizeChecked(void* heap,unsigned long flags,const void* memory)
{if(heap!=GetProcessHeap()||flags!=1||memory!=allocated)exit(9);++sizeCalls;return HeapSize(heap,flags,memory);}
extern "C"{
void*(__stdcall* GetHeapSlot)()=GetHeapChecked;
void*(__stdcall* HeapAllocSlot)(void*,unsigned long,unsigned long)=HeapAllocChecked;
unsigned long(__stdcall* HeapSizeSlot)(void*,unsigned long,const void*)=HeapSizeChecked;
}
int main()
{
    unsigned long cases=0;unsigned char storage[sizeof(OSAccess)];OSAccess* owner=reinterpret_cast<OSAccess*>(storage);
    const unsigned long alignments[]={4,8,16},counts[]={0,1,3,254,255,256,257},strides[]={16,64};
    for(unsigned long a=0;a<3;++a)for(unsigned long n=0;n<7;++n)for(unsigned long s=0;s<2;++s){
        memset(storage,0,sizeof(storage));unsigned long alignment=alignments[a],count=counts[n],stride=strides[s],header=(24+4+alignment-1)/alignment*alignment-4;
        *reinterpret_cast<unsigned long*>(storage+0xc14)=alignment;*reinterpret_cast<unsigned long*>(storage+0xc18)=a+2;
        owner->StatAreaSize=stride;owner->ChunkHeaderSize=header;owner->NoAreasPerChunk=count;owner->ChunkSize=header+stride*count;
        wanted=owner->ChunkSize+((alignment-1)&~7UL);unsigned long before=requests;
        CMemoryAllocatorFixedSize::CMemoryAreaChunk* chain[2];
        for(unsigned long i=0;i<2;++i){
            chain[i]=owner->NewChunk();if(!chain[i]||chain[i]->ThisChunkMem!=allocated||reinterpret_cast<unsigned long>(chain[i])%alignment)return 1;
            unsigned long actual=HeapSize(GetProcessHeap(),0,allocated);unsigned char* expected=new unsigned char[actual];memset(expected,0xa5,actual);
            unsigned long offset=reinterpret_cast<unsigned long>(chain[i])-reinterpret_cast<unsigned long>(allocated);
            CMemoryAllocatorFixedSize::CMemoryAreaChunk* predicted=reinterpret_cast<CMemoryAllocatorFixedSize::CMemoryAreaChunk*>(expected+offset);
            predicted->ThisChunkMem=allocated;predicted->NoAllocatedAreas=0;predicted->PNextChunk=i?chain[0]:0;predicted->PPreviousChunk=0;predicted->PFirstAllocatedArea=0;
            predicted->PFirstFreeArea=count?reinterpret_cast<CMemoryAllocatorFixedSize::CMemoryArea*>(reinterpret_cast<unsigned char*>(chain[i])+header+(count-1)*stride):0;
            for(unsigned long j=0;j<count;++j){unsigned char* node=expected+offset+header+j*stride;node[0]=static_cast<unsigned char>(j);node[1]=j?static_cast<unsigned char>(j-1):255;node[2]=255;node[3]=0;if(j)*(node-stride+2)=static_cast<unsigned char>(j);}
            if(memcmp(expected,allocated,actual))return 2;delete[] expected;
            if(owner->PFirstChunk!=chain[i]||owner->PLastChunk!=chain[0]||owner->NoEmptyChunks!=static_cast<long>(i+1))return 3;
            if(i&&chain[0]->PPreviousChunk!=chain[1])return 4;
        }
        if(requests-before!=2||heapCalls!=requests*2||sizeCalls!=requests)return 5;
        owner->PCurrentAllocatorChunk=chain[0];owner->DeleteChunk(chain[1]);owner->DeleteChunk(chain[0]);
        if(owner->PFirstChunk||owner->PLastChunk||owner->PCurrentAllocatorChunk||owner->NoEmptyChunks!=2||!HeapValidate(GetProcessHeap(),0,0))return 6;
        ++cases;
    }
    printf("FIXED_NEW_CHUNK_OS_OK cases=%lu allocations=%lu\n",cases,requests);return 0;
}
