#include <cstdio>
struct C3DVector { float x, y, z; };
struct CTerrainResult { float GetHeight(); };
struct CTerrainThing { CTerrainResult* Lookup(const C3DVector& v); };
struct CGroundNavigatorExternalInterface { void* pad0; CTerrainThing* field4; };
float __fastcall CGroundNavigatorExternalInterface_GetTerrainHeightAt(CGroundNavigatorExternalInterface* self, void* edx, const C3DVector& v)
{
    CTerrainResult* r = self->field4->Lookup(v);
    return r->GetHeight();
}

static C3DVector g_seen;
static CTerrainThing* g_thingSeen;
static CTerrainResult g_result;
CTerrainResult* CTerrainThing::Lookup(const C3DVector& v){
    g_thingSeen = this; g_seen = v;
    return &g_result;
}
float CTerrainResult::GetHeight(){
    if (this == &g_result) return 42.5f;
    return -1.0f;
}
int main(){
    CTerrainThing thing;
    CGroundNavigatorExternalInterface obj; obj.pad0=0; obj.field4=&thing;
    C3DVector v; v.x=1; v.y=2; v.z=3;
    float h = CGroundNavigatorExternalInterface_GetTerrainHeightAt(&obj, 0, v);
    if (h==42.5f && g_thingSeen==&thing && g_seen.x==1 && g_seen.y==2 && g_seen.z==3) printf("OK_006cd9c0\n");
    else printf("FAIL h=%f\n", h);
    return 0;
}