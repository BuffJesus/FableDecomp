#include "engine/CGoldDef.h"

struct CDefClassBase { char pad[0x2c]; };
struct CGoldDef_Methods : CGoldDef { void BaseClone(const CDefClassBase* other); };

void __fastcall Copy(CGoldDef* self, int /*edx*/, const CDefClassBase* other)
{
    ((CGoldDef_Methods*)self)->BaseClone(other);
    self->Gold = ((const CGoldDef*)other)->Gold;
}
