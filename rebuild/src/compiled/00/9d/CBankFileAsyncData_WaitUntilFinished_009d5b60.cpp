#include "fable_threaded_file.h"
#include "fable_game_text_bank.h"
#include "fable_critical_section_lock.h"
bool CBankFileAsyncData::WaitUntilFinished()
{
    CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(BankFile.CriticalSection),false);
    if(!BankFile.DisableAsync && !Finished) {
        if(!SetupBuffer(true))return false;
        AllowRestart=false;
        BankFile.RestartAbortedReads();
        lock.Release();
        BankFile.ThreadedFile->WaitForObjectToFinish(ThreadedFileUID);
    }
    return true;
}
