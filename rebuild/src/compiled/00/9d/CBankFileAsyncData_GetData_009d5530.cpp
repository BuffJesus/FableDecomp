#include "fable_game_text_bank.h"
#include "fable_handle_memory_pool.h"
#include "fable_critical_section_lock.h"
void* CBankFileAsyncData::GetData()
{
    CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(BankFile.CriticalSection), false);
    if (MemoryPool && MemoryHandle != -1 && !BufferLocked)
    {
        MemoryPool->Lock(MemoryHandle);
        BufferLocked = true;
    }
    return Data;
}
