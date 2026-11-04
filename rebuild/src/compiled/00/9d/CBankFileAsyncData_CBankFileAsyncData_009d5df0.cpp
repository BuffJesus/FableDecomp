#include "fable_game_text_bank.h"
#include "fable_critical_section_lock.h"

// Retail 009D5D30: caller-supplied buffer; no pool, no skip.
CBankFileAsyncData::CBankFileAsyncData(void* buffer, unsigned long length, CBankFileAsync& bank)
    : Data(buffer), Buffer(buffer), Length(length), SkipBytes(0),
      ThreadedFileUID(0), MemoryPool(0), MemoryHandle(-1),
      BankFile(bank), Finished(false), BufferLocked(false),
      AllowRestart(false), Aborted(false)
{
    CCriticalSectionLock lock(
        *reinterpret_cast<_RTL_CRITICAL_SECTION*>(BankFile.CriticalSection), false);
    BankFile.AsyncDataInstances.insert(this);
    InternalAllocatedBufferSize = 0;
}

// Retail 009D5DF0: genuine private four-argument constructor.
// Base construction and both vtables are compiler-generated, not patched here.
CBankFileAsyncData::CBankFileAsyncData(
    unsigned long length, unsigned long skip,
    CMovableResourceMemoryPool* pool, CBankFileAsync& bank)
    : Data(0), Buffer(0), Length(length), SkipBytes(skip),
      ThreadedFileUID(0), MemoryPool(pool), MemoryHandle(-1),
      BankFile(bank), Finished(false), BufferLocked(false),
      AllowRestart(false), Aborted(false)
{
    CCriticalSectionLock lock(
        *reinterpret_cast<_RTL_CRITICAL_SECTION*>(BankFile.CriticalSection), false);
    BankFile.AsyncDataInstances.insert(this);
    // Deliberately uninitialized during lock acquisition and registration.
    InternalAllocatedBufferSize = 0;
}
