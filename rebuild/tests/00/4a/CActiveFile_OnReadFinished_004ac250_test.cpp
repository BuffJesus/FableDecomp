#include <cstdio>

struct CActiveFile;

static int g_base_called = 0;
static void* g_freed = (void*)0;

extern "C" void __fastcall base_helper(CActiveFile* self);
extern "C" void __cdecl fable_free(void* p);

struct CActiveFile {
    void* p0;
};

extern "C" void __fastcall base_helper(CActiveFile* self) { (void)self; g_base_called++; }
extern "C" void __cdecl fable_free(void* p) { g_freed = p; }

void __fastcall CActiveFile__OnReadFinished(CActiveFile* self);

int main()
{
    int dummy = 0;
    CActiveFile a; a.p0 = &dummy;
    g_base_called = 0; g_freed = (void*)0;
    CActiveFile__OnReadFinished(&a);
    bool ok1 = (g_base_called == 1) && (g_freed == &dummy);

    CActiveFile b; b.p0 = (void*)0;
    g_base_called = 0; g_freed = (void*)1;
    CActiveFile__OnReadFinished(&b);
    bool ok2 = (g_base_called == 1) && (g_freed == (void*)1);

    if (ok1 && ok2)
        printf("004ac250_TEST PASS\n");
    else
        printf("FAIL b=%d f=%p\n", g_base_called, g_freed);
    return 0;
}