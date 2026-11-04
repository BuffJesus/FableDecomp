#include "fable_game_text_bank.h"
#include "fable_critical_section_lock.h"
void CBankFileAsync::ProcessDisposalList()
{
    CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection), false);
    while (DisposalList.size() && DisposalList.front()->Abort())
        DisposalList.pop_front();
}
