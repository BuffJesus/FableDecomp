#include <cstdio>
#include <cstdlib>

struct CActiveFile {
    void* buffer;
};

static int g_baseCalls = 0;
static int g_freeCalls = 0;
static void* g_lastFreed = 0;

void __fastcall CActiveFile_BaseOnReadFinished(void* self) {
    (void)self;
    g_baseCalls++;
}
void __cdecl CActiveFile_FreeBuffer(void* p) {
    g_freeCalls++;
    g_lastFreed = p;
}

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

int main() {
    int dummy = 0;

    // Case 1: non-null buffer -> base called + freed
    CActiveFile a;
    a.buffer = &dummy;
    CActiveFile_OnReadFinished(&a);
    if (g_baseCalls != 1) { printf("FAIL base1\n"); return 1; }
    if (g_freeCalls != 1) { printf("FAIL free1\n"); return 1; }
    if (g_lastFreed != &dummy) { printf("FAIL freed_ptr\n"); return 1; }

    // Case 2: null buffer -> base called, no free
    CActiveFile b;
    b.buffer = 0;
    CActiveFile_OnReadFinished(&b);
    if (g_baseCalls != 2) { printf("FAIL base2\n"); return 1; }
    if (g_freeCalls != 1) { printf("FAIL free2\n"); return 1; }

    printf("ONREADFINISHED_OK\n");
    return 0;
}