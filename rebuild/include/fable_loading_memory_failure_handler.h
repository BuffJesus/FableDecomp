#pragma once
#include "fable_carray.h"
#include "fable_failed_allocation_handler.h"
class CBankFileAsync;
// RTTI (vtable 0x0129B8C4): base CFailedAllocationHandler (vtable 0x0129B874).
class CLoadingMemoryFailureHandler : public CFailedAllocationHandler {
public:
    CLoadingMemoryFailureHandler();
    virtual ~CLoadingMemoryFailureHandler();
    virtual bool HandleFailedAllocation(unsigned long size);
    void RegisterBank(CBankFileAsync* bank);
    void UnregisterBank(CBankFileAsync* bank);
    CArrayVector<CBankFileAsync*> Banks;
    unsigned char CriticalSection[0x18];
};
FABLE_STATIC_ASSERT(sizeof(CLoadingMemoryFailureHandler)==0x28);
FABLE_STATIC_ASSERT(offsetof(CLoadingMemoryFailureHandler,Banks)==4);
FABLE_STATIC_ASSERT(offsetof(CLoadingMemoryFailureHandler,CriticalSection)==0x10);
