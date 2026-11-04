#include "fable_threaded_file.h"
#include "fable_game_text_bank.h"
#include "fable_critical_section_lock.h"
void CBankFileAsync::RestartAbortedReads()
{
    CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection), false);
    for (_STL::set<CBankFileAsyncData*>::iterator it=ReadsToRestart.begin();
         it!=ReadsToRestart._M_t.end(); ++it) {
        CBankFileAsyncData* data=*it;
        data->ThreadedFileUID=data->BankFile.ThreadedFile->AddObject(data->LoadDesc);
    }
    ReadsToRestart._M_t.clear();
}
