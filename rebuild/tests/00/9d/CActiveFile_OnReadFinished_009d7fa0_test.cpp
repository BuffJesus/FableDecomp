#include <cstdio>

static int g_helperCalls = 0;
static int g_freeCalls = 0;
static void* g_lastFreed = 0;

extern "C" void __cdecl EngineFree_bfea14(void* p) {
    g_freeCalls++;
    g_lastFreed = p;
}

struct CActiveFile {
    void* field0;
    void Helper_9d7db0();
    void OnReadFinished();
};

void CActiveFile::Helper_9d7db0() {
    g_helperCalls++;
}

void CActiveFile::OnReadFinished()
{
    Helper_9d7db0();
    void* p = field0;
    if (p != 0) {
        EngineFree_bfea14(p);
    }
}

int main() {
    // Case 1: field0 non-null -> helper called, free called with field0
    int dummy = 42;
    CActiveFile a;
    a.field0 = &dummy;
    g_helperCalls = g_freeCalls = 0; g_lastFreed = 0;
    a.OnReadFinished();
    bool ok1 = (g_helperCalls == 1) && (g_freeCalls == 1) && (g_lastFreed == &dummy);

    // Case 2: field0 null -> helper called, free NOT called
    CActiveFile b;
    b.field0 = 0;
    g_helperCalls = g_freeCalls = 0; g_lastFreed = 0;
    b.OnReadFinished();
    bool ok2 = (g_helperCalls == 1) && (g_freeCalls == 0);

    if (ok1 && ok2) {
        printf("ONREADFINISHED_OK\n");
        return 0;
    }
    printf("FAIL c1=%d c2=%d\n", (int)ok1, (int)ok2);
    return 1;
}