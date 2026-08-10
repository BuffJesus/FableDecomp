struct C3DVector { float x, y, z; };
struct CGroundNavigator { float FindPosZ(const C3DVector& pos, int flag, bool b); };
struct CGroundNavigatorExternalInterface { void* pad0; CGroundNavigator* nav; };

float __fastcall FindPosZAt(CGroundNavigatorExternalInterface* self, void* edx, const C3DVector& pos, bool b)
{
    return self->nav->FindPosZ(pos, 0, b);
}