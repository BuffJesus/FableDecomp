struct C3DVector { float x, y, z; };
struct Nav { float FindPosZAt(const C3DVector& v, bool a, bool b); };
struct CGroundNavigatorExternalInterface { char pad[4]; Nav* f4; };
float __fastcall CGroundNavigatorExternalInterface_FindPosZAtIncludingThemes(CGroundNavigatorExternalInterface* self, void* edx, const C3DVector& v, bool inc)
{
    return self->f4->FindPosZAt(v, true, inc);
}