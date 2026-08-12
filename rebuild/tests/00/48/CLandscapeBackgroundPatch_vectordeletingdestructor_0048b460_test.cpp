#include <stdio.h>

struct CLandscapeBackgroundPatch { int dummy; };

static int g_dtor_calls = 0;
static int g_free_calls = 0;
static void* g_freed = 0;

void __fastcall CLandscapeBackgroundPatch_Destroy(CLandscapeBackgroundPatch* self)
{
    g_dtor_calls++;
    (void)self;
}

extern "C" void FreeMem(void* p)
{
    g_free_calls++;
    g_freed = p;
}

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(
    CLandscapeBackgroundPatch* self, int edx, unsigned int flags)
{
    CLandscapeBackgroundPatch_Destroy(self);
    if (flags & 1)
        FreeMem(self);
    return self;
}

int main()
{
    CLandscapeBackgroundPatch a; a.dummy = 7;

    // flags with bit0 clear: dtor runs, no free, returns self
    g_dtor_calls = g_free_calls = 0; g_freed = 0;
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_destructor(&a, 0, 0);
    if (r1 != &a || g_dtor_calls != 1 || g_free_calls != 0) { printf("FAIL1\n"); return 1; }

    // flags with bit0 set: dtor runs, free(self), returns self
    g_dtor_calls = g_free_calls = 0; g_freed = 0;
    void* r2 = CLandscapeBackgroundPatch_vector_deleting_destructor(&a, 0, 1);
    if (r2 != &a || g_dtor_calls != 1 || g_free_calls != 1 || g_freed != &a) { printf("FAIL2\n"); return 1; }

    // higher bits set but bit0 clear -> no free
    g_dtor_calls = g_free_calls = 0; g_freed = 0;
    void* r3 = CLandscapeBackgroundPatch_vector_deleting_destructor(&a, 0, 2);
    if (r3 != &a || g_free_calls != 0) { printf("FAIL3\n"); return 1; }

    printf("VDD_PARITY_OK\n");
    return 0;
}