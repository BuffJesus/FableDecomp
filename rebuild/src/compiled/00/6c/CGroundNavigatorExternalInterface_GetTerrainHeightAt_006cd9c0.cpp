#include "engine/CGroundNavigatorExternalInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct C3DVector { float x, y, z; };
struct CTerrainResult { float GetHeight(); };
struct CThingCreatureBase { CTerrainResult* Lookup(const C3DVector& v); };
float __fastcall CGroundNavigatorExternalInterface_GetTerrainHeightAt(CGroundNavigatorExternalInterface* self, void* edx, const C3DVector& v)
{
    CTerrainResult* r = self->Creature->Lookup(v);
    return r->GetHeight();
}