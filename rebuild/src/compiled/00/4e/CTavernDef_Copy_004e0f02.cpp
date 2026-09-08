#include "engine/CTavernDef.h"

struct CDefClassBase;
struct CTavernDef_Methods : CTavernDef { void base_clone(const CDefClassBase* src); };

void __fastcall CTavernDef_Copy(CTavernDef* self, int /*edx*/, const CDefClassBase* src)
{
    ((CTavernDef_Methods*)self)->base_clone(src);
    self->BedCost = ((const CTavernDef*)src)->BedCost;
}
