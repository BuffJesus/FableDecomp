#include "fable_loading_memory_failure_handler.h"
#include "fable_critical_section_lock.h"
#include <new>
#include "fable_stlport_vector_impl.h"

// Retail 009D6420: append the bank under the handler lock.
void CLoadingMemoryFailureHandler::RegisterBank(CBankFileAsync* bank)
{
    CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection), false);
    Banks.push_back(bank);
}
