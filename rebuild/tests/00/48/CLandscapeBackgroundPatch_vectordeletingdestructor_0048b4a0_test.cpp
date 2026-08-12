#include <cstdio>

static int g_destroyed = 0;
static int g_freed = 0;

void __cdecl operator_delete_impl(void* p) { (void)p; g_freed = 1; }

struct CLandscapeBackgroundPatch
{
    int dummy;
    void destroy();
};
void CLandscapeBackgroundPatch::destroy() { g_destroyed = 1; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(
        CLandscapeBackgroundPatch* self, int edx, unsigned int flags);

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.dummy = 42;

    // flags bit0 clear -> destroy but no free
    g_destroyed = 0; g_freed = 0;
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r1 != &obj) { printf("FAIL ret1\n"); return 1; }
    if (g_destroyed != 1) { printf("FAIL destroy1\n"); return 1; }
    if (g_freed != 0) { printf("FAIL free1\n"); return 1; }

    // flags bit0 set -> destroy AND free
    g_destroyed = 0; g_freed = 0;
    void* r2 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r2 != &obj) { printf("FAIL ret2\n"); return 1; }
    if (g_destroyed != 1) { printf("FAIL destroy2\n"); return 1; }
    if (g_freed != 1) { printf("FAIL free2\n"); return 1; }

    // even flag (bit0 clear) -> no free
    g_freed = 0;
    CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 2);
    if (g_freed != 0) { printf("FAIL free3\n"); return 1; }

    printf("VDD_PARITY_OK\n");
    return 0;
}