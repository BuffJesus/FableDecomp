// CEngineSubPrimitiveGameMesh::`vector deleting destructor'  0x00af7c80
// Standard MSVC vector-deleting-destructor thunk:
//   push esi; mov esi,ecx; call dtor(this); test [esp+8],1; je; mov ecx,esi; call delop(this);
//   mov eax,esi; pop esi; ret 4
// Both callees are __fastcall (this in ecx), so model as members.

struct CEngineSubPrimitiveGameMesh {
    void dtor_helper();          // 0x00af7c10 (scalar/base cleanup)
    void free_helper();          // 0x00af8ad0 (thiscall deallocator)
    void* vector_deleting_destructor(unsigned int flags);
};

void* CEngineSubPrimitiveGameMesh::vector_deleting_destructor(unsigned int flags)
{
    this->dtor_helper();
    if (flags & 1)
        this->free_helper();
    return this;
}