// CEngineInternalPrimitiveStaticMesh::SetPosition @ 00b9cfa0
struct CEnginePrimitivePosition {
    char pad[0xc];
    int payload; // member at +0xc
};

// callee at 0xbebaa0 is a __fastcall-shaped this-call:
//   this  -> ecx (self->sub, i.e. this+0x70)
//   rawPos-> edx (the CEnginePrimitivePosition pointer)
//   stack -> (payloadPtr = &pos.payload, field58 = self->[+0x58])
// It cleans its own 2 stack args (8 bytes); this function cleans its 1 ref arg (ret 4).
void __fastcall CEngineInternalPrimitiveStaticMesh_Sub_Apply(
        void* self, const CEnginePrimitivePosition* rawPos,
        const void* payloadPtr, int field58);

struct CEngineInternalPrimitiveStaticMesh {
    char pad0[0x58];
    int field58;                 // +0x58
    char pad1[0x70 - 0x58 - 4];
    char sub;                    // +0x70 (embedded sub-object)
};

// __fastcall method: 'this' in ecx, the CEnginePrimitivePosition reference on the
// stack. Modeled as __fastcall(self, dummy-edx, stack-ref) so the ref stays on the
// stack and the function ends with ret 4.
void __fastcall CEngineInternalPrimitiveStaticMesh_SetPosition(
        CEngineInternalPrimitiveStaticMesh* self,
        int /*edx*/,
        const CEnginePrimitivePosition& pos)
{
    CEngineInternalPrimitiveStaticMesh_Sub_Apply(
        &self->sub, &pos, &pos.payload, self->field58);
}