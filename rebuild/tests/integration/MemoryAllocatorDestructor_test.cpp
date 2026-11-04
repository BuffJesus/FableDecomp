#include "fable_memory_allocator.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
static unsigned long events[64],calls,*liveCount;
static int mode,active,audit;
static void record(unsigned long value) { if(calls>=64)abort();events[calls++]=value; }
extern "C" void __cdecl free(void* pointer) { if(active)record(reinterpret_cast<unsigned long>(pointer)); }
static int __stdcall Release(void* pointer,unsigned long size,unsigned long flags) {
    if(!active || size || flags!=0x8000)abort();record(0x100000+reinterpret_cast<unsigned long>(pointer));
    if(calls==1+(audit?1:0) && (mode==4 || mode==5))*liveCount=mode==4?3:1;
    return mode==7?0:1;
}
static void __stdcall Destroy(_RTL_CRITICAL_SECTION* pointer) {
    if(!active || pointer!=&g_CMemoryAllocatorCriticalSection_013CAA70)abort();record(0xdead);
}
extern "C" {
    int (__stdcall* ReleaseRecording)(void*,unsigned long,unsigned long)=Release;
    void (__stdcall* DestroyRecording)(_RTL_CRITICAL_SECTION*)=Destroy;
}
struct View : CMemoryAllocator {
    static CAllocatorInfo* Info() { return AllocatorInfo; }
    static void DestroyOwner(View* owner) { owner->CMemoryAllocator::~CMemoryAllocator(); }
};
// RETAIL_EXPECTATIONS
int main()
{
    static unsigned long storage[0x49c/4],before[0x49c/4];
    static const unsigned long counts[8]={0,3,30,1,1,3,0,3};
    for(mode=0;mode<8;++mode) {
        audit=mode==1 || mode==2 || mode==4 || mode==5 || mode==7;
        memset(storage,0x77,sizeof(storage));storage[0x404/4]=audit?0x1100:0;storage[0x410/4]=counts[mode];liveCount=storage+0x410/4;
        for(int i=0;i<15;++i)storage[0x460/4+i]=mode==2 || ((mode==1 || mode==3 || mode==6 || mode==7)&&(i==0 || i==7 || i==14))?0x3000+i*4:0;
        for(int i=0;i<30;++i)View::Info()[i].PAllocator=mode==3?0:reinterpret_cast<CMemoryAllocatorBase*>(0x2000+i*4);
        memcpy(before,storage,sizeof(storage));calls=0;active=1;View::DestroyOwner(reinterpret_cast<View*>(storage));active=0;
        before[0x410/4]=expectedCount[mode];
        if(calls!=expectedCalls[mode] || memcmp(events,expected[mode],calls*4) || *liveCount!=expectedCount[mode] || memcmp(storage,before,sizeof(storage))) { printf("FAIL mode=%d calls=%lu count=%lu\n",mode,calls,*liveCount);return 1; }
    }
    puts("MEMORY_ALLOCATOR_DTOR_OK cases=8");return 0;
}
