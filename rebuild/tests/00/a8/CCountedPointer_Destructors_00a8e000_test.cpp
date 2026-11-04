// Behaviour fixture for the out-of-line CCountedPointer<T> destructors (provider CCountedPointer_Destructors_00a8e000.cpp;
// it defines none of them). Each destructor is reached by its mangled name through /alternatename (the template body
// is visible here, so a direct call would test an inline copy); the pointer is raw storage {Data, Info}.
#include "fable_counted_pointer.h"
#include <stdio.h>
#include <string.h>

static int g_fail, g_deletes;
static void* g_deleted;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)
static void FABLE_FASTCALL TestDelete(void* data) { ++g_deletes; g_deleted = data; }

extern "C" void __fastcall DtorStripifier(void*);
extern "C" void __fastcall DtorPartition(void*);
extern "C" void __fastcall DtorSuperChunk(void*);
extern "C" void __fastcall DtorActiveSound(void*);
extern "C" void __fastcall DtorAnimRoot(void*);
extern "C" void __fastcall DtorMeshRoot(void*);
extern "C" void __fastcall DtorMapChunk(void*);
#pragma comment(linker, "/alternatename:@DtorStripifier@4=??1?$CCountedPointer@VCTriangleStripifierBase@@@@QAE@XZ")
#pragma comment(linker, "/alternatename:@DtorPartition@4=??1?$CCountedPointer@VC3DMeshPartitionElement@@@@QAE@XZ")
#pragma comment(linker, "/alternatename:@DtorSuperChunk@4=??1?$CCountedPointer@VCChunkedFileSuperChunk@@@@QAE@XZ")
#pragma comment(linker, "/alternatename:@DtorActiveSound@4=??1?$CCountedPointer@VCActiveSound@CSoundEmitterLH@@@@QAE@XZ")
#pragma comment(linker, "/alternatename:@DtorAnimRoot@4=??1?$CCountedPointer@VC3DAnimFileRootChunk@@@@QAE@XZ")
#pragma comment(linker, "/alternatename:@DtorMeshRoot@4=??1?$CCountedPointer@VC3DMeshFileRootChunk@@@@QAE@XZ")
#pragma comment(linker, "/alternatename:@DtorMapChunk@4=??1?$CCountedPointer@VC3DMeshFileMapChunk@@@@QAE@XZ")

int main()
{
    void (__fastcall* dtors[7])(void*) = { DtorStripifier, DtorPartition, DtorSuperChunk, DtorActiveSound, DtorAnimRoot, DtorMeshRoot, DtorMapChunk };
    static int payload;
    for (int i = 0; i < 7; ++i)
    {
        CCPPointerInfo info(&TestDelete, &payload);
        info.RefCount = 2;
        void* ptr[2] = { &payload, &info };
        g_deletes = 0;
        dtors[i](ptr);                                   // another owner remains
        CHECK(info.RefCount == 1 && g_deletes == 0);
        void* ptr2[2] = { &payload, &info };
        dtors[i](ptr2);                                  // last owner: the data's delete function runs
        CHECK(g_deletes == 1 && g_deleted == &payload);
        void* empty[2] = { 0, 0 };
        dtors[i](empty);                                 // no shared block: nothing to release
        CHECK(g_deletes == 1);
    }
    printf(g_fail ? "FAIL\n" : "COUNTED_POINTER_DTORS_OK\n");
    return g_fail != 0;
}
