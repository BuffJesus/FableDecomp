#include <cstdio>

struct CActiveFile { void* m0; };

static int g_base_calls = 0;
static void* g_freed = (void*)-1;

extern "C" void base_helper(CActiveFile* self) { (void)self; g_base_calls++; }
void free_helper(void* p) { g_freed = p; }

// mirror the modeled source
struct CActiveFile2 { void* m0; };
void run(CActiveFile* self)
{
    base_helper(self);
    void* p = self->m0;
    if (p) free_helper(p);
}

int main()
{
    CActiveFile a; a.m0 = (void*)0x1234;
    g_base_calls = 0; g_freed = (void*)-1;
    run(&a);
    bool ok1 = (g_base_calls == 1) && (g_freed == (void*)0x1234);

    CActiveFile b; b.m0 = 0;
    g_base_calls = 0; g_freed = (void*)-1;
    run(&b);
    bool ok2 = (g_base_calls == 1) && (g_freed == (void*)-1); // not freed

    if (ok1 && ok2) printf("0062e820_TEST PASS\n");
    else printf("FAIL b=%d f=%p\n", g_base_calls, g_freed);
    return 0;
}