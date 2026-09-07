#include "engine/CGroundNavigatorExternalInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct C3DVector { float x, y, z; };
struct CThingCreatureBase { float FindPosZAt(const C3DVector& v, bool a, bool b); };
float __fastcall CGroundNavigatorExternalInterface_FindPosZAtIncludingThemes(CGroundNavigatorExternalInterface* self, void* edx, const C3DVector& v, bool inc)
{
    return self->Creature->FindPosZAt(v, true, inc);
}