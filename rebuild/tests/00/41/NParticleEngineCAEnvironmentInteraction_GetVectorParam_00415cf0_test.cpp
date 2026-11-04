// Behaviour fixture for CAEnvironmentInteraction::GetVectorParam (0x00415CF0).
// Links the compiled source object; called non-virtually on raw storage.
#include "fable_particle_engine.h"

#include <stdio.h>
#include <string.h>

int main()
{
    int fail = 0;
    unsigned long raw[9];
    memset(raw, 0x5A, sizeof(raw));
    const NParticleEngine::CAEnvironmentInteraction* p =
        reinterpret_cast<const NParticleEngine::CAEnvironmentInteraction*>(raw);
    // The name is never read: any address will do, even an unreadable one.
    const CCharString* name = reinterpret_cast<const CCharString*>(0x10);

    C3DVector v(7.0f, 8.0f, 9.0f);
    v = p->NParticleEngine::CAEnvironmentInteraction::GetVectorParam(*name);
    if (v.X != 0.0f || v.Y != 0.0f || v.Z != 0.0f) fail |= 1;
    for (int i = 0; i < 9; ++i)
        if (raw[i] != 0x5A5A5A5A)
            fail |= 2;

    if (fail == 0)
        printf("CAENVIRONMENTINTERACTION_GETVECTORPARAM_PASS\n");
    else
        printf("CAENVIRONMENTINTERACTION_GETVECTORPARAM_FAIL %d\n", fail);
    return 0;
}