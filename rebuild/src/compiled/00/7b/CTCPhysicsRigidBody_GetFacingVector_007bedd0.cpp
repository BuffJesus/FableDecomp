// CTCPhysicsRigidBody::GetFacingVector  (retail 0x007BEDD0)
//
// __fastcall struct-return accessor. It copies the 12-byte C3DVector facing
// value stored at raw byte offset 0x51 in the rigid body into the caller's
// hidden return buffer:
//     mov edx,[ecx+0x51]; mov eax,[esp+4]; mov [eax],edx; ...
//
// Under the MSVC7.1 __fastcall struct-return ABI, `this` arrives in ecx and the
// address of the return object is a hidden stack argument (at [esp+4] on entry).
// Modeling this as a __fastcall free function (ecx=this-source, edx=unused pad,
// stack=out) reproduces the exact register allocation and the interleaved
// load/store the compiler emits for an out-parameter copy that may alias its
// source. Writing the natural by-value __fastcall member instead makes VC7.1
// emit an NRVO temporary (extra spills) and does not byte-match.

struct C3DVector { unsigned long x, y, z; };

#pragma pack(push, 1)
struct CTCPhysicsRigidBody {
    char pad[0x51];
    unsigned long facing_x;   // +0x51
    unsigned long facing_y;   // +0x55
    unsigned long facing_z;   // +0x59
};
#pragma pack(pop)

void __fastcall GetFacingVector(CTCPhysicsRigidBody* self, int /*edx pad*/, C3DVector* out)
{
    out->x = self->facing_x;
    out->y = self->facing_y;
    out->z = self->facing_z;
}