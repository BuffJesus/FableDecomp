#include "fable_game_text_bank.h"
#include "fable_critical_section_lock.h"

bool CBankFileAsyncData::CanRelocateData(long) const
{
    CCriticalSectionLock lock(
        *reinterpret_cast<_RTL_CRITICAL_SECTION*>(BankFile.CriticalSection), false);
    if (Finished && !BufferLocked)
        return true;
    else
        return false;
}
