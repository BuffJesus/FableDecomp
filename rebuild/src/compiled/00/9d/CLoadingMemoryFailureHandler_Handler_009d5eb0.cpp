#include "fable_loading_memory_failure_handler.h"
#include "fable_game_text_bank.h"
#include "fable_critical_section_lock.h"

extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(void*);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(void*);

// Retail 009D5EB0.
CLoadingMemoryFailureHandler::CLoadingMemoryFailureHandler()
{
    InitializeCriticalSection(CriticalSection);
}

// Inlined into the compiler-generated scalar deleting destructor 009D8140.
CLoadingMemoryFailureHandler::~CLoadingMemoryFailureHandler()
{
    DeleteCriticalSection(CriticalSection);
}

// Retail 009D5F10: ask each registered bank in turn to release a finished read.
bool CLoadingMemoryFailureHandler::HandleFailedAllocation(unsigned long)
{
    CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection), false);
    for (unsigned long i = 0; i < Banks.size(); ++i)
        if (Banks[i]->HandleFailedAllocation())
            return true;
    return false;
}
