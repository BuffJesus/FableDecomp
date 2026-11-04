#include "fable_game_text_bank.h"
#include "fable_critical_section_lock.h"

// Retail 009D5670. VC71/Win32 unsigned long is the raw 32-bit pointer width.
void CBankFileAsyncData::RelocateData(long, void* oldBase, void* newBase, long)
{
    CCriticalSectionLock lock(
        *reinterpret_cast<_RTL_CRITICAL_SECTION*>(BankFile.CriticalSection), false);
    if (Buffer)
        Buffer = reinterpret_cast<void*>(
            reinterpret_cast<unsigned long>(Buffer) -
            reinterpret_cast<unsigned long>(oldBase) +
            reinterpret_cast<unsigned long>(newBase));
    if (Data)
        Data = reinterpret_cast<void*>(
            reinterpret_cast<unsigned long>(Data) -
            reinterpret_cast<unsigned long>(oldBase) +
            reinterpret_cast<unsigned long>(newBase));
}
