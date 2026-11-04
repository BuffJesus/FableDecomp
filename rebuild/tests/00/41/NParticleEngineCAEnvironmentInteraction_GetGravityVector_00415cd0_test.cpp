// Behaviour fixture for CAEnvironmentInteraction::GetGravityVector (0x00415CD0).
// Links the compiled source object; called non-virtually on raw storage.
#include "fable_particle_engine.h"

#include <stdio.h>
#include <string.h>

int main()
{
    int fail = 0;
    unsigned long raw[9];
    memset(raw, 0xA5, sizeof(raw));
    const NParticleEngine::CAEnvironmentInteraction* p =
        reinterpret_cast<const NParticleEngine::CAEnvironmentInteraction*>(raw);

    C3DVector g(1.0f, 2.0f, 3.0f);
    g = p->NParticleEngine::CAEnvironmentInteraction::GetGravityVector();
    if (g.X != 0.0f || g.Y != 0.0f || g.Z != -9.82f) fail |= 1;
    // Bit-exact: 0xC11D1EB8.
    unsigned long bits;
    memcpy(&bits, &g.Z, 4);
    if (bits != 0xC11D1EB8) fail |= 2;
    for (int i = 0; i < 9; ++i)
        if (raw[i] != 0xA5A5A5A5)
            fail |= 4;

    if (fail == 0)
        printf("CAENVIRONMENTINTERACTION_GETGRAVITYVECTOR_PASS\n");
    else
        printf("CAENVIRONMENTINTERACTION_GETGRAVITYVECTOR_FAIL %d\n", fail);
    return 0;
}