#include <cstdio>

struct CParticleSimulationRenderData { int dummy; };

static bool IsBusy_model(CParticleSimulationRenderData* self)
{
    return false;
}

int main()
{
    CParticleSimulationRenderData a; a.dummy = 0;
    CParticleSimulationRenderData b; b.dummy = 123;
    bool r1 = IsBusy_model(&a);
    bool r2 = IsBusy_model(&b);
    if (r1 != false) { printf("FAIL r1\n"); return 1; }
    if (r2 != false) { printf("FAIL r2\n"); return 2; }
    printf("OK_00bb1dd0\n");
    return 0;
}