// Retail 0x00b9c130 : CEngineSubPrimitiveGameMesh::`vector_deleting_destructor'
// __fastcall, returns void* (this). Standard MSVC vector-deleting-destructor shape:
//   push esi; mov esi,ecx; call <dtor>; test [esp+8],1; je +; mov ecx,esi; call <free>;
//   mov eax,esi; pop esi; ret 4
//
// Both callees receive `this` in ecx (thiscall). Model them as thiscall members on
// a forward struct so the compiler emits the exact ecx-loaded call sequence.

struct CEngineSubPrimitiveGameMesh
{
    void Destroy();                 // scalar destructor  -> 0x0099a300
    void FreeThis();                // thiscall free/delete -> 0x00beb550
    void* VectorDeletingDtor(unsigned int flags);
};

// Destroy() / FreeThis() are declared-only (defined elsewhere in the image) so
// the compiler emits real thiscall CALL instructions to external symbols.

void* CEngineSubPrimitiveGameMesh::VectorDeletingDtor(unsigned int flags)
{
    this->Destroy();
    if (flags & 1)
        this->FreeThis();
    return this;
}