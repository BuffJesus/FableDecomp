// Retail 0x00b9c150 CEngineSubPrimitiveGameMesh::`vector_deleting_destructor'
// push esi; mov esi,ecx; call scalar_dtor(this); test [esp+8],1;
// if set { mov ecx,esi; call operator_delete(this); } mov eax,esi; ret 4

struct CEngineSubPrimitiveGameMesh;

// scalar destructor at 0x0099a300 (this in ecx)
extern void __fastcall SubPrim_ScalarDtor(CEngineSubPrimitiveGameMesh* self);
// operator delete at 0x00beb550 (this in ecx)
extern void __fastcall SubPrim_Delete(CEngineSubPrimitiveGameMesh* self);

void* __fastcall CEngineSubPrimitiveGameMesh_vector_deleting_destructor(
        CEngineSubPrimitiveGameMesh* self, int /*edx*/, unsigned int flags)
{
    SubPrim_ScalarDtor(self);
    if (flags & 1)
        SubPrim_Delete(self);
    return self;
}