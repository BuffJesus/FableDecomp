// CEngineSubPrimitiveGameMesh::`vector deleting destructor' @ 0x00529ab0
// Pattern: call scalar dtor (this in ecx), then if (flags&1) operator delete(this),
// return this. Modeled as a free __fastcall so it always has external linkage.

struct CEngineSubPrimitiveGameMesh;

void __fastcall dtor_scalar_529ad0(CEngineSubPrimitiveGameMesh* self);  // 0x529ad0
void __fastcall op_delete_4c73b0(CEngineSubPrimitiveGameMesh* self);    // 0x4c73b0

void* __fastcall CEngineSubPrimitiveGameMesh_vector_deleting_destructor(
    CEngineSubPrimitiveGameMesh* self, int /*edx*/, unsigned int flags)
{
    dtor_scalar_529ad0(self);
    if (flags & 1)
        op_delete_4c73b0(self);
    return self;
}