// Behaviour fixture for C3DClothPrimitive::~C3DClothPrimitive (provider C3DClothPrimitive_Destructor_00ac3b80.cpp),
// called non-virtually on raw storage. Checked: a shared particle program only loses a reference; the last reference
// deletes the program through its counted-pointer block (delete function, then the block is freed); the pointer and
// block are zeroed; the CBaseClassNonCopyable destructor (supplied here, recording) runs last; the primitive's index
// fields are untouched.
#include <stdio.h>
#include <string.h>
#include "fable_3d_cloth_primitive.h"

static int g_fail, g_deletes, g_baseDtors; static void* g_deleted; static unsigned long g_infoAtBase;
static unsigned char* g_raw;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() { ++g_baseDtors; memcpy(&g_infoAtBase, g_raw + 8, 4); }
static void FABLE_FASTCALL DeleteProgram(void* p) { ++g_deletes; g_deleted = p; }
static unsigned long Word(const unsigned char* r, unsigned o) { unsigned long v; memcpy(&v, r + o, 4); return v; }

int main()
{
    static unsigned char raw[0x14], program[16];
    g_raw = raw;
    C3DClothPrimitive* c = reinterpret_cast<C3DClothPrimitive*>(raw);
    CCPPointerInfo* info = new CCPPointerInfo(DeleteProgram, program);
    info->RefCount = 2;
    for (int pass = 0; pass < 2; ++pass)
    {
        memset(raw, 0x5A, sizeof(raw));
        void* d = program; memcpy(raw + 4, &d, 4); memcpy(raw + 8, &info, 4);
        c->C3DClothPrimitive::~C3DClothPrimitive();
        CHECK(g_baseDtors == pass + 1 && g_infoAtBase == 0);
        CHECK(Word(raw, 4) == 0 && Word(raw, 8) == 0 && Word(raw, 0xC) == 0x5A5A5A5Au && Word(raw, 0x10) == 0x5A5A5A5Au);
        if (pass == 0) CHECK(g_deletes == 0 && info->RefCount == 1);
        else CHECK(g_deletes == 1 && g_deleted == program);
    }
    if (g_fail) { printf("CLOTH_PRIMITIVE_DTOR_FAIL %d\n", g_fail); return 1; }
    printf("CLOTH_PRIMITIVE_DTOR_OK\n");
    return 0;
}
