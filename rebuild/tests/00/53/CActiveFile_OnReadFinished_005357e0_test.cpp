#include <stdio.h>

struct CActiveFile {
    void* buffer;
};

static int g_base_called = 0;
static void* g_freed = (void*)0;

void __fastcall base_OnReadFinished(CActiveFile* self)
{
    (void)self;
    g_base_called++;
}

// shadow free so we observe it
static void my_free(void* p) { g_freed = p; }
#define free my_free

void __fastcall OnReadFinished(CActiveFile* self)
{
    base_OnReadFinished(self);
    void* p = self->buffer;
    if (p) {
        free(p);
    }
}
#undef free

int main()
{
    // case 1: non-null buffer -> base called + freed
    int dummy = 0;
    CActiveFile a;
    a.buffer = &dummy;
    g_base_called = 0; g_freed = (void*)0;
    OnReadFinished(&a);
    if (g_base_called != 1) { printf("FAIL base1\n"); return 1; }
    if (g_freed != &dummy) { printf("FAIL free1\n"); return 1; }

    // case 2: null buffer -> base called, no free
    CActiveFile b;
    b.buffer = (void*)0;
    g_base_called = 0; g_freed = (void*)0;
    OnReadFinished(&b);
    if (g_base_called != 1) { printf("FAIL base2\n"); return 1; }
    if (g_freed != (void*)0) { printf("FAIL free2\n"); return 1; }

    printf("005357e0_TEST PASS\n");
    return 0;
}