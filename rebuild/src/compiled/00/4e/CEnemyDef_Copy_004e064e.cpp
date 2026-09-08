#include "engine/CEnemyDef.h"

struct CDefClassBase;
struct CEnemyDef_Methods : CEnemyDef { void base_clone(const CDefClassBase* src); };

void __fastcall CEnemyDef_Copy(CEnemyDef* self, int /*edx*/, const CDefClassBase* src)
{
    ((CEnemyDef_Methods*)self)->base_clone(src);
    self->Faction = ((const CEnemyDef*)src)->Faction;
}
