#include <stdio.h>

static int g_processCalled = 0;
static void* g_freed = (void*)-1;

void __cdecl ActiveFileFree(void* p) { g_freed = p; }

struct CActiveFile {
    void* buffer;
    void ProcessReadDone();
    void OnReadFinished();
};

void CActiveFile::ProcessReadDone() { g_processCalled++; }

void CActiveFile::OnReadFinished()
{
    ProcessReadDone();
    void* p = buffer;
    if (p)
        ActiveFileFree(p);
}

int main()
{
    // case 1: non-null buffer -> process called, freed
    CActiveFile a;
    int dummy;
    a.buffer = &dummy;
    g_processCalled = 0; g_freed = (void*)-1;
    a.OnReadFinished();
    if (g_processCalled != 1) { printf("FAIL proc1\n"); return 1; }
    if (g_freed != &dummy) { printf("FAIL free1\n"); return 1; }

    // case 2: null buffer -> process called, NOT freed
    CActiveFile b;
    b.buffer = 0;
    g_processCalled = 0; g_freed = (void*)-1;
    b.OnReadFinished();
    if (g_processCalled != 1) { printf("FAIL proc2\n"); return 1; }
    if (g_freed != (void*)-1) { printf("FAIL free2\n"); return 1; }

    printf("ONREADFINISHED_OK\n");
    return 0;
}