#include "engine/CQ_GuildTrainingPreMeleeScript.h"  // retyped onto the PDB layout; byte parity re-verified

extern int g_vtbl_c2c6b0;

// These four words belong to the script base subobject, whose donor PDB fields
// are unnamed padding in the derived-class layout.
struct CGuildTrainingScriptBaseState {
    void* __vftable;
    unsigned short State0;
    unsigned short State1;
    unsigned short State2;
    unsigned short State3;
};

struct CQ_GuildTrainingPreMeleeScript_Methods : CQ_GuildTrainingPreMeleeScript {
    CQ_GuildTrainingPreMeleeScript_Methods* Init();
};

CQ_GuildTrainingPreMeleeScript_Methods*
CQ_GuildTrainingPreMeleeScript_Methods::Init()
{
    CGuildTrainingScriptBaseState* base =
        (CGuildTrainingScriptBaseState*)this;
    base->__vftable = &g_vtbl_c2c6b0;
    base->State0 = 0;
    base->State1 = 0;
    base->State2 = 0;
    base->State3 = 0;
    return this;
}
