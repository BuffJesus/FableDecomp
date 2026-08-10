#include <stdio.h>
struct CActiveFile { void* buf; };
static int g_helper_called = 0;
static void* g_freed = 0;
extern "C" void __fastcall CActiveFile_OnReadHelper(void* self){ g_helper_called++; }
extern "C" void __cdecl my_free(void* p){ g_freed = p; }
void __fastcall OnReadFinished(CActiveFile* self);
int main(){
    int marker = 0x1234;
    void* pm = (void*)(&marker);
    CActiveFile a; a.buf = pm;
    OnReadFinished(&a);
    if (g_helper_called != 1) { printf("FAIL helper\n"); return 1; }
    if (g_freed != pm) { printf("FAIL free\n"); return 1; }
    CActiveFile b; b.buf = 0; g_helper_called = 0; g_freed = (void*)1;
    OnReadFinished(&b);
    if (g_helper_called != 1) { printf("FAIL helper2\n"); return 1; }
    if (g_freed != (void*)1) { printf("FAIL nofree\n"); return 1; }
    printf("OK_005bb5ae\n");
    return 0;
}