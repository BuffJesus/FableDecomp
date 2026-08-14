// vector deleting destructor for CEngineSubPrimitiveGameMesh @ 0x00bdfd60
struct CEngineSubPrimitiveGameMesh;

// scalar destructor @ 0xbdfd80 : __fastcall(self) -> void
extern void __fastcall CEngineSubPrimitiveGameMesh_dtor(CEngineSubPrimitiveGameMesh* self);
// deallocator @ 0xbf9920 : ecx=self, edx=0
extern void __fastcall CEngineSubPrimitiveGameMesh_dealloc(CEngineSubPrimitiveGameMesh* self, int edx);

void* __fastcall CEngineSubPrimitiveGameMesh_vector_deleting_destructor(CEngineSubPrimitiveGameMesh* self, int edx, unsigned int flags)
{
    (void)edx;
    CEngineSubPrimitiveGameMesh_dtor(self);
    if (flags & 1) {
        CEngineSubPrimitiveGameMesh_dealloc(self, 0);
    }
    return self;
}