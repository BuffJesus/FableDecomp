// CEngineSubPrimitiveGameMesh::`vector deleting destructor'  @ 0x00bb1740
struct CEngineSubPrimitiveGameMesh {
    void Destroy();   // scalar destructor -> 0x0099a300 (this in ecx)
    void* VDD(unsigned int flags);
};

// free-the-object helper (this in ecx, cleans own stack) -> 0x00beb550
void __fastcall FreeMesh(CEngineSubPrimitiveGameMesh* self);

void* CEngineSubPrimitiveGameMesh::VDD(unsigned int flags)
{
    this->Destroy();          // call 0x0099a300
    if (flags & 1)
        FreeMesh(this);       // mov ecx,esi; call 0x00beb550
    return this;              // mov eax,esi
}