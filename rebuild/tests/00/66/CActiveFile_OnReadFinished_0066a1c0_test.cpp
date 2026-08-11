#include <cstdio>
#include <cstdlib>

struct CActiveFile { void* buf; };

static int g_base_called = 0;
static void* g_freed = 0;

void __fastcall base_read(void* self) { g_base_called = 1; }

extern "C" void my_free(void* p) { g_freed = p; }
#define free my_free

void __fastcall OnReadFinished(CActiveFile* self)
{
    base_read(self);
    void* p = self->buf;
    if (p) {
        free(p);
    }
}
#undef free

int main()
{
    void* dummy = (void*)0x12345678;
    CActiveFile a; a.buf = dummy;
    g_base_called = 0; g_freed = 0;
    OnReadFinished(&a);
    if (g_base_called != 1) { printf("FAIL base\n"); return 1; }
    if (g_freed != dummy) { printf("FAIL free\n"); return 1; }

    CActiveFile b; b.buf = 0;
    g_base_called = 0; g_freed = (void*)0xdead;
    OnReadFinished(&b);
    if (g_base_called != 1) { printf("FAIL base2\n"); return 1; }
    if (g_freed != (void*)0xdead) { printf("FAIL free2\n"); return 1; }

    printf("0066a1c0_TEST PASS\n");
    return 0;
}