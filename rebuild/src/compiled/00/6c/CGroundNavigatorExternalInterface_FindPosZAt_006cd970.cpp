#include "engine/CGroundNavigatorExternalInterface.h"

struct C3DVector { float x, y, z; };
struct CThingCreatureBase { float FindPosZ(const C3DVector& pos, int flag, bool b); };

float __fastcall FindPosZAt(CGroundNavigatorExternalInterface* self, void* /*edx*/, const C3DVector& pos, bool b)
{
    return self->Creature->FindPosZ(pos, 0, b);
}
