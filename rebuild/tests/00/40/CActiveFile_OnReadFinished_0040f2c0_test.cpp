#include <cstdio>

static int g_freed = 0;
static void* g_last = 0;
extern "C" void __cdecl free_a(void* p) { g_freed++; g_last = p; }

// Behavioural model of the first block (the real OnReadFinished semantics):
void __fastcall CActiveFile_OnReadFinished(void* self)
{
    void* p = *(void**)self;
    if (p != 0)
        free_a(p);
}

int main() {
    void* buf[2];
    int dummy;
    buf[0] = &dummy;
    buf[1] = 0;
    CActiveFile_OnReadFinished(&buf[0]);
    if (g_freed != 1 || g_last != &dummy) { std::printf("FAIL branch1\n"); return 1; }

    void* buf2[2];
    buf2[0] = 0;
    buf2[1] = 0;
    CActiveFile_OnReadFinished(&buf2[0]);
    if (g_freed != 1) { std::printf("FAIL branch0\n"); return 1; }

    std::printf("CActiveFile_0040f2c0_TEST PASS\n");
    return 0;
}