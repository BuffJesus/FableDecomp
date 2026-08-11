// CEngineSubPrimitiveGameMesh::`vector deleting destructor'
// Retail 0x00704280. MSVC VC7.1 compiler-emitted vector-deleting-destructor thunk.

struct CEngineSubPrimitiveGameMesh
{
    void Destroy();   // scalar destructor body at 0x7042a0 (thiscall, this in ecx)
    void Free();      // custom deallocator at 0x4c73b0 (thiscall, this in ecx)
};

void* __fastcall CEngineSubPrimitiveGameMesh_vector_deleting_destructor(
    CEngineSubPrimitiveGameMesh* self, int edx, unsigned int flags)
{
    self->Destroy();
    if (flags & 1)
        self->Free();
    return self;
}