// Behaviour fixture for CEnvironment::UpdateInterpolation (provider CEnvironment_UpdateInterpolation_006b32a0.cpp). The
// environment's constructor and vtable are not linked, so it is raw storage laid out like retail; the fixture defines a
// recording CEnvironmentSpellEffectInterface::FrameUpdate. The call must reach the interface held at +0x1960 (not the
// interpolator at +0x1964) once per update, and leave the environment untouched.
#include <stdio.h>
#include <string.h>
#include "fable_environment.h"

static int g_fail, g_calls;
static const void* g_this;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

void CEnvironmentSpellEffectInterface::FrameUpdate() { ++g_calls; g_this = this; }

int main()
{
    static unsigned char raw[0x1A00];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = (unsigned char)i;
    char interfaceA[4], interfaceB[4], interpolator[4];
    void* a = interfaceA; void* in = interpolator;
    memcpy(raw + 0x1960, &a, 4); memcpy(raw + 0x1964, &in, 4);
    static unsigned char before[sizeof(raw)];
    memcpy(before, raw, sizeof(raw));
    CEnvironment* environment = reinterpret_cast<CEnvironment*>(raw);

    environment->UpdateInterpolation();
    CHECK(g_calls == 1 && g_this == interfaceA);
    CHECK(memcmp(raw, before, sizeof(raw)) == 0);
    void* b = interfaceB; memcpy(raw + 0x1960, &b, 4);
    environment->UpdateInterpolation();
    CHECK(g_calls == 2 && g_this == interfaceB);

    if (g_fail) { printf("ENVIRONMENT_UPDATE_INTERPOLATION_FAIL %d\n", g_fail); return 1; }
    printf("ENVIRONMENT_UPDATE_INTERPOLATION_OK\n");
    return 0;
}
