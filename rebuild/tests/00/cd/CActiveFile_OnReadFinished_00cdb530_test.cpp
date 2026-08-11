#include <stdio.h>

struct CActiveFile
{
    void* field0;
    void Helper();
    void OnReadFinished();
};

static int g_helperCalls = 0;
static int g_freeCalls = 0;
static void* g_freedPtr = 0;

void CActiveFile::Helper()
{
    g_helperCalls++;
}

void __cdecl FreeThing(void* p)
{
    g_freeCalls++;
    g_freedPtr = p;
}

int main()
{
    int dummy = 0;

    // Case 1: field0 non-null -> Helper called + FreeThing(field0)
    CActiveFile a;
    a.field0 = &dummy;
    a.OnReadFinished();
    if (g_helperCalls != 1) { printf("FAIL h1\n"); return 1; }
    if (g_freeCalls != 1) { printf("FAIL f1\n"); return 1; }
    if (g_freedPtr != &dummy) { printf("FAIL p1\n"); return 1; }

    // Case 2: field0 null -> Helper called, FreeThing NOT called
    CActiveFile b;
    b.field0 = 0;
    b.OnReadFinished();
    if (g_helperCalls != 2) { printf("FAIL h2\n"); return 1; }
    if (g_freeCalls != 1) { printf("FAIL f2\n"); return 1; }

    printf("PASS_OnReadFinished_OK\n");
    return 0;
}