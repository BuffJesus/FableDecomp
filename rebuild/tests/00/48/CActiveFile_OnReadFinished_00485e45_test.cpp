#include <cstdio>

struct CActiveFile { void* buf; };
static int g_helper_called = 0;
static void* g_freed = (void*)0;

void __fastcall CActiveFile_Helper(CActiveFile* self){ g_helper_called++; (void)self; }
void CActiveFile_Free(void* p){ g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Helper(self);
    void* p = self->buf;
    if (p)
        CActiveFile_Free(p);
}

int main(){
    int dummy = 0;
    CActiveFile a; a.buf = &dummy;
    g_helper_called = 0; g_freed = (void*)0;
    CActiveFile_OnReadFinished(&a);
    if (g_helper_called != 1) { printf("FAIL helper\n"); return 1; }
    if (g_freed != &dummy) { printf("FAIL free\n"); return 1; }

    CActiveFile b; b.buf = (void*)0;
    g_helper_called = 0; g_freed = (void*)1;
    CActiveFile_OnReadFinished(&b);
    if (g_helper_called != 1) { printf("FAIL helper2\n"); return 1; }
    if (g_freed != (void*)1) { printf("FAIL nofree\n"); return 1; }

    printf("OK_00485e45\n");
    return 0;
}