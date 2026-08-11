#include <cstdio>

static int g_baseCalled = 0;
static void* g_freed = 0;

struct CActiveFileBase { void* buffer; };
struct CActiveFile : CActiveFileBase {};

void __fastcall BaseReadDone(CActiveFileBase* self) { g_baseCalled++; }

// shadow free for the test
extern "C" void __cdecl my_free(void* p) { g_freed = p; }
#define free my_free
extern "C" void __cdecl my_free(void*);

void __fastcall OnReadFinished(CActiveFile* self)
{
    BaseReadDone(self);
    if (self->buffer)
        free(self->buffer);
}
#undef free

int main()
{
    // case 1: non-null buffer -> base called, freed
    CActiveFile a;
    a.buffer = (void*)0x1234;
    g_baseCalled = 0; g_freed = 0;
    OnReadFinished(&a);
    bool ok1 = (g_baseCalled == 1) && (g_freed == (void*)0x1234);

    // case 2: null buffer -> base called, not freed
    CActiveFile b;
    b.buffer = 0;
    g_baseCalled = 0; g_freed = (void*)0xdead;
    OnReadFinished(&b);
    bool ok2 = (g_baseCalled == 1) && (g_freed == (void*)0xdead);

    if (ok1 && ok2)
        printf("004ba720_TEST PASS\n");
    else
        printf("FAIL b=%d f=%p\n", g_baseCalled, g_freed);
    return 0;
}