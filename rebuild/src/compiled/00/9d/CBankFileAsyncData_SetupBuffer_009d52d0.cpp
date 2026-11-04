#define FABLE_MEMORY_ALLOCATOR_OUT_OF_LINE_SUPPORT
#include "fable_game_text_bank.h"
#include "fable_handle_memory_pool.h"
#include "fable_memory_allocator.h"
#include "fable_critical_section_lock.h"
extern fable_u32 g_CBankFileAsyncFailureHandlingMode_013CA868;
bool CBankFileAsyncData::SetupBuffer(bool force)
{
    CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(BankFile.CriticalSection),!force);
    if(!lock.IsLocked())return false;
    if(Buffer)return true;
    if(MemoryPool) {
        unsigned long size=BankFile.Align(Length+SkipBytes);
        unsigned long flags=g_CBankFileAsyncFailureHandlingMode_013CA868;
        if(!force)flags|=3;
        if(MemoryPool->AllocateResource(&MemoryHandle,this,size,flags)) {
            Buffer=MemoryPool->GetPointerFromHandle(MemoryHandle);
            Data=(char*)Buffer+SkipBytes;
            return true;
        }
        if(!force)return false;
    }
    InternalAllocatedBufferSize=BankFile.Align(Length+SkipBytes);
    void* buffer=CMemoryAllocator::Allocate(InternalAllocatedBufferSize,0);
    Data=(char*)buffer+SkipBytes;
    Buffer=buffer;
    return true;
}
