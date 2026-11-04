#pragma once
#include <stddef.h>
#include "rebuild_abi.h"
struct _RTL_CRITICAL_SECTION;
class CCriticalSectionLock {
public:
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool tryOnly);
    ~CCriticalSectionLock();
    void Release();
    void Lock();
    bool IsLocked() const { return Locked; }
    _RTL_CRITICAL_SECTION* CriticalSection;
    bool Locked;
};
FABLE_STATIC_ASSERT(sizeof(CCriticalSectionLock) == 8);
FABLE_STATIC_ASSERT(offsetof(CCriticalSectionLock, Locked) == 4);
