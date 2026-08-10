struct C3DVector { float x, y, z; };
struct CTerrainResult { float GetHeight(); };
struct CTerrainThing { CTerrainResult* Lookup(const C3DVector& v); };
struct CGroundNavigatorExternalInterface { void* pad0; CTerrainThing* field4; };
float __fastcall CGroundNavigatorExternalInterface_GetTerrainHeightAt(CGroundNavigatorExternalInterface* self, void* edx, const C3DVector& v)
{
    CTerrainResult* r = self->field4->Lookup(v);
    return r->GetHeight();
}