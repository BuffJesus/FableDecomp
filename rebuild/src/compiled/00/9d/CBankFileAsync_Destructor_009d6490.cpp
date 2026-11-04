#include "fable_game_text_bank.h"
#include "fable_critical_section_lock.h"
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION*);
namespace _STL {
    template<> inline list<CCountedPointer<CBankFileAsyncData> >::~list() {
        clear();
        if(_M_node) free(_M_node);
    }
}
CBankFileAsync::~CBankFileAsync() {
    {
        CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection),false);
        while(!AsyncDataInstances.empty()) {
            FableBankOwnedSet<CBankFileAsyncData*>::iterator it=AsyncDataInstances.begin();
            if((*it)->Abort()) AsyncDataInstances.erase(it);
            else { lock.Release(); Sleep(10); lock.Lock(); }
        }
        lock.Release();
        if(MemoryFailureHandler.Data) MemoryFailureHandler->UnregisterBank(this);
        DeleteCriticalSection(reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection));
    }
}
