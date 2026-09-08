#include <cstdio>
#include "engine/CSmashableDef.h"

struct CBaseDef {
    void Copy(const CBaseDef* src);
};
static int g_base_called = 0;
void CBaseDef::Copy(const CBaseDef* src) { (void)src; g_base_called = 1; }

void __fastcall CSmashableDef_Copy(CSmashableDef* self, void* edx, const CSmashableDef* src)
{
    ((CBaseDef*)self)->Copy((const CBaseDef*)src);
    self->Smashable = src->Smashable;
    self->ReplacementObject_Val = src->ReplacementObject_Val;
    self->SmashParticleEmitter = src->SmashParticleEmitter;
}

int main()
{
    CSmashableDef dst; CSmashableDef s;
    s.Smashable = true; s.ReplacementObject_Val = 0x11223344; s.SmashParticleEmitter = 0x55667788;
    dst.Smashable = false; dst.ReplacementObject_Val = 0; dst.SmashParticleEmitter = 0;
    CSmashableDef_Copy(&dst, 0, &s);
    if (g_base_called && dst.Smashable && dst.ReplacementObject_Val == 0x11223344 && dst.SmashParticleEmitter == 0x55667788) {
        std::printf("CSmashableDef_00453842_TEST PASS\n");
        return 0;
    }
    std::printf("CSmashableDef_00453842_TEST FAIL\n");
    return 1;
}
