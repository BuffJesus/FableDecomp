#include <stdio.h>

struct CActiveFile {
    void* field0;
};

static int g_base_called = 0;
static void* g_freed = (void*)0;

void __fastcall CActiveFile_base(CActiveFile* self) {
    g_base_called = 1;
}

extern "C" void free_func(void* p) {
    g_freed = p;
}

void __fastcall OnReadFinished(CActiveFile* self);

int main() {
    // case: non-null field
    int dummy = 0;
    CActiveFile a;
    a.field0 = &dummy;
    g_base_called = 0; g_freed = (void*)0;
    OnReadFinished(&a);
    if (!g_base_called) { printf("FAIL base\n"); return 1; }
    if (g_freed != &dummy) { printf("FAIL free\n"); return 1; }

    // case: null field -> no free
    CActiveFile b;
    b.field0 = (void*)0;
    g_base_called = 0; g_freed = (void*)0x1;
    OnReadFinished(&b);
    if (!g_base_called) { printf("FAIL base2\n"); return 1; }
    if (g_freed != (void*)0x1) { printf("FAIL free2\n"); return 1; }

    printf("0070b8f0_TEST PASS\n");
    return 0;
}