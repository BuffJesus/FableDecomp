#include <cstdio>

struct CActiveFile {
    void* m_buffer;
    void PreFinish();
    void OnReadFinished();
};

static int g_preCalled = 0;
static int g_freeCalled = 0;
static void* g_freedPtr = 0;

void CActiveFile::PreFinish() { g_preCalled++; }

extern "C" void __cdecl ReleaseBuffer(void* p) { g_freeCalled++; g_freedPtr = p; }

int main()
{
    // Case 1: non-null buffer -> pre called, free called with buffer
    int dummy = 42;
    CActiveFile a;
    a.m_buffer = &dummy;
    a.OnReadFinished();
    if (g_preCalled != 1) { printf("FAIL pre1\n"); return 1; }
    if (g_freeCalled != 1) { printf("FAIL free1\n"); return 1; }
    if (g_freedPtr != &dummy) { printf("FAIL ptr1\n"); return 1; }

    // Case 2: null buffer -> pre called, free NOT called
    CActiveFile b;
    b.m_buffer = 0;
    b.OnReadFinished();
    if (g_preCalled != 2) { printf("FAIL pre2\n"); return 1; }
    if (g_freeCalled != 1) { printf("FAIL free2\n"); return 1; }

    printf("READFIN_OK\n");
    return 0;
}