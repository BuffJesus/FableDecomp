// Behaviour fixture for the CCountedPointer<C3DClothPrimitive> COMDATs (provider CCountedPointer_ClothPrimitive_00a93a60.cpp;
// it defines both). Both are reached by mangled name through /alternatename (the template bodies are visible here).
// DeleteData must run the primitive's deleting destructor (vtable slot 0) with the delete flag, and skip null; the
// destructor must release one reference, run the shared block's delete function on the last one, and null itself.
#include <stdio.h>
#include "fable_counted_pointer.h"

static int g_fail, g_calls, g_deletes;
static void* g_self;
static void* g_deleted;
static unsigned g_flags;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static void* __fastcall FakeDeletingDtor(void* self, int, unsigned flags) { ++g_calls; g_self = self; g_flags = flags; return self; }
static void FABLE_FASTCALL TestDelete(void* data) { ++g_deletes; g_deleted = data; }

extern "C" void __fastcall DeleteCloth(void*);
extern "C" void __fastcall DtorCloth(void*);
// One-argument __thiscall members: this in ecx, the argument on the stack, callee pops - as __fastcall(this, edx, arg).
extern "C" void* __fastcall CtorCloth(void* self, int, void* data);
extern "C" void __fastcall ResetCloth(void* self, int, void* data);
#pragma comment(linker, "/alternatename:@DeleteCloth@4=?DeleteData@?$CCountedPointer@VC3DClothPrimitive@@@@CIXPAX@Z")
#pragma comment(linker, "/alternatename:@DtorCloth@4=??1?$CCountedPointer@VC3DClothPrimitive@@@@QAE@XZ")
#pragma comment(linker, "/alternatename:@CtorCloth@12=??0?$CCountedPointer@VC3DClothPrimitive@@@@QAE@PAVC3DClothPrimitive@@@Z")
#pragma comment(linker, "/alternatename:@ResetCloth@12=?Reset@?$CCountedPointer@VC3DClothPrimitive@@@@QAEXPAVC3DClothPrimitive@@@Z")

int main()
{
    static void* vtable[1] = { (void*)&FakeDeletingDtor };
    static void* primitive[5];
    primitive[0] = vtable;
    DeleteCloth(primitive);
    CHECK(g_calls == 1 && g_self == primitive && g_flags == 1);
    DeleteCloth(0);
    CHECK(g_calls == 1);

    static int payload;
    static CCPPointerInfo* info = new CCPPointerInfo(&TestDelete, &payload);
    info->RefCount = 2;
    void* ptr[2] = { &payload, info };
    DtorCloth(ptr);                                       // another owner remains
    CHECK(info->RefCount == 1 && g_deletes == 0 && ptr[0] == 0 && ptr[1] == 0);
    void* last[2] = { &payload, info };
    DtorCloth(last);                                      // last owner: the data's delete function runs, block freed
    CHECK(g_deletes == 1 && g_deleted == &payload && last[0] == 0 && last[1] == 0);
    void* empty[2] = { 0, 0 };
    DtorCloth(empty);
    CHECK(g_deletes == 1);

    // Constructor: a shared block {1, DeleteData, data} for non-null data, none for null; returns this.
    void* made[2] = { (void*)1, (void*)1 };
    CHECK(CtorCloth(made, 0, primitive) == made && made[0] == primitive && made[1] != 0);
    CCPPointerInfo* block = static_cast<CCPPointerInfo*>(made[1]);
    CHECK(block->RefCount == 1 && block->Data == primitive);
    g_calls = 0;
    block->DeleteFunc(block->Data);                       // the stored delete function is DeleteData
    CHECK(g_calls == 1 && g_self == primitive && g_flags == 1);
    void* none[2] = { (void*)1, (void*)1 };
    CHECK(CtorCloth(none, 0, 0) == none && none[0] == 0 && none[1] == 0);
    // Reset: release the old reference (last owner -> delete), then own the new data with a fresh block.
    g_deletes = 0;
    CCPPointerInfo* old = new CCPPointerInfo(&TestDelete, &payload);
    void* held[2] = { &payload, old };
    ResetCloth(held, 0, primitive);
    CHECK(g_deletes == 1 && g_deleted == &payload && held[0] == primitive && held[1] != 0 && held[1] != 0);
    CHECK(static_cast<CCPPointerInfo*>(held[1])->RefCount == 1 && static_cast<CCPPointerInfo*>(held[1])->Data == primitive);
    ResetCloth(held, 0, 0);
    CHECK(held[0] == 0 && held[1] == 0);

    if (g_fail) { printf("CLOTH_COUNTED_POINTER_FAIL %d\n", g_fail); return 1; }
    printf("CLOTH_COUNTED_POINTER_OK\n");
    return 0;
}
