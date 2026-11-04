#include "fable_memory_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static unsigned long storage[0x49c/4],order[16],calls;
static unsigned mode,failAllocation,allocationCount;
static void record(unsigned long value) { if(calls>=16)abort();order[calls++]=value; }
static void* __stdcall Allocate(void* hint,unsigned long size,unsigned long flags,unsigned long protection) {
    if(hint || size!=0x49c || flags!=0x3000 || protection!=4)abort();record(1);return failAllocation?0:storage;
}
static void* __stdcall Heap() { record(2);return reinterpret_cast<void*>(0x12340000); }
struct Status { unsigned long Length,Load,TotalPhysical,AvailablePhysical,TotalPageFile,AvailablePageFile,TotalVirtual,AvailableVirtual; };
static void __stdcall MemoryStatus(Status* value) {
    if(g_FableMemoryAllocator_013CAA90!=(failAllocation?0:reinterpret_cast<CMemoryAllocator*>(storage)))abort();
    record(4);memset(value,0,sizeof(*value));value->Length=32;value->Load=51;value->TotalPhysical=mode==3?16:1024;value->AvailablePhysical=mode==3?32:512;
}
static void __stdcall Destroy(_RTL_CRITICAL_SECTION* pointer) { if(pointer!=&g_CMemoryAllocatorCriticalSection_013CAA70)abort();record(5); }
static int __stdcall Release(void* pointer,unsigned long size,unsigned long flags) {
    if(pointer!=storage || size || flags!=0x8000 || g_FableMemoryAllocator_013CAA90!=reinterpret_cast<CMemoryAllocator*>(storage))abort();record(6);return mode==6?0:1;
}
extern "C" {
    void* (__stdcall* AllocateRecording)(void*,unsigned long,unsigned long,unsigned long)=Allocate;
    void* (__stdcall* HeapRecording)()=Heap;
    void (__stdcall* StatusRecording)(Status*)=MemoryStatus;
    void (__stdcall* DestroyRecording)(_RTL_CRITICAL_SECTION*)=Destroy;
    int (__stdcall* ReleaseRecording)(void*,unsigned long,unsigned long)=Release;
}
CMemoryAllocator::CMemoryAllocator(void* heap) {
    if(this!=reinterpret_cast<CMemoryAllocator*>(storage) || heap!=reinterpret_cast<void*>(0x12340000))abort();record(3);
    memset(this,0,sizeof(*this));Heap=heap;NoAllocations=allocationCount;
}
struct View : CMemoryAllocator {
    static void Make() { Create(); }
    static unsigned long& Baseline() { return MemoryUseBeforeAllocator; }
};
// RETAIL_EXPECTATIONS
int main()
{
    static const unsigned specs[10][4]={{1,0,0,0},{0,0,0,0},{0,0,1,0},{0,0,17,0},{1,1,0,1},{0,1,0,1},{0,0,0,0},{0,0,0,0},{0,1,0,0},{0,0,2,1}};
    for(mode=0;mode<10;++mode) {
        calls=0;failAllocation=specs[mode][0];allocationCount=specs[mode][2];
        memset(storage,0,sizeof(storage));storage[0x40c/4]=allocationCount;
        g_FableMemoryAllocator_013CAA90=specs[mode][3]?reinterpret_cast<CMemoryAllocator*>(storage):0;
        g_FableAllocatorShutdownRequested_013CAA94=specs[mode][1]!=0;View::Baseline()=1234;g_FableAllocatorMemoryBaseline_013CAAB0=5678;
        if(mode<7)View::Make();
        if((g_FableMemoryAllocator_013CAA90!=0)!=expected[mode][0] || View::Baseline()!=expected[mode][1] || g_FableAllocatorMemoryBaseline_013CAAB0!=expected[mode][2] || g_FableAllocatorShutdownRequested_013CAA94!=expected[mode][3])return 1;
        FableMemoryAllocatorShutdown_009f2d20();
        if((g_FableMemoryAllocator_013CAA90!=0)!=expected[mode][4] || g_FableAllocatorShutdownRequested_013CAA94!=expected[mode][5])return 2;
        if(mode==9) { storage[0x40c/4]=0;FableMemoryAllocatorShutdown_009f2d20(); }
        if((g_FableMemoryAllocator_013CAA90!=0)!=expected[mode][6] || g_FableAllocatorShutdownRequested_013CAA94!=expected[mode][7] || calls!=expectedCalls[mode] || memcmp(order,expectedOrder[mode],calls*4))return 3;
    }
    puts("ALLOCATOR_CREATION_SHUTDOWN_OK cases=10");return 0;
}
