#include <cstdio>

static int g_dtor = 0;
static int g_del = 0;
static void* g_last_del = 0;

void __fastcall CLandscapeBackgroundPatch_dtor(void* self) { g_dtor++; }
void __cdecl operator_delete(void* p) { g_del++; g_last_del = p; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(void* self, int edx_dummy, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        operator_delete(self);
    return self;
}

int main()
{
    int obj = 0;
    void* p = &obj;

    g_dtor = g_del = 0; g_last_del = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(p, 0, 0);
    if (r != p) { std::printf("FAIL ret0\n"); return 1; }
    if (g_dtor != 1) { std::printf("FAIL dtor0\n"); return 1; }
    if (g_del != 0) { std::printf("FAIL del0\n"); return 1; }

    g_dtor = g_del = 0; g_last_del = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(p, 0, 1);
    if (r != p) { std::printf("FAIL ret1\n"); return 1; }
    if (g_dtor != 1) { std::printf("FAIL dtor1\n"); return 1; }
    if (g_del != 1) { std::printf("FAIL del1\n"); return 1; }
    if (g_last_del != p) { std::printf("FAIL delarg\n"); return 1; }

    g_dtor = g_del = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(p, 0, 3);
    if (g_del != 1) { std::printf("FAIL del3\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_004545ee_TEST PASS\n");
    return 0;
}