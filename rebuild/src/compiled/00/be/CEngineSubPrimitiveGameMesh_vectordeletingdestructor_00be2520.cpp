// CEngineSubPrimitiveGameMesh::`vector_deleting_destructor'  @ 0x00be2520
// Standard MSVC vector deleting destructor idiom:
//   push esi; mov esi,ecx; call scalar_dtor; test [esp+8],1; je L;
//   mov ecx,esi; call delete_helper; L: mov eax,esi; pop esi; ret 4

struct CEngineSubPrimitiveGameMesh
{
    void Destroy();      // scalar destructor body   @ 0x0099a300
    void FreeMemory();   // delete-path helper        @ 0x00beb550
    void* vector_deleting_destructor(unsigned int flags);
};

// Destroy() / FreeMemory() are intentionally left undefined in this TU so the
// compiler must emit real external thiscall CALLs (matching retail).

void* CEngineSubPrimitiveGameMesh::vector_deleting_destructor(unsigned int flags)
{
    this->Destroy();
    if (flags & 1)
        this->FreeMemory();
    return this;
}