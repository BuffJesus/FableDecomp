#include <cstdio>
struct CActiveFile { void* buf; };
static int g_helper_calls = 0;
static int g_free_calls = 0;
static void* g_freed = 0;
void __cdecl engine_free(void* p) { g_free_calls++; g_freed = p; }
void __fastcall CActiveFile_helper(CActiveFile* self) { g_helper_calls++; (void)self; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_helper(self);
    void* p = self->buf;
    if (p)
        engine_free(p);
}

int main()
{
    int marker = 0;
    void* mp = (void*)(&marker);
    CActiveFile a; a.buf = mp;
    CActiveFile_OnReadFinished(&a);
    if (g_helper_calls != 1) { std::printf("FAIL helper\n"); return 1; }
    if (g_free_calls != 1 || g_freed != mp) { std::printf("FAIL free\n"); return 1; }

    g_helper_calls = 0; g_free_calls = 0;
    CActiveFile b; b.buf = 0;
    CActiveFile_OnReadFinished(&b);
    if (g_helper_calls != 1) { std::printf("FAIL helper2\n"); return 1; }
    if (g_free_calls != 0) { std::printf("FAIL nofree\n"); return 1; }

    std::printf("CActiveFile_00451ac6_TEST PASS\n");
    return 0;
}