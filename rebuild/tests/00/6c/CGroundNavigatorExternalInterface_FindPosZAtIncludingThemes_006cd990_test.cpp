#include "engine/CGroundNavigatorExternalInterface.h"
#include <cstdio>
struct C3DVector { float x, y, z; };
struct CThingCreatureBase { float FindPosZAt(const C3DVector& v, bool a, bool b){ return v.x + (a?10.0f:0.0f) + (b?100.0f:0.0f); } };
float __fastcall CGroundNavigatorExternalInterface_FindPosZAtIncludingThemes(CGroundNavigatorExternalInterface* self, void* edx, const C3DVector& v, bool inc);
float __fastcall CGroundNavigatorExternalInterface_FindPosZAtIncludingThemes(CGroundNavigatorExternalInterface* self, void* edx, const C3DVector& v, bool inc)
{
    return self->Creature->FindPosZAt(v, true, inc);
}
int main(){
    CThingCreatureBase n; CGroundNavigatorExternalInterface o; o.Creature=&n;
    C3DVector v; v.x=1.0f; v.y=2.0f; v.z=3.0f;
    float r = CGroundNavigatorExternalInterface_FindPosZAtIncludingThemes(&o, 0, v, true);
    if (r == 111.0f) printf("OK_006cd990\n");
    else printf("FAIL %f\n", r);
    return 0;
}