// CEngineSubPrimitiveGameMesh::`vector_deleting_destructor'
// retail 0x005299a0  __fastcall(void* this, unsigned int flags) -> void*
//
//   push esi; mov esi,ecx
//   call 0x838a00              ; scalar destructor helper (this in ecx)
//   test byte ptr [esp+8],1    ; flags & 1
//   je  skip
//   mov ecx,esi; call 0x4c73b0 ; this-in-ecx delete helper
//   skip: mov eax,esi; pop esi; ret 4

struct CEngineSubPrimitiveGameMesh
{
    // scalar destructor helper (retail 0x838a00) — this-in-ecx member call
    void ScalarDtor();
    // this-in-ecx delete helper (retail 0x4c73b0)
    void DeleteHelper();

    void* VectorDeletingDestructor(unsigned int flags);
};

void* CEngineSubPrimitiveGameMesh::VectorDeletingDestructor(unsigned int flags)
{
    this->ScalarDtor();
    if (flags & 1)
        this->DeleteHelper();
    return this;
}