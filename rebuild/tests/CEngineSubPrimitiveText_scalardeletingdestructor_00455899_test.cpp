#include <cstdio>

struct CEngineSubPrimitiveText { int x; };

static int g_subDtor = 0;
static int g_dtor = 0;
static int g_deleted = 0;

void __fastcall Sub_Dtor(void* p) { (void)p; g_subDtor++; }
void __fastcall CEngineSubPrimitiveText_Dtor(CEngineSubPrimitiveText* self) { (void)self; g_dtor++; }
void __cdecl operator_delete(void* p) { (void)p; g_deleted++; }

void* __fastcall CEngineSubPrimitiveText_scalar_deleting_destructor(CEngineSubPrimitiveText* self, int edx, unsigned int flags)
{
    (void)edx;
    Sub_Dtor((char*)self + 0x74);
    CEngineSubPrimitiveText_Dtor(self);
    if (flags & 1)
        operator_delete(self);
    return self;
}

int main()
{
    CEngineSubPrimitiveText obj;
    void* r = CEngineSubPrimitiveText_scalar_deleting_destructor(&obj, 0, 0);
    if (r != &obj) { std::printf("FAIL: return\n"); return 1; }
    if (g_subDtor != 1 || g_dtor != 1 || g_deleted != 0) { std::printf("FAIL: no-delete path\n"); return 1; }
    r = CEngineSubPrimitiveText_scalar_deleting_destructor(&obj, 0, 1);
    if (g_deleted != 1) { std::printf("FAIL: delete path\n"); return 1; }
    std::printf("CEngineSubPrimitiveText_00455899_TEST PASS\n");
    return 0;
}