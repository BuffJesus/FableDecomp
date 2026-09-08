#include "engine/CScriptThing.h"

struct CScriptThingVtbl {
    unsigned char _pad[0x12c];
    unsigned char (__fastcall *IsAlive)(CScriptThing* self);
};

bool __fastcall CScriptThing_IsDead(CScriptThing* self)
{
    return ((CScriptThingVtbl*)self->__vftable)->IsAlive(self) == 0;
}
