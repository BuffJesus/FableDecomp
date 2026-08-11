// CEngineSubPrimitiveGameMesh::`vector_deleting_destructor' @ 0x00529ca0
// Standard MSVC deleting destructor:
//   push esi; mov esi,ecx; call <dtor>; test [esp+8],1; je +; mov ecx,esi; call <delete>; mov eax,esi; ret 4

struct CEngineSubPrimitiveGameMesh {
    void *vfptr;
};

// Base/class destructor forwarded to (direct call, this in ecx).
void __fastcall CEngineSubPrimitiveGameMesh_dtor(CEngineSubPrimitiveGameMesh *self);
// Fable operator-delete wrapper: pointer passed in ecx (thiscall-style).
void __fastcall CEngineSubPrimitiveGameMesh_del(void *p);

void * __fastcall CEngineSubPrimitiveGameMesh_vector_deleting_destructor(
    CEngineSubPrimitiveGameMesh *self, int /*edx*/, unsigned int flags)
{
    CEngineSubPrimitiveGameMesh_dtor(self);
    if (flags & 1)
        CEngineSubPrimitiveGameMesh_del(self);
    return self;
}