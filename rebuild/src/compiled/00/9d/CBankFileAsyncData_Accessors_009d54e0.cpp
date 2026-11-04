#include "fable_game_text_bank.h"
#include "fable_critical_section_lock.h"
bool CBankFileAsyncData::IsFinished() const
{
    CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(BankFile.CriticalSection), false);
    if (Finished) {
        AllowRestart = false;
        return true;
    }
    return false;
}
unsigned long CBankFileAsyncData::Sizeof() const
{
    return sizeof(*this) + InternalAllocatedBufferSize;
}
