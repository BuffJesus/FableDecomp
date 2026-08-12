// CEngineSubPrimitiveGameMesh::`vector_deleting_destructor'
// retail 0x00b9c110 __fastcall(this, uint flags) -> void*
// Standard MSVC scalar/vector deleting destructor shape:
//   push esi; mov esi,ecx; call ~dtor; test [flags],1; je; mov ecx,esi; call op_delete; mov eax,esi; pop esi; ret 4

struct CEngineSubPrimitiveGameMesh
{
    void Destroy();      // scalar destructor body  (retail 0x0099a300)
    void FreeSelf();     // operator delete forwarder (retail 0x00beb550)

    void* VecDelDtor_CEngineSubPrimitiveGameMesh(unsigned int flags);
};

void* CEngineSubPrimitiveGameMesh::VecDelDtor_CEngineSubPrimitiveGameMesh(unsigned int flags)
{
    this->Destroy();
    if (flags & 1)
        this->FreeSelf();
    return this;
}