#include "fable_loading_memory_failure_handler.h"
#include "fable_critical_section_lock.h"
#include <stdio.h>
#include <string.h>
static CLoadingMemoryFailureHandler* handler;
static unsigned locks,unlocks,errors;
static bool held;
CCriticalSectionLock::CCriticalSectionLock(_RTL_CRITICAL_SECTION& section,bool tryOnly) : CriticalSection(&section),Locked(true) {
    if(held || tryOnly || &section!=reinterpret_cast<_RTL_CRITICAL_SECTION*>(handler->CriticalSection))++errors;
    held=true;++locks;
}
CCriticalSectionLock::~CCriticalSectionLock() {if(!held || !Locked)++errors;held=false;++unlocks;}
int main() {
    unsigned char storage[sizeof(CLoadingMemoryFailureHandler)];memset(storage,0xA5,sizeof(storage));handler=reinterpret_cast<CLoadingMemoryFailureHandler*>(storage);
    CBankFileAsync* a=reinterpret_cast<CBankFileAsync*>(0x1000);
    CBankFileAsync* b=reinterpret_cast<CBankFileAsync*>(0x2000);
    CBankFileAsync* c=reinterpret_cast<CBankFileAsync*>(0x3000);
    CBankFileAsync* entries[8]={a,b,b,c};
    handler->Banks._M_set(entries,entries+4,entries+8);
    handler->UnregisterBank(b);
    if(errors || held || locks!=1 || unlocks!=1 || handler->Banks.size()!=3 || entries[0]!=a || entries[1]!=b || entries[2]!=c)return 1;
    handler->UnregisterBank(c);
    if(handler->Banks.size()!=2 || entries[0]!=a || entries[1]!=b)return 2;
    handler->UnregisterBank(c);
    if(handler->Banks.size()!=2 || entries[0]!=a || entries[1]!=b)return 3;
    handler->UnregisterBank(a);handler->UnregisterBank(b);handler->UnregisterBank(b);
    if(errors || held || locks!=6 || unlocks!=6 || handler->Banks.size()!=0 || handler->Banks.begin()!=entries || handler->Banks.capacity()!=8)return 4;
    handler->Banks._M_set(0,0,0);handler->UnregisterBank(a);
    if(errors || locks!=7 || unlocks!=7 || held)return 5;
    for(unsigned i=0x10;i<sizeof(storage);++i)if(storage[i]!=0xA5)return 6;
    if(*reinterpret_cast<unsigned long*>(storage)!=0xA5A5A5A5)return 7;
    puts("ASYNC_BANK_UNREGISTER_009d5a30_PASS");return 0;
}
