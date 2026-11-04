#include "fable_game_text_bank.h"
#include "fable_critical_section_lock.h"

// Retail 009D5440: priority is unused, SetupBuffer remains a child boundary.
bool CBankFileAsyncData::OnFileReadBegin(void** output, EThreadedFilePriority)
{
    CCriticalSectionLock lock(
        *reinterpret_cast<_RTL_CRITICAL_SECTION*>(BankFile.CriticalSection), true);
    if (lock.IsLocked() && SetupBuffer(false))
    {
        *output = Buffer;
        return true;
    }
    return false;
}
