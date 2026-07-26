#include <cstdio>

struct CInner;
static int g_process_called = 0;
static int g_free_called = 0;
static void* g_freed = 0;

void __fastcall Inner_Process(CInner* self, int arg, char* out) {
    g_process_called = 1;
    *out = (char)(arg & 0xFF);
    (void)self;
}
void __cdecl Free_Buf(void* p) {
    g_free_called = 1;
    g_freed = p;
}

struct CActiveFile {
    CInner* p;
    int     arg;
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    char local;
    Inner_Process(self->p, self->arg, &local);
    if (self->p != 0)
        Free_Buf(self->p);
}

int main() {
    CActiveFile a;
    a.p = (CInner*)0x1234;
    a.arg = 42;
    g_process_called = g_free_called = 0;
    CActiveFile_OnReadFinished(&a);
    if (!g_process_called) { std::printf("FAIL: process not called\n"); return 1; }
    if (!g_free_called || g_freed != (void*)0x1234) { std::printf("FAIL: free path\n"); return 1; }

    CActiveFile b;
    b.p = 0;
    b.arg = 7;
    g_process_called = g_free_called = 0;
    CActiveFile_OnReadFinished(&b);
    if (!g_process_called) { std::printf("FAIL: process not called (null)\n"); return 1; }
    if (g_free_called) { std::printf("FAIL: freed null\n"); return 1; }

    std::printf("CActiveFile_0045334a_TEST PASS\n");
    return 0;
}