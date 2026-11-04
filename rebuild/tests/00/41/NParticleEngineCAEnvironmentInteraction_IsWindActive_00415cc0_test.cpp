// Behaviour fixture for CAEnvironmentInteraction::IsWindActive (0x00415CC0).
// Links the compiled source object; called non-virtually on raw storage.
#include "fable_particle_engine.h"

#include <stdio.h>
#include <string.h>

int main()
{
    int fail = 0;
    unsigned long raw[9];
    memset(raw, 0, sizeof(raw));
    const NParticleEngine::CAEnvironmentInteraction* p =
        reinterpret_cast<const NParticleEngine::CAEnvironmentInteraction*>(raw);

    // The base reports wind regardless of state.
    if (p->NParticleEngine::CAEnvironmentInteraction::IsWindActive() != true) fail |= 1;
    raw[5] = 0;   // a derived WindActive=false byte does not matter to the base
    if (!p->NParticleEngine::CAEnvironmentInteraction::IsWindActive()) fail |= 2;
    for (int i = 0; i < 9; ++i)
        if (raw[i] != 0)
            fail |= 4;

    if (fail == 0)
        printf("PASS_00415cc0\n");
    else
        printf("FAIL_00415cc0 %d\n", fail);
    return 0;
}
