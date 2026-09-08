#include "engine/CEngineScreenEffectColourFilter.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEngineVideoOptionsDef {
    char pad[0x72];
    unsigned char flag72;   // +0x72
};


// __fastcall modeled: self in ecx, one stack arg (def). __fastcall with a
// dummy edx placeholder pushes the real arg onto the stack -> ret 4.
void __fastcall SetEffectQuality(CEngineScreenEffectColourFilter* self, int /*edx*/, CEngineVideoOptionsDef* def)
{
    if (def->flag72 && self->EffectSupported)
        self->enabled = 1;
    else
        self->enabled = 0;
}