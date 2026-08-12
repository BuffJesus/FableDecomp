#include <stdio.h>

struct CLandscapeBackgroundPatch
{
    int marker;
    void Destroy();
};

static int g_destroyed = 0;
static int g_deleted = 0;

void CLandscapeBackgroundPatch::Destroy() { g_destroyed++; }
void ClassOperatorDelete(void* p) { g_deleted++; (void)p; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(
    CLandscapeBackgroundPatch* self, int edx, unsigned int flags);

int main()
{
    CLandscapeBackgroundPatch a; a.marker = 7;
    CLandscapeBackgroundPatch b; b.marker = 9;

    // flags & 1 == 0 : destroy only, no delete
    g_destroyed = 0; g_deleted = 0;
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_destructor(&a, 0, 0);
    if (r1 != &a) { printf("FAIL r1\n"); return 1; }
    if (g_destroyed != 1) { printf("FAIL destroy1\n"); return 1; }
    if (g_deleted != 0) { printf("FAIL nodelete\n"); return 1; }

    // flags & 1 == 1 : destroy + delete
    g_destroyed = 0; g_deleted = 0;
    void* r2 = CLandscapeBackgroundPatch_vector_deleting_destructor(&b, 0, 1);
    if (r2 != &b) { printf("FAIL r2\n"); return 1; }
    if (g_destroyed != 1) { printf("FAIL destroy2\n"); return 1; }
    if (g_deleted != 1) { printf("FAIL delete2\n"); return 1; }

    // even flag bit set but bit0 clear -> no delete
    g_deleted = 0;
    CLandscapeBackgroundPatch_vector_deleting_destructor(&a, 0, 2);
    if (g_deleted != 0) { printf("FAIL bit0\n"); return 1; }

    printf("VDD_PARITY_OK\n");
    return 0;
}