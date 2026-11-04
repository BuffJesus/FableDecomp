#define FABLE_MEMORY_ALLOCATOR_OUT_OF_LINE_SUPPORT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_memory_allocator.h"
struct InstallerBufferChunk { void* allocation; unsigned long unknown04; };
void FABLE_FASTCALL FableInstallerDestroyBufferRange(InstallerBufferChunk*,InstallerBufferChunk*,void*);
CMemoryAllocator* g_FableMemoryAllocator_013CAA90;
bool g_FableAllocatorShutdownRequested_013CAA94;
struct _RTL_CRITICAL_SECTION { unsigned long words[6]; };
_RTL_CRITICAL_SECTION g_CMemoryAllocatorCriticalSection_013CAA70;
static InstallerBufferChunk chunks[3];
static unsigned mode, countValue, depth, position, length, chunkIndex;
struct Event { unsigned kind,owner,arg,global,shutdown,depth,values[6]; };
static Event expected[32];
static unsigned asword(void* p) {return (unsigned)p;}
static void require(bool ok) { if(!ok)abort(); }
static void record(unsigned kind,unsigned owner,unsigned arg)
{
    require(position<length);
    Event e; e.kind=kind;e.owner=owner;e.arg=arg;e.global=asword(g_FableMemoryAllocator_013CAA90);e.shutdown=g_FableAllocatorShutdownRequested_013CAA94;e.depth=depth;
    memcpy(e.values,chunks,sizeof(chunks));
    require(!memcmp(&e,&expected[position++],sizeof(e)));
}
static void __stdcall EnterRecording(_RTL_CRITICAL_SECTION* lock)
{
    record(1,0,0);++depth;
    while(chunkIndex<3 && !chunks[chunkIndex].allocation)++chunkIndex;
    if(mode==1) {g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)0x2000;chunks[chunkIndex].allocation=0;}
}
static void __stdcall LeaveRecording(_RTL_CRITICAL_SECTION* lock) {record(6,0,0);--depth;++chunkIndex;}
void CMemoryAllocator::InternalDeallocate(void* allocation)
{
    record(2,asword(this),asword(allocation));
    if(mode==2){g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)0x2000;g_FableAllocatorShutdownRequested_013CAA94=true;}
}
unsigned long CMemoryAllocator::GetNoAllocations()
{
    record(3,asword(this),0);
    if(mode==3)g_FableMemoryAllocator_013CAA90=0;
    return countValue;
}
CMemoryAllocator::~CMemoryAllocator()
{
    record(4,asword(this),0);
    if(mode==4)g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)0x3000;
}
static int __stdcall FreeRecording(void* p,unsigned long size,unsigned long flags) {require(size==0&&flags==0x8000);record(5,asword(p),0);return 1;}
extern "C" {
void(__stdcall* EnterSlot)(_RTL_CRITICAL_SECTION*)=EnterRecording;
void(__stdcall* LeaveSlot)(_RTL_CRITICAL_SECTION*)=LeaveRecording;
int(__stdcall* FreeSlot)(void*,unsigned long,unsigned long)=FreeRecording;
}
// Independent event prediction from the retail instruction sequence. Each callback
// snapshot includes every chunk word and the singleton/shutdown/lock state.
static void predict(unsigned mask,unsigned initialOwner,unsigned shutdown)
{
    unsigned values[6],global=initialOwner,flag=shutdown,lock=0;
    for(unsigned i=0;i!=3;++i){values[2*i]=(mask&(1<<i))?0x4000+i*16:0;values[2*i+1]=0xa500+i;}
    memcpy(chunks,values,sizeof(chunks)); length=0;
    for(i=0;i!=3;++i) {
        unsigned allocation=values[2*i];if(!allocation)continue;
        Event* e=&expected[length++];e->kind=1;e->owner=e->arg=0;e->global=global;e->shutdown=flag;e->depth=lock;memcpy(e->values,values,sizeof(values));++lock;
        if(mode==1){global=0x2000;values[2*i]=0;}
        if(global) {
            e=&expected[length++];e->kind=2;e->owner=global;e->arg=allocation;e->global=global;e->shutdown=flag;e->depth=lock;memcpy(e->values,values,sizeof(values));
            if(mode==2){global=0x2000;flag=1;}
            if(flag){
                e=&expected[length++];e->kind=3;e->owner=global;e->arg=0;e->global=global;e->shutdown=flag;e->depth=lock;memcpy(e->values,values,sizeof(values));
                if(mode==3)global=0;
                if(!countValue){
                    unsigned deleted=global;
                    if(deleted){
                        e=&expected[length++];e->kind=4;e->owner=deleted;e->arg=0;e->global=global;e->shutdown=flag;e->depth=lock;memcpy(e->values,values,sizeof(values));
                        if(mode==4)global=0x3000;
                        e=&expected[length++];e->kind=5;e->owner=deleted;e->arg=0;e->global=global;e->shutdown=flag;e->depth=lock;memcpy(e->values,values,sizeof(values));
                    }
                    global=0;
                }
            }
        }
        e=&expected[length++];e->kind=6;e->owner=e->arg=0;e->global=global;e->shutdown=flag;e->depth=lock;memcpy(e->values,values,sizeof(values));--lock;
    }
}
int main()
{
    unsigned cases=0;
    for(mode=0;mode!=5;++mode)for(countValue=0;countValue!=2;++countValue)for(unsigned owner=0;owner!=2;++owner)for(unsigned flag=0;flag!=2;++flag)for(unsigned mask=0;mask!=8;++mask)
    {
        predict(mask,owner?0x1000:0,flag);
        g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)(owner?0x1000:0);g_FableAllocatorShutdownRequested_013CAA94=flag!=0;depth=position=chunkIndex=0;
        FableInstallerDestroyBufferRange(chunks,chunks+3,(void*)0xffffffff);
        require(position==length && depth==0);++cases;
    }
    printf("INSTALLER_DESTROY_RANGE_OK cases=%u\n",cases);
    return 0;
}
