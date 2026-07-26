#include <cstdio>

struct CActiveFile {
    void* p; // +0x0
    void* q; // +0x4
};

static int g_helper_called = 0;
static int g_free_called = 0;
static void* g_freed = 0;

extern "C" void __fastcall CActiveFile_Helper(CActiveFile* self){ g_helper_called++; }
extern "C" void __cdecl CActiveFile_Free(void* p){ g_free_called++; g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Helper(self);
    void* p = self->p;
    if (p)
        CActiveFile_Free(p);
}

int main(){
    int dummy = 0;
    CActiveFile a;
    a.p = &dummy; a.q = 0;
    CActiveFile_OnReadFinished(&a);
    if (g_helper_called != 1) { std::printf("FAIL helper\n"); return 1; }
    if (g_free_called != 1 || g_freed != &dummy) { std::printf("FAIL free path\n"); return 1; }

    g_helper_called = 0; g_free_called = 0; g_freed = 0;
    CActiveFile b;
    b.p = 0; b.q = 0;
    CActiveFile_OnReadFinished(&b);
    if (g_helper_called != 1) { std::printf("FAIL helper2\n"); return 1; }
    if (g_free_called != 0) { std::printf("FAIL null path\n"); return 1; }

    std::printf("CActiveFile_004405d0_TEST PASS\n");
    return 0;
}