#include <stdio.h>

struct CActiveFile {
    void* buf;
};

static int g_helper_called = 0;
static void* g_freed = (void*)0;

extern "C" void __fastcall Helper_OnRead(CActiveFile* self) {
    g_helper_called = 1;
    (void)self;
}
extern "C" void __cdecl retail_free(void* p) {
    g_freed = p;
}

void __fastcall OnReadFinished(CActiveFile* self);

int main() {
    CActiveFile a;
    int dummy = 5;
    a.buf = &dummy;
    g_helper_called = 0; g_freed = (void*)0;
    OnReadFinished(&a);
    if (!g_helper_called) { printf("FAIL helper1\n"); return 1; }
    if (g_freed != &dummy) { printf("FAIL free1\n"); return 1; }

    CActiveFile b;
    b.buf = (void*)0;
    g_helper_called = 0; g_freed = (void*)1;
    OnReadFinished(&b);
    if (!g_helper_called) { printf("FAIL helper2\n"); return 1; }
    if (g_freed != (void*)1) { printf("FAIL free2\n"); return 1; }

    printf("OK_0066063f\n");
    return 0;
}