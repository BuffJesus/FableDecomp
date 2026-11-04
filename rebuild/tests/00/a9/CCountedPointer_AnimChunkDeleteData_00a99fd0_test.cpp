// Behaviour fixture for the anim-file CCountedPointer<T>::DeleteData instantiations (provider
// CCountedPointer_AnimChunkDeleteData_00a99fd0.cpp; it defines none of them). DeleteData is private, so each instantiation is
// reached by its mangled name through /alternatename. The "chunk" is a fake object whose vtable slot 0 (the deleting
// destructor) records the call: DeleteData must run it with the delete flag on non-null data and skip null data.
#include <stdio.h>

static int g_fail, g_calls;
static void* g_self;
static unsigned g_flags;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static void* __fastcall FakeDeletingDtor(void* self, int, unsigned flags) { ++g_calls; g_self = self; g_flags = flags; return self; }

extern "C" void __fastcall DeleteSuper(void*);
extern "C" void __fastcall DeleteAnimRoot(void*);
extern "C" void __fastcall DeleteChunk(void*);
extern "C" void __fastcall DeleteAnimFile(void*);
#pragma comment(linker, "/alternatename:@DeleteSuper@4=?DeleteData@?$CCountedPointer@VCChunkedFileSuperChunk@@@@CIXPAX@Z")
#pragma comment(linker, "/alternatename:@DeleteAnimRoot@4=?DeleteData@?$CCountedPointer@VC3DAnimFileRootChunk@@@@CIXPAX@Z")
#pragma comment(linker, "/alternatename:@DeleteChunk@4=?DeleteData@?$CCountedPointer@VCChunkedFileChunk@@@@CIXPAX@Z")
#pragma comment(linker, "/alternatename:@DeleteAnimFile@4=?DeleteData@?$CCountedPointer@VC3DAnimFile@@@@CIXPAX@Z")

int main()
{
    static void* vtable[1] = { (void*)&FakeDeletingDtor };
    static void* object[4];
    object[0] = vtable;
    void (__fastcall* fns[4])(void*) = { DeleteSuper, DeleteAnimRoot, DeleteChunk, DeleteAnimFile };
    for (int i = 0; i < 4; ++i)
    {
        g_calls = 0; g_self = 0; g_flags = 0;
        fns[i](object);
        CHECK(g_calls == 1 && g_self == object && g_flags == 1);
        fns[i](0);
        CHECK(g_calls == 1);
    }
    if (g_fail) { printf("ANIM_CHUNK_DELETEDATA_FAIL %d\n", g_fail); return 1; }
    printf("ANIM_CHUNK_DELETEDATA_OK\n");
    return 0;
}
