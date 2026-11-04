// Behaviour fixture for the chunk CCountedPointer<T>::DeleteData instantiations (provider
// CCountedPointer_ChunkDeleteData_00acf3b0.cpp; it defines none of them). DeleteData is private, so each instantiation is
// reached by its mangled name through /alternatename. The "chunk" is a fake object whose vtable slot 0 (the deleting
// destructor) records the call: DeleteData must run it with the delete flag on non-null data and skip null data.
#include <stdio.h>
#include <string.h>

static int g_fail, g_calls;
static void* g_self;
static unsigned g_flags;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static void* __fastcall FakeDeletingDtor(void* self, int, unsigned flags) { ++g_calls; g_self = self; g_flags = flags; return self; }

extern "C" void __fastcall DeleteGroup(void*);
extern "C" void __fastcall DeleteVertices(void*);
extern "C" void __fastcall DeleteTriangles(void*);
extern "C" void __fastcall DeletePrimitive(void*);
extern "C" void __fastcall DeleteRoot(void*);
extern "C" void __fastcall DeleteMap(void*);
extern "C" void __fastcall DeleteMeshFile(void*);
#pragma comment(linker, "/alternatename:@DeleteGroup@4=?DeleteData@?$CCountedPointer@VC3DMeshFileGroupChunk@@@@CIXPAX@Z")
#pragma comment(linker, "/alternatename:@DeleteVertices@4=?DeleteData@?$CCountedPointer@VC3DMeshFileVerticesChunk@@@@CIXPAX@Z")
#pragma comment(linker, "/alternatename:@DeleteTriangles@4=?DeleteData@?$CCountedPointer@VC3DMeshFileTrianglesChunk@@@@CIXPAX@Z")
#pragma comment(linker, "/alternatename:@DeletePrimitive@4=?DeleteData@?$CCountedPointer@VC3DMeshFilePrimitiveChunk@@@@CIXPAX@Z")
#pragma comment(linker, "/alternatename:@DeleteRoot@4=?DeleteData@?$CCountedPointer@VC3DMeshFileRootChunk@@@@CIXPAX@Z")
#pragma comment(linker, "/alternatename:@DeleteMap@4=?DeleteData@?$CCountedPointer@VC3DMeshFileMapChunk@@@@CIXPAX@Z")
#pragma comment(linker, "/alternatename:@DeleteMeshFile@4=?DeleteData@?$CCountedPointer@VC3DMeshFile@@@@CIXPAX@Z")

int main()
{
    static void* vtable[1] = { (void*)&FakeDeletingDtor };
    static void* object[4];
    object[0] = vtable;
    void (__fastcall* fns[7])(void*) = { DeleteGroup, DeleteVertices, DeleteTriangles, DeletePrimitive, DeleteRoot, DeleteMap, DeleteMeshFile };
    for (int i = 0; i < 7; ++i)
    {
        g_calls = 0; g_self = 0; g_flags = 0;
        fns[i](object);
        CHECK(g_calls == 1 && g_self == object && g_flags == 1);
        fns[i](0);
        CHECK(g_calls == 1);
    }
    printf(g_fail ? "FAIL\n" : "CHUNK_DELETEDATA_OK\n");
    return g_fail != 0;
}
