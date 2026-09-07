#include "engine/CThingSearchTools.h"  // retyped onto the PDB layout; byte parity re-verified
struct TypeList { char data[20]; }; // sizeof 20

struct CThingManager {
    TypeList* base;  // +0x0
    TypeList* lists; // +0x4
};


// __fastcall: this in ecx, type param on stack (ret 0x4); sizeof(list)=20
TypeList* __fastcall CThingSearchTools_PeekTypeList(CThingSearchTools* self, int /*edx*/, int type)
{
    return self->ThingManager->lists + (unsigned char)type;
}