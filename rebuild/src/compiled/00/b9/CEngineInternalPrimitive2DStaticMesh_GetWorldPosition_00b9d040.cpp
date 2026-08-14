struct C3DVector { float x, y, z; };

struct CEngineInternalPrimitive2DStaticMesh {
    char pad[0x108];
    C3DVector m_worldPos;
};

// this-call: self in ecx, out-vector& as the single stack arg.
// Model with a dummy edx param so the real arg lands on the stack.
bool __fastcall CEngineInternalPrimitive2DStaticMesh_GetWorldPosition(
    CEngineInternalPrimitive2DStaticMesh* self, int edx_dummy, C3DVector& out)
{
    (void)edx_dummy;
    out = self->m_worldPos;
    return true;
}