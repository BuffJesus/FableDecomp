#include <cstdio>

struct Inner { char pad[0x134]; float f134; };
struct Other { char pad[0x34]; float f34; };
struct CThingCreatureBase {
    char pad0[0x60];
    Other* q;
    char pad1[0xe0 - 0x64];
    Inner* p;
};
float g_scale = 2.0f;
float g_thresh = 5.0f;

float __fastcall CThingCreatureBase_GetCombatRadius(const CThingCreatureBase* self)
{
    float t = self->p->f134;
    if (t * g_scale > g_thresh)
        return t * g_scale;
    return self->q->f34;
}

int main()
{
    Inner inr;
    inr.f134 = 10.0f;
    Other oth;
    oth.f34 = 99.0f;
    CThingCreatureBase c;
    c.p = &inr;
    c.q = &oth;
    float r1 = CThingCreatureBase_GetCombatRadius(&c);
    if (r1 != 20.0f) { std::printf("FAIL r1=%f\n", r1); return 1; }

    inr.f134 = 1.0f;
    float r2 = CThingCreatureBase_GetCombatRadius(&c);
    if (r2 != 99.0f) { std::printf("FAIL r2=%f\n", r2); return 1; }

    std::printf("CThingCreatureBase_00661f90_TEST PASS\n");
    return 0;
}