// One retail translation unit: FreeBuffer (009D5410) is auto-inlined (/O2 implies /Ob2)
// into the destructor (009D61F0) and Abort (009D6350) and also emitted out of line.
#define FABLE_MEMORY_ALLOCATOR_OUT_OF_LINE_SUPPORT
#include "fable_threaded_file.h"
#include "fable_game_text_bank.h"
#include "fable_stlport_tree_bounds.h"
#include "fable_handle_memory_pool.h"
#include "fable_critical_section_lock.h"
#include "fable_memory_allocator.h"
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long);

__forceinline void FABLE_FASTCALL CMemoryAllocator::Deallocate(void* allocation)
{
    EnterSynchronisation();
    if (allocation && Get()) {
        Get()->InternalDeallocate(allocation);
        if (g_FableAllocatorShutdownRequested_013CAA94 && Get()->GetNoAllocations()==0) {
            delete Get();
            g_FableMemoryAllocator_013CAA90=0;
        }
    }
    LeaveSynchronisation();
}

void CBankFileAsyncData::FreeBuffer()
{
    if (MemoryPool && MemoryHandle!=-1) {
        MemoryPool->FreeResource(MemoryHandle);
        MemoryHandle=-1;
        Buffer=0;
        Data=0;
    }
}

CBankFileAsyncData::~CBankFileAsyncData()
{
    if (!Aborted) {
        CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(BankFile.CriticalSection),false);
        BankFile.AsyncDataInstances.erase(this);
        BankFile.ReadsToRestart.erase(this);
        if (ThreadedFileUID) {
            while (!BankFile.ThreadedFile->RemoveObject(ThreadedFileUID)) {
                lock.Release();
                Sleep(0);
                lock.Lock();
            }
        }
        FreeBuffer();
    }
    if (InternalAllocatedBufferSize>0)
        CMemoryAllocator::Deallocate(Buffer);
}

bool CBankFileAsyncData::Abort()
{
    CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(BankFile.CriticalSection),false);
    if (Finished) return true;
    bool result=false;
    if (BankFile.ReadsToRestart.erase(this)!=0) {
        result=true;
    } else if (BankFile.ThreadedFile->RemoveObject(ThreadedFileUID)) {
        ThreadedFileUID=0;
        result=true;
    }
    if (result) {
        BankFile.AsyncDataInstances.erase(this);
        FreeBuffer();
        Aborted=true;
    }
    return result;
}

// Retail 009D5BF0: rotate through the live reads, reclaim the first finished
// restartable buffer, and queue that read to be restarted when memory is free.
bool CBankFileAsync::HandleFailedAllocation()
{
    CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection),false);
    unsigned long count=AsyncDataInstances.size();
    if (!count) return false;
    _STL::set<CBankFileAsyncData*>::iterator it=AsyncDataInstances.upper_bound(LastRestartingData);
    for (unsigned long i=0;i<count;++i) {
        if (it==AsyncDataInstances.end()) it=AsyncDataInstances.begin();
        CBankFileAsyncData* data=*it;
        if (data->Finished && data->AllowRestart) {
            data->Finished=false;
            data->FreeBuffer();
            ReadsToRestart.insert(data);
            LastRestartingData=data;
            return true;
        }
        ++it;
    }
    return false;
}
