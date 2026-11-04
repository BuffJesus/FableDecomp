#include "fable_game_text_bank.h"
#include "fable_critical_section_lock.h"

// Retail 009D54A0: incoming completion status is unused.
void CBankFileAsyncData::OnFileReadCompletion(bool)
{
    CCriticalSectionLock lock(
        *reinterpret_cast<_RTL_CRITICAL_SECTION*>(BankFile.CriticalSection), false);
    Finished = true;
    ThreadedFileUID = 0;
    if (MemoryHandle != -1)
        AllowRestart = true;
}
