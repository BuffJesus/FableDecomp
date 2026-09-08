#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

struct CEntityLimboView {
    unsigned char _pad_0x00[0x91];
    unsigned char Flags;
};

bool __stdcall CGameScriptInterface_IsEntityInLimbo(CGameScriptInterface* self)
{
    CEntityLimboView* (__fastcall **vtable)(void*) =
        (CEntityLimboView* (__fastcall **)(void*))self->__vftable;
    CEntityLimboView* entity =
        (*(CEntityLimboView* (__fastcall **)(void*))((char*)vtable + 0x2c))(self);
    if (entity == 0)
        return false;
    unsigned char flags = entity->Flags;
    if (!(flags & 1))
        return (flags & 0x20) ? true : false;
    return false;
}
