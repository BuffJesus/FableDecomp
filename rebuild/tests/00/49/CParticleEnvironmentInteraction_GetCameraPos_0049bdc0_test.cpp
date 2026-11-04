// Behaviour fixture for CParticleEnvironmentInteraction::GetCameraPos (0x0049BDC0).
// Links the compiled source object. The world is a stand-in object whose
// vtable slot 8 (CWorld::GetGameLODCentre) records the call.
#include "fable_particle_engine.h"

#include <stdio.h>
#include <string.h>

static int g_lodCalls;
static const void* g_lodThis;

// Same slot layout as CWorld's primary vtable up to slot 8.
class CFakeWorld
{
public:
    virtual void Slot0() {}
    virtual void Slot1() {}
    virtual void Slot2() {}
    virtual void Slot3() {}
    virtual void Slot4() {}
    virtual void Slot5() {}
    virtual void Slot6() {}
    virtual void Slot7() {}
    virtual C3DVector GetGameLODCentre() const
    {
        ++g_lodCalls;
        g_lodThis = this;
        return C3DVector(11.0f, -22.0f, 33.5f);
    }
};

int main()
{
    int fail = 0;
    unsigned char raw[sizeof(CParticleEnvironmentInteraction)];
    memset(raw, 0, sizeof(raw));
    CParticleEnvironmentInteraction* pei = reinterpret_cast<CParticleEnvironmentInteraction*>(raw);
    C3DVector fixed(1.5f, 2.5f, 3.5f);
    memcpy(raw + 0x15, &fixed, sizeof(fixed));   // CameraPos at +0x15
    C3DVector wind(9.0f, 9.0f, 9.0f);
    memcpy(raw + 0x08, &wind, sizeof(wind));

    // No world: the fixed camera position (read from +0x15).
    C3DVector out(0.0f, 0.0f, 0.0f);
    pei->CParticleEnvironmentInteraction::GetCameraPos(out);
    if (out.X != 1.5f || out.Y != 2.5f || out.Z != 3.5f || g_lodCalls != 0) fail |= 1;

    // With a world: its game LOD centre.
    CFakeWorld world;
    const CWorld* worldPtr = reinterpret_cast<const CWorld*>(&world);
    memcpy(raw + 0x04, &worldPtr, 4);
    pei->CParticleEnvironmentInteraction::GetCameraPos(out);
    if (out.X != 11.0f || out.Y != -22.0f || out.Z != 33.5f) fail |= 2;
    if (g_lodCalls != 1 || g_lodThis != &world) fail |= 4;

    // The interaction itself is untouched.
    C3DVector check;
    memcpy(&check, raw + 0x15, sizeof(check));
    if (check.X != 1.5f || raw[0x14] != 0) fail |= 8;

    if (fail == 0)
        printf("CPARTICLEENVIRONMENTINTERACTION_GETCAMERAPOS_PASS\n");
    else
        printf("CPARTICLEENVIRONMENTINTERACTION_GETCAMERAPOS_FAIL %d\n", fail);
    return 0;
}