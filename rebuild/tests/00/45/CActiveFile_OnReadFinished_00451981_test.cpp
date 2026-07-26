#include <cstdio>

struct CActiveFile { void* buf; };

static int g_helper_called = 0;
static void* g_freed = (void*)0;

extern "C" void __fastcall CActiveFile_helper(CActiveFile* self) { g_helper_called++; }
extern "C" void __cdecl free_buf(void* p) { g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_helper(self);
    void* p = self->buf;
    if (p)
        free_buf(p);
}

int main()
{
    int marker = 42;
    void* markerp = (void*)(&marker);
    CActiveFile a;
    a.buf = markerp;
    CActiveFile_OnReadFinished(&a);
    if (g_helper_called != 1) { std::printf("FAIL helper\n"); return 1; }
    if (g_freed != markerp) { std::printf("FAIL freed\n"); return 1; }

    g_helper_called = 0;
    g_freed = (void*)0;
    CActiveFile b;
    b.buf = (void*)0;
    CActiveFile_OnReadFinished(&b);
    if (g_helper_called != 1) { std::printf("FAIL helper2\n"); return 1; }
    if (g_freed != (void*)0) { std::printf("FAIL freed2\n"); return 1; }

    std::printf("CActiveFile_00451981_TEST PASS\n");
    return 0;
}