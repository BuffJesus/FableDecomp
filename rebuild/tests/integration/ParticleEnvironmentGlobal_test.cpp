#include "fable_particle_engine.h"
#include <stdio.h>

static char g_data[3][16];

int main()
{
    using namespace NParticleEngine;
    if (PEnvironmentInteraction.Data || PEnvironmentInteraction.Info) return 1;
    int cases = 0;
    for (int selector = 0; selector < 3; ++selector)
    for (unsigned long count = 0; count < 4; ++count)
    {
        CAEnvironmentInteraction* data = selector ?
            (CAEnvironmentInteraction*)g_data[selector] : 0;
        CCPPointerInfo* info = new CCPPointerInfo(0, data);
        info->RefCount = count;
        PEnvironmentInteraction.Data = data;
        PEnvironmentInteraction.Info = info;
        if (PEnvironmentInteraction.Data != data ||
            PEnvironmentInteraction.Info != info ||
            PEnvironmentInteraction.Info->RefCount != count) return 2;
        PEnvironmentInteraction.Data = 0;
        PEnvironmentInteraction.Info = 0;
        delete info;
        ++cases;
    }
    printf("PARTICLE_ENVIRONMENT_GLOBAL_BEHAVIOR_PASS cases=%d\n", cases);
    return 0;
}
