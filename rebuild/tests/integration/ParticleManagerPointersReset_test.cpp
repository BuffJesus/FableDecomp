#include "fable_particle_engine.h"
#include "fable_symbol_map.h"
#include <stdio.h>

// Link-only destructor boundaries for the typed DeleteData callbacks. The
// fixture's old blocks use DeleteBoundary, and new blocks are released by
// deleting their control blocks without invoking these pointee destructors.
namespace NParticleEngine
{
    CParticleEmitterDatabase::~CParticleEmitterDatabase() {}
    CParticleEmitterManager::~CParticleEmitterManager() {}
    CPSCManager::~CPSCManager() {}
}
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CCharString::~CCharString() {}
template<> _STL::set<CCharString>::~set() {}

static char g_old_data[16], g_new_data[3][16];
static int g_callbacks;
static void __fastcall DeleteBoundary(void*) { ++g_callbacks; }

template<typename T> static int Run(int old_present, int count_index, int selector)
{
    const unsigned long counts[] = {0, 1, 2, 0xffffffffUL};
    union { double align; char bytes[sizeof(CCountedPointer<T>)]; } store;
    CCountedPointer<T>* pointer = (CCountedPointer<T>*)&store;
    CCPPointerInfo* oldInfo = new CCPPointerInfo(&DeleteBoundary, g_old_data);
    oldInfo->RefCount = counts[count_index];
    pointer->Data = (T*)g_old_data;
    pointer->Info = old_present ? oldInfo : 0;
    T* newData = selector ? (T*)g_new_data[selector] : 0;
    g_callbacks = 0;
    pointer->Reset(newData);
    int failed = pointer->Data != newData ||
        (pointer->Info == 0) != (newData == 0) ||
        g_callbacks != int(old_present && counts[count_index] == 1);
    if (newData && pointer->Info)
        failed |= pointer->Info->RefCount != 1 ||
                  pointer->Info->DeleteFunc == 0 ||
                  pointer->Info->Data != newData;
    if (old_present && counts[count_index] != 1)
        failed |= oldInfo->RefCount != counts[count_index] - 1;
    if (pointer->Info) delete pointer->Info;
    pointer->Data = 0;
    pointer->Info = 0;
    if (!old_present || counts[count_index] != 1) delete oldInfo;
    return failed;
}

int main()
{
    int cases = 0;
    for (int old = 0; old < 2; ++old)
    for (int count = 0; count < 4; ++count)
    for (int next = 0; next < 3; ++next)
    {
        if (Run<NParticleEngine::CParticleEmitterDatabase>(old, count, next) ||
            Run<NParticleEngine::CParticleEmitterManager>(old, count, next) ||
            Run<NParticleEngine::CPSCManager>(old, count, next)) return 1;
        cases += 3;
    }
    printf("PARTICLE_MANAGER_RESET_BEHAVIOR_PASS cases=%d\n", cases);
    return 0;
}
