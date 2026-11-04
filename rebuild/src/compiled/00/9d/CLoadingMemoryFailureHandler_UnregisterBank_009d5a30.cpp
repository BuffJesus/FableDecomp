#include "fable_loading_memory_failure_handler.h"
#include "fable_critical_section_lock.h"
#include "fable_async_bank_find.h"
#include <string.h>
void CLoadingMemoryFailureHandler::UnregisterBank(CBankFileAsync* bank) {
    CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection),false);
    CBankFileAsync** position=_STL::__find(Banks.begin(),Banks.end(),bank,_STL::random_access_iterator_tag());
    if(position!=Banks.end()) {
        _STL::__copy_trivial(position+1,Banks.end(),position);
        --Banks._M_finish;
    }
}
