// CEngineSubPrimitiveGameMesh::`vector deleting destructor' @ 0052abf0
// Standard MSVC scalar/vector deleting destructor thunk.

struct CEngineSubPrimitiveGameMesh
{
    void destroy();               // the real (non-virtual-call) destructor body @ 0x6aa230
    void* vec_deleting_destructor(unsigned int flags);
};

// operator delete equivalent (@ 0x4c73b0) modeled as __fastcall-ish free fn so the
// compiler emits a direct call with 'this' in ecx.
void __fastcall mesh_free(void* self);

void* CEngineSubPrimitiveGameMesh::vec_deleting_destructor(unsigned int flags)
{
    this->destroy();
    if (flags & 1)
        mesh_free(this);
    return this;
}