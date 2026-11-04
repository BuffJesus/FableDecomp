#include "fable_critical_section_lock.h"
#include <stdio.h>
#include <string.h>
struct _RTL_CRITICAL_SECTION { int token; };
static unsigned enterCalls, tryCalls, leaveCalls;
static int tryResult;
static _RTL_CRITICAL_SECTION* lastSection;
static void __stdcall fakeEnter(_RTL_CRITICAL_SECTION* p) { ++enterCalls; lastSection=p; }
static int __stdcall fakeTry(_RTL_CRITICAL_SECTION* p) { ++tryCalls; lastSection=p; return tryResult; }
static void __stdcall fakeLeave(_RTL_CRITICAL_SECTION* p) { ++leaveCalls; lastSection=p; }
extern "C" __declspec(dllimport) int __stdcall VirtualProtect(void*, unsigned long, unsigned long, unsigned long*);
extern "C" char __ImageBase;
    void** FindImportSlot(const char* name)
    {
        char* base = &__ImageBase;
        char* nt = base + *reinterpret_cast<fable_u32*>(base + 0x3C);
        const fable_u32 rva = *reinterpret_cast<fable_u32*>(nt + 0x80);
        if (rva == 0)
            return 0;
        for (fable_u32* entry = reinterpret_cast<fable_u32*>(base + rva);
             entry[3] != 0;
             entry += 5)
        {
            fable_u32* names = reinterpret_cast<fable_u32*>(
                base + (entry[0] ? entry[0] : entry[4]));
            void** slots = reinterpret_cast<void**>(base + entry[4]);
            for (int i = 0; names[i] != 0; ++i)
            {
                if ((names[i] & 0x80000000u) == 0 &&
                    strcmp(base + names[i] + 2, name) == 0)
                {
                    return &slots[i];
                }
            }
        }
        return 0;
    }

    bool SwapImport(const char* name, void* replacement, void** previous)
    {
        void** slot = FindImportSlot(name);
        unsigned long oldProtect = 0;
        if (slot == 0 ||
            !VirtualProtect(slot, sizeof(void*), 0x04, &oldProtect))
        {
            return false;
        }
        if (previous != 0)
            *previous = *slot;
        *slot = replacement;
        VirtualProtect(slot, sizeof(void*), oldProtect, &oldProtect);
        return true;
    }


static int RunCases() {
    _RTL_CRITICAL_SECTION section;
    unsigned char storage[8]; memset(storage,0xA5,8);
    CCriticalSectionLock* lock=reinterpret_cast<CCriticalSectionLock*>(storage);
    lock->CriticalSection=&section;
    lock->Locked=true;
    lock->Release();
    if(leaveCalls!=1 || lastSection!=&section || lock->Locked!=false || lock->CriticalSection!=&section) return 1;
    lock->Release();
    if(leaveCalls!=1 || tryCalls || enterCalls) return 2;
    if(storage[5]!=0xA5 || storage[6]!=0xA5 || storage[7]!=0xA5) return 3;
    return 0;
}

int main() {
    void* previous0=0; if(!SwapImport("LeaveCriticalSection",(void*)&fakeLeave,&previous0)) return 90;
    int result=RunCases();
    if(!SwapImport("LeaveCriticalSection",previous0,0)) return 91;
    if(!result) puts("CRITICAL_SECTION_RELEASE_PASS");
    return result;
}
