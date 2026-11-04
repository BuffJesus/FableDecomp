#include "fable_particle_engine.h"
#include <stdio.h>

static char g_data[3][16];

template<typename T> static int Run(CCountedPointer<T>& pointer, int selector, unsigned long count)
{
    T* data = selector ? (T*)g_data[selector] : 0;
    CCPPointerInfo* info = new CCPPointerInfo(0, data);
    info->RefCount = count;
    pointer.Data = data;
    pointer.Info = info;
    int failed = pointer.Data != data || pointer.Info != info || pointer.Info->RefCount != count;
    pointer.Data = 0;
    pointer.Info = 0;
    delete info;
    return failed;
}

int main()
{
    using namespace NParticleEngine;
    if (PEmitterDatabase.Data || PEmitterDatabase.Info ||
        PEmitterManager.Data || PEmitterManager.Info ||
        PPSCManager.Data || PPSCManager.Info) return 1;
    int cases = 0;
    for (int selector = 0; selector < 3; ++selector)
    for (unsigned long count = 0; count < 4; ++count)
    {
        if (Run(PEmitterDatabase, selector, count) ||
            Run(PEmitterManager, selector, count) ||
            Run(PPSCManager, selector, count)) return 2;
        cases += 3;
    }
    printf("PARTICLE_MANAGER_GLOBALS_BEHAVIOR_PASS cases=%d\n", cases);
    return 0;
}
